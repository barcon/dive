#ifndef DIVE_WEAKFORMS_PRESSURE_CROSSED_PDU_HPP_
#define DIVE_WEAKFORMS_PRESSURE_CROSSED_PDU_HPP_

#include "dive_weakforms_basic.hpp"
#include "dive_problems.hpp"

namespace dive
{
    namespace weakforms
    {
		using ProblemFluidPtr = problems::ProblemFluidPtr;

		CrossedPressure_Udp_Udp_Ptr CreateWeakFormCrossedPressure_Udp_Udp();

		class CrossedPressure_Udp : public WeakFormElement, virtual public std::enable_shared_from_this<CrossedPressure_Udp>
		{
		public:
			virtual ~CrossedPressure_Udp() = default;

			static CrossedPressure_UdpPtr Create();
			CrossedPressure_UdpPtr GetPtr();
			ConstCrossedPressure_UdpPtr GetPtr() const;

			void WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const override;

			void SetProblemMomentum(IProblemPtr problemMomentum);

		protected:
			CrossedPressure_Udp() = default;

			Matrix FormMatrix_N(IElementPtr element, const Vector& local) const;
			Matrix FormMatrix_dN(IElementPtr element, const Vector& local, CacheIndex cacheIndex) const;

			IProblemPtr problemMomentum_{ nullptr };

			using std::enable_shared_from_this<CrossedPressure_Udp>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_PRESSURE_CROSSED_PDU_HPP_ */