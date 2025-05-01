#ifndef DIVE_ELEMENTS_HPP_
#define DIVE_ELEMENTS_HPP_

#include "dive_types.hpp"
#include "dive_timers.hpp"

#include <set>

namespace dive
{
	namespace elements
	{
		using ShapePtr = Scalar(*)(const Vector&);

		using Parametric = std::size_t;
		static const Parametric parametric_linear = 1;
		static const Parametric parametric_quadratic = 2;

		using Order = std::size_t;
		static const Order order_linear = 1;
		static const Order order_quadratic = 2;

		static const Type element_mass = 1;
		static const Type element_spring = 2;
		static const Type element_hexa8 = 3;
		static const Type element_hexa20 = 4;

		struct IntegralAreaHelper
		{
			Dimension dim1;
			Dimension dim2;
			Dimension dim3;

			Scalar coord3;
		};

		struct IntegralEdgeHelper
		{
			Dimension dim1;
			Dimension dim2;
			Dimension dim3;

			Scalar coord2;
			Scalar coord3;
		};

		static const NodeIndex nodeIndexInvalid = 0xFFFFFFFF;

		class IShape
		{
		public:
			virtual ~IShape() = default;

			virtual Matrix u() const = 0;
			virtual Matrix u(const Vector& local) const = 0;

			virtual Matrix du() const = 0;
			virtual Matrix du(const Vector& local) const = 0;
			virtual Matrix du(const Vector& local, const Dimension& dimension) const = 0;

			virtual Matrix J(const Vector& local) const = 0;
			virtual Matrix InvJ(const Vector& local) const = 0;
			virtual Matrix N(const Vector& local) const = 0;
			virtual Matrix dN(const Vector& local) const = 0;

			virtual const Matrix& J(const Vector& local, CacheIndex cacheIndex) const = 0;
			virtual const Matrix& InvJ(const Vector& local, CacheIndex cacheIndex) const = 0;
			virtual const Matrix& N(const Vector& local, CacheIndex cacheIndex) const = 0;
			virtual const Matrix& dN(const Vector& local, CacheIndex cacheIndex) const = 0;

			virtual Scalar DetJ(const Vector& local) const = 0;
			virtual Scalar DetJ(const Vector& local, CacheIndex cacheIndex) const = 0;

			virtual Vector LocalCoordinates(INodePtr node) const = 0;
			virtual Vector LocalCoordinates(const NodeIndex& nodeIndex) const = 0;

			virtual Vector GlobalCoordinates(INodePtr node) const = 0;
			virtual Vector GlobalCoordinates(const NodeIndex& nodeIndex) const = 0;
			virtual Vector GlobalCoordinates(const Vector& local) const = 0;
			virtual Vector GlobalDerivatives(const Vector& local, const Dimension& dim) const = 0;

			virtual Order GetOrder() const = 0;
			virtual Parametric GetParametric() const = 0;

			virtual void SetOrder(const Order& order) = 0;
			virtual void SetParametric(const Parametric& order) = 0;

			virtual void InitializeCache() = 0;
		};

		class IElement
		{
		public:
			virtual ~IElement() = default;

			virtual INodePtr GetNode(const NodeIndex& nodeIndex) const = 0;
			virtual const Nodes& GetNodes() const = 0;
			virtual Type GetType() const = 0;
			virtual NodeIndex GetNodeIndex(INodePtr node) const = 0;
			virtual NumberNodes GetNumberNodes() const = 0;
			virtual Dimension GetDimension() const = 0;
			virtual Vector GetCenter() const = 0;
			virtual Tag GetTag() const = 0;
			virtual ElementIndex GetElementIndex() const = 0;
			virtual NumberDof GetNumberDof() const = 0;
			virtual IMaterialPtr GetMaterial() const = 0;
			virtual IValuePtr GetProperty(String key) const = 0;

			virtual void SetNumberDof(NumberDof numberDof) = 0;
			virtual void SetNode(const NodeIndex& nodeIndex, INodePtr node) = 0;
			virtual void SetTag(Tag elementTag) = 0;
			virtual void SetElementIndex(ElementIndex index) = 0;
			virtual void SetMaterial(IMaterialPtr material) = 0;
			virtual void SetProperty(IValuePtr value) = 0;

			virtual bool IsUsed(INodePtr node) const = 0;
			virtual bool IsIntegrable() const = 0;

			virtual Scalar Size() const = 0;
		};

		class IElementMass : virtual public IElement
		{
		public:
			virtual ~IElementMass() = default;

			virtual void SetMass(IScalarPtr mass) = 0;
			virtual Scalar GetMass() const = 0;
		};

		class IElementSpring : virtual public IElement
		{
		public:
			virtual ~IElementSpring() = default;

			virtual void SetStiffness(IScalar1DPtr stiffness) = 0;
			virtual Scalar GetStiffness() const = 0;
		};

		class IElement1D : virtual public IElement, virtual public IShape
		{
		public:
			virtual ~IElement1D() = default;

			virtual Scalar DelL(const Vector& local) const = 0;

			virtual Scalar Length() const = 0;

			virtual IGaussPtr IntegralEdge() const = 0;
		};

		class IElement2D : virtual public IElement, virtual public IShape
		{
		public:
			virtual ~IElement2D() = default;

			virtual Scalar DelA(const Vector& local) const = 0;
			virtual Scalar DelL(const Vector& local, const Dimension& dim1) const = 0;

			virtual Scalar Area() const = 0;
			virtual Scalar Length(const EdgeIndex& edge) const = 0;

			virtual INodePtr GetNodeEdge(const EdgeIndex& edgeIndex, const NodeIndex& nodeIndex) const = 0;

			virtual NumberNodes GetNumberNodesEdge(const EdgeIndex& edgeIndex) const = 0;
			virtual NumberEdges GetNumberEdges() const = 0;

			virtual IGaussPtr IntegralArea() const = 0;
			virtual IGaussPtr IntegralEdge() const = 0;

			virtual IntegralEdgeHelper GetIntegralEdgeHelper(EdgeIndex edgeIndex) const = 0;
		};

		class IElement3D : virtual public IElement, virtual public IShape
		{
		public:
			virtual ~IElement3D() = default;

			virtual Scalar DelA(const Vector& local, const Dimension& dim1, const Dimension& dim2) const = 0;
			virtual Scalar DelL(const Vector& local, const Dimension& dim1) const = 0;

			virtual Scalar Volume() const = 0;
			virtual Scalar Area(const FaceIndex& face) const = 0;
			virtual Scalar Length(const EdgeIndex& edge) const = 0;

			virtual INodePtr GetNodeFace(const FaceIndex& faceIndex, const NodeIndex& nodeIndex) const = 0;
			virtual INodePtr GetNodeEdge(const EdgeIndex& edgeIndex, const NodeIndex& nodeIndex) const = 0;

			virtual NumberNodes GetNumberNodesFace(const FaceIndex& faceIndex) const = 0;
			virtual NumberNodes GetNumberNodesEdge(const EdgeIndex& edgeIndex) const = 0;
			virtual NumberFaces GetNumberFaces() const = 0;
			virtual NumberEdges GetNumberEdges() const = 0;

			virtual IGaussPtr IntegralVolume() const = 0;
			virtual IGaussPtr IntegralArea() const = 0;
			virtual IGaussPtr IntegralEdge() const = 0;

			virtual IntegralAreaHelper GetIntegralAreaHelper(FaceIndex faceIndex) const = 0;
			virtual IntegralEdgeHelper GetIntegralEdgeHelper(EdgeIndex edgeIndex) const = 0;
		};
	}
}

#endif