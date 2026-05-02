#include "dive_value_matrix_congruent.hpp"
#include "dive_element.hpp"

namespace value
{
    Matrix GetValueMatrixCoordinates(IMatrixCoordinatesPtr field, const Vector& point, IElementPtr element)
    {
        Matrix res;

        if (field->GetType() == value::value_matrix_coordinates_congruent)
        {
            res = std::static_pointer_cast<value::ValueMatrixCoordinatesCongruent>(field)->GetValue(point, element->GetElementIndex());
        }
        else if (field->GetType() == value::value_matrix_coordinates_scalars)
        {
            res = field->GetValue(point);
        }
        else
        {
            res = field->GetValue(element->GlobalCoordinates(point));
        }

        return res;
    }

    ValueMatrixCoordinatesCongruentPtr CreateValueMatrixCoordinatesCongruent(IMeshPtr mesh)
    {
        return ValueMatrixCoordinatesCongruent::Create(mesh);
    }
    ValueMatrixCoordinatesCongruentPtr ValueMatrixCoordinatesCongruent::Create(IMeshPtr mesh)
    {
        class MakeSharedEnabler : public ValueMatrixCoordinatesCongruent
        {
        };

        auto res = std::make_shared<MakeSharedEnabler>();

        res->SetMesh(mesh);

        return res;
    }
    Type ValueMatrixCoordinatesCongruent::GetType() const
    {
        return type_;
    }
    const String& ValueMatrixCoordinatesCongruent::GetName() const
    {
        return name_;
    }
    const String& ValueMatrixCoordinatesCongruent::GetKey() const
    {
        return key_;
    }
    NumberCoordinates ValueMatrixCoordinatesCongruent::GetNumberCoordinates() const
    {
        return mesh_->GetNumberCoordinates();
    }
    Matrix ValueMatrixCoordinatesCongruent::GetValue(const Vector& point) const
    {
        return GetValue(point, elementIndex_);
    }
    Matrix ValueMatrixCoordinatesCongruent::GetValue(const Vector& point, ElementIndex elementIndex) const
    {
        const auto& element = std::dynamic_pointer_cast<dive::element::IElementMapped>(mesh_->GetElements()[elementIndex]);

        return element->u(point);
    }
    IMeshPtr ValueMatrixCoordinatesCongruent::GetMesh() const
    {
        return mesh_;
    }
    void ValueMatrixCoordinatesCongruent::SetName(const String& name)
    {
        name_ = name;
    }
    void ValueMatrixCoordinatesCongruent::SetKey(const String& key)
    {
        key_ = key;
    }
    void ValueMatrixCoordinatesCongruent::SetMesh(IMeshPtr mesh)
    {
        if (mesh == nullptr)
        {
            throw std::invalid_argument("Mesh cannot be null.");
        }

        mesh_ = mesh;
    }
    void ValueMatrixCoordinatesCongruent::SetElementIndex(ElementIndex elementIndex)
    {
        elementIndex_ = elementIndex;
    }
}