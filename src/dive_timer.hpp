#ifndef DIVE_TIMER_HPP_
#define DIVE_TIMER_HPP_

#include "dive_types.hpp"

namespace dive
{
	namespace timer
	{
		static const Type timer_stationary = 1;
		static const Type timer_stepped = 2;

		class ITimer
		{
		public:
			virtual ~ITimer() = default;

			virtual Scalar GetCurrent() const = 0;
			virtual Scalar GetInitial() const = 0;
			virtual Scalar GetEnd() const = 0;
			virtual Scalar GetStepSize() const = 0;
			virtual Number GetStepNumber() const = 0;
			virtual Type GetType() const = 0;
			virtual Tag	GetTag() const = 0;

			virtual void SetNextStep() = 0;
			virtual void SetInitial(Scalar initialTime) = 0;
			virtual void SetEnd(Scalar endTime) = 0;
			virtual void SetStepSize(Scalar stepSize) = 0;
			virtual void SetTag(Tag tag) = 0;
		};

	} //namespace timer
} //namespace dive

#endif /* DIVE_TIMER_HPP_ */