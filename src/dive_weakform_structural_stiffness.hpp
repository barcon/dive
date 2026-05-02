#ifndef DIVE_WEAKFORM_STRUCTURAL_STIFFNESS_HPP_
#define DIVE_WEAKFORM_STRUCTURAL_STIFFNESS_HPP_

#include "dive_weakform.hpp"

namespace dive
{
    namespace weakform
    {
		StiffnessStructuralPtr CreateWeakFormStiffnessStructural();

		class StiffnessStructural : public IWeakFormElement, virtual public std::enable_shared_from_this<StiffnessStructural>
		{
		public:
			virtual ~StiffnessStructural() = default;

			static StiffnessStructuralPtr Create();
			StiffnessStructuralPtr GetPtr();
			ConstStiffnessStructuralPtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const override;

			void SetTemperature(IScalarCoordinatesPtr temperature);
			void SetPressure(IScalarCoordinatesPtr pressure);

		protected:
			StiffnessStructural() = default;

			Matrix FormMatrix_D(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Matrix FormMatrix_B(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;

			IScalarCoordinatesPtr temperature_{ nullptr };
			IScalarCoordinatesPtr pressure_{ nullptr };

			using std::enable_shared_from_this<StiffnessStructural>::shared_from_this;
		};

	} //namespace weakform
} //namespace dive

#endif /* DIVE_WEAKFORM_STRUCTURAL_STIFFNESS_HPP_ */