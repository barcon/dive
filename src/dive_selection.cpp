#include "dive_selection.hpp"

#include <algorithm>

namespace dive
{
	namespace selection
	{

		SpecNodesByCoordinate::SpecNodesByCoordinate(IBasisPtr basis, Axis axis, Scalar pos, Scalar tol)
		{
			basis_ = basis;
			axis_ = axis;
			pos_ = pos;
			tol_ = tol;
		}
		bool SpecNodesByCoordinate::IsSatisfied(INodePtr item)
		{
			auto local = basis_->GlobalToLocal(item->GetPoint());

			if (std::fabs(local(axis_) - pos_) <= tol_)
			{
				return true;
			}

			return false;
		}

		SpecNodesByRange::SpecNodesByRange(IBasisPtr basis, Axis axis, Scalar min, Scalar max, Scalar tol)
		{
			basis_ = basis;
			axis_ = axis;
			min_ = min;
			max_ = max;
			tol_ = tol;
		}
		bool SpecNodesByRange::IsSatisfied(INodePtr item)
		{
			auto local = basis_->GlobalToLocal(item->GetPoint());

			if ((local(axis_) >= (min_ - tol_)) && (local(axis_) <= (max_ + tol_)))
			{
				return true;
			}

			return false;
		}

		SpecNodesByTag::SpecNodesByTag(Tag min, Tag max)
		{
			min_ = min;
			max_ = max;
		}
		bool SpecNodesByTag::IsSatisfied(INodePtr item)
		{
			auto tag = item->GetTag();

			if (tag >= min_ && tag <= max_)
			{
				return true;
			}

			return false;
		}

		SpecElementsByTag::SpecElementsByTag(Tag min, Tag max)
		{
			min_ = min;
			max_ = max;
		}
		bool SpecElementsByTag::IsSatisfied(IElementPtr item)
		{
			auto tag = item->GetTag();

			if (tag >= min_ && tag <= max_)
			{
				return true;
			}

			return false;
		}

		SpecElementsByType::SpecElementsByType()
		{
		}
		bool SpecElementsByType::IsSatisfied(IElementPtr item)
		{
			if (selectionTypes_.find(item->GetType()) != selectionTypes_.end())
			{
				return false;
			}

			selectionTypes_.insert(item->GetType());

			return true;
		}

		Nodes FilterNodes(SpecificationFilter<INodePtr>& spec, const Nodes& input)
		{
			SelectionNodes res;

			for (auto& item : input)
			{
				if (spec.IsSatisfied(item))
				{
					res.insert(item);
				}
			}

			return Nodes(res.begin(), res.end());
		}
		Nodes FilterNodesByCoordinate(const Nodes& input, IBasisPtr basis, Axis axis, Scalar pos, Scalar tol)
		{
			SpecNodesByCoordinate spec(basis, axis, pos, tol);

			return FilterNodes(spec, input);
		}
		Nodes FilterNodesByRange(const Nodes& input, IBasisPtr basis, Axis axis, Scalar min, Scalar max, Scalar tol)
		{
			SpecNodesByRange spec(basis, axis, min, max, tol);

			return FilterNodes(spec, input);
		}
		Nodes FilterNodesByTag(const Nodes& input, Tag min, Tag max)
		{
			SpecNodesByTag spec(min, max);

			return FilterNodes(spec, input);
		}
		Nodes FilterNodesByElements(const Elements& input)
		{
			SelectionNodes res;

			for (auto& element : input)
			{
				for (auto& node: element->GetNodes())
				{
					res.insert(node);
				}
			}

			return Nodes(res.begin(), res.end());
		}
		Nodes FilterNodesRemoveDuplicates(const Nodes& input)
		{
			SelectionNodes res;

			for (auto& item : input)
			{
				res.insert(item);
			}

			return Nodes(res.begin(), res.end());
		}

		void FilterNodes(SpecificationFilter<INodePtr>& spec, Nodes& input)
		{
			SelectionNodes res;

			for (auto& item : input)
			{
				if (spec.IsSatisfied(item))
				{
					res.insert(item);
				}
			}

			input = Nodes(res.begin(), res.end());
		}
		void FilterNodesByCoordinate(Nodes& input, IBasisPtr basis, Axis axis, Scalar pos, Scalar tol)
		{
			SpecNodesByCoordinate spec(basis, axis, pos, tol);

			FilterNodes(spec, input);
		}
		void FilterNodesByRange(Nodes& input, IBasisPtr basis, Axis axis, Scalar min, Scalar max, Scalar tol)
		{
			SpecNodesByRange spec(basis, axis, min, max, tol);

			FilterNodes(spec, input);
		}
		void FilterNodesRemoveDuplicates(Nodes& input)
		{
			SelectionNodes res;

			for (auto& item : input)
			{
				res.insert(item);
			}

			input = Nodes(res.begin(), res.end());
		}

		Elements FilterElements(SpecificationFilter<IElementPtr>& spec, const Elements& input)
		{
			SelectionElements res;

			for (auto& item : input)
			{
				if (spec.IsSatisfied(item))
				{
					res.insert(item);
				}
			}

			return Elements(res.begin(), res.end());
		}
		Elements FilterElementsByTag(const Elements& input, Tag min, Tag max)
		{
			SpecElementsByTag spec(min, max);

			return FilterElements(spec, input);
		}
		Elements FilterElementsByType(const Elements& input)
		{
			SpecElementsByType spec;

			return FilterElements(spec, input);
		}
		Elements FilterElementsByNodes(const Nodes& input)
		{
			SelectionElements res;

			for (auto& node : input)
			{
				for (auto& element : node->GetConnectivity().elements)
				{
					res.insert(element);
				}
			}

			return  Elements(res.begin(), res.end());
		}
		Elements FilterElementsRemoveDuplicates(const Elements& input)
		{
			SelectionElements res;

			for (auto& item : input)
			{
				res.insert(item);
			}

			return Elements(res.begin(), res.end());
		}

		void FilterElements(SpecificationFilter<IElementPtr>& spec, Elements& input)
		{
			SelectionElements res;

			for (auto& item : input)
			{
				if (spec.IsSatisfied(item))
				{
					res.insert(item);
				}
			}
			
			input = Elements(res.begin(), res.end());
		}
		void FilterElementsByTag(Elements& input, Tag min, Tag max)
		{
			SpecElementsByTag spec(min, max);

			FilterElements(spec, input);
		}
		void FilterElementsRemoveDuplicates(Elements& input)
		{
			SelectionElements res;

			for (auto& item : input)
			{
				res.insert(item);
			}

			input = Elements(res.begin(), res.end());
		}

		SpecSortNodesByCoordinate::SpecSortNodesByCoordinate(IBasisPtr basis, Axis axis)
		{
			basis_ = basis;
			axis_ = axis;
		}
		bool SpecSortNodesByCoordinate::IsSatisfied(INodePtr item1, INodePtr item2)
		{
			auto local1 = basis_->GlobalToLocal(item1->GetPoint());
			auto local2 = basis_->GlobalToLocal(item2->GetPoint());

			return local1(axis_) < local2(axis_);
		}

		Nodes SortNodes(SpecificationSort<INodePtr>& spec, const Nodes& input)
		{
			auto res = input;

			std::sort(res.begin(), res.end(),
			[&](INodePtr node1, INodePtr node2) -> bool 
			{
				return spec.IsSatisfied(node1, node2);
			});

			return res;
		}
		Nodes SortNodesByCoordinate(const Nodes& input, IBasisPtr basis, Axis axis)
		{
			SpecSortNodesByCoordinate spec(basis, axis);

			return SortNodes(spec, input);
		}
	} // namespace selection
} //namespace dive