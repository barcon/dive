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
		void StiffnessPressure::WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const
		{
			auto dN = FormMatrix_dN(element, local, cacheIndex);

			output = dN.Transpose() * dN;
		}
		Matrix StiffnessPressure::FormMatrix_dN(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const
		{
			return element->InvJ(local, cacheIndex) * element->dN(local, cacheIndex);
		}
	} // namespace problems
} // namespace dive