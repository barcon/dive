#ifndef DIVE_WEAKFORMS_STRUCTURAL_MASS_LUMPED_HPP_
#define DIVE_WEAKFORMS_STRUCTURAL_MASS_LUMPED_HPP_

#include "dive_weakforms.hpp"

namespace dive
{
    namespace weakforms
    {
		MassLumpedStructuralPtr CreateWeakFormMassLumpedStructural();

		class MassLumpedStructural : public IWeakFormElement, virtual public std::enable_shared_from_this<MassLumpedStructural>
		{
		public:
			virtual ~MassLumpedStructural() = default;

			static MassLumpedStructuralPtr Create();
			MassLumpedStructuralPtr GetPtr();
			ConstMassLumpedStructuralPtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, const Vector& point, Matrix& output, const CacheIndex& cacheIndex) const override;

			void SetTemperature(IScalar3DPtr temperature);
			void SetPressure(IScalar3DPtr pressure);

		protected:
			MassLumpedStructural() = default;

			Matrix FormMatrix_NN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Scalar FormDensity(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;

			IScalar3DPtr temperature_{ nullptr };
			IScalar3DPtr pressure_{ nullptr };

			using std::enable_shared_from_this<MassLumpedStructural>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_STRUCTURAL_MASS_LUMPED_HPP_ */