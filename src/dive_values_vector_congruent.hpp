#ifndef DIVE_VALUES_VECTOR_CONGRUENT_HPP_
#define DIVE_VALUES_VECTOR_CONGRUENT_HPP_

#include "dive_types.hpp"
#include "dive_mesh.hpp"

namespace values
{
	using IMeshPtr = dive::IMeshPtr;
	using IElementPtr = dive::IElementPtr;
	using ElementIndex = dive::ElementIndex;

	Vector GetValueVector3D(IVector3DPtr field, const Vector& point, IElementPtr element);

	class ValueVector3DCongruent;
	using ValueVector3DCongruentPtr = std::shared_ptr<ValueVector3DCongruent>;
	using ConstValueVector3DCongruentPtr = std::shared_ptr< const ValueVector3DCongruent >;

	ValueVector3DCongruentPtr CreateValueVector3DCongruent(IMeshPtr mesh);

	class ValueVector3DCongruent : public IVector3D
	{
	public:
		virtual ~ValueVector3DCongruent() = default;

		static ValueVector3DCongruentPtr Create();

		Type GetType() const override;
		const String& GetName() const override;
		const String& GetKey() const override;

		Vector GetValue(Scalar x, Scalar y, Scalar z) const override;
		Vector GetValue(Scalar x, Scalar y, Scalar z, ElementIndex elementIndex) const;
		Vector GetValue(const Vector& point) const override;
		Vector GetValue(const Vector& point, ElementIndex elementIndex) const;
		IMeshPtr GetMesh() const;

		void SetName(const String& name) override;
		void SetKey(const String& key) override;
		void SetMesh(IMeshPtr mesh);
		void SetElementIndex(ElementIndex elementIndex);

	protected:
		ValueVector3DCongruent() = default;

		String name_{};
		String key_{};
		IMeshPtr mesh_;
		ElementIndex elementIndex_{ 0 };
		
		Type type_{ value_vector3D_congruent };
	};

} // namespace values

#endif /* DIVE_VALUES_VECTOR_CONGRUENT_HPP_*/