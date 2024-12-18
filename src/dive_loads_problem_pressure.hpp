#ifndef DIVE_LOADS_PROBLEM_PRESSURE_HPP_
#define DIVE_LOADS_PROBLEM_PRESSURE_HPP_

#include "dive_loads.hpp"

namespace dive
{
	namespace loads
	{
		LoadProblemPressurePtr CreateLoadProblemPressure();

		class LoadProblemPressure : public ILoad, virtual public std::enable_shared_from_this<LoadProblemPressure>
		{
		public:
			virtual ~LoadProblemPressure() = default;

			static LoadProblemPressurePtr Create();
			LoadProblemPressurePtr GetPtr();
			ConstLoadProblemPressurePtr GetPtr() const;

			Type GetType() const override;
			IElementPtr GetElement() const override;
			DofIndex GetDofIndex() const override;
			Scalar GetValue(const Vector& point, Scalar time) const override;
			FaceIndex GetFaceIndex() const;

			void SetElement(IElementPtr element) override;
			void SetDofIndex(DofIndex dofIndex) override;
			void SetElementVelocity(IElementPtr elementVelocity);
			void SetFaceIndex(FaceIndex faceIndex);

		protected:
			LoadProblemPressure() = default;

			const Type type_{ load_problemPressure };

			DofIndex dofIndex_{ 0 };
			FaceIndex faceIndex_{ 0 };
			IElementPtr element_{ nullptr };
			IElementPtr elementVelocity_{ nullptr };
		};

	} //namespace loads
} //namespace dive

#endif /* DIVE_LOADS_PROBLEM_PRESSURE_HPP_ */