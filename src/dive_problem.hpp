#ifndef DIVE_PROBLEM_HPP_
#define DIVE_PROBLEM_HPP_

#include "dive_types.hpp"
#include "dive_timer.hpp"
#include "dive_mesh.hpp"
#include "dive_weakform.hpp"
#include "dive_load.hpp"

namespace dive
{
	namespace problem
	{
		static const Type problem_fluid			= 1;
		static const Type problem_pressure		= 2;
		static const Type problem_thermal		= 3;
		static const Type problem_structural	= 4;
		static const Type problem_deformation	= 5;

		Sparse IntegralForm(IWeakFormElementPtr weakForm, IProblemPtr problem1, IProblemPtr problem2);
		Vector IntegralForm(IWeakFormLoadPtr weakForm, IProblemPtr problem1, const Loads& loads);
		
		template <typename T>
		std::enable_if_t<std::is_same_v<T, Vectors>, T> Partition(const typename T::value_type& vector, NumberDof totalDof, DofIndex pivot)
		{
			auto v0 = vector.Region(0, pivot - 1);
			auto v1 = vector.Region(pivot, totalDof - 1);

			return { v0 , v1 };
		}

		template <typename T>
		std::enable_if_t<std::is_same_v<T, Sparses>, T> Partition(const typename T::value_type& matrix, NumberDof totalDof, DofIndex pivot)
		{
			auto m00 = matrix.Region(0, 0, pivot - 1, pivot - 1);
			auto m01 = matrix.Region(0, pivot, pivot - 1, totalDof - 1);
			auto m10 = matrix.Region(pivot, 0, totalDof - 1, pivot - 1);
			auto m11 = matrix.Region(pivot, pivot, totalDof - 1, totalDof - 1);

			return { m00, m01, m10, m11 };
		}

		void UpdateMeshElements(IMeshPtr mesh, NumberDof numberDof);
		void UpdateDofMeshIndices(IMeshPtr mesh, NumberDof& totalDof, DofMeshIndices& dofMeshIndices);
		void UpdateNodeMeshIndices(IMeshPtr mesh, const DofMeshIndices& dofMeshIndices, NodeMeshIndices& nodeMeshIndices);
		void UpdateDirichletIndices(const Dirichlets& dirichlets, DofIndex& pivot, DofMeshIndices& dofMeshIndices, DirichletMeshIndices& dirichletMeshIndices);
		void UpdateDirichletLoads(const DofMeshIndices& dofMeshIndices);
		void Reorder(IMeshPtr mesh, NumberDof numberDof, NodeMeshIndices& nodeMeshIndices, DofMeshIndices& dofMeshIndices);
		void SortDofMeshIndices(DofMeshIndices& dofMeshIndices);

		class IProblem
		{
		public:
			virtual ~IProblem() = default;

			virtual NumberDof GetNumberDof() const = 0;
			virtual IMeshPtr GetMesh() const = 0;
			virtual Type GetType() const = 0;
			virtual Tag	GetTag() const = 0;

			virtual Loads& GetLoads() = 0;
	
			virtual void SetTag(Tag tag) = 0;
			virtual void ApplyLoad(ILoadPtr load) = 0;

		protected:
			virtual void SetMesh(IMeshPtr mesh) = 0;
		};

		class IExtra
		{
		public:
			virtual ~IExtra() = default;

			virtual NumberDof GetTotalDof() const = 0;
			virtual DofIndex GetPivot() const = 0;

			virtual const DofMeshIndices& GetDofMeshIndices() const = 0;
			virtual const NodeMeshIndices& GetNodeMeshIndices() const = 0;
			virtual const DirichletMeshIndices& GetDirichletMeshIndices() const = 0;

			virtual void Initialize() = 0;
		};

		class IDeformation : public IProblem
		{
		public:
			virtual ~IDeformation() = default;

			virtual void DeformMesh(IMeshPtr mesh) = 0;
		};

		class IDeformationLaplace : public IDeformation, public IExtra
		{
		public:
			virtual ~IDeformationLaplace() = default;
			
			virtual void SetTemperature(IScalarCoordinatesPtr temperature) = 0;
			virtual void SetPressure(IScalarCoordinatesPtr pressure) = 0;

			virtual void UpdateMeshValues(const Vector& u) = 0;
			virtual void UpdateMeshValues(const Vector& u0, const Vector& u1) = 0;

			virtual Sparse Stiffness() const = 0;
			virtual Vector Displacement() const = 0;
		};

		class IFluid : public IProblem, public IExtra
		{
		public:
			virtual ~IFluid() = default;

			virtual IScalarCoordinatesPtr GetTemperature() const = 0;
			virtual IScalarCoordinatesPtr GetPressure() const = 0;
			virtual IMatrixCoordinatesPtr GetVelocity() const = 0;

			virtual void SetTemperature(IScalarCoordinatesPtr temperature) = 0;
			virtual void SetPressure(IScalarCoordinatesPtr pressure) = 0;
			
			virtual void UpdateMeshValues(const Vector& u) = 0;
			virtual void UpdateMeshValues(const Vector& u0, const Vector& u1) = 0;
			virtual void UpdateMeshValuesMomentum(const Vector& q) = 0;
			virtual void UpdateMeshValuesMomentum(const Vector& q0, const Vector& q1) = 0;

			virtual Sparse Mass(bool lumped = false) const = 0;
			virtual Sparse Stiffness() const = 0;
			virtual Sparse Convection() const = 0;
			virtual Sparse Stabilization() const = 0;		
			virtual Vector LoadDistributedVolume() const = 0;
			virtual Vector LoadDistributedVolumeStabilization() const = 0;	
			virtual Vector Momentum() const = 0;
			virtual Vector Velocity() const = 0;
		};

		class IPressure : public IProblem, public IExtra
		{
		public:
			virtual ~IPressure() = default;

			virtual IScalarCoordinatesPtr GetTemperature() const = 0;
			virtual IScalarCoordinatesPtr GetPressure() const = 0;

			virtual void SetTemperature(IScalarCoordinatesPtr temperature) = 0;

			virtual void UpdateMeshValues(const Vector& u) = 0;
			virtual void UpdateMeshValues(const Vector& u0, const Vector& u1) = 0;

			virtual Sparse Mass(IFluidPtr problemMomentum, bool lumped = false) const = 0;
			virtual Sparse Stiffness() const = 0;
			virtual Sparse Crossed(IProblemPtr problemMomentum) const = 0;
			virtual Sparse Stabilization(IProblemPtr problemMomentum) const = 0;
			virtual Sparse DistributedVolumeDivergence(IProblemPtr problemMomentum) const = 0;
			virtual Vector Pressure() const = 0;
		};

		class IStructural : public IProblem, public IExtra
		{
		public:
			virtual ~IStructural() = default;

			virtual IScalarCoordinatesPtr GetTemperature() const = 0;
			virtual IScalarCoordinatesPtr GetPressure() const = 0;

			virtual void SetTemperature(IScalarCoordinatesPtr temperature) = 0;
			virtual void SetPressure(IScalarCoordinatesPtr pressure) = 0;

			virtual void UpdateMeshValues(const Vector& u) = 0;
			virtual void UpdateMeshValues(const Vector& u0, const Vector& u1) = 0;

			virtual Sparse Mass(bool lumped = false) const = 0;
			virtual Sparse Stiffness() const = 0;
			virtual Sparse Damping() const = 0;
			virtual Vector LoadDistributedEdge() const = 0;
			virtual Vector LoadDistributedFace() const = 0;
			virtual Vector LoadDistributedVolume() const = 0;
			virtual Vector LoadNode() const = 0;
			virtual Vector Displacement() const = 0;
		};

		class IThermal : public IProblem, public IExtra
		{
		public:
			virtual ~IThermal() = default;

			virtual IScalarCoordinatesPtr GetTemperature() const = 0;
			virtual IScalarCoordinatesPtr GetPressure() const = 0;

			virtual void SetPressure(IScalarCoordinatesPtr pressure) = 0;

			virtual void UpdateMeshValues(const Vector& u) = 0;
			virtual void UpdateMeshValues(const Vector& u0, const Vector& u1) = 0;

			virtual Sparse Mass(bool lumped = false) const = 0;
			virtual Sparse Stiffness() const = 0;
			virtual Sparse Convection(IProblemPtr problemMomentum) const = 0;
			virtual Sparse Stabilization(IProblemPtr problemMomentum) const = 0;
			virtual Vector Energy() const = 0;
		};
	} //namespace problem
} //namespace dive

#endif /* DIVE_PROBLEM_HPP_ */