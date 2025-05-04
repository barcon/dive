#ifndef DIVE_WEAKFORMS_STRUCTURAL_LOAD_DISTRIBUTED_FACE_HPP_
#define DIVE_WEAKFORMS_STRUCTURAL_LOAD_DISTRIBUTED_FACE_HPP_

#include "dive_weakforms.hpp"

namespace dive
{
    namespace weakforms
    {
		LoadDistributedFaceStructuralPtr CreateWeakFormLoadDistributedFaceStructural();

		class LoadDistributedFaceStructural : public IWeakFormLoad, virtual public std::enable_shared_from_this<LoadDistributedFaceStructural>
		{
		public:
			virtual ~LoadDistributedFaceStructural() = default;

			static LoadDistributedFaceStructuralPtr Create();
			LoadDistributedFaceStructuralPtr GetPtr();
			ConstLoadDistributedFaceStructuralPtr GetPtr() const;

			void WeakFormulation(ILoadPtr load, const Vector& point, Vector& output) const override;

		protected:
			LoadDistributedFaceStructural() = default;

			Matrix FormMatrix_N(IElementMappedPtr element, const Vector& point) const;

			using std::enable_shared_from_this<LoadDistributedFaceStructural>::shared_from_this;
		};

	} //namespace weakforms
} //namespace dive

#endif /* DIVE_WEAKFORMS_STRUCTURAL_LOAD_DISTRIBUTED_FACE_HPP_ */