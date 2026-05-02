#include "dive_weakform_pressure_mass.hpp"
#include "dive_value_scalar_congruent.hpp"
#include "dive_value_matrix_congruent.hpp"

namespace dive {
	namespace weakform {
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
		void MassPressure::WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const
		{
			Scalar uconv{ 0.0 };
			Scalar udiff{ 0.0 };
			Scalar beta{ 0.0 };

			auto N = FormMatrix_N(element, local, cacheIndex);
			auto u = FormVelocity(element, local, cacheIndex);
			auto nu = FormKineticViscosity(element, local, cacheIndex);
			auto h = element->SizeMinimum();

			uconv = nu / h;
			udiff = eilig::NormP2(u);
			beta = std::max({ 0.5, uconv, udiff });

			output = (1.0 / (beta * beta)) * N.Transpose() * N;
		}
		void MassPressure::SetTemperature(IScalarCoordinatesPtr temperature)
		{
			temperature_ = temperature;
		}
		void MassPressure::SetPressure(IScalarCoordinatesPtr pressure)
		{
			pressure_ = pressure;
		}
		void MassPressure::SetVelocity(IMatrixCoordinatesPtr velocity)
		{
			velocity_ = velocity;
		}
		Matrix MassPressure::FormVelocity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			return value::GetValueMatrixCoordinates(velocity_, local, element) ;
		}
		Scalar MassPressure::FormKineticViscosity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto material = std::static_pointer_cast<material::IMaterialFluid>(element->GetMaterial());		
			
			auto state = Vector(2);
			state(0) = value::GetValueScalarCoordinates(temperature_, local, element);
			state(1) = value::GetValueScalarCoordinates(pressure_, local, element);

			auto dynamicViscosity = material->GetDynamicViscosity(state);
			auto density = material->GetDensity(state);

			return dynamicViscosity / density;
		}
		Matrix MassPressure::FormMatrix_N(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			return element->N(cacheIndex);
		}
	} // namespace weakform
} // namespace dive