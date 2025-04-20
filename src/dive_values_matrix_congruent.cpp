#include "dive_values_matrix_congruent.hpp"

namespace values
{
    Matrix GetValueMatrix3D(IMatrix3DPtr field, const Vector& point, IElementPtr element)
    {
        Matrix res;

        if (field->GetType() == values::value_matrix3D_congruent)
        {
            res = std::static_pointer_cast<values::ValueMatrix3DCongruent>(field)->GetValue(point, element->GetElementIndex());
        }
        else
        { 
            res = field->GetValue(element->GlobalCoordinates(point));
        }

        return res;
    }

    ValueMatrix3DCongruentPtr CreateValueMatrix3DCongruent(IMeshPtr mesh)
    {
        auto res = ValueMatrix3DCongruent::Create();

        res->SetMesh(mesh);

        return res;
    }
    ValueMatrix3DCongruentPtr CreateValueMatrix3DCongruent(IMeshPtr mesh, String name, String key)
    {
        auto res = ValueMatrix3DCongruent::Create();

        res->SetName(name);
        res->SetKey(key);
        res->SetMesh(mesh);

        return res;
    }
    ValueMatrix3DCongruentPtr ValueMatrix3DCongruent::Create()
    {
        class MakeSharedEnabler : public ValueMatrix3DCongruent
        {
        };

        auto res = std::make_shared<MakeSharedEnabler>();

        return res;
    }
    Type ValueMatrix3DCongruent::GetType() const
    {
        return type_;
    }
    const String& ValueMatrix3DCongruent::GetName() const
    {
        return name_;
    }
    const String& ValueMatrix3DCongruent::GetKey() const
    {
        return key_;
    }
    Matrix ValueMatrix3DCongruent::GetValue(Scalar x, Scalar y, Scalar z) const
    {   
        return GetValue(x, y, z, elementIndex_);
    }
    Matrix ValueMatrix3DCongruent::GetValue(Scalar x, Scalar y, Scalar z, ElementIndex elementIndex) const
    {
        Vector point(3);

        point(0) = x;
        point(1) = y;
        point(2) = z;

        return GetValue(point, elementIndex);
    }
    Matrix ValueMatrix3DCongruent::GetValue(const Vector& point) const
    {
        return GetValue(point, elementIndex_);
    }
    Matrix ValueMatrix3DCongruent::GetValue(const Vector& point, ElementIndex elementIndex) const
    {
        return mesh_->GetElements()[elementIndex]->u(point);
    }
    IMeshPtr ValueMatrix3DCongruent::GetMesh() const
    {
        return mesh_;
    }
    void ValueMatrix3DCongruent::SetName(const String& name)
    {
        name_ = name;
    }
    void ValueMatrix3DCongruent::SetKey(const String& key)
    {
        key_ = key;
    }
    void ValueMatrix3DCongruent::SetMesh(IMeshPtr mesh)
    {
        mesh_ = mesh;
    }
    void ValueMatrix3DCongruent::SetElementIndex(ElementIndex elementIndex)
    {
        elementIndex_ = elementIndex;
    }
}