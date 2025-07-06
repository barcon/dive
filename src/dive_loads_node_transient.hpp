#ifndef DIVE_LOADS_NODE_TRANSIENT_HPP_
#define DIVE_LOADS_NODE_TRANSIENT_HPP_

#include "dive_loads.hpp"

namespace dive
{
	namespace loads
	{
		LoadNodeTransientPtr CreateLoadNodeTransient(INodePtr node, IVector3DTimePtr value);
		LoadNodeTransientPtr CastToLoadNodeTransient(ILoadPtr load);

		class LoadNodeTransient : public ILoadNodeTransient, virtual public std::enable_shared_from_this<LoadNodeTransient>
		{
		public:
			virtual ~LoadNodeTransient() = default;

			static LoadNodeTransientPtr Create();
			LoadNodeTransientPtr GetPtr();
			ConstLoadNodeTransientPtr GetPtr() const;

			Type GetType() const override;
			INodePtr GetNode() const override;
			Vector GetValue(Scalar time) const override;

			void SetNode(INodePtr node) override;
			void SetValue(IVector3DTimePtr value) override;

		protected:
			LoadNodeTransient() = default;

			const Type type_{ load_node_transient };

			INodePtr node_{ nullptr };
			IVector3DTimePtr value_{ nullptr };
		};

	} //namespace loads
} //namespace dive

#endif /* DIVE_LOADS_NODE_TRANSIENT_HPP_ */