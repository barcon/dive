#include "dive_weakforms_pressure_stiffness.hpp"

namespace dive {
	namespace weakforms {
		StiffnessPressurePtr CreateWeakFormStiffnessPressure()
		{
			auto res = StiffnessPressure::Create();

			return res;
		}
		StiffnessPressurePtr StiffnessPressure::Create()
		{
			class MakeSharedEnabler : public StiffnessPressure
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		StiffnessPressurePtr StiffnessPressure::GetPtr()
		{
			return std::dynamic_pointer_cast<StiffnessPressure>(shared_from_this());
		}
		ConstStiffnessPressurePtr StiffnessPressure::GetPtr() const
		{
			return const_cast<StiffnessPressure*>(this)->GetPtr();
		}
		void StiffnessPressure::WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output) const
		{
			auto dN = FormMatrix_dN(element, local);

			output = dN.Transpose() * dN;
		}
		Matrix StiffnessPressure::FormMatrix_dN(IElementMappedPtr element, const Vector& local) const
		{
			return element->InvJ(local) * element->dN(local);
		}
	} // namespace problems
} // namespace dive