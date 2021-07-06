#ifndef EVOROLE_SIMULATION_H_INCLUDED
#define EVOROLE_SIMULATION_H_INCLUDED

#include <memory>
#include <random>
#include <functional>
#include <rndutils.hpp>
#include "evorole.h"


namespace evorole {


  // individual state
  struct Individual
  {
    Individual(const Parameter& param, Sex sex);

    int pool_stay;      // count-down max days in pool
    genome_t genome;
    int matings;        // counter matings
    int offbefden;      //counter offspring before density-dependent selection
    int offspring;      // counter mature offspring
    int matingduration; // counter mating duration
    int matings_day;    // counter matings per day
    Sex sex;
  };


  using pool_t = std::vector<Individual>;         // individuals in a pool
  using hpop_t = std::array<pool_t, Pool::max>;   // half-population
  using population_t = std::array<hpop_t, 2>;     // population [0|false]: females, [1|true]: males


  class Simulation
  {
  public:
    Simulation(const Parameter& param, std::unique_ptr<class Recorder>&& recorder);
    void run();

    // recorder needs to know this lot
    const population_t& pop() const { return pop_; }
    size_t pop_size() const;

  private:
    void pool_survival(int day, Pool dst);
    void reproduction(int day);
    void pool_transition(Pool src, Pool dst, std::array<int, 2> dst_stay);

  private:
    size_t randomized_mating_pairs();
    size_t collect_random_mating_pairs();
    size_t collect_fem_sel_mating_pairs();
    Individual create_offspring(int day, Sex sex, int juv_stay, const Individual& famale, const Individual& male);
    int draw_pool_days(int max_days, double mu);

  private:
    Parameter param_;
    population_t pop_;
    rndutils::default_engine reng_;
    std::unique_ptr<class Recorder> recorder_;
  };


}

#endif

