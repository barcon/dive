%inline
%{
	#include "dive_problems.hpp"
	#include "dive_problems_thermal.hpp"
	#include "dive_problems_fluid.hpp"
	#include "dive_problems_pressure.hpp"

	typedef std::shared_ptr<dive::problems::IProblem> IProblemPtr;
	typedef std::shared_ptr<const dive::problems::IProblem> ConstIProblemPtr;
	
	typedef std::shared_ptr<dive::problems::ProblemFluid> ProblemFluidPtr;
	typedef std::shared_ptr<const dive::problems::ProblemFluid> ConstProblemFluidPtr;	
	
	typedef std::shared_ptr<dive::problems::ProblemPressure> ProblemPressurePtr;
	typedef std::shared_ptr<const dive::problems::ProblemPressure> ConstProblemPressurePtr;

	typedef std::shared_ptr<dive::problems::ProblemThermal> ProblemThermalPtr;
	typedef std::shared_ptr<const dive::problems::ProblemThermal> ConstProblemThermalPtr;
%}

%shared_ptr(dive::problems::IThermal);
%shared_ptr(dive::problems::IFluid);
%shared_ptr(dive::problems::IPressure);
%shared_ptr(dive::problems::IProblem);

%shared_ptr(dive::problems::ProblemThermal);
%shared_ptr(dive::problems::ProblemFluid);
%shared_ptr(dive::problems::ProblemPressure);

%include "dive_problems.hpp"
%include "dive_problems_thermal.hpp"
%include "dive_problems_fluid.hpp"
%include "dive_problems_pressure.hpp"
