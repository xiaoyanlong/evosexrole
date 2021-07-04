#include <cmath>
#include <iostream>
#include <fstream>
#include <numeric>
#include "recorder.h"
#include "simulation.h"


namespace evorole {


  namespace {

    constexpr const char* PoolName[Pool::max] =
    {
      "care",
      "mate",
      "pre",
      "juv"
    };
    constexpr const char* SexLabel[2] = { "female", "male" };


    ////////////////////////////////////////////////////////////////////////
    // Some statistics ;)
    ////////////////////////////////////////////////////////////////////////


    // asr: male/(male+female), for all adults
    double asrValue(const Simulation& sim)
    {
      size_t adult_females = 0;
      size_t adult_males = 0;
      for (size_t i = 0; i < Pool::max - 1; ++i) {
          adult_females += sim.pop()[Sex::female][i].size();
          adult_males += sim.pop()[Sex::male][i].size();
      }
      
      const size_t adults = adult_females + adult_males;
      return adults ? static_cast<double>(adult_males) / adults : 0.5;
    }


    // osr: male/(male+female), for adults in the mating state
    double osrValue(const Simulation& sim)
    {
      const size_t mating_males = sim.pop()[Sex::male][Pool::mate].size();
      const size_t mating_ind = mating_males + sim.pop()[Sex::female][Pool::mate].size();
      return mating_ind ? static_cast<double>(mating_males) / mating_ind : 0.5;
    }


    // psr: male/(male+female), for juveniles
    double psrValue(const Simulation& sim)
    {
      const size_t juv_males = sim.pop()[Sex::male][Pool::juv].size();
      const size_t juveniles = juv_males + sim.pop()[Sex::female][Pool::juv].size();
      return juveniles ? static_cast<double>(juv_males) / juveniles : 0.5;
    }

  }


  genome_sum& genome_sum::operator+=(const genome_t& g)
  {
    P += g.P;
    tau += g.tau;
    pc1 += static_cast<double>(g.pc1);
    pc2 += static_cast<double>(g.pc2);
    ++c_;
    return *this;
  }


  genome_sum genome_sum::average() const
  {
    return c_
      ? genome_sum{ P / c_, tau / c_, pc1 / c_, pc2 / c_, 1 }
    : genome_sum{};
  }


  std::ostream& operator<<(std::ostream& os, const genome_t& g)
  {
    os << g.P << ',' << g.tau << ',' << g.pc1 << ',' << g.pc2;
    return os;
  }


  std::ostream& operator<<(std::ostream& os, const genome_sum& g)
  {
    os << g.P << ',' << g.tau << ',' << g.pc1 << ',' << g.pc2;
    return os;
  }


  void Recorder::stream_out() const
  {
    const char* sex_prefix[] = { "female", "male" };
    // pump out data files.
    { // all traits
      auto os = std::ofstream(out_ / "all_traits.csv");
      os << "day,osr,asr,psr,preference,ornaments,femalePC,malePC";
      for (auto sex : { Sex::female, Sex::male }) {
        for (int p = 0; p < Pool::max; ++p) {
          os << ',' << sex_prefix[sex] << '_' << PoolName[p];
        }
      }
      os << ",pop_size\n";
      for (const auto& rec : all_traits_) {
        os << rec.day << ',' << rec.osr << ',' << rec.asr << ',' << rec.psr << ',';
        os << rec.ave_genome;
        size_t s = 0;
        for (auto sex : { Sex::female, Sex::male }) {
          for (int p = 0; p < Pool::max; ++p) {
            os << ',' << rec.size[sex][p];
            s += rec.size[sex][p];
          }
        }
        os << ',' << s << '\n';
      }
    }
    //{ // deaths
    //  auto os = std::ofstream(out_ / "deaths.csv");
    //  os << "day,individual,preference,ornaments,pc1,pc2,matingT,matingduration,numOff,numSurOff,\n";
    //  for (const auto& rec : deaths_) {
    //    os << rec.day << ',' << sex_prefix[rec.ind.sex] << '_' << PoolName[rec.src] << ',';
    //    os << rec.ind.genome << ',';
    //    os << rec.ind.matings << ',' << rec.ind.matingduration<< ','<<
    //        rec.ind.offbefden << ',' << rec.ind.offspring << '\n';
    //  }
    //}
    //{ // mating
    //    auto os = std::ofstream(out_ / "mating.csv");
    //    os << "day,sex,preference,ornaments,pc1,pc2,mating_day,matingT,matingduration,numOff,numSurOff,\n";
    //    for (const auto& rec : mating_) {
    //        os << rec.day << ',' << sex_prefix[rec.ind.sex] << ',';
    //        os << rec.ind.genome << ',';
    //        os << rec.ind.matings_day << ',' << rec.ind.matings << ',' << rec.ind.matingduration << ',' <<
    //            rec.ind.offbefden << ',' << rec.ind.offspring << '\n';
    //    }
    //}
    { // offspring
      auto os = std::ofstream(out_ / "offspring.csv");
      os << "day,sex,malep1,malet,malepc1,malepc2,femalep1,femalet,femalepc1,femalepc2,offp1,offt,offpc1,offpc2,offjuvdays\n";
      for (const auto& rec : offspring_) {
        os << rec.day << ',' << sex_prefix[rec.ind.sex] << ',';
        os << rec.male.genome << ',';
        os << rec.female.genome << ',';
        os << rec.ind.genome << ',';
        os << rec.ind.pool_stay << '\n';
      }
    }
  }


  void Recorder::record_deaths(int day, Pool src, typename Recorder::cind_iter first, typename Recorder::cind_iter last)
  {
    if (day % param_.event_interval) return;
    for (; first != last; ++first) {
      deaths_.push_back({ day, src, *first });
    }
  }


  void Recorder::record_death(int day, Pool src, const Individual& ind)
  {
    if (day % param_.event_interval) return;
    deaths_.push_back({ day, src, ind });
  }

  void Recorder::record_mating(int day, const Individual& ind)
  {
      if (day > 190000) {
          if (day % 1) return;
          mating_.push_back({ day, ind });
      }
  }

  void Recorder::record_offspring(int day, const Individual& ind, const Individual& female, const Individual& male)
  {
    if (day % param_.event_interval) return;
    offspring_.push_back({ day, ind, female, male });
  }


  void Recorder::record_traits_begin(int day, const class Simulation& sim)
  {
    if (day % param_.trait_interval) return;
    all_traits_.push_back({ day, osrValue(sim), asrValue(sim), psrValue(sim) });
    auto& rec = all_traits_.back();
    for (auto sex : { Sex::female, Sex::male }) {
      for (size_t i = 0; i < Pool::max; ++i) {
        rec.size[sex][i] = sim.pop()[sex][i].size();
      }
    }
    if (verbose_) {
      std::cout << day << ':';
      for (auto sex : { Sex::female, Sex::male }) {
        std::cout << '\t' << SexLabel[sex] << " [  ";
        for (size_t i = 0; i < Pool::max; ++i) {
          std::cout << PoolName[i] << ' ' << sim.pop()[sex][i].size() << "  ";
          rec.size[sex][i] = sim.pop()[sex][i].size();
        }
        std::cout << ']';
      }
      std::cout << '\n';
    }
  }


  void Recorder::record_traits_end(int day, const class Simulation& sim)
  {
    if (day % param_.trait_interval) return;
    auto& rec = all_traits_.back();      // resume where record_traits_begin ended
    genome_sum gs{};   // tot. sum over alleles
    for (auto sex : { Sex::female, Sex::male }) {
      for (size_t i = 0; i < Pool::max; ++i) {
        const auto& pool = sim.pop()[sex][i];
        for (const auto& ind : pool) {
          gs += ind.genome;
        }
      }
    }
    rec.ave_genome = gs.average();
    return;
  }

}
