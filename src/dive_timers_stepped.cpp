#include "dive_timers_stepped.hpp"

namespace dive
{
	namespace timers
	{
		TimerSteppedPtr CreateTimerStepped()
		{
			auto res = TimerStepped::Create();

			return res;
		}
		TimerSteppedPtr CreateTimerStepped(Tag timerTag, Scalar t0, Scalar t1, Scalar dt)
		{
			auto res = TimerStepped::Create();

			res->SetTag(timerTag);
			res->SetInitialTime(t0);
			res->SetEndTime(t1);
			res->SetStepSize(dt);

			return res;
		}
		TimerSteppedPtr TimerStepped::Create()
		{
			class MakeSharedEnabler : public TimerStepped
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		TimerSteppedPtr TimerStepped::GetPtr()
		{
			return std::dynamic_pointer_cast<TimerStepped>(shared_from_this());
		}
		ConstTimerSteppedPtr TimerStepped::GetPtr() const
		{
			return const_cast<TimerStepped*>(this)->GetPtr();
		}
		Scalar TimerStepped::GetCurrentTime() const
		{
			return t_;
		}
		Scalar TimerStepped::GetInitialTime() const
		{
			return t0_;
		}
		Scalar TimerStepped::GetEndTime() const
		{
			return t1_;
		}
		Scalar TimerStepped::GetStepSize() const
		{
			return dt_;
		}
		Number TimerStepped::GetStepNumber() const
		{
			return counter_;
		}
		Type TimerStepped::GetType() const
		{
			return type_;
		}
		Tag TimerStepped::GetTag() const
		{
			return tag_;
		}
		void TimerStepped::SetNextStep()
		{
			if (t_ == t1_)
			{
				return;
			}

			if (t_ + dt_ < t1_)
			{
				t_ = t_ + dt_;
			}
			else
			{
				t_ = t1_;
			}

			counter_++;
		}
		void TimerStepped::SetInitialTime(Scalar initialTime)
		{
			t_ = initialTime;
			t0_ = t_;
		}
		void TimerStepped::SetEndTime(Scalar endTime)
		{
			if (t1_ < t0_)
			{
				logger::Error(headerDive, "End time must be bigger than initial time (t1 > t0)");

				return;
			}

			t1_ = endTime;
		}
		void TimerStepped::SetStepSize(Scalar stepSize)
		{
			if (stepSize <= 0.0)
			{
				logger::Error(headerDive, "Step size must be > 0.0");
				return;
			}

			if (stepSize > (t1_ - t0_))
			{
				logger::Error(headerDive, "Step size bigger than final minus initial time");
				return;
			}

			dt_ = stepSize;
		}
		void TimerStepped::SetTag(Tag tag)
		{
			tag_ = tag;
		}

	} //namespace timers
} //namespace dive