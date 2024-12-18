#ifndef DIVE_WEAKFORMS_HPP_
#define DIVE_WEAKFORMS_HPP_

#include "dive_types.hpp"
#include "dive_elements.hpp"
#include "dive_loads.hpp"

namespace dive
{
    namespace weakforms
    {
		class IWeakFormElement
		{
		public:
			virtual ~IWeakFormElement() = default;

			virtual void WeakFormulation(IElementPtr element, CacheIndex cacheIndex, const Vector& local, Matrix& output) const = 0;
			virtual void IntegrationVolume(IElementPtr element, Matrix& output) const = 0;
		};

		class IWeakFormLoad
		{
		public:
			virtual ~IWeakFormLoad() = default;

			virtual void WeakFormulation(ILoadPtr load, const Vector& local, Vector& output) const = 0;
			virtual void IntegrationVolume(ILoadPtr load, Vector& output) const = 0;
			virtual void IntegrationFace(ILoadPtr load, Vector& output) const = 0;
			virtual void IntegrationEdge(ILoadPtr load, Vector& output) const = 0;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_HPP_ */