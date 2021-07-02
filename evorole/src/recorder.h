#ifndef EVOROLE_RECORDER_H_INCLUDED
#define EVOROLE_RECORDER_H_INCLUDED

#include "simulation.h"


namespace evorole {


  struct genome_sum
  {
    genome_sum& operator+=(const genome_t& g);
    genome_sum average() const;

    double P = 0.0;
    double tau = 0.0;
    double pc1 = 0.0;
    double pc2 = 0.0;
    size_t c_ = 0;
  };


  class Recorder
  {
  public:
    // const iterator to individual
    using cind_iter = typename std::vector<Individual>::const_iterator;

  public:
    Recorder(const Parameter& param, const std::filesystem::path& out, bool verbose) :
      param_(param.recorder),
      verbose_(verbose),
      out_(out)
    {}

    void stream_out() const;
    void record_deaths(int day, Pool src, cind_iter first, cind_iter last);
    void record_death(int day, Pool src, const Individual& ind);
    void record_mating(int day, const Individual& ind);
    void record_offspring(int day, const Individual& ind, const Individual& female, const Individual& male);
    void record_traits_begin(int day, const class Simulation& sim);
    void record_traits_end(int day, const class Simulation& sim);

  private:
    struct death_record_t
    {
      int day;
      Pool src;
      Individual ind;
    };

    struct mating_record_t
    {
        int day;
        Individual ind;
    };

    struct offspring_record_t
    {
      int day;
      Individual ind;
      Individual female;
      Individual male;
    };

    struct all_trait_record_t
    {
      int day;
      double osr, asr, psr;
      genome_sum ave_genome;
      std::array<std::array<size_t, Pool::max>, 2> size;
    };

  private:
    recorder_t param_;
    std::vector<death_record_t> deaths_;
    std::vector<mating_record_t> mating_;
    std::vector<offspring_record_t> offspring_;
    std::vector<all_trait_record_t> all_traits_;
    bool verbose_;
    std::filesystem::path out_;
  };

}

#endif
