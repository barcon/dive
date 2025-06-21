#ifndef DIVE_LOADS_HPP_
#define DIVE_LOADS_HPP_

#include "dive_types.hpp"
#include "dive_elements.hpp"

namespace dive
{
	namespace loads
	{
		static const Type load_boundary_condition			= 1;
		static const Type load_distributed_volume			= 2;
		static const Type load_distributed_face				= 3;
		static const Type load_distributed_edge				= 4;
		static const Type load_node							= 5;
		static const Type load_node_transient				= 6;
		//static const Type load_initial_strain				= 7;
		//static const Type load_temperature_strain			= 8;
		//static const Type load_heat_source_volume			= 9;
		//static const Type load_heat_conduction_face		= 10;
		//static const Type load_coupling_equation			= 11;
		
		class IBoundaryCondition
		{
		public:
			virtual ~IBoundaryCondition() = default;

			virtual Type GetType() const = 0;
			virtual INodePtr GetNode() const = 0;
			virtual DofIndex GetDofIndex() const = 0;
			virtual Scalar GetValue() const = 0;

			virtual void SetNode(INodePtr node) = 0;
			virtual void SetDofIndex(DofIndex dofIndex) = 0;
			virtual void SetValue(IScalar3DPtr value) = 0;
		};

		class ILoad
		{
		public:
			virtual ~ILoad() = default;

			virtual Type GetType() const = 0;
		};

		class ILoadDistributedVolume : public ILoad
		{
		public:
			virtual ~ILoadDistributedVolume() = default;

			virtual IElementMappedPtr GetElement() const = 0;
			virtual Vector GetValue(const Vector& point) const = 0;

			virtual void SetElement(IElementMappedPtr element) = 0;
			virtual void SetValue(IVector3DPtr value) = 0;
		};

		class ILoadDistributedFace : public ILoad
		{
		public:
			virtual ~ILoadDistributedFace() = default;

			virtual IElementMappedPtr GetElement() const = 0;
			virtual FaceIndex GetFaceIndex() const = 0;
			virtual Vector GetValue(const Vector& point) const = 0;

			virtual void SetElement(IElementMappedPtr element) = 0;
			virtual void SetFaceIndex(FaceIndex faceIndex) = 0;
			virtual void SetValue(IVector3DPtr value) = 0;
		};

		class ILoadDistributedEdge : public ILoad
		{
		public:
			virtual ~ILoadDistributedEdge() = default;

			virtual IElementMappedPtr GetElement() const = 0;
			virtual EdgeIndex GetEdgeIndex() const = 0;
			virtual Vector GetValue(const Vector& point) const = 0;

			virtual void SetElement(IElementMappedPtr element) = 0;
			virtual void SetEdgeIndex(EdgeIndex edgeIndex) = 0;
			virtual void SetValue(IVector3DPtr value) = 0;
		};

		class ILoadNode : public ILoad
		{
		public:
			virtual ~ILoadNode() = default;

			virtual INodePtr GetNode() const = 0;
			virtual Vector GetValue() const = 0;

			virtual void SetNode(INodePtr node) = 0;
			virtual void SetValue(IVector3DPtr value) = 0;
		};

		class ILoadNodeTransient : public ILoad
		{
		public:
			virtual ~ILoadNodeTransient() = default;

			virtual INodePtr GetNode() const = 0;
			virtual Vector GetValue(Scalar time) const = 0;

			virtual void SetNode(INodePtr node) = 0;
			virtual void SetValue(IVector3DTimePtr value) = 0;
		};
	} //namespace loads
} //namespace dive

#endif /* DIVE_LOADS_HPP_ */