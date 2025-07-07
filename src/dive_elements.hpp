#ifndef DIVE_ELEMENTS_HPP_
#define DIVE_ELEMENTS_HPP_

#include "dive_types.hpp"

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
		static const Type element_combined = 2;
		static const Type element_hexa8 = 3;
		static const Type element_hexa20 = 4;

		static const NumberDimensions dimension_point = 0;
		static const NumberDimensions dimension_beam = 1;
		static const NumberDimensions dimension_shell = 2;
		static const NumberDimensions dimension_solid = 3;
		
		static const NumberCoordinates coordinate_one = 1;
		static const NumberCoordinates coordinate_two = 2;
		static const NumberCoordinates coordinate_three = 3;

		static const NodeIndex nodeIndexInvalid = 0xFFFFFFFF;

		struct IntegralAreaHelper
		{
			CoordinateIndex index1;
			CoordinateIndex index2;
			CoordinateIndex index3;

			Scalar coord3;
		};

		struct IntegralEdgeHelper
		{
			CoordinateIndex index1;
			CoordinateIndex index2;
			CoordinateIndex index3;

			Scalar coord2;
			Scalar coord3;
		};

		class IElement
		{
		public:
			virtual ~IElement() = default;

			virtual Tag GetTag() const = 0;
			virtual Type GetType() const = 0;
			virtual Vector GetCenter() const = 0;
			virtual IMaterialPtr GetMaterial() const = 0;
			virtual IValuePtr GetProperty(String key) const = 0;

			virtual NodeIndex GetNodeIndex(INodePtr node) const = 0;
			virtual ElementIndex GetElementIndex() const = 0;

			virtual NumberDof GetNumberDof() const = 0;
			virtual NumberNodes GetNumberNodes() const = 0;
			virtual NumberFaces GetNumberFaces() const = 0;
			virtual NumberEdges GetNumberEdges() const = 0;
			virtual NumberNodes GetNumberNodesFace(const FaceIndex& faceIndex) const = 0;
			virtual NumberNodes GetNumberNodesEdge(const EdgeIndex& edgeIndex) const = 0;
			virtual NumberDimensions GetNumberDimensions() const = 0;
			virtual NumberCoordinates GetNumberCoordinates() const = 0;

			virtual INodePtr GetNode(const NodeIndex& nodeIndex) const = 0;
			virtual INodePtr GetNodeFace(const FaceIndex& faceIndex, const NodeIndex& nodeIndex) const = 0;
			virtual INodePtr GetNodeEdge(const EdgeIndex& edgeIndex, const NodeIndex& nodeIndex) const = 0;
			virtual const Nodes& GetNodes() const = 0;

			virtual void SetTag(Tag elementTag) = 0;
			virtual void SetNumberDof(NumberDof numberDof) = 0;
			virtual void SetNode(const NodeIndex& nodeIndex, INodePtr node) = 0;
			virtual void SetElementIndex(ElementIndex index) = 0;
			virtual void SetMaterial(IMaterialPtr material) = 0;
			virtual void SetProperty(IValuePtr value) = 0;

			virtual Vector LocalCoordinates(INodePtr node) const = 0;
			virtual Vector LocalCoordinates(const NodeIndex& nodeIndex) const = 0;

			virtual Vector GlobalCoordinates(INodePtr node) const = 0;
			virtual Vector GlobalCoordinates(const NodeIndex& nodeIndex) const = 0;
			virtual Vector GlobalCoordinates(const Vector& local) const = 0;

			virtual Scalar SizeMinimum() const = 0;
			virtual Scalar SizeMaximum() const = 0;

			virtual bool IsUsed(INodePtr node) const = 0;
			virtual bool IsMapped() const = 0;
			virtual bool IsCacheable() const = 0;

			virtual void InitializeCache() = 0;
		};

		class IElementMapped: virtual public IElement
		{
		public:
			virtual ~IElementMapped() = default;

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
			virtual Scalar DelA(const Vector& local, const Dimension& dim1, const Dimension& dim2) const = 0;
			virtual Scalar DelL(const Vector& local, const Dimension& dim1) const = 0;

			virtual Order GetOrder() const = 0;
			virtual Parametric GetParametric() const = 0;

			virtual void SetOrder(const Order& order) = 0;
			virtual void SetParametric(const Parametric& order) = 0;

			virtual void IntegralWeakFormElement(IWeakFormElementPtr weakForm, Matrix& output) const = 0;
			virtual void IntegralWeakFormLoad(IWeakFormLoadPtr weakForm, ILoadPtr load, Vector& output) const = 0;
		};

		class IElementBeam : virtual public IElementMapped
		{
		public:
			virtual ~IElementBeam() = default;

			virtual Scalar Length() const = 0;

		};

		class IElementShell : virtual public IElementMapped
		{
		public:
			virtual ~IElementShell() = default;

			virtual Scalar GetThickness() const = 0;
			virtual void SetThickness(Scalar thickness) = 0;

			virtual Scalar Area() const = 0;
			virtual Scalar Length(const EdgeIndex& edge) const = 0;
		};

		class IElementSolid : virtual public IElementMapped
		{
		public:
			virtual ~IElementSolid() = default;

			virtual Scalar Volume() const = 0;
			virtual Scalar Area(const FaceIndex& face) const = 0;
			virtual Scalar Length(const EdgeIndex& edge) const = 0;
		};
	}
}

#endif