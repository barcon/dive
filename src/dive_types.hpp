#ifndef DIVE_TYPES_HPP_
#define DIVE_TYPES_HPP_

#include "utils.hpp"
#include "logger.hpp"
#include "eilig.hpp"
#include "basis.hpp"
#include "node.hpp"
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
#include <utility>

#define TIMER

#ifdef NOTIMER
	#define TimerStart() ((void)0)
	#define TimerElapsed(function) ((void)0)
#else
	#define TimerStart() Timer timer
	#define TimerElapsed(function) logger::Info(headerDive, utils::string::Format("Elapsed time = {:.1f} ({})", timer.Elapsed(), function))
#endif

namespace dive
{
	using Axis = eilig::Axis;
	using Scalar = eilig::Scalar;
	using Vector = eilig::Vector;
	using Matrix = eilig::Matrix;
	using Sparse = eilig::Ellpack;

#ifdef EILIG_ENABLE_OPENCL
	using KernelsPtr = eilig::opencl::KernelsPtr;
	using VectorCL = eilig::opencl::Vector;
	using SparseCL = eilig::opencl::Ellpack;
#endif

	using Scalars = std::vector<Scalar>;
	using Vectors = std::vector<Vector>;
	using Matrices = std::vector<Matrix>;
	
	using Status = long long int;
	using Tag = std::size_t;
	using Type = std::size_t;

	using Dimension = std::size_t;
	using Coordinate = std::size_t;
	using Index = std::size_t;
	using DofIndex = Index;
	using NodeIndex = Index;
	using ElementIndex = Index;
	using FaceIndex = Index;
	using EdgeIndex = Index;
	using LoadIndex = Index;
	using DimensionIndex = Index;
	using CoordinateIndex = Index;
	using CacheIndex = Index;

	using Timer = utils::Timer;
	using Cache = std::unordered_map<Type, Matrices>;

	using Number = std::size_t;
	using NumberDof = Number;
	using NumberNodes = Number;
	using NumberFaces = Number;
	using NumberEdges = Number;
	using NumberElements = Number;
	using NumberDimensions = Number;
	using NumberCoordinates = Number;

	using String = utils::String;
	static const String headerDive = "DIVE";

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
		using ConstIElementPtr = std::shared_ptr<const IElement>;

		class IElementMapped;
		using IElementMappedPtr = std::shared_ptr<IElementMapped>;
		using ConstIElementMappedPtr = std::shared_ptr<const IElementMapped>;

		class IElementBeam;
		using IElementBeamPtr = std::shared_ptr<IElementBeam>;
		using ConstIElementBeamPtr = std::shared_ptr<const IElementBeam>;

		class IElementShell;
		using IElementShellPtr = std::shared_ptr<IElementShell>;
		using ConstIElementShellPtr = std::shared_ptr<const IElementShell>;

		class IElementSolid;
		using IElementSolidPtr = std::shared_ptr<IElementSolid>;
		using ConstIElementSolidPtr = std::shared_ptr<const IElementSolid>;

		class ElementHexa;
		using ElementHexaPtr = std::shared_ptr<ElementHexa>;
		using ConstElementHexaPtr = std::shared_ptr<const ElementHexa>;

		class ElementMass;
		using ElementMassPtr = std::shared_ptr<ElementMass>;
		using ConstElementMassPtr = std::shared_ptr<const ElementMass>;

		class ElementCombined;
		using ElementCombinedPtr = std::shared_ptr<ElementCombined>;
		using ConstElementCombinedPtr = std::shared_ptr<const ElementCombined>;

		struct FacePair;
		struct EdgePair;
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

		class LoadNodeStructural;
		using LoadNodeStructuralPtr = std::shared_ptr<LoadNodeStructural>;
		using ConstLoadNodeStructuralPtr = std::shared_ptr<const LoadNodeStructural>;
	}

	namespace loads
	{
		class ILoad;
		using ILoadPtr = std::shared_ptr<ILoad>;
		using ConstIBoundaryPtr = std::shared_ptr<const ILoad>;

		class ILoadDirichlet;
		using ILoadDirichletPtr = std::shared_ptr<ILoadDirichlet>;
		using ConstILoadDirichletPtr = std::shared_ptr<const ILoadDirichlet>;

		class LoadDirichlet;
		using LoadDirichletPtr = std::shared_ptr<LoadDirichlet>;
		using ConstLoadDirichletPtr = std::shared_ptr<const LoadDirichlet>;

		class LoadDistributedVolume;
		using LoadDistributedVolumePtr = std::shared_ptr<LoadDistributedVolume>;
		using ConstLoadDistributedVolumePtr = std::shared_ptr<const LoadDistributedVolume>;

		class LoadDistributedFace;
		using LoadDistributedFacePtr = std::shared_ptr<LoadDistributedFace>;
		using ConstLoadDistributedFacePtr = std::shared_ptr<const LoadDistributedFace>;

		class LoadDistributedEdge;
		using LoadDistributedEdgePtr = std::shared_ptr<LoadDistributedEdge>;
		using ConstLoadDistributedEdgePtr = std::shared_ptr<const LoadDistributedEdge>;

		class LoadNode;
		using LoadNodePtr = std::shared_ptr<LoadNode>;
		using ConstLoadNodePtr = std::shared_ptr<const LoadNode>;
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

	using IValuePtr = values::IValuePtr;
	using IBoolPtr = values::IBoolPtr;
	using IStringPtr = values::IStringPtr;
	
	using IScalarPtr = values::IScalarPtr;
	using IScalar1DPtr = values::IScalar1DPtr;
	using IScalar2DPtr = values::IScalar2DPtr;
	using IScalar3DPtr = values::IScalar3DPtr;
	
	using IVectorPtr = values::IVectorPtr;
	using IVector1DPtr = values::IVector1DPtr;
	using IVector2DPtr = values::IVector2DPtr;
	using IVector3DPtr = values::IVector3DPtr;
	
	using IMatrixPtr = values::IMatrixPtr;
	using IMatrix1DPtr = values::IMatrix1DPtr;
	using IMatrix2DPtr = values::IMatrix2DPtr;
	using IMatrix3DPtr = values::IMatrix3DPtr;
	
	using ITimerPtr = timers::ITimerPtr;
	using IBasisPtr = basis::IBasisPtr;
	using INodePtr = node::INodePtr;
	using IElementPtr = elements::IElementPtr;
	using IElementMappedPtr = elements::IElementMappedPtr;
	using IMeshPtr = meshes::IMeshPtr;
	using IWeakFormElementPtr = weakforms::IWeakFormElementPtr;
	using IWeakFormLoadPtr = weakforms::IWeakFormLoadPtr;
	using IMaterialPtr = material::IMaterialPtr;
	using IGaussPtr = quadrature::IGaussPtr;
	using IInterpolationPtr = interpolation::IInterpolationPtr;
	using ILoadPtr = loads::ILoadPtr;
	using ILoadDirichletPtr = loads::ILoadDirichletPtr;
	using IProblemPtr = problems::IProblemPtr;

	using FacePair = elements::FacePair;
	using EdgePair = elements::EdgePair;

	using Tags = std::vector<Tag>;
	using Indices = std::vector<Index>;
	using Strings = utils::Strings;
	using Bases = std::vector<IBasisPtr>;
	using Timers = std::vector<ITimerPtr>;
	using Values = std::vector<IValuePtr>;
	using Nodes = std::vector<INodePtr>;
	using Elements = std::vector<IElementPtr>;
	using Loads = std::vector<ILoadPtr>;
	using Dirichlets = std::vector<ILoadDirichletPtr>;
	using Materials = std::vector<IMaterialPtr>;
	using Meshes = std::vector<IMeshPtr>;
	using Problems = std::vector<IProblemPtr>;
	using Properties = std::unordered_map<String, IValuePtr>;
	using FacePairs = std::vector<FacePair>;
	using EdgePairs = std::vector<EdgePair>;

	using BoundaryConditions = std::map<String, Loads>;
}

#endif /* DIVE_TYPES_HPP_ */