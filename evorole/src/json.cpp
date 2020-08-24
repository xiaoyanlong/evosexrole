#include <stdexcept>
#include <fstream>
#include <nlohmann/json.hpp>
#include "json.h"


namespace evorole {

  // we must tell the json-library about our types
  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(genome_t, P, tau, pc1, pc2)
  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(female_t, mu, preT,  random_mating, gamma, kappa, alpha)
  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(male_t, mu, preT, beta, gamma)
  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(common_t, juvT, D, sigma)
  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(mutation_t, m_P, sigma_P, smax_P, m_tau, sigma_tau, smax_tau, m_pc)
  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(recorder_t, event_interval, trait_interval)
  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Parameter, N, tEnd, genome, female, male, common, mutation, recorder)

    
  std::vector<Parameter> ParametersFromJson(const std::filesystem::path& jfile)
  {
    std::vector<Parameter> params;
    if (!std::filesystem::exists(jfile)) {
      throw std::runtime_error("parameter file doesn't exists");
    }
    auto is = std::ifstream(jfile);
    nlohmann::json J;
    is >> J;
    if (J.is_array()) {
      for (size_t i = 0; i < J.size(); ++i) {
        params.push_back(J[i].get<Parameter>());
      }
    }
    else {
      params.push_back(J.get<Parameter>());
    }
    for (const auto& p : params) {
      Parameter::sanity_check(p);
    }
    return params;
  }


  void ParameterToJson(const Parameter& param, const std::filesystem::path& jfile)
  {
    nlohmann::json J = param;
    auto os = std::ofstream(jfile);
    os << J;
  }

}
