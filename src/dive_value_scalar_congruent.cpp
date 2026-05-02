#include "dive_value_scalar_congruent.hpp"
#include "value_scalar_function.hpp"

namespace value
{
    Scalar GetValueScalarCoordinates(IScalarCoordinatesPtr field, const Vector& point, IElementPtr element)
    {
        Scalar res{ 0.0 };

        if (field->GetType() == value::value_scalar_coordinates_congruent)
        {
            res = std::static_pointer_cast<value::ValueScalarCoordinatesCongruent>(field)->GetValue(point, element->GetElementIndex());
        }
        else if(field->GetType() == value::value_scalar_coordinates)
        {
            res = field->GetValue(point);
        }
        else
        {
            res = field->GetValue(element->GlobalCoordinates(point));
        }

        return res;
    }

    ValueScalarCoordinatesCongruentPtr CreateValueScalarCoordinatesCongruent(IMeshPtr mesh)
    {
        return ValueScalarCoordinatesCongruent::Create(mesh);
    }
    ValueScalarCoordinatesCongruentPtr ValueScalarCoordinatesCongruent::Create(IMeshPtr mesh)
    {
        class MakeSharedEnabler : public ValueScalarCoordinatesCongruent
        {
        };

        auto res = std::make_shared<MakeSharedEnabler>();

        res->SetMesh(mesh);

        return res;
    }
    Type ValueScalarCoordinatesCongruent::GetType() const
    {
        return type_;
    }
    const String& ValueScalarCoordinatesCongruent::GetName() const
    {
        return name_;
    }
    const String& ValueScalarCoordinatesCongruent::GetKey() const
    {
        return key_;
    }
    NumberCoordinates ValueScalarCoordinatesCongruent::GetNumberCoordinates() const
    {
        return mesh_->GetNumberCoordinates();
    }
    Scalar ValueScalarCoordinatesCongruent::GetValue(const Vector& point) const
    {
        return GetValue(point, elementIndex_);
    }
    Scalar ValueScalarCoordinatesCongruent::GetValue(const Vector& point, ElementIndex elementIndex) const
    {
        const auto& element = std::dynamic_pointer_cast<dive::element::IElementMapped>(mesh_->GetElements()[elementIndex]);

        return element->u(point)(0);
    }
    IMeshPtr ValueScalarCoordinatesCongruent::GetMesh() const
    {
        return mesh_;
    }
    void ValueScalarCoordinatesCongruent::SetName(const String& name)
    {
        name_ = name;
    }
    void ValueScalarCoordinatesCongruent::SetKey(const String& key)
    {
        key_ = key;
    }
    void ValueScalarCoordinatesCongruent::SetMesh(IMeshPtr mesh)
    {
        if (mesh == nullptr)
        {
            throw std::invalid_argument("Mesh cannot be null.");
        }

        mesh_ = mesh;
    }
    void ValueScalarCoordinatesCongruent::SetElementIndex(ElementIndex elementIndex)
    {
        elementIndex_ = elementIndex;
    }
}