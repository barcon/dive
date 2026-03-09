#include "dive_weakforms_fluid_mass_lumped.hpp"

namespace dive {
	namespace weakforms {
		MassLumpedFluidPtr CreateWeakFormMassLumpedFluid()
		{
			auto res = MassLumpedFluid::Create();

			return res;
		}
		MassLumpedFluidPtr MassLumpedFluid::Create()
		{
			class MakeSharedEnabler : public MassLumpedFluid
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		MassLumpedFluidPtr MassLumpedFluid::GetPtr()
		{
			return std::dynamic_pointer_cast<MassLumpedFluid>(shared_from_this());
		}
		ConstMassLumpedFluidPtr MassLumpedFluid::GetPtr() const
		{
			return const_cast<MassLumpedFluid*>(this)->GetPtr();
		}
		void MassLumpedFluid::WeakFormulation(IElementMappedPtr element, const Vector& point, Matrix& output, const CacheIndex& cacheIndex) const
		{
			auto NN = FormMatrix_NN(element, point, cacheIndex);
			auto trace = NN.Trace();			
			auto total{ 0.0};

			switch (element->GetNumberDimensions())
			{
			case elements::dimension_beam:
				total = std::dynamic_pointer_cast<elements::IElementBeam>(element)->Length();
				break;
			case elements::dimension_shell:
				total = std::dynamic_pointer_cast<elements::IElementShell>(element)->Area();
				break;
			case elements::dimension_solid:
				total = std::dynamic_pointer_cast<elements::IElementSolid>(element)->Volume();
				break;
			default:
				break;
			}

			output = (total / trace) * NN.Diagonal();
		}
		Matrix MassLumpedFluid::FormMatrix_NN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			return element->NN(cacheIndex);
		}
	} // namespace problems
} // namespace dive