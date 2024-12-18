%inline
%{
	#include "dive_timers.hpp"
	#include "dive_timers_stationary.hpp"
	#include "dive_timers_stepped.hpp"

	typedef std::shared_ptr<dive::timers::ITimer> ITimerPtr;
	typedef std::shared_ptr<const dive::timers::ITimer> ConstITimerPtr;

	typedef std::shared_ptr<dive::timers::TimerStationary> TimerStationaryPtr;
	typedef std::shared_ptr<const dive::timers::TimerStationary> ConstTimerStationaryPtr;

	typedef std::shared_ptr<dive::timers::TimerStepped> TimerSteppedPtr;
	typedef std::shared_ptr<const dive::timers::TimerStepped> ConstTimerSteppedPtr;
%}

%shared_ptr(dive::timers::ITimer);
%shared_ptr(dive::timers::TimerStationary);
%shared_ptr(dive::timers::TimerStepped);

%include "dive_timers.hpp"
%include "dive_timers_stationary.hpp"
%include "dive_timers_stepped.hpp"