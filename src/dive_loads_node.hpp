#ifndef DIVE_LOADS_NODE_HPP_
#define DIVE_LOADS_NODE_HPP_

#include "dive_loads.hpp"

namespace dive
{
	namespace loads
	{
		LoadNodePtr CreateLoadNode(INodePtr node, IVector3DPtr value);

		class LoadNode : public ILoadNode, virtual public std::enable_shared_from_this<LoadNode>
		{
		public:
			virtual ~LoadNode() = default;

			static LoadNodePtr Create();
			LoadNodePtr GetPtr();
			ConstLoadNodePtr GetPtr() const;

			Type GetType() const override;
			INodePtr GetNode() const override;
			Vector GetValue() const override;

			void SetNode(INodePtr node) override;
			void SetValue(IVector3DPtr value) override;

		protected:
			LoadNode() = default;

			const Type type_{ load_node };

			INodePtr node_{ nullptr };
			IVector3DPtr value_{ nullptr };
		};

	} //namespace loads
} //namespace dive

#endif /* DIVE_LOADS_NODE_HPP_ */