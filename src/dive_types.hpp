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
#include "value.hpp"

#include <algorithm>
#include <vector>
#include <unordered_map>
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
	using namespace eilig;
	using namespace basis;
	using namespace node;
	using namespace quadrature;
	using namespace interpolation;
	using namespace material;
	using namespace value;
	
	using NodeIndex = Index;
	using ElementIndex = Index;
	using DimensionIndex = Index;
	using FaceIndex = Index;
	using EdgeIndex = Index;
	using LoadIndex = Index;
	using CacheIndex = Index;
	using Cache = std::unordered_map<Type, Matrices>;
	
	using NumberFaces = Number;
	using NumberEdges = Number;
	using NumberElements = Number;
	using NumberDimensions = Number;
	
	using Timer = utils::Timer;

	static const String headerDive = "DIVE";

	namespace timer
	{
		class ITimer;
		using ITimerPtr = std::shared_ptr<ITimer>;

		class TimerStationary;
		using TimerStationaryPtr = std::shared_ptr<TimerStationary>;

		class TimerStepped;
		using TimerSteppedPtr = std::shared_ptr<TimerStepped>;
	}

	namespace element
	{
		class IElement;
		using IElementPtr = std::shared_ptr<IElement>;

		class IElementMapped;
		using IElementMappedPtr = std::shared_ptr<IElementMapped>;

		class IElementBeam;
		using IElementBeamPtr = std::shared_ptr<IElementBeam>;

		class IElementShell;
		using IElementShellPtr = std::shared_ptr<IElementShell>;

		class IElementSolid;
		using IElementSolidPtr = std::shared_ptr<IElementSolid>;

		class ElementHexa;
		using ElementHexaPtr = std::shared_ptr<ElementHexa>;

		class ElementMass;
		using ElementMassPtr = std::shared_ptr<ElementMass>;

		class ElementCombined;
		using ElementCombinedPtr = std::shared_ptr<ElementCombined>;

		struct FacePair;
		struct EdgePair;
	}

	namespace mesh
	{
		class IMesh;
		using IMeshPtr = std::shared_ptr<IMesh>;

		class Mesh;
		using MeshPtr = std::shared_ptr<Mesh>;
	}

	namespace weakform
	{
		class IWeakFormElement;
		using IWeakFormElementPtr = std::shared_ptr<IWeakFormElement>;

		class IWeakFormLoad;
		using IWeakFormLoadPtr = std::shared_ptr<IWeakFormLoad>;

		//---------------------------------------------------------------------
		class MassFluid;
		using MassFluidPtr = std::shared_ptr<MassFluid>;

		class StiffnessFluid;
		using StiffnessFluidPtr = std::shared_ptr<StiffnessFluid>;

		class ConvectionFluid;
		using ConvectionFluidPtr = std::shared_ptr<ConvectionFluid>;

		class StabilizationFluid;
		using StabilizationFluidPtr = std::shared_ptr<StabilizationFluid>;

		class LoadDistributedVolumeFluid;
		using LoadDistributedVolumeFluidPtr = std::shared_ptr<LoadDistributedVolumeFluid>;

		class LoadDistributedVolumeStabilizationFluid;
		using LoadDistributedVolumeStabilizationFluidPtr = std::shared_ptr<LoadDistributedVolumeStabilizationFluid>;

		//---------------------------------------------------------------------
		class MassPressure;
		using MassPressurePtr = std::shared_ptr<MassPressure>;

		class StiffnessPressure;
		using StiffnessPressurePtr = std::shared_ptr<StiffnessPressure>;

		class CrossedPressure;
		using CrossedPressurePtr = std::shared_ptr<CrossedPressure>;

		class StabilizationPressure;
		using StabilizationPressurePtr = std::shared_ptr<StabilizationPressure>;

		class LoadDistributedFaceFlowPressure;
		using LoadDistributedFaceFlowPressurePtr = std::shared_ptr<LoadDistributedFaceFlowPressure>;

		//---------------------------------------------------------------------
		class MassThermal;
		using MassThermalPtr = std::shared_ptr<MassThermal>;

		class StiffnessThermal;
		using StiffnessThermalPtr = std::shared_ptr<StiffnessThermal>;

		class ConvectionThermal;
		using ConvectionThermalPtr = std::shared_ptr<ConvectionThermal>;

		class StabilizationThermal;
		using StabilizationThermalPtr = std::shared_ptr<StabilizationThermal>;

		//---------------------------------------------------------------------
		class MassStructural;
		using MassStructuralPtr = std::shared_ptr<MassStructural>;

		class StiffnessStructural;
		using StiffnessStructuralPtr = std::shared_ptr<StiffnessStructural>;

		class LoadDistributedVolumeStructural;
		using LoadDistributedVolumeStructuralPtr = std::shared_ptr<LoadDistributedVolumeStructural>;

		class LoadDistributedFaceStructural;
		using LoadDistributedFaceStructuralPtr = std::shared_ptr<LoadDistributedFaceStructural>;

		class LoadDistributedEdgeStructural;
		using LoadDistributedEdgeStructuralPtr = std::shared_ptr<LoadDistributedEdgeStructural>;

		class LoadNodeStructural;
		using LoadNodeStructuralPtr = std::shared_ptr<LoadNodeStructural>;
		
		//---------------------------------------------------------------------	
	}

	namespace load
	{
		class ILoad;
		using ILoadPtr = std::shared_ptr<ILoad>;

		class ILoadDirichlet;
		using ILoadDirichletPtr = std::shared_ptr<ILoadDirichlet>;

		class LoadDirichlet;
		using LoadDirichletPtr = std::shared_ptr<LoadDirichlet>;

		class LoadDistributedVolume;
		using LoadDistributedVolumePtr = std::shared_ptr<LoadDistributedVolume>;

		class LoadDistributedFace;
		using LoadDistributedFacePtr = std::shared_ptr<LoadDistributedFace>;

		class LoadDistributedEdge;
		using LoadDistributedEdgePtr = std::shared_ptr<LoadDistributedEdge>;

		class LoadNode;
		using LoadNodePtr = std::shared_ptr<LoadNode>;
	}

	namespace problem
	{
		class IProblem;
		using IProblemPtr = std::shared_ptr<IProblem>;

		class IDeformation;
		using IDeformationPtr = std::shared_ptr<IDeformation>;

		class IFluid;
		using IFluidPtr = std::shared_ptr<IFluid>;

		class IPressure;
		using IPressurePtr = std::shared_ptr<IPressure>;

		class IThermal;
		using IThermalPtr = std::shared_ptr<IThermal>;

		class IStructural;
		using IStructuralPtr = std::shared_ptr<IStructural>;

		class ProblemThermal;
		using ProblemThermalPtr = std::shared_ptr<ProblemThermal>;

		class ProblemFluid;
		using ProblemFluidPtr = std::shared_ptr<ProblemFluid>;

		class ProblemPressure;
		using ProblemPressurePtr = std::shared_ptr<ProblemPressure>;

		class ProblemStructural;
		using ProblemStructuralPtr = std::shared_ptr<ProblemStructural>;

		class ProblemDeformationInterpolation;
		using ProblemDeformationInterpolationPtr = std::shared_ptr<ProblemDeformationInterpolation>;

		class ProblemDeformationStrain;
		using ProblemDeformationStrainPtr = std::shared_ptr<ProblemDeformationStrain>;
	}

	using ITimerPtr = timer::ITimerPtr;
	using IElementPtr = element::IElementPtr;
	using IElementMappedPtr = element::IElementMappedPtr;
	using IMeshPtr = mesh::IMeshPtr;
	using IWeakFormElementPtr = weakform::IWeakFormElementPtr;
	using IWeakFormLoadPtr = weakform::IWeakFormLoadPtr;
	using ILoadPtr = load::ILoadPtr;
	using ILoadDirichletPtr = load::ILoadDirichletPtr;
	using IProblemPtr = problem::IProblemPtr;

	struct FacePair
	{
		IElementPtr element{ nullptr };
		FaceIndex faceIndex{ 0 };
	};

	struct EdgePair
	{
		IElementPtr element{ nullptr };
		EdgeIndex edgeIndex{ 0 };
	};

	using Tags = std::vector<Tag>;
	using Bases = std::vector<IBasisPtr>;
	using Timers = std::vector<ITimerPtr>;
	using Values = std::unordered_map<String, IValuePtr>;
	using Nodes = std::vector<INodePtr>;
	using Elements = std::vector<IElementPtr>;
	using Loads = std::vector<ILoadPtr>;
	using Dirichlets = std::vector<ILoadDirichletPtr>;
	using Materials = std::vector<IMaterialPtr>;
	using Meshes = std::vector<IMeshPtr>;
	using Problems = std::vector<IProblemPtr>;
	using FacePairs = std::vector<FacePair>;
	using EdgePairs = std::vector<EdgePair>;
	using Properties = std::unordered_map<String, IValuePtr>;
}

#endif /* DIVE_TYPES_HPP_ */