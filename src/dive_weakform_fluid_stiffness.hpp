#ifndef DIVE_WEAKFORM_FLUID_STIFFNESS_HPP_
#define DIVE_WEAKFORM_FLUID_STIFFNESS_HPP_

#include "dive_weakform.hpp"

namespace dive
{
    namespace weakform
    {
		StiffnessFluidPtr CreateWeakFormStiffnessFluid();

		class StiffnessFluid : public IWeakFormElement, virtual public std::enable_shared_from_this<StiffnessFluid>
		{
		public:
			virtual ~StiffnessFluid() = default;

			static StiffnessFluidPtr Create();
			StiffnessFluidPtr GetPtr();
			ConstStiffnessFluidPtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const override;

			void SetTemperature(IScalarCoordinatesPtr temperature);
			void SetPressure(IScalarCoordinatesPtr pressure);

		protected:
			StiffnessFluid() = default;

			Matrix FormMatrix_Id(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Matrix FormMatrix_B(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Vector FormVector_Z(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;

			IScalarCoordinatesPtr temperature_{ nullptr };
			IScalarCoordinatesPtr pressure_{ nullptr };

			using std::enable_shared_from_this<StiffnessFluid>::shared_from_this;
		};

	} //namespace weakform
} //namespace dive

#endif /* DIVE_WEAKFORM_FLUID_STIFFNESS_HPP_ */