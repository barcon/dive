#ifndef DIVE_LOADS_HPP_
#define DIVE_LOADS_HPP_

#include "dive_types.hpp"
#include "dive_elements.hpp"

namespace dive
{
	namespace loads
	{
		static const Type load_dirichlet			= 1;
		static const Type load_distributedVolume	= 2;
		static const Type load_distributedFace		= 3;
		static const Type load_distributedEdge		= 4;
		static const Type load_forceNode			= 5;
		static const Type load_initialStrain		= 6;
		static const Type load_temperatureStrain	= 7;
		static const Type load_heatSourceVolume		= 8;
		static const Type load_heatConductionFace	= 9;
		static const Type load_couplingEquation		= 10;
		
		class IDirichlet
		{
		public:
			virtual ~IDirichlet() = default;

			virtual Type GetType() const = 0;
			virtual INodePtr GetNode() const = 0;
			virtual DofIndex GetDofIndex() const = 0;
			virtual Scalar GetValue() const = 0;

			virtual void SetNode(INodePtr node) = 0;
			virtual void SetDofIndex(DofIndex dofIndex) = 0;
			virtual void SetValue(IScalar3DPtr value) = 0;
		};

		class ILoad
		{
		public:
			virtual ~ILoad() = default;

			virtual Type GetType() const = 0;
			virtual IElementPtr GetElement() const = 0;

			virtual void SetElement(IElementPtr element) = 0;
		};
	} //namespace loads
} //namespace dive

#endif /* DIVE_LOADS_HPP_ */