#ifndef DIVE_WEAKFORMS_STRUCTURAL_STIFFNESS_HPP_
#define DIVE_WEAKFORMS_STRUCTURAL_STIFFNESS_HPP_

#include "dive_weakforms.hpp"

namespace dive
{
    namespace weakforms
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

			void SetTemperature(IScalar3DPtr temperature);
			void SetPressure(IScalar3DPtr pressure);

		protected:
			StiffnessStructural() = default;

			Matrix FormMatrix_D(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Matrix FormMatrix_B(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;

			IScalar3DPtr temperature_{ nullptr };
			IScalar3DPtr pressure_{ nullptr };

			using std::enable_shared_from_this<StiffnessStructural>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_STRUCTURAL_STIFFNESS_HPP_ */