#ifndef DIVE_LOAD_NODE_HPP_
#define DIVE_LOAD_NODE_HPP_

#include "dive_load.hpp"

namespace dive
{
	namespace load
	{
		LoadNodePtr CreateLoadNode(INodePtr node, IVector3DPtr value);
		LoadNodePtr CastToLoadNode(ILoadPtr load);

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

	} //namespace load
} //namespace dive

#endif /* DIVE_LOAD_NODE_HPP_ */