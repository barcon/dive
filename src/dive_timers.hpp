#ifndef DIVE_TIMERS_HPP_
#define DIVE_TIMERS_HPP_

#include "dive_types.hpp"

namespace dive
{
	namespace timers
	{
		static const Type timer_stationary = 1;
		static const Type timer_stepped = 2;

		class ITimer
		{
		public:
			virtual ~ITimer() = default;

			virtual Scalar GetCurrentTime() const = 0;
			virtual Scalar GetInitialTime() const = 0;
			virtual Scalar GetEndTime() const = 0;
			virtual Scalar GetStepSize() const = 0;
			virtual Number GetStepNumber() const = 0;
			virtual Type GetType() const = 0;
			virtual Tag	GetTag() const = 0;

			virtual void SetNextStep() = 0;
			virtual void SetInitialTime(Scalar initialTime) = 0;
			virtual void SetEndTime(Scalar endTime) = 0;
			virtual void SetStepSize(Scalar stepSize) = 0;
			virtual void SetTag(Tag tag) = 0;
		};

	} //namespace timers
} //namespace dive

#endif /* DIVE_TIMERS_HPP_ */