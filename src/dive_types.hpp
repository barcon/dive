#ifndef DIVE_TYPES_HPP_
#define DIVE_TYPES_HPP_

/*
#if _WIN32 || _WIN64
#ifdef DLL_EXPORT
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#define DLL_CALL __cdecl
#else
#define DLL_API
#define DLL_CALL
#endif
*/

#include "utils.hpp"
#include "logger.hpp"
#include "eilig.hpp"
#include "basis.hpp"
#include "nodes.hpp"
#include "material.hpp"
#include "quadrature.hpp"
#include "interpolation.hpp"
#include "values.hpp"

#include <algorithm>
#include <vector>
#include <unordered_map>
#include <map>
#include <memory>
#include <cstdint>
#include <functional>
#include <cmath>
#include <limits>
#include <variant>

#ifdef NOTIMER
	#define TimerStart() ((void)0)
	#define TimerElapsed(function) ((void)0)
#else
	#define TimerStart() Timer timer
	#define TimerElapsed(function) logger::Info(headerDive, "Elapsed time = %.1f (%s)", timer.Elapsed(), function)
#endif

namespace dive
{
	using Axis = eilig::Axis;
	using Scalar = eilig::Scalar;
	using Vector = eilig::Vector;
	using Matrix = eilig::Matrix;
	using Sparse = eilig::Ellpack;

	using Scalars = std::vector<Scalar>;
	using Vectors = std::vector<Vector>;
	using Matrices = std::vector<Matrix>;
	using Sparses = std::vector<Sparse>;	

	using Status = long long int;
	using Tag = std::size_t;
	using Type = std::size_t;

	using Dimension = std::size_t;
	using Index = std::size_t;
	using DofIndex = Index;
	using NodeIndex = Index;
	using ElementIndex = Index;
	using FaceIndex = Index;
	using EdgeIndex = Index;
	using DirichletIndex = Index;

	using Cache = std::unordered_map<Type, Matrices>;
	using CacheIndex = Index;

	using Number = std::size_t;
	using NumberDof = Number;
	using NumberElements = Number;
	using NumberNodes = Number;
	using NumberFaces = Number;
	using NumberEdges = Number;
	using NumberElements = Number;

	using Timer = utils::Timer;

	using String = utils::String;
	static const String headerDive = "DIVE";

	using Function_Pointer_D_D	= values::Function_Pointer_D_D;
	using Function_Pointer_D_DD	= values::Function_Pointer_D_DD;
	using Function_Pointer_D_DDD= values::Function_Pointer_D_DDD;

	namespace timers
	{
		class ITimer;
		using ITimerPtr = std::shared_ptr<ITimer>;
		using ConstITimerPtr = std::shared_ptr<const ITimer>;

		class TimerStationary;
		using TimerStationaryPtr = std::shared_ptr<TimerStationary>;
		using ConstTimerStationaryPtr = std::shared_ptr<const TimerStationary>;

		class TimerStepped;
		using TimerSteppedPtr = std::shared_ptr<TimerStepped>;
		using ConstTimerSteppedPtr = std::shared_ptr<const TimerStepped>;
	}

	namespace elements
	{
		class IElement;
		using IElementPtr = std::shared_ptr<IElement>;
		using ConstIBasePtr = std::shared_ptr<const IElement>;

		class ElementHexa;
		using ElementHexaPtr = std::shared_ptr<ElementHexa>;
		using ConstElementHexaPtr = std::shared_ptr<const ElementHexa>;
	}

	namespace meshes
	{
		class IMesh;
		using IMeshPtr = std::shared_ptr<IMesh>;
		using ConstIMeshPtr = std::shared_ptr<const IMesh>;

		class Mesh;
		using MeshPtr = std::shared_ptr<Mesh>;
		using ConstMeshPtr = std::shared_ptr<const Mesh>;
	}

	namespace weakforms
	{
		class IWeakFormElement;
		using IWeakFormElementPtr = std::shared_ptr<IWeakFormElement>;
		using ConstIWeakFormElementPtr = std::shared_ptr<const IWeakFormElement>;

		class IWeakFormLoad;
		using IWeakFormLoadPtr = std::shared_ptr<IWeakFormLoad>;
		using ConstIWeakFormLoadPtr = std::shared_ptr<const IWeakFormLoad>;

		class WeakFormElement;
		using WeakFormElementPtr = std::shared_ptr<WeakFormElement>;
		using ConstWeakFormElementPtr = std::shared_ptr<const WeakFormElement>;

		class WeakFormLoad;
		using WeakFormLoadPtr = std::shared_ptr<WeakFormLoad>;
		using ConstWeakFormLoadPtr = std::shared_ptr<const WeakFormLoad>;

		//---------------------------------------------------------------------
		class MassFluid;
		using MassFluidPtr = std::shared_ptr<MassFluid>;
		using ConstMassFluidPtr = std::shared_ptr<const MassFluid>;

		class StiffnessFluid;
		using StiffnessFluidPtr = std::shared_ptr<StiffnessFluid>;
		using ConstStiffnessFluidPtr = std::shared_ptr<const StiffnessFluid>;

		class ConvectionFluid;
		using ConvectionFluidPtr = std::shared_ptr<ConvectionFluid>;
		using ConstConvectionFluidPtr = std::shared_ptr<const ConvectionFluid>;

		class ConvectionSymmetricFluid;
		using ConvectionSymmetricFluidPtr = std::shared_ptr<ConvectionSymmetricFluid>;
		using ConstConvectionSymmetricFluidPtr = std::shared_ptr<const ConvectionSymmetricFluid>;

		class ConvectionAsymmetricFluid;
		using ConvectionAsymmetricFluidPtr = std::shared_ptr<ConvectionAsymmetricFluid>;
		using ConstConvectionAsymmetricFluidPtr = std::shared_ptr<const ConvectionAsymmetricFluid>;

		class StabilizationFluid;
		using StabilizationFluidPtr = std::shared_ptr<StabilizationFluid>;
		using ConstStabilizationFluidPtr = std::shared_ptr<const StabilizationFluid>;

		class LoadDistributedVolumeFluid;
		using LoadDistributedVolumeFluidPtr = std::shared_ptr<LoadDistributedVolumeFluid>;
		using ConstLoadDistributedVolumeFluidPtr = std::shared_ptr<const LoadDistributedVolumeFluid>;

		class LoadDistributedVolumeStabilizationFluid;
		using LoadDistributedVolumeStabilizationFluidPtr = std::shared_ptr<LoadDistributedVolumeStabilizationFluid>;
		using ConstLoadDistributedVolumeStabilizationFluidPtr = std::shared_ptr<const LoadDistributedVolumeStabilizationFluid>;

		//---------------------------------------------------------------------
		class MassPressure;
		using MassPressurePtr = std::shared_ptr<MassPressure>;
		using ConstMassPressurePtr = std::shared_ptr<const MassPressure>;

		class StiffnessPressure;
		using StiffnessPressurePtr = std::shared_ptr<StiffnessPressure>;
		using ConstStiffnessPressurePtr = std::shared_ptr<const StiffnessPressure>;

		class CrossedPressure;
		using CrossedPressurePtr = std::shared_ptr<CrossedPressure>;
		using ConstCrossedPressurePtr = std::shared_ptr<const CrossedPressure>;

		class StabilizationPressure;
		using StabilizationPressurePtr = std::shared_ptr<StabilizationPressure>;
		using ConstStabilizationPressurePtr = std::shared_ptr<const StabilizationPressure>;

		class DistributedVolumeDivergencePressure;
		using DistributedVolumeDivergencePressurePtr = std::shared_ptr<DistributedVolumeDivergencePressure>;
		using ConstDistributedVolumeDivergencePressurePtr = std::shared_ptr<const DistributedVolumeDivergencePressure>;

		//---------------------------------------------------------------------
		class MassThermal;
		using MassThermalPtr = std::shared_ptr<MassThermal>;
		using ConstMassThermalPtr = std::shared_ptr<const MassThermal>;

		class StiffnessThermal;
		using StiffnessThermalPtr = std::shared_ptr<StiffnessThermal>;
		using ConstStiffnessThermalPtr = std::shared_ptr<const StiffnessThermal>;

		class ConvectionThermal;
		using ConvectionThermalPtr = std::shared_ptr<ConvectionThermal>;
		using ConstConvectionThermalPtr = std::shared_ptr<const ConvectionThermal>;

		class StabilizationThermal;
		using StabilizationThermalPtr = std::shared_ptr<StabilizationThermal>;
		using ConstStabilizationThermalPtr = std::shared_ptr<const StabilizationThermal>;

		//---------------------------------------------------------------------
		class MassStructural;
		using MassStructuralPtr = std::shared_ptr<MassStructural>;
		using ConstMassStructuralPtr = std::shared_ptr<const MassStructural>;

		class StiffnessStructural;
		using StiffnessStructuralPtr = std::shared_ptr<StiffnessStructural>;
		using ConstStiffnessStructuralPtr = std::shared_ptr<const StiffnessStructural>;

		class LoadDistributedVolumeStructural;
		using LoadDistributedVolumeStructuralPtr = std::shared_ptr<LoadDistributedVolumeStructural>;
		using ConstLoadDistributedVolumeStructuralPtr = std::shared_ptr<const LoadDistributedVolumeStructural>;

		class LoadDistributedFaceStructural;
		using LoadDistributedFaceStructuralPtr = std::shared_ptr<LoadDistributedFaceStructural>;
		using ConstLoadDistributedFaceStructuralPtr = std::shared_ptr<const LoadDistributedFaceStructural>;

		class LoadDistributedEdgeStructural;
		using LoadDistributedEdgeStructuralPtr = std::shared_ptr<LoadDistributedEdgeStructural>;
		using ConstLoadDistributedEdgeStructuralPtr = std::shared_ptr<const LoadDistributedEdgeStructural>;
	}

	namespace loads
	{
		class IDirichlet;
		using IDirichletPtr = std::shared_ptr<IDirichlet>;
		using ConstIDirichletPtr = std::shared_ptr<const IDirichlet>;

		class Dirichlet;
		using DirichletPtr = std::shared_ptr<Dirichlet>;
		using ConstDirichletPtr = std::shared_ptr<const Dirichlet>;

		class DirichletFunction;
		using DirichletFunctionPtr = std::shared_ptr<DirichletFunction>;
		using ConstDirichletFunctionPtr = std::shared_ptr<const DirichletFunction>;

		class INeumann;
		using INeumannPtr = std::shared_ptr<INeumann>;
		using ConstINeumannPtr = std::shared_ptr<const INeumann>;

		class NeumannHeatConductionFace;
		using NeumannHeatConductionFacePtr = std::shared_ptr<NeumannHeatConductionFace>;
		using ConstNeumannHeatConductionFacePtr = std::shared_ptr<const NeumannHeatConductionFace>;

		class ILoad;
		using ILoadPtr = std::shared_ptr<ILoad>;
		using ConstIBoundaryPtr = std::shared_ptr<const ILoad>;

		class LoadDistributedVolume;
		using LoadDistributedVolumePtr = std::shared_ptr<LoadDistributedVolume>;
		using ConstLoadDistributedVolumePtr = std::shared_ptr<const LoadDistributedVolume>;

		class LoadDistributedFace;
		using LoadDistributedFacePtr = std::shared_ptr<LoadDistributedFace>;
		using ConstLoadDistributedFacePtr = std::shared_ptr<const LoadDistributedFace>;

		class LoadDistributedEdge;
		using LoadDistributedEdgePtr = std::shared_ptr<LoadDistributedEdge>;
		using ConstLoadDistributedEdgePtr = std::shared_ptr<const LoadDistributedEdge>;
	}

	namespace problems
	{
		class IProblem;
		using IProblemPtr = std::shared_ptr<IProblem>;
		using ConstIProblemPtr = std::shared_ptr<const IProblem>;

		class Problem;
		using ProblemPtr = std::shared_ptr<Problem>;
		using ConstProblemPtr = std::shared_ptr<const Problem>;

		class ProblemThermal;
		using ProblemThermalPtr = std::shared_ptr<ProblemThermal>;
		using ConstProblemThermalPtr = std::shared_ptr<const ProblemThermal>;

		class ProblemFluid;
		using ProblemFluidPtr = std::shared_ptr<ProblemFluid>;
		using ConstProblemFluidPtr = std::shared_ptr<const ProblemFluid>;

		class ProblemPressure;
		using ProblemPressurePtr = std::shared_ptr<ProblemPressure>;
		using ConstProblemPressurePtr = std::shared_ptr<const ProblemPressure>;

		class ProblemStructural;
		using ProblemStructuralPtr = std::shared_ptr<ProblemStructural>;
		using ConstProblemStructuralPtr = std::shared_ptr<const ProblemStructural>;
	}

	using ITimerPtr = timers::ITimerPtr;
	using IBasisPtr = basis::IBasisPtr;

	using IValuePtr = values::IValuePtr;
	using IScalarPtr = values::IScalarPtr;
	using IScalar1DPtr = values::IScalar1DPtr;
	using IScalar2DPtr = values::IScalar2DPtr;
	using IScalar3DPtr = values::IScalar3DPtr;
	using IVector3DPtr = values::IVector3DPtr;
	using IMatrix3DPtr = values::IMatrix3DPtr;
	using IStringPtr = values::IStringPtr;

	using INodePtr = nodes::INodePtr;
	using IElementPtr = elements::IElementPtr;
	using IMeshPtr = meshes::IMeshPtr;
	using IWeakFormElementPtr = weakforms::IWeakFormElementPtr;
	using IWeakFormLoadPtr = weakforms::IWeakFormLoadPtr;
	using IMaterialPtr = material::IMaterialPtr;
	using IGaussPtr = quadrature::IGaussPtr;
	using IInterpolationPtr = interpolation::IInterpolationPtr;
	using IDirichletPtr = loads::IDirichletPtr;
	using INeumannPtr = loads::INeumannPtr;
	using ILoadPtr = loads::ILoadPtr;
	using IProblemPtr = problems::IProblemPtr;

	using Tags = std::vector<Tag>;
	using Indices = std::vector<Index>;
	using Strings = utils::Strings;

	using Bases = std::vector<IBasisPtr>;
	using Timers = std::vector<ITimerPtr>;
	using Values = std::vector<IValuePtr>;
	using Nodes = std::vector<INodePtr>;
	using Elements = std::vector<IElementPtr>;
	using Dirichlets = std::vector<IDirichletPtr>;
	using Neumanns = std::vector<INeumannPtr>;
	using Loads = std::vector<ILoadPtr>;
	using Materials = std::vector<IMaterialPtr>;
	using Meshes = std::vector<IMeshPtr>;
	using Problems = std::vector<IProblemPtr>;
}

#endif