#ifndef DIVE_WEAKFORMS_STRUCTURAL_MASS_HPP_
#define DIVE_WEAKFORMS_STRUCTURAL_MASS_HPP_

#include "dive_weakforms.hpp"

namespace dive
{
    namespace weakforms
    {
		MassStructuralPtr CreateWeakFormMassStructural();

		class MassStructural : public IWeakFormElement, virtual public std::enable_shared_from_this<MassStructural>
		{
		public:
			virtual ~MassStructural() = default;

			static MassStructuralPtr Create();
			MassStructuralPtr GetPtr();
			ConstMassStructuralPtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, CacheIndex cacheIndex, const Vector& point, Matrix& output) const override;

			void SetTemperature(IScalar3DPtr temperature);
			void SetPressure(IScalar3DPtr pressure);

		protected:
			MassStructural() = default;

			Matrix FormMatrix_N(IElementMappedPtr element, const Vector& local, CacheIndex cacheIndex) const;
			Scalar FormDensity(IElementMappedPtr element, const Vector& local) const;

			IScalar3DPtr temperature_{ nullptr };
			IScalar3DPtr pressure_{ nullptr };

			using std::enable_shared_from_this<MassStructural>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_STRUCTURAL_MASS_HPP_ */