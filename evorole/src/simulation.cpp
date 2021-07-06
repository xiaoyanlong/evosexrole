#include <chrono>
#include <algorithm>
#include "simulation.h"
#include "recorder.h"


namespace evorole {


  namespace {

    //inline void unstable_erease(pool_t& p, size_t idx)
    //{
    //  p[idx] = p.back();    // self-assignment is fine for individuals
    //  p.pop_back();
    //}


    template <typename SURVIVAL_PRED>
    void do_pool_survival
    (
      int day, int sex, Pool dst, std::unique_ptr<Recorder>& rec,
      population_t& pop, 
      SURVIVAL_PRED&& survival_pred)
    {
      auto& src = pop[sex][dst];
      auto it = std::partition(src.begin(), src.end(), survival_pred);
      rec->record_deaths(day, dst, it, src.end());
      src.erase(it, src.end());
    }


    template <typename RENG>
    inline double add_p_tau_cauchy_mutation_if(double val, double P, double sigma, double maxs, RENG& reng)
    {
      if (std::bernoulli_distribution(P)(reng)) {
        const auto s = std::clamp(std::cauchy_distribution<>(0.0, sigma)(reng), -maxs, +maxs);
        val = std::max(0.0, val + s);
      }
      return val;
    }

  }


  Individual::Individual(const Parameter& param, Sex sex) :
      pool_stay(0),
      genome(param.genome),
      matings(0),
      matingduration(0),
      offbefden(0),
      offspring(0),
      matings_day(0),
      sex(sex)
  {}


  Simulation::Simulation(const Parameter& param, std::unique_ptr<Recorder>&& recorder) :
    param_(param),
    reng_(rndutils::make_random_engine<>()),
    recorder_(std::move(recorder))
  {
    pop_[0][Pool::mate].resize(param.N / 2, Individual(param, Sex::female));
    pop_[1][Pool::mate].resize(param.N / 2, Individual(param, Sex::male));
  }


  void Simulation::run()
  {
    for (int day = 0; day < param_.tEnd; ++day)
    {
      recorder_->record_traits_begin(day, *this);
      pool_survival(day, Pool::pre);      // explicit
      pool_survival(day, Pool::mate);     // explicit
      reproduction(day);                  // depletes mate, populates care & juv
      pool_transition(Pool::care, Pool::pre, { param_.female.preT, param_.male.preT });
      pool_transition(Pool::pre, Pool::mate, { 0, 0 });    // undefined max pool_stay in mate pool
      pool_transition(Pool::juv, Pool::mate, { 0, 0 });
      recorder_->record_traits_end(day, *this);
    }
    recorder_->stream_out();
  }


  size_t Simulation::pop_size() const
  {
    size_t s = 0;
    for (auto sex : {Sex::female, Sex::male}) {
      for (const auto& pool : pop_[sex]) {
        s += pool.size();
      }
    }
    return s;
  }


  // explicit simulation of survival in dst dst  per day
  void Simulation::pool_survival(int day, Pool dst)
  {
    do_pool_survival(day, Sex::female, dst, recorder_, pop_, [&](const Individual& ind) {
      const double P = param_.female.mu[dst];
      return std::bernoulli_distribution(1.0 - P)(reng_);
    });
    do_pool_survival(day, Sex::male, dst, recorder_, pop_, [&](const Individual& ind) {
      const double P = param_.male.mu[dst] + param_.male.beta[dst] * ind.genome.tau * ind.genome.tau;
      return std::bernoulli_distribution(1.0 - P)(reng_);
    });
  }


  void Simulation::reproduction(int day)
  {
    const size_t N = pop_size();   // assumed constant over infant state
    const size_t last = randomized_mating_pairs();
    // mating pairs in [0, last)
    auto& f = pop_[Sex::female][Pool::mate];
    auto& m = pop_[Sex::male][Pool::mate];
    //if (f.size()-last > 0)              // unmated females or males have to wait for another day
    //{
    //    for (size_t i = last; i < f.size(); ++i) {
    //        ++f[i].matingduration;
    //        recorder_->record_mating(day, f[i]);
    //    }

    //}
    //if (m.size()-last > 0)
    //{
    //    for (size_t i = last; i < m.size(); ++i) {
    //        ++m[i].matingduration;
    //        recorder_->record_mating(day, m[i]);
    //    }
    //}

    const double fmu = param_.female.mu[Pool::care];
    const double mmu = param_.male.mu[Pool::care];
    const double beta = param_.male.beta[Pool::care];
    const double DD = param_.common.D * param_.common.D;
    const int juvT = param_.common.juvT;
    auto binary = rndutils::binary_distribution{};
    for (size_t i = 0; i < last; ++i) {
      ++f[i].matings;
      ++m[i].matings;
      ++f[i].matings_day;
      ++m[i].matings_day;
     /* recorder_->record_mating(day, f[i]);
      recorder_->record_mating(day, m[i]);*/
      --f[i].matings_day;
      --m[i].matings_day;
      // draw effective care days
      const double pc1 = f[i].pool_stay = draw_pool_days(f[i].genome.pc1, fmu); 
      const double pc2 = m[i].pool_stay = draw_pool_days(m[i].genome.pc2, mmu + beta * m[i].genome.tau * m[i].genome.tau);
      // offspring survival: look ahead until maturation
      auto osex = static_cast<Sex>(binary(reng_));
      const double gamma = (osex == Sex::female) ? param_.female.gamma : param_.male.gamma;
      const double T_tot = pc1 + pc2 + param_.common.sigma * pc1 * pc2;
      const double S = (T_tot * T_tot) / (T_tot * T_tot + DD);
      if (std::bernoulli_distribution(S)(reng_))
      {
          ++f[i].offbefden;
          ++m[i].offbefden;
      }
      if (std::bernoulli_distribution(S / (1.0 + gamma * N))(reng_)) {
        // will survive infant state
        const double jmu = (osex == Sex::female) ? param_.female.mu[Pool::juv] : param_.male.mu[Pool::juv];
        if (juvT == draw_pool_days(juvT, jmu)) {
          // will survive juvenile state too.
          const int pc = std::max(f[i].pool_stay, m[i].pool_stay);    // (max) parent care days
          auto offspring = create_offspring(day, osex, pc + juvT, f[i], m[i]);
          pop_[osex][Pool::juv].push_back(offspring);              // hello world
          recorder_->record_offspring(day, offspring, f[i], m[i]);
          ++f[i].offspring;
          ++m[i].offspring;
        }
      }
      else {
        // no recording of dead non-adults
      }
      // adults survive their time in the care dst if pool_stay == genome.pc1|2.
      (f[i].pool_stay == f[i].genome.pc1)
        ? pop_[Sex::female][Pool::care].push_back(f[i])
        : recorder_->record_death(day, Pool::care, f[i]);   // exact: day <- day + pc1
      (m[i].pool_stay == m[i].genome.pc2)
        ? pop_[Sex::male][Pool::care].push_back(m[i])
        : recorder_->record_death(day, Pool::care, m[i]);   // exact: day <- day + pc2
    }

    f.erase(f.begin(), f.begin() + last);
    m.erase(m.begin(), m.begin() + last);
  }


  // partitions the two mate-pools in mating/non-mating.
  // returns the index on the first non-mating pair, so that
  // the range of mating pairs is [0, ret) for both pools.
  size_t Simulation::randomized_mating_pairs()
  {
    // randomize the mating populations
    // note that std::random_shuffle is deprecated.
    std::shuffle(pop_[Sex::female][Pool::mate].begin(), pop_[Sex::female][Pool::mate].end(), reng_);
    std::shuffle(pop_[Sex::male][Pool::mate].begin(), pop_[Sex::male][Pool::mate].end(), reng_);
    return param_.female.random_mating 
         ? collect_random_mating_pairs() 
         : collect_fem_sel_mating_pairs(); 
  }

  // partitions the two mate-pools in mating/non-mating.
  // returns the index on the first non-mating pair, so that
  // the range of mating pairs is [0, ret) for both pools.
  size_t Simulation::collect_random_mating_pairs()
  {
    // now, this is easy...
    return std::min(pop_[Sex::female][Pool::mate].size(), pop_[Sex::male][Pool::mate].size());
  }


  // partitions the two mate-pools in mating/non-mating.
  // returns the index on the first non-mating pair, so that
  // the range of mating pairs is [0, ret) for both pools.
  size_t Simulation::collect_fem_sel_mating_pairs()
  {
    const double kappa = param_.female.kappa;
    const double alpha = param_.female.alpha;
    auto& f = pop_[Sex::female][Pool::mate];
    auto& m = pop_[Sex::male][Pool::mate];
    size_t res = std::min(f.size(), m.size());    // swap index + 1
    size_t i = 0;
    while (i != res) {
      const double attr = 1.0 / (1.0 + kappa * std::exp(alpha * (f[i].genome.P - m[i].genome.tau)));
      if (! std::bernoulli_distribution(attr)(reng_)) {
        // the female doesn't like what she sees. Swap the pair out of the rotation
        --res;
        std::swap(f[i], f[res]);
        std::swap(m[i], m[res]);
      }
      else {
        ++i;
      }
    }
    return res;
  }


  Individual Simulation::create_offspring(int day, Sex sex, int juv_stay, const Individual& female, const Individual& male)
  {
    const auto& mut_par = param_.mutation;
    auto binary = rndutils::binary_distribution{};
    Individual offspring{ param_, sex };
    offspring.pool_stay = juv_stay;
    auto& og = offspring.genome;
    // 50/50 inheritance of alleles
    og.P = (binary(reng_)) ? female.genome.P : male.genome.P;
    og.tau = (binary(reng_)) ? female.genome.tau : male.genome.tau;
    og.pc1 = (binary(reng_)) ? female.genome.pc1 : male.genome.pc1;
    og.pc2 = (binary(reng_)) ? female.genome.pc2 : male.genome.pc2;
    // mutation
    og.P = add_p_tau_cauchy_mutation_if(og.P, mut_par.m_P, mut_par.sigma_P, mut_par.smax_P, reng_);
    og.tau = add_p_tau_cauchy_mutation_if(og.tau, mut_par.m_tau, mut_par.sigma_tau, mut_par.smax_tau, reng_);
    auto mutation_pc = (day > 50000) ? mut_par.m_pc : 0.0;
    auto day_mut = std::bernoulli_distribution(mutation_pc);
    if (day_mut(reng_)) og.pc1 = std::max(0, og.pc1 + (2 * binary(reng_) - 1));
    if (day_mut(reng_)) og.pc2 = std::max(0, og.pc2 + (2 * binary(reng_) - 1));
    return offspring;
  }


  // moves individuals from src to dst if source-stay-time reaches 0
  // moved individuals are assigned dst_stay pool_stay days.
  void Simulation::pool_transition(Pool src_pool, Pool dst_pool, std::array<int,2> dst_stay)
  {
    for (auto sex : { Sex::female, Sex::male }) {
      auto& src = pop_[sex][src_pool];
      auto& dst = pop_[sex][dst_pool];
      // what we want to do is:
      //    std::partition(src.begin(), src.end(), [](auto& ind) { return 0 < --ind.pool_stay; })
      // however, std::partition doesn't allow mutating predicates. Thus, if we want to use
      // a single loop, we have to do this by foot. as a bonus, we can incorporate the copy.
      auto it = src.begin();
      auto last = src.end();
      while (it != last) {
        if (0 <= --(it->pool_stay)) {
          ++it;
        }
        else {
          dst.push_back(*it);
          dst.back().pool_stay = dst_stay[sex];
          std::iter_swap(it, --last);
        }
      }
      src.erase(it, src.end());   // bulk-erase from source dst, O(1)
    }
  }


  // draws the number of survived days given the per-day mortality mu.
  int Simulation::draw_pool_days(int max_days, double mu)
  {
    return std::min(max_days, std::geometric_distribution<int>(mu)(reng_));
  }

}
