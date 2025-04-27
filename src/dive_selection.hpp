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
		struct Face 
		{
			FaceIndex faceIndex;
			IElementPtr element;
		};

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

		struct FaceCmp {
			bool operator()(Face face1, Face face2) const
			{
				if (face1.element->GetTag() == face2.element->GetTag())
				{
					return face1.faceIndex < face2.faceIndex;
				}

				return face1.element->GetTag() < face2.element->GetTag();
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
		using SelectionFaces = std::set<Face, FaceCmp>;
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
		Nodes FilterNodesRemoveDuplicates(const Nodes& input);
		Nodes FilterNodesByCoordinate(const Nodes& input, IBasisPtr basis, Axis axis, Scalar pos, Scalar tol);
		Nodes FilterNodesByRange(const Nodes& input, IBasisPtr basis, Axis axis, Scalar min, Scalar max, Scalar tol);
		Nodes FilterNodesByTag(const Nodes& input, Tag min, Tag max);
		Nodes FilterNodesByElements(const Elements& input);

		Elements FilterElements(SpecificationFilter<IElementPtr>& spec, const Elements& input);
		Elements FilterElementsRemoveDuplicates(const Elements& input);
		Elements FilterElementsByTag(const Elements& input, Tag min, Tag max);
		Elements FilterElementsByType(const Elements& input);
		Elements FilterElementsByNodes(const Nodes& input);

		template <typename T> struct SpecificationSort
		{
			virtual bool IsSatisfied(T item1, T item2) = 0;
		};

		struct SpecSortNodesByCoordinate : SpecificationSort<INodePtr>
		{
			Axis axis_;
			IBasisPtr basis_;

			explicit SpecSortNodesByCoordinate(IBasisPtr basis, Axis axis);
			bool IsSatisfied(INodePtr item1, INodePtr item2) override;
		};

		Nodes SortNodes(SpecificationSort<INodePtr>& spec, const Nodes& input);;
		Nodes SortNodesByCoordinate(const Nodes& input, IBasisPtr basis, Axis axis);

	} //namespace selection
} //namespace dive

#endif /* DIVE_SELECTION_HPP_ */