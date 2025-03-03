#ifndef DIVE_ELEMENTS_HPP_
#define DIVE_ELEMENTS_HPP_

#include "dive_types.hpp"
#include "dive_timers.hpp"

#include <set>

namespace dive
{
	namespace elements
	{
		using Parametric = std::size_t;

		using Order = std::size_t;
		static const Order linear = 1;
		static const Order quadratic = 2;

		static const Type element_hexa8 = 5;
		static const Type element_hexa20 = 17;

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

		class IElement
		{
		public:
			virtual ~IElement() = default;

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
			virtual Scalar DelA(const Vector& local, const Dimension& dim1, const Dimension& dim2) const = 0;
			virtual Scalar DelL(const Vector& local, const Dimension& dim1) const = 0;

			virtual Scalar DetJ(const Vector& local, CacheIndex cacheIndex) const = 0;

			virtual Scalar Volume() const = 0;
			virtual Scalar Area(const FaceIndex& face) const = 0;
			virtual Scalar Length(const EdgeIndex& edge) const = 0;
			virtual Scalar Size() const = 0;

			virtual Vector LocalCoordinates(INodePtr node) const = 0;
			virtual Vector LocalCoordinates(const NodeIndex& nodeIndex) const = 0;
			
			virtual Vector GlobalCoordinates(INodePtr node) const = 0;
			virtual Vector GlobalCoordinates(const NodeIndex& nodeIndex) const = 0;
			virtual Vector GlobalCoordinates(const Vector& local) const = 0;
			virtual Vector GlobalDerivatives(const Vector& local, const Dimension& dim) const = 0;

			virtual INodePtr GetNodeFace(const FaceIndex& faceIndex, const NodeIndex& nodeIndex) const = 0;
			virtual INodePtr GetNodeEdge(const EdgeIndex& edgeIndex, const NodeIndex& nodeIndex) const = 0;
			virtual INodePtr GetNode(const NodeIndex& nodeIndex) const = 0;
			virtual const Nodes& GetNodes() const = 0;
			virtual Order GetOrder() const = 0;
			virtual Parametric GetParametric() const = 0;
			virtual Type GetType() const = 0;
			virtual NodeIndex GetNodeIndex(INodePtr node) const = 0;
			virtual NumberNodes GetNumberNodesFace(const FaceIndex& faceIndex) const = 0;
			virtual NumberNodes GetNumberNodesEdge(const EdgeIndex& edgeIndex) const = 0;
			virtual NumberNodes GetNumberNodes() const = 0;
			virtual NumberFaces GetNumberFaces() const = 0;
			virtual NumberEdges GetNumberEdges() const = 0;
			virtual Dimension GetDimension() const = 0;
			virtual Vector GetCenter() const = 0;
			virtual Tag GetTag() const = 0;
			virtual IMaterialPtr GetMaterial() const = 0;
			virtual ElementIndex GetElementIndex() const = 0;
			virtual NumberDof GetNumberDof() const = 0;

			virtual IGaussPtr IntegralVolume() const = 0;
			virtual IGaussPtr IntegralArea() const = 0;
			virtual IGaussPtr IntegralEdge() const = 0;

			virtual IntegralAreaHelper GetIntegralAreaHelper(FaceIndex faceIndex) const = 0;
			virtual IntegralEdgeHelper GetIntegralEdgeHelper(EdgeIndex edgeIndex) const = 0;

			virtual void SetNumberDof(NumberDof numberDof) = 0;
			virtual void SetNode(const NodeIndex& nodeIndex, INodePtr node) = 0;
			virtual void SetOrder(const Order& order) = 0;
			virtual void SetParametric(const Parametric& order) = 0;
			virtual void SetTag(Tag elementTag) = 0;
			virtual void SetMaterial(IMaterialPtr material) = 0;
			virtual void SetElementIndex(ElementIndex index) = 0;
			
			virtual bool IsUsed(INodePtr node) const = 0;
			virtual void InitializeCache() = 0;
		};
	}
}

#endif