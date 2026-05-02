#include "dive_weakform_fluid_mass.hpp"

namespace dive {
	namespace weakform {
		MassFluidPtr CreateWeakFormMassFluid()
		{
			auto res = MassFluid::Create();

			return res;
		}
		MassFluidPtr MassFluid::Create()
		{
			class MakeSharedEnabler : public MassFluid
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		MassFluidPtr MassFluid::GetPtr()
		{
			return std::dynamic_pointer_cast<MassFluid>(shared_from_this());
		}
		ConstMassFluidPtr MassFluid::GetPtr() const
		{
			return const_cast<MassFluid*>(this)->GetPtr();
		}
		void MassFluid::WeakFormulation(IElementMappedPtr element, const Vector& point, Matrix& output, const CacheIndex& cacheIndex) const
		{
			auto NN = FormMatrix_NN(element, point, cacheIndex);

			output = NN;
		}
		Matrix MassFluid::FormMatrix_NN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			return element->NN(cacheIndex);
		}
	} // namespace weakform
} // namespace dive