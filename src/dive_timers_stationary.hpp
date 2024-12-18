#ifndef DIVE_TIMERS_STATIONARY_HPP_
#define DIVE_TIMERS_STATIONARY_HPP_

#include "dive_timers.hpp"

namespace dive
{
	namespace timers
	{

		TimerStationaryPtr CreateTimerStationary();
		TimerStationaryPtr CreateTimerStationary(Tag timerTag, Scalar t0);

		class TimerStationary : public ITimer, virtual public std::enable_shared_from_this<TimerStationary>
		{
		public:
			virtual ~TimerStationary() = default;

			static TimerStationaryPtr Create();
			TimerStationaryPtr GetPtr();
			ConstTimerStationaryPtr GetPtr() const;

			Scalar GetCurrentTime() const override;
			Scalar GetInitialTime() const override;
			Scalar GetEndTime() const override;
			Scalar GetStepSize() const override;
			Number GetStepNumber() const override;
			Type GetType() const override;
			Tag	GetTag() const override;

			void SetNextStep() override;
			void SetInitialTime(Scalar initialTime) override;
			void SetEndTime(Scalar endTime) override;
			void SetStepSize(Scalar stepSize) override;
			void SetTag(Tag tag) override;

		protected:
			TimerStationary() = default;

			Tag tag_{ 0 };
			const Type type_{ timer_stationary };

			Scalar t_{ 0.0 };
			Scalar t0_{ 0.0 };
			Scalar t1_{ 0.0 };
			Scalar dt_{ 0.0 };

			Number counter_{ 0 };
		};

	} //namespace timers
} //namespace dive

#endif /* DIVE_TIMERS_STATIONARY_HPP_ */