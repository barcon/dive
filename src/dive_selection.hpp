#ifndef DIVE_SELECTION_HPP_
#define DIVE_SELECTION_HPP_

#include "dive_types.hpp"
#include "dive_elements.hpp"
#include "nodes.hpp"

#include <set>

namespace dive
{
	namespace selection
	{
		struct NodeCmp {
			bool operator()(INodePtr node1, INodePtr node2) const
			{
				return node1->GetTag() < node2->GetTag();
			}
		};

		struct ElementCmp {
			bool operator()(IElementPtr element1, IElementPtr element2) const
			{
				return element1->GetTag() < element2->GetTag();
			}
		};

		struct TypeCmp {
			bool operator()(Type type1, Type type2) const
			{
				return type1 < type2;
			}
		};

		using SelectionNodes = std::set<INodePtr, NodeCmp>;
		using SelectionElements = std::set<IElementPtr, ElementCmp>;
		using SelectionTypes = std::set<Type, TypeCmp>;

		template <typename T> struct SpecificationFilter
		{
			virtual bool IsSatisfied(T item) = 0;
		};

		struct SpecNodesByCoordinate : SpecificationFilter<INodePtr>
		{
			Axis axis_;
			Scalar pos_;
			Scalar tol_;
			IBasisPtr basis_;

			explicit SpecNodesByCoordinate(IBasisPtr basis, Axis axis, Scalar pos, Scalar tol);
			bool IsSatisfied(INodePtr item) override;
		};
		struct SpecNodesByRange : SpecificationFilter<INodePtr>
		{
			Axis axis_;
			Scalar min_;
			Scalar max_;
			Scalar tol_;
			IBasisPtr basis_;

			explicit SpecNodesByRange(IBasisPtr basis, Axis axis, Scalar min, Scalar max, Scalar tol);
			bool IsSatisfied(INodePtr item) override;
		};
		struct SpecNodesByTag : SpecificationFilter<INodePtr>
		{
			Tag min_;
			Tag max_;

			explicit SpecNodesByTag(Tag min, Tag max);
			bool IsSatisfied(INodePtr item) override;
		};

		struct SpecElementsByTag : SpecificationFilter<IElementPtr>
		{
			Tag min_;
			Tag max_;

			explicit SpecElementsByTag(Tag min, Tag max);
			bool IsSatisfied(IElementPtr item) override;
		};
		struct SpecElementsByType : SpecificationFilter<IElementPtr>
		{
			SelectionTypes selectionTypes_;

			explicit SpecElementsByType();
			bool IsSatisfied(IElementPtr item) override;
		};

		Nodes FilterNodes(SpecificationFilter<INodePtr>& spec, const Nodes& input);
		Nodes FilterNodesByCoordinate(const Nodes& input, IBasisPtr basis, Axis axis, Scalar pos, Scalar tol);
		Nodes FilterNodesByRange(const Nodes& input, IBasisPtr basis, Axis axis, Scalar min, Scalar max, Scalar tol);
		Nodes FilterNodesByTag(const Nodes& input, Tag min, Tag max);
		Nodes FilterNodesByElements(const Elements& input);
		Nodes FilterNodesRemoveDuplicates(const Nodes& input);

		Elements FilterElements(SpecificationFilter<IElementPtr>& spec, const Elements& input);
		Elements FilterElementsByTag(const Elements& input, Tag min, Tag max);
		Elements FilterElementsByType(const Elements& input);
		Elements FilterElementsByNodes(const Nodes& input);
		Elements FilterElementsByNodesIntersection(const Nodes& input);
		Elements FilterElementsRemoveDuplicates(const Elements& input);

		FacePair FilterFaceByNodes(IElementPtr element, const Nodes& input);
		EdgePair FilterEdgeByNodes(IElementPtr element, const Nodes& input);

		template <typename T> struct SpecificationSort
		{
			virtual bool IsSatisfied(T item1, T item2) = 0;
		};

		struct SpecSortNodesByTag : SpecificationSort<INodePtr>
		{
			explicit SpecSortNodesByTag();
			bool IsSatisfied(INodePtr item1, INodePtr item2) override;
		};
		struct SpecSortNodesByCoordinate : SpecificationSort<INodePtr>
		{
			Axis axis_;
			IBasisPtr basis_;

			explicit SpecSortNodesByCoordinate(IBasisPtr basis, Axis axis);
			bool IsSatisfied(INodePtr item1, INodePtr item2) override;
		};

		Nodes SortNodes(SpecificationSort<INodePtr>& spec, const Nodes& input);
		Nodes SortNodesByTag(const Nodes& input);
		Nodes SortNodesByCoordinate(const Nodes& input, IBasisPtr basis, Axis axis);

		struct SpecSortElementsByTag : SpecificationSort<IElementPtr>
		{
			explicit SpecSortElementsByTag();
			bool IsSatisfied(IElementPtr item1, IElementPtr item2) override;
		};

		Elements SortElements(SpecificationSort<IElementPtr>& spec, const Elements& input);
		Elements SortElementsByTag(const Elements& input);
	} //namespace selection
} //namespace dive

#endif /* DIVE_SELECTION_HPP_ */