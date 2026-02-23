#include "dive_timer_stationary.hpp"

namespace dive
{
	namespace timer
	{
		TimerStationaryPtr CreateTimerStationary()
		{
			auto res = TimerStationary::Create();

			return res;
		}
		TimerStationaryPtr CreateTimerStationary(Tag timerTag, Scalar t0)
		{
			auto res = TimerStationary::Create();

			res->SetTag(timerTag);
			res->SetInitial(t0);

			return res;
		}
		TimerStationaryPtr TimerStationary::Create()
		{
			class MakeSharedEnabler : public TimerStationary
			{
			};

			auto res = std::make_shared<MakeSharedEnabler>();

			return res;
		}
		TimerStationaryPtr TimerStationary::GetPtr()
		{
			return std::dynamic_pointer_cast<TimerStationary>(shared_from_this());
		}
		ConstTimerStationaryPtr TimerStationary::GetPtr() const
		{
			return const_cast<TimerStationary*>(this)->GetPtr();
		}
		Scalar TimerStationary::GetCurrent() const
		{
			return t_;
		}
		Scalar TimerStationary::GetInitial() const
		{
			return t0_;
		}
		Scalar TimerStationary::GetEnd() const
		{
			return t1_;
		}
		Scalar TimerStationary::GetStepSize() const
		{
			return dt_;
		}
		Number TimerStationary::GetStepNumber() const
		{
			return counter_;
		}
		Type TimerStationary::GetType() const
		{
			return type_;
		}
		Tag TimerStationary::GetTag() const
		{
			return tag_;
		}
		void TimerStationary::SetNextStep()
		{
		}
		void TimerStationary::SetInitial(Scalar initialTime)
		{
			t_ = initialTime;
			t0_ = t_;
			t1_ = t_;
		}
		void TimerStationary::SetEnd(Scalar endTime)
		{
			t_ = endTime;
			t0_ = t_;
			t1_ = t_;
		}
		void TimerStationary::SetStepSize(Scalar stepSize)
		{
			dt_ = 0.0;
		}
		void TimerStationary::SetTag(Tag tag)
		{
			tag_ = tag;
		}
	} //namespace timer
} //namespace dive