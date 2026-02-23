#ifndef DIVE_VALUES_MATRIX_CONGRUENT_HPP_
#define DIVE_VALUES_MATRIX_CONGRUENT_HPP_

#include "dive_types.hpp"
#include "dive_mesh.hpp"

namespace values
{
	using IMeshPtr = dive::IMeshPtr;
	using IElementPtr = dive::IElementPtr;
	using ElementIndex = dive::ElementIndex;

	Matrix GetValueMatrix3D(IMatrix3DPtr field, const Vector& point, IElementPtr element);

	class ValueMatrix3DCongruent;
	using ValueMatrix3DCongruentPtr = std::shared_ptr<ValueMatrix3DCongruent>;
	using ConstValueMatrix3DCongruentPtr = std::shared_ptr< const ValueMatrix3DCongruent >;

	ValueMatrix3DCongruentPtr CreateValueMatrix3DCongruent(IMeshPtr mesh);

	class ValueMatrix3DCongruent : public IMatrix3D
	{
	public:
		virtual ~ValueMatrix3DCongruent() = default;

		static ValueMatrix3DCongruentPtr Create();

		Type GetType() const override;
		const String& GetName() const override;
		const String& GetKey() const override;

		Matrix GetValue(Scalar x, Scalar y, Scalar z) const override;
		Matrix GetValue(Scalar x, Scalar y, Scalar z, ElementIndex elementIndex) const;
		Matrix GetValue(const Vector& point) const override;
		Matrix GetValue(const Vector& point, ElementIndex elementIndex) const;
		IMeshPtr GetMesh() const;

		void SetName(const String& name) override;
		void SetKey(const String& key) override;
		void SetMesh(IMeshPtr mesh);
		void SetElementIndex(ElementIndex elementIndex);

	protected:
		ValueMatrix3DCongruent() = default;

		String name_{};
		String key_{};
		IMeshPtr mesh_;
		ElementIndex elementIndex_{ 0 };
		
		Type type_{ value_matrix3D_congruent };
	};

} // namespace values

#endif /* DIVE_VALUES_MATRIX_CONGRUENT_HPP_*/