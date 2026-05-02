#ifndef DIVE_VALUE_VECTOR_CONGRUENT_HPP_
#define DIVE_VALUE_VECTOR_CONGRUENT_HPP_

#include "dive_types.hpp"
#include "dive_mesh.hpp"

namespace value
{
	using IMeshPtr = dive::IMeshPtr;
	using IElementPtr = dive::IElementPtr;
	using ElementIndex = dive::ElementIndex;

	Vector GetValueVectorCoordinates(IVectorCoordinatesPtr field, const Vector& point, IElementPtr element);

	class ValueVectorCoordinatesCongruent;
	using ValueVectorCoordinatesCongruentPtr = std::shared_ptr<ValueVectorCoordinatesCongruent>;
	using ConstValueVectorCoordinatesCongruentPtr = std::shared_ptr< const ValueVectorCoordinatesCongruent >;

	ValueVectorCoordinatesCongruentPtr CreateValueVectorCoordinatesCongruent(IMeshPtr mesh);

	class ValueVectorCoordinatesCongruent : public IVectorCoordinates
	{
	public:
		virtual ~ValueVectorCoordinatesCongruent() = default;

		static ValueVectorCoordinatesCongruentPtr Create(IMeshPtr mesh);

		Type GetType() const override;
		const String& GetName() const override;
		const String& GetKey() const override;
		NumberCoordinates GetNumberCoordinates() const override;
		Vector GetValue(const Vector& point) const override;
		Vector GetValue(const Vector& point, ElementIndex elementIndex) const;
		IMeshPtr GetMesh() const;

		void SetName(const String& name) override;
		void SetKey(const String& key) override;
		void SetElementIndex(ElementIndex elementIndex);

	protected:
		ValueVectorCoordinatesCongruent() = default;

		void SetMesh(IMeshPtr mesh);

		String name_{};
		String key_{};
		IMeshPtr mesh_{ nullptr };
		ElementIndex elementIndex_{ 0 };
		
		Type type_{ value_vector_coordinates_congruent };
	};

} // namespace value

#endif /* DIVE_VALUE_VECTOR_CONGRUENT_HPP_*/