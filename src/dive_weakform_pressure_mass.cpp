#include "dive_weakform_pressure_mass.hpp"
#include "dive_value_scalar_congruent.hpp"
#include "dive_value_matrix_congruent.hpp"

namespace dive {
	namespace weakform {
		MassPressurePtr CreateMassPressure()
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
		void MassPressure::WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const
		{
			Scalar speedDiffusion{ 0.0 };
			Scalar speedConvection{ 0.0 };
			Scalar beta{ 0.5 };

			auto N = FormMatrix_N(element, local, cacheIndex);
			auto u = FormVelocity(element, local, cacheIndex);
			auto nu = FormKineticViscosity(element, local, cacheIndex);
			auto h = element->SizeMinimum();

			speedDiffusion = nu / h;
			speedConvection = eilig::NormP2(u);
			beta = std::max({ beta, speedConvection, speedDiffusion });

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
		void MassPressure::SetMomentum(IMatrixCoordinatesPtr momentum)
		{
			momentum_ = momentum;
		}
		Matrix MassPressure::FormVelocity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const
		{
			auto state = Vector(2);
			state(0) = value::GetValueScalarCoordinates(temperature_, local, element);
			state(1) = value::GetValueScalarCoordinates(pressure_, local, element);

			auto density = element->GetMaterial()->GetDensity(state);

			return value::GetValueMatrixCoordinates(momentum_, local, element) * (1.0 / density);
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