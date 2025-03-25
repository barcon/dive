#ifndef DIVE_PROBLEMS_HPP_
#define DIVE_PROBLEMS_HPP_

#include "dive_types.hpp"
#include "dive_timers.hpp"
#include "dive_meshes.hpp"
#include "dive_weakforms.hpp"
#include "dive_loads.hpp"

namespace dive
{
	namespace problems
	{
		static const Type problem_thermal	= 1;
		static const Type problem_fluid		= 2;
		static const Type problem_pressure	= 3;
		static const Type problem_structural= 4;

		class IProblem
		{
		public:
			virtual ~IProblem() = default;

			virtual NumberDof	GetNumberDof() const = 0;
			virtual NumberDof	GetTotalDof() const = 0;
			virtual DofIndex	GetPivot() const = 0;

			virtual ITimerPtr GetTimer() const = 0;
			virtual IMeshPtr GetMesh() const = 0;
			virtual Type GetType() const = 0;
			virtual Tag	GetTag() const = 0;

			virtual const Dirichlets& GetDirichlets() const = 0;
			virtual const Loads& GetLoads() const = 0;

			virtual const DofMeshIndices& GetDofMeshIndices() const = 0;
			virtual const NodeMeshIndices& GetNodeMeshIndices() const = 0;
			virtual const DirichletMeshIndices& GetDirichletMeshIndices() const = 0;

			virtual void SetTimer(ITimerPtr timer) = 0;
			virtual void SetMesh(IMeshPtr mesh) = 0;
			virtual void SetTag(Tag tag) = 0;

			virtual void AddDirichlet(IDirichletPtr dirichlet) = 0;
			virtual void AddLoad(ILoadPtr load) = 0;
			virtual void Initialize() = 0;
			virtual void UpdateMeshValues(const Vector& u) = 0;
		};

		class IFluid : public IProblem
		{
		public:
			virtual ~IFluid() = default;

			virtual IScalar3DPtr GetTemperature() const = 0;
			virtual IScalar3DPtr GetPressure() const = 0;
			virtual IMatrix3DPtr GetVelocity() const = 0;

			virtual void SetTemperature(IScalar3DPtr temperature) = 0;
			virtual void SetPressure(IScalar3DPtr pressure) = 0;

			virtual Sparse Mass() const = 0;
			virtual Sparse Stiffness() const = 0;
			virtual Sparse Convection() const = 0;
			virtual Sparse ConvectionSymmetric() const = 0;
			virtual Sparse ConvectionAsymmetric() const = 0;
			virtual Sparse Stabilization() const = 0;
			virtual Vector LoadDistributedVolume() const = 0;
			virtual Vector LoadDistributedVolumeStabilization() const = 0;
			
			virtual Vector Momentum() const = 0;
			virtual Vector Velocity() const = 0;
			//virtual Vector Vorticity() const = 0;
			//virtual Vector Divergence() const = 0;
		};

		class IPressure : public IProblem
		{
		public:
			virtual ~IPressure() = default;

			virtual IScalar3DPtr GetTemperature() const = 0;
			virtual IScalar3DPtr GetPressure() const = 0;
			virtual IMatrix3DPtr GetVelocity() const = 0;

			virtual void SetTemperature(IScalar3DPtr temperature) = 0;
			virtual void SetVelocity(IMatrix3DPtr velocity) = 0;

			virtual Sparse Mass() const = 0;
			virtual Sparse Stiffness() const = 0;
			virtual Sparse Crossed(IProblemPtr problemMomentum) const = 0;
			virtual Sparse Stabilization(IProblemPtr problemMomentum) const = 0;
			virtual Sparse DistributedVolumeDivergence(IProblemPtr problemMomentum) const = 0;

			virtual Vector Pressure() const = 0;
		};

		class IThermal : public IProblem
		{
		public:
			virtual ~IThermal() = default;

			virtual IScalar3DPtr GetTemperature() const = 0;
			virtual IScalar3DPtr GetPressure() const = 0;
			virtual IMatrix3DPtr GetVelocity() const = 0;

			virtual void SetPressure(IScalar3DPtr pressure) = 0;
			virtual void SetVelocity(IMatrix3DPtr velocity) = 0;

			virtual Sparse Mass() const = 0;
			virtual Sparse Stiffness() const = 0;
			virtual Sparse Convection() const = 0;
			virtual Sparse Stabilization() const = 0;

			virtual Vector Energy() const = 0;
		};
	} //namespace problems
} //namespace dive

#endif /* DIVE_PROBLEMS_HPP_ */