#include "dive_weakforms_structural_load_distributed_edge.hpp"
#include "dive_load_distributed_edge.hpp"

namespace dive {
	namespace weakforms {
		LoadDistributedEdgeStructuralPtr CreateWeakFormLoadDistributedEdgeStructural()
		{
			auto res = LoadDistributedEdgeStructural::Create();

			return res;
		}
		LoadDistributedEdgeStructuralPtr LoadDistributedEdgeStructural::Create()
		{
			class MakeSharedEnabler : public LoadDistributedEdgeStructural
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		LoadDistributedEdgeStructuralPtr LoadDistributedEdgeStructural::GetPtr()
		{
			return std::dynamic_pointer_cast<LoadDistributedEdgeStructural>(shared_from_this());
		}
		ConstLoadDistributedEdgeStructuralPtr LoadDistributedEdgeStructural::GetPtr() const
		{
			return const_cast<LoadDistributedEdgeStructural*>(this)->GetPtr();
		}
		void LoadDistributedEdgeStructural::WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const
		{
			auto force = std::static_pointer_cast<load::LoadDistributedEdge>(load);
			auto element = force->GetElement();

			auto N = FormMatrix_N(element, point);
			auto f = force->GetValue(point);

			output = N.Transpose() * f;
		}
		Matrix LoadDistributedEdgeStructural::FormMatrix_N(IElementMappedPtr element, const Vector& point) const
		{
			return element->N(point);
		}
	} // namespace problems
} // namespace dive