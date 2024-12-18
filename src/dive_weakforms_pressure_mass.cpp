#include "dive_weakforms_pressure_mass.hpp"
#include "dive_values_scalar_congruent.hpp"
#include "dive_values_matrix_congruent.hpp"

namespace dive {
	namespace weakforms {
		MassPressurePtr CreateWeakFormMassPressure()
		{
			auto res = MassPressure::Create();

			return res;
		}
		MassPressurePtr MassPressure::Create()
		{
			class MakeSharedEnabler : public MassPressure
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		MassPressurePtr MassPressure::GetPtr()
		{
			return std::dynamic_pointer_cast<MassPressure>(shared_from_this());
		}
		ConstMassPressurePtr MassPressure::GetPtr() const
		{
			return const_cast<MassPressure*>(this)->GetPtr();
		}
		void MassPressure::WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const
		{
			Scalar uconv{ 0.0 };
			Scalar udiff{ 0.0 };
			Scalar beta{ 0.0 };

			auto N = FormMatrix_N(element, local, cacheIndex);
			auto u = FormVelocity(element, local);
			auto nu = FormKineticViscosity(element, local);
			auto h = element->GetHeight();

			uconv = nu / h;
			udiff = eilig::NormP2(u);
			beta = std::max({ 0.5, uconv, udiff });

			output = (1.0 / (beta * beta)) * N.Transpose() * N;
		}
		void MassPressure::SetTemperature(IScalar3DPtr temperature)
		{
			temperature_ = temperature;
		}
		void MassPressure::SetPressure(IScalar3DPtr pressure)
		{
			pressure_ = pressure;
		}
		void MassPressure::SetVelocity(IMatrix3DPtr velocity)
		{
			velocity_ = velocity;
		}
		Matrix MassPressure::FormVelocity(IElementPtr element, const Vector& local) const
		{
			return values::GetValueMatrix3D(velocity_, local, element) ;
		}
		Scalar MassPressure::FormKineticViscosity(IElementPtr element, const Vector& local) const
		{
			auto material = std::static_pointer_cast<material::IMaterialFluid>(element->GetMaterial());
			auto temperature = values::GetValue(temperature_, local, element);
			auto pressure = values::GetValue(pressure_, local, element);

			auto dynamicViscosity = material->GetDynamicViscosity(temperature, pressure);
			auto density = material->GetDensity(temperature, pressure);

			return dynamicViscosity / density;
		}
		Matrix MassPressure::FormMatrix_N(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const
		{
			return element->N(local, cacheIndex);
		}
	} // namespace problems
} // namespace dive