#ifndef DIVE_VALUES_SCALAR_CONGRUENT_HPP_
#define DIVE_VALUES_SCALAR_CONGRUENT_HPP_

#include "dive_types.hpp"
#include "dive_mesh.hpp"

namespace values
{
	using IMeshPtr = dive::IMeshPtr;
	using IElementPtr = dive::IElementPtr;
	using ElementIndex = dive::ElementIndex;

	Scalar GetValueScalarCoordinates(IScalarCoordinatesPtr field, const Vector& point, IElementPtr element);

	class ValueScalarCoordinatesCongruent;
	using ValueScalarCoordinatesCongruentPtr = std::shared_ptr<ValueScalarCoordinatesCongruent>;
	using ConstValueScalarCoordinatesCongruentPtr = std::shared_ptr< const ValueScalarCoordinatesCongruent >;

	ValueScalarCoordinatesCongruentPtr CreateValueScalarCoordinatesCongruent(IMeshPtr mesh);

	class ValueScalarCoordinatesCongruent : public IScalarCoordinates
	{
	public:
		virtual ~ValueScalarCoordinatesCongruent() = default;

		static ValueScalarCoordinatesCongruentPtr Create(IMeshPtr mesh);

		Type GetType() const override;
		const String& GetName() const override;
		const String& GetKey() const override;
		NumberCoordinates GetNumberCoordinates() const override;
		Scalar GetValue(const Vector& point) const override;
		Scalar GetValue(const Vector& point, ElementIndex elementIndex) const;
		IMeshPtr GetMesh() const;

		void SetName(const String& name) override;
		void SetKey(const String& key) override;
		void SetElementIndex(ElementIndex elementIndex);

	protected:
		ValueScalarCoordinatesCongruent() = default;

		void SetMesh(IMeshPtr mesh);

		String name_{};
		String key_{};
		IMeshPtr mesh_{ nullptr };
		ElementIndex elementIndex_{ 0 };
		
		Type type_{ value_scalar_coordinates_congruent };
	};

} // namespace values

#endif /* DIVE_VALUES_SCALAR_CONGRUENT_HPP_*/