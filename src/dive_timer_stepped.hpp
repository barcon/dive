#ifndef DIVE_TIMER_STEPPED_HPP_
#define DIVE_TIMER_STEPPED_HPP_

#include "dive_timer.hpp"

namespace dive
{
	namespace timer
	{
		TimerSteppedPtr CreateTimerStepped();
		TimerSteppedPtr CreateTimerStepped(Tag timerTag, Scalar t0, Scalar t1, Scalar dt);

		class TimerStepped : public ITimer, virtual public std::enable_shared_from_this<TimerStepped>
		{
		public:
			virtual ~TimerStepped() = default;

			static TimerSteppedPtr Create();
			TimerSteppedPtr GetPtr();
			ConstTimerSteppedPtr GetPtr() const;

			Scalar GetCurrent() const override;
			Scalar GetInitial() const override;
			Scalar GetEnd() const override;
			Scalar GetStepSize() const override;
			Number GetStepNumber() const override;
			Type GetType() const override;
			Tag	GetTag() const override;

			void SetNextStep() override;
			void SetInitial(Scalar initialTime) override;
			void SetEnd(Scalar endTime) override;
			void SetStepSize(Scalar stepSize) override;
			void SetTag(Tag tag) override;

		protected:
			TimerStepped() = default;

			Tag tag_{ 0 };
			const Type type_{ timer_stepped };

			Scalar t_{ 0.0 };
			Scalar t0_{ 0.0 };
			Scalar t1_{ 1.0 };
			Scalar dt_{ 0.01 };

			Number counter_{ 0 };
		};

	} //namespace timer
} //namespace dive

#endif /* DIVE_TIMER_STEPPED_HPP_ */