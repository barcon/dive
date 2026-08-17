#ifndef ROUTINES_HPP_
#define ROUTINES_HPP_

#include "utils.hpp"
#include "basis.hpp"
#include "material.hpp"
#include "dive.hpp"
#include "gmsh.h"

#include <nlohmann/json.hpp>
#include <unordered_map>

using Json = nlohmann::json;

void InitializeGmsh(const dive::String& meshFile);
void FinalizeGmsh();

dive::problem::ProblemFluidPtr CreateProblemFluid(dive::Tag problemTag, const dive::String problemName, const dive::Values& loads);
dive::problem::ProblemThermalPtr CreateProblemThermal(dive::Tag problemTag, const dive::String problemName, const dive::Values& loads);
dive::problem::ProblemPressurePtr CreateProblemPressure(dive::Tag problemTag, const dive::String problemName, const dive::Values& loads);
dive::problem::ProblemStructuralPtr CreateProblemStructural(dive::Tag problemTag, const dive::String problemName, const dive::Values& loads);
dive::problem::ProblemDeformationLaplacePtr CreateProblemDeformationLaplace(dive::Tag problemTag, const dive::String problemName, const dive::Values& loads);
dive::problem::ProblemDeformationInterpolationPtr CreateProblemDeformationInterpolation(dive::Tag problemTag, const dive::String problemName, const dive::Values& loads);

#endif /* GRADIENT_ROUTINES_HPP_ */