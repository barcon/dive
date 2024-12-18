#ifndef DIVE_WEAKFORMS_BASIC_HPP_
#define DIVE_WEAKFORMS_BASIC_HPP_

#include "dive_weakforms.hpp"

namespace dive
{
    namespace weakforms
    {
		class WeakFormElement: public IWeakFormElement
		{
		public:
			virtual ~WeakFormElement() = default;

			void IntegrationVolume(IElementPtr element, Matrix& output) const override;

		protected:
			WeakFormElement() = default;
		};

		class WeakFormLoad: public IWeakFormLoad
		{
		public:
			virtual ~WeakFormLoad() = default;

			void IntegrationVolume(ILoadPtr load, Vector& output) const override;
			void IntegrationFace(ILoadPtr load, Vector& output) const override;
			void IntegrationEdge(ILoadPtr load, Vector& output) const override;

		protected:
			WeakFormLoad() = default;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_BASIC_HPP_ */