#ifndef ROUTINES_HPP_
#define ROUTINES_HPP_

#include "dive.hpp"
#include "gmsh.h"

#include <nlohmann/json.hpp>
#include <unordered_map>

using namespace dive;
using Json = nlohmann::json;

void SaveToFile(const String& fileName, const Nodes& nodes);
void InitializeGmsh(const String& meshFile);
void FinalizeGmsh();

problem::ProblemFluidPtr CreateProblemFluid(Tag problemTag, const String problemName, const Values& loads);
problem::ProblemThermalPtr CreateProblemThermal(Tag problemTag, const String problemName, const Values& loads);
problem::ProblemPressurePtr CreateProblemPressure(Tag problemTag, const String problemName, const Values& loads);
problem::ProblemStructuralPtr CreateProblemStructural(Tag problemTag, const String problemName, const Values& loads);
problem::ProblemDeformationInterpolationPtr CreateProblemDeformationInterpolation(Tag problemTag, const String problemName, const Values& loads);
problem::ProblemDeformationStrainPtr CreateProblemDeformationStrain(Tag problemTag, const String problemName, const Values& loads);


#endif /* ROUTINES_HPP_ */