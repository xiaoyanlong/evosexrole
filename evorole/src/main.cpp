#include <iostream>
#include <future>
#include <atomic>
#include <queue>
#include <algorithm>
#include <cmd_line.h>
#include "json.h"


const char help_msg[] = R"(usage:
evorole [Options] [rep] param outdir
Options
  --help    display this text and exit
  --vebose  verbose console output 
  --force   force override of output directory

Optional arguments
  rep       number of repetitions [1]

Mandatory arguments
  param     parameter json file
  outdir    output directory
    
Example:
  evorole --verbose rep=10 param=parameter.json out=data
  )";



class queue
{
public:
  struct elem_t {
      std::filesystem::path outdir;
      evorole::Parameter param;
  };

public:
  queue(std::vector<evorole::Parameter> params, const std::filesystem::path& outdir, size_t rep, bool verbose, bool sim_verbose)
  : futures_(std::thread::hardware_concurrency()),
    verbose_(verbose), 
    sim_verbose_(sim_verbose)
  {
    for (size_t p = 0; p < params.size(); ++p) {
      auto par_out = outdir / (std::string("param") + std::to_string(p));
      if (!std::filesystem::create_directory(par_out)) {
        throw std::runtime_error("unable to create simulation directory");
      }
      evorole::ParameterToJson(params[p], par_out / "param.json");
      for (size_t r = 0; r < rep; ++r) {
        auto rep_out = par_out / std::string("rep" + std::to_string(r));
        if (!std::filesystem::create_directory(rep_out)) {
          throw std::runtime_error("unable to create repetition directory");
        }
        queue_.push({ rep_out, params[p] });
      }
    }
  }

  ~queue()
  {
    for (size_t i = 0; i < futures_.size(); ++i) {
      if (futures_[i].valid()) {
        futures_[i].get();
      }
    }
  }

  void run()
  {
    const auto sims = queue_.size();
    elem_t elem{};
    while (!queue_.empty()) {
      elem = queue_.front();
      queue_.pop();
      auto slot = select_slot();
      if (verbose_) {
        std::cout << "starting simulation " << sims - queue_.size() << " of " << sims << " -> ";
        std::cout << std::filesystem::absolute(elem.outdir) << std::endl;
      }
      futures_[slot] = std::async(std::launch::async, evorole::run_simulation, elem.param, elem.outdir, sim_verbose_);
    }
  }

private:
  size_t select_slot()
  {
    for (;;) {
      for (size_t i = 0; i < futures_.size(); ++i) {
        if (!futures_[i].valid()) {
          return i;
        }
      }
      for (size_t i = 0; i < futures_.size(); ++i) {
        if (futures_[i].valid()) {
          if (std::future_status::ready == futures_[i].wait_for(std::chrono::milliseconds(100))) {
            futures_[i].get();
            break;
          }
        }
      }
    }
    return -1;
  }

  std::queue<elem_t> queue_;
  std::vector<std::future<void>> futures_;
  const bool verbose_, sim_verbose_;
};



void run_batch(std::vector<evorole::Parameter> params, const std::filesystem::path& outdir, size_t rep, bool verbose)
{
  const bool sim_verbose = ((rep == 1) && (1 == params.size())) ? verbose : false;
  auto Q = queue(params, outdir, rep, verbose, sim_verbose);
  Q.run();
}


int main(int argc, const char** argv)
{
  try {
    cmd::cmd_line_parser clp(argc, argv);
    if (clp.flag("--help")) {
      std::cout << help_msg;
      return 0;
    }
    auto outdir = clp.required<std::filesystem::path>("outdir");
    if (!clp.flag("--force") && std::filesystem::exists(outdir)) {
      // accidentally overwritten data is nightmarish
      throw cmd::parse_error("outdir exists. Consider --force to force override");
    }
    size_t rep = 1;
    clp.optional("rep", rep);
    auto param_file = clp.required<std::filesystem::path>("param");
    bool verbose = clp.flag("--verbose");
    // check for unseen arguments - common source: typos in optional arguments.
    if (!clp.unrecognized().empty()) {
      throw cmd::parse_error("Found gibberish in command line. Use --help for reference");
    }
    auto params = evorole::ParametersFromJson(param_file);
    std::filesystem::remove_all(outdir);
    if (!std::filesystem::create_directory(outdir)) {
      throw std::runtime_error("unable to create top-level output directory");
    }
    // finally do something...
    run_batch(params, outdir, rep, verbose);
    std::cout << "Regards\n";
    return 0;
  }
  catch (const std::exception& err) {
    std::cerr << err.what() << '\n';
  }
  std::cerr << "Bailing out.\n";
  return 1;
}
