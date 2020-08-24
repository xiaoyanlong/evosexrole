#ifndef EVOROLE_JSON_H_INCLUDED
#define EVOROLE_JSON_H_INCLUDED

#include <filesystem>
#include <vector>
#include "evorole.h"


namespace evorole {

  std::vector<Parameter> ParametersFromJson(const std::filesystem::path& jfile);
  void ParameterToJson(const Parameter& param, const std::filesystem::path& jfile);

}

#endif
