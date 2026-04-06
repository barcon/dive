#ifndef DIVE_LOAD_DIRICHLET_HPP_
#define DIVE_LOAD_DIRICHLET_HPP_

#include "dive_load.hpp"

namespace dive
{
	namespace load
	{
		LoadDirichletPtr CreateLoadDirichlet(INodePtr node, DofIndex dofIndex, IScalarCoordinatesPtr value);

		class LoadDirichlet : public ILoadDirichlet, virtual public std::enable_shared_from_this<LoadDirichlet>
		{
		public:
			virtual ~LoadDirichlet() = default;

			static LoadDirichletPtr Create();
			LoadDirichletPtr GetPtr();
			ConstLoadDirichletPtr GetPtr() const;

			Type GetType() const override;
			INodePtr GetNode() const override;
			DofIndex GetDofIndex() const override;
			Scalar GetValue() const override;

			void SetNode(INodePtr node) override;
			void SetDofIndex(DofIndex dofIndex) override;
			void SetValue(IScalarCoordinatesPtr value) override;

		protected:
			LoadDirichlet() = default;

			const Type type_{ load_dirichlet };

			INodePtr node_{ nullptr };
			DofIndex dofIndex_{ 0 };
			IScalarCoordinatesPtr value_{ nullptr };
		};

	} //namespace load
} //namespace dive

#endif /* DIVE_LOAD_DIRICHLET_HPP_ */