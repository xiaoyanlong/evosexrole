#ifndef EVOROLE_EVOROLE_H_INCLUDED
#define EVOROLE_EVOROLE_H_INCLUDED

#include <stddef.h>
#include <filesystem>
#include <array>


namespace evorole {


  // pool types
  enum Pool
  {
    care = 0,   // caring
    mate = 1,   // mating
    pre = 2,    // pre-mating, recovery
    juv = 3,    // juvenile, better: non-adult - lumps infants and juveniles together
    max = 4     // guard
  };

  enum Sex
  {
    female = 0,
    male = 1
  };

  // Parameter

  // evolve-able alleles 
  struct genome_t
  {
    double P = 0.0;     // female preference
    double tau = 0.0;   // male ornamentation
    int pc1 = 20;       // female care days
    int pc2 = 20;       // male care days
  };


  // female specific parameter
  struct female_t
  {
    std::array<double, Pool::max> mu = { 0.001, 0.001, 0.001, 0.00005 };   // baseline mortality per pool
    int preT = 5;                 // days in recovery pool
    bool random_mating = true;    // random mating: no sexual selection
    double gamma = 0.003;         // degree of female mortality density dependence
    double kappa = 0.03;          // scaling factor that affects the intensity of sexual selection
    double alpha = 2.0;           // scaling factor that affects the intensity of sexual selection
  };


  // male specific parameter
  struct male_t
  {
    std::array<double, Pool::max> mu = { 0.001, 0.001, 0.001, 0.00005 };      // baseline mortality per pool
    std::array<double, Pool::max> beta = { 0.0001, 0.00001, 0.00001, 0.0 };   // ornament induced mortality per pool
    int preT = 5;                 // days in recovery pool
    double gamma = 0.003;         // degree of male mortality density dependence
  };


  // common parameters for females and males
  struct common_t
  {
    int juvT = 20;        // days in juvenile pool
    double D = 20.0;      // the care demand of offspring [days]
    double sigma = 0.0;   // synergy parameter
  };


  // Model parameters: mutation
  struct mutation_t
  {
    double m_P = 0.0;             // mutation rate for gene p (preferences)
    double sigma_P = 0.01;        // distribution parameter for p 
    double smax_P = 0.05;         // maximum mutation stepsize for p 
    double m_tau = 0.0;           // mutation rate for tau genes (ornaments)
    double sigma_tau = 0.01;      // distribution parameter for tau 
    double smax_tau = 0.1;        // maximum mutation stepsize for tau
    double m_pc = 0.005;          // mutational rate for gene pc1|2 (parental care)
  };


  // data collection
  struct recorder_t
  {
    int event_interval = 40000;   // interval ind. events (death, offspring)
    int trait_interval = 1000;    // interval all traits
  };


  struct Parameter
  {
    int N = 2000;                   // initial mate pool size 
    int tEnd = 5000001;             // simulation time ticks

    genome_t genome;
    female_t female;
    male_t male;
    common_t common;
    mutation_t mutation;
    recorder_t recorder;

    static void sanity_check(const Parameter& param);
  };


  // entry point simulation
  void run_simulation(const Parameter& param, const std::filesystem::path& out, bool verbose);

}

#endif
