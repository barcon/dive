#include "dive_weakforms_structural_load_distributed_face.hpp"
#include "dive_loads_distributed_face.hpp"

namespace dive {
	namespace weakforms {
		LoadDistributedFaceStructuralPtr CreateWeakFormLoadDistributedFaceStructural()
		{
			auto res = LoadDistributedFaceStructural::Create();

			return res;
		}
		LoadDistributedFaceStructuralPtr LoadDistributedFaceStructural::Create()
		{
			class MakeSharedEnabler : public LoadDistributedFaceStructural
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		LoadDistributedFaceStructuralPtr LoadDistributedFaceStructural::GetPtr()
		{
			return std::dynamic_pointer_cast<LoadDistributedFaceStructural>(shared_from_this());
		}
		ConstLoadDistributedFaceStructuralPtr LoadDistributedFaceStructural::GetPtr() const
		{
			return const_cast<LoadDistributedFaceStructural*>(this)->GetPtr();
		}
		void LoadDistributedFaceStructural::WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const
		{
			auto force = std::static_pointer_cast<loads::LoadDistributedFace>(load);
			auto element = force->GetElement();

			auto N = FormMatrix_N(element, point);
			auto f = force->GetValue(point);

			output = N.Transpose() * f;
		}
		Matrix LoadDistributedFaceStructural::FormMatrix_N(IElementPtr element, const Vector& point) const
		{
			auto numberNodes = element->GetNumberNodes();
			auto numberDof = element->GetNumberDof();
			auto N = element->N(point);

			Matrix res(numberDof, numberNodes * numberDof, 0.0);

			for (DofIndex i = 0; i < numberDof; ++i)
			{
				for (NodeIndex j = 0; j < numberNodes; ++j)
				{
					res(i, j * numberDof + i) = N(j);
				}
			}

			return res;
		}
	} // namespace problems
} // namespace dive