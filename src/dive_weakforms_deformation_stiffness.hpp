#ifndef DIVE_WEAKFORMS_DEFORMATION_STIFFNESS_HPP_
#define DIVE_WEAKFORMS_DEFORMATION_STIFFNESS_HPP_

#include "dive_weakforms.hpp"

namespace dive
{
    namespace weakforms
    {
		StiffnessDeformationPtr CreateWeakFormStiffnessDeformation();

		class StiffnessDeformation : public IWeakFormElement, virtual public std::enable_shared_from_this<StiffnessDeformation>
		{
		public:
			virtual ~StiffnessDeformation() = default;

			static StiffnessDeformationPtr Create();
			StiffnessDeformationPtr GetPtr();
			ConstStiffnessDeformationPtr GetPtr() const;

			void WeakFormulation(IElementMappedPtr element, const Vector& local, Matrix& output, const CacheIndex& cacheIndex) const override;

			void SetTemperature(IScalarCoordinatesPtr temperature);
			void SetPressure(IScalarCoordinatesPtr pressure);

		protected:
			StiffnessDeformation() = default;

			Matrix FormMatrix_K(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;
			Matrix FormMatrix_dN(IElementMappedPtr element, const Vector& local, const CacheIndex& cacheIndex) const;

			IScalarCoordinatesPtr temperature_{ nullptr };
			IScalarCoordinatesPtr pressure_{ nullptr };

			using std::enable_shared_from_this<StiffnessDeformation>::shared_from_this;
		};
	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_DEFORMATION_STIFFNESS_HPP_ */