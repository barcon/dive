#ifndef DIVE_VALUES_SCALAR_CONGRUENT_HPP_
#define DIVE_VALUES_SCALAR_CONGRUENT_HPP_

#include "dive_types.hpp"
#include "dive_meshes.hpp"

namespace values
{
	using IMeshPtr = dive::IMeshPtr;
	using IElementPtr = dive::IElementPtr;
	using ElementIndex = dive::ElementIndex;

	Scalar GetValue(IScalar3DPtr field, const Vector& point, IElementPtr element);

	class ValueScalar3DCongruent;
	using ValueScalar3DCongruentPtr = std::shared_ptr<ValueScalar3DCongruent>;
	using ConstValueScalar3DCongruentPtr = std::shared_ptr< const ValueScalar3DCongruent >;

	ValueScalar3DCongruentPtr CreateValueScalar3DCongruent(IMeshPtr mesh);
	ValueScalar3DCongruentPtr CreateValueScalar3DCongruent(IMeshPtr mesh, String name, String key);

	class ValueScalar3DCongruent : public IScalar3D
	{
	public:
		virtual ~ValueScalar3DCongruent() = default;

		static ValueScalar3DCongruentPtr Create();

		Type GetType() const override;
		const String& GetName() const override;
		const String& GetKey() const override;

		Scalar GetValue(Scalar x, Scalar y, Scalar z) const override;
		Scalar GetValue(Scalar x, Scalar y, Scalar z, ElementIndex elementIndex) const;
		Scalar GetValue(const Vector& point) const override;
		Scalar GetValue(const Vector& point, ElementIndex elementIndex) const;
		IMeshPtr GetMesh() const;

		void SetName(const String& name) override;
		void SetKey(const String& key) override;
		void SetMesh(IMeshPtr mesh);
		void SetElementIndex(ElementIndex elementIndex);

	protected:
		ValueScalar3DCongruent() = default;

		String name_{};
		String key_{};
		IMeshPtr mesh_;
		ElementIndex elementIndex_{ 0 };
		
		Type type_{ value_scalar3D_congruent };
	};

} // namespace values

#endif /* DIVE_VALUES_SCALAR_CONGRUENT_HPP_*/