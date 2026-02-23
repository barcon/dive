#include "dive_timer_stepped.hpp"

namespace dive
{
	namespace timer
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
			res->SetInitial(t0);
			res->SetEnd(t1);
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
		Scalar TimerStepped::GetCurrent() const
		{
			return t_;
		}
		Scalar TimerStepped::GetInitial() const
		{
			return t0_;
		}
		Scalar TimerStepped::GetEnd() const
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
				counter_++;
			}
			else
			{
				t_ = t1_;
			}
		}
		void TimerStepped::SetInitial(Scalar initialTime)
		{
			t_ = initialTime;
			t0_ = t_;
		}
		void TimerStepped::SetEnd(Scalar endTime)
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

	} //namespace timer
} //namespace dive