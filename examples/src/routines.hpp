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

static inline eilig::CallbackIterative callbackIterativeMomentum = [](Index iteration, Scalar residual) -> long long int
    {
        Scalar tolerance{ 1e-5 };

        //logger::Info("DIVE Momentum", utils::string::Format("Iteration: {}, Residual: {:.3e}", iteration, residual));

        if (std::isnan(residual))
        {
            return eilig::EILIG_NOT_CONVERGED;
        }

        if (residual < tolerance)
        {
            return eilig::EILIG_SUCCESS;
        }

        return eilig::EILIG_CONTINUE;
    };

static inline eilig::CallbackIterative callbackIterativePressure = [](Index iteration, Scalar residual) -> long long int
    {
        Scalar tolerance{ 1e-5 };

        //logger::Info("DIVE Pressure", utils::string::Format("Iteration: {}, Residual: {:.3e}", iteration, residual));

        if (std::isnan(residual))
        {
            return eilig::EILIG_NOT_CONVERGED;
        }

        if (residual < tolerance)
        {
            return eilig::EILIG_SUCCESS;
        }

        return eilig::EILIG_CONTINUE;
    };

#endif /* ROUTINES_HPP_ */