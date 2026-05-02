#ifndef DIVE_WEAKFORM_STRUCTURAL_MASS_HPP_
#define DIVE_WEAKFORM_STRUCTURAL_MASS_HPP_

#include "dive_weakform.hpp"

namespace dive
{
    namespace weakform
    {
		MassStructuralPtr CreateWeakFormMassStructural();

		class MassStructural : public IWeakFormElement, virtual public std::enable_shared_from_this<MassStructural>
		{
		public:
			virtual ~MassStructural() = default;

			static MassStructuralPtr Create();
			MassStructuralPtr GetPtr();
			ConstMassStructuralPtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, const Vector& point, Matrix& output, const CacheIndex& cacheIndex) const override;

			void SetTemperature(IScalarCoordinatesPtr temperature);
			void SetPressure(IScalarCoordinatesPtr pressure);

		protected:
			MassStructural() = default;

			Matrix FormMatrix_NN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Scalar FormDensity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;

			IScalarCoordinatesPtr temperature_{ nullptr };
			IScalarCoordinatesPtr pressure_{ nullptr };

			using std::enable_shared_from_this<MassStructural>::shared_from_this;
		};

	} //namespace weakform
} //namespace dive

#endif /* DIVE_WEAKFORM_STRUCTURAL_MASS_HPP_ */