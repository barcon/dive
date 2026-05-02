#include "dive_value_vector_congruent.hpp"
#include "dive_element.hpp"

namespace value
{
    Vector GetValueVectorCoordinates(IVectorCoordinatesPtr field, const Vector& point, IElementPtr element)
    {
        Vector res;

        if (field->GetType() == value::value_vector_coordinates_congruent)
        {
            res = std::static_pointer_cast<value::ValueVectorCoordinatesCongruent>(field)->GetValue(point, element->GetElementIndex());
        }
        else if (field->GetType() == value::value_vector_coordinates_scalars)
        {
            res = field->GetValue(point);
        }
        else
        {
            res = field->GetValue(element->GlobalCoordinates(point));
        }

        return res;
    }

    ValueVectorCoordinatesCongruentPtr CreateValueVectorCoordinatesCongruent(IMeshPtr mesh)
    {
        return ValueVectorCoordinatesCongruent::Create(mesh);
    }
    ValueVectorCoordinatesCongruentPtr ValueVectorCoordinatesCongruent::Create(IMeshPtr mesh)
    {
        class MakeSharedEnabler : public ValueVectorCoordinatesCongruent
        {
        };

        auto res = std::make_shared<MakeSharedEnabler>();

        res->SetMesh(mesh);

        return res;
    }
    Type ValueVectorCoordinatesCongruent::GetType() const
    {
        return type_;
    }
    const String& ValueVectorCoordinatesCongruent::GetName() const
    {
        return name_;
    }
    const String& ValueVectorCoordinatesCongruent::GetKey() const
    {
        return key_;
    }
    NumberCoordinates ValueVectorCoordinatesCongruent::GetNumberCoordinates() const
    {
        return mesh_->GetNumberCoordinates();
    }
    Vector ValueVectorCoordinatesCongruent::GetValue(const Vector& point) const
    {
        return GetValue(point, elementIndex_);
    }
    Vector ValueVectorCoordinatesCongruent::GetValue(const Vector& point, ElementIndex elementIndex) const
    {
        const auto& element = std::dynamic_pointer_cast<dive::element::IElementMapped>(mesh_->GetElements()[elementIndex]);

        return Vector(element->u(point), 0);
    }
    IMeshPtr ValueVectorCoordinatesCongruent::GetMesh() const
    {
        return mesh_;
    }
    void ValueVectorCoordinatesCongruent::SetName(const String& name)
    {
        name_ = name;
    }
    void ValueVectorCoordinatesCongruent::SetKey(const String& key)
    {
        key_ = key;
    }
    void ValueVectorCoordinatesCongruent::SetMesh(IMeshPtr mesh)
    {
        if(mesh == nullptr)
        {
            throw std::invalid_argument("Mesh cannot be null.");
		}
        
        mesh_ = mesh;
    }
    void ValueVectorCoordinatesCongruent::SetElementIndex(ElementIndex elementIndex)
    {
        elementIndex_ = elementIndex;
    }
}