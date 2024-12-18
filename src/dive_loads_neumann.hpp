#ifndef DIVE_LOADS_NEUMANN_HPP_
#define DIVE_LOADS_NEUMANN_HPP_

#include "dive_loads.hpp"
#include "dive_problems.hpp"

namespace dive
{
	namespace loads
	{
		NeumannPtr CreateNeumann();
		NeumannPtr CreateNeumann(IProblemPtr problem, INodePtr node, DofIndex dofIndex, Scalar value);

		class Neumann : public INeumann, virtual public std::enable_shared_from_this<Neumann>
		{
		public:
			virtual ~Neumann() = default;

			static NeumannPtr Create();
			NeumannPtr GetPtr();
			ConstNeumannPtr GetPtr() const;

			virtual Type GetType() const = 0;
			virtual DofIndex GetDofIndex() const = 0;
			virtual Scalar GetValue() const = 0;
			virtual FaceIndex GetFaceIndex() const = 0;

			virtual void SetElement(IElementPtr element) = 0;
			virtual void SetDofIndex(DofIndex dofIndex) = 0;
			virtual void SetFaceIndex(FaceIndex faceIndex) = 0;

		protected:
			Neumann() = default;

			const Type type_{ load_neumann };

			INodePtr node_{ nullptr };
			DofIndex dofIndex_{ 0 };
			Scalar value_{ 0.0 };
		};

	} //namespace loads
} //namespace dive

#endif /* DIVE_LOADS_NEUMANN_HPP_ */