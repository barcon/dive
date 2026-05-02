#ifndef DIVE_VALUE_MATRIX_CONGRUENT_HPP_
#define DIVE_VALUE_MATRIX_CONGRUENT_HPP_

#include "dive_types.hpp"
#include "dive_mesh.hpp"

namespace value
{
	using IMeshPtr = dive::IMeshPtr;
	using IElementPtr = dive::IElementPtr;
	using ElementIndex = dive::ElementIndex;

	Matrix GetValueMatrixCoordinates(IMatrixCoordinatesPtr field, const Vector& point, IElementPtr element);

	class ValueMatrixCoordinatesCongruent;
	using ValueMatrixCoordinatesCongruentPtr = std::shared_ptr<ValueMatrixCoordinatesCongruent>;
	using ConstValueMatrixCoordinatesCongruentPtr = std::shared_ptr< const ValueMatrixCoordinatesCongruent >;

	ValueMatrixCoordinatesCongruentPtr CreateValueMatrixCoordinatesCongruent(IMeshPtr mesh);

	class ValueMatrixCoordinatesCongruent : public IMatrixCoordinates
	{
	public:
		virtual ~ValueMatrixCoordinatesCongruent() = default;

		static ValueMatrixCoordinatesCongruentPtr Create(IMeshPtr mesh);

		Type GetType() const override;
		const String& GetName() const override;
		const String& GetKey() const override;
		NumberCoordinates GetNumberCoordinates() const override;
		Matrix GetValue(const Vector& point) const override;
		Matrix GetValue(const Vector& point, ElementIndex elementIndex) const;
		IMeshPtr GetMesh() const;

		void SetName(const String& name) override;
		void SetKey(const String& key) override;
		void SetElementIndex(ElementIndex elementIndex);

	protected:
		ValueMatrixCoordinatesCongruent() = default;

		void SetMesh(IMeshPtr mesh);

		String name_{};
		String key_{};
		IMeshPtr mesh_{ nullptr };
		ElementIndex elementIndex_{ 0 };
		
		Type type_{ value_matrix_coordinates_congruent };
	};

} // namespace value

#endif /* DIVE_VALUE_MATRIX_CONGRUENT_HPP_*/