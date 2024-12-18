#include "dive_values_scalar_congruent.hpp"
#include "values_scalar_function.hpp"

namespace values
{
    Scalar GetValue(IScalar3DPtr field, const Vector& point, IElementPtr element)
    {
        Scalar res{ 0.0 };

        switch (field->GetType())
        {
        case values::value_scalar3D_congruent:
            res = std::static_pointer_cast<values::ValueScalar3DCongruent>(field)->GetValue(point, element->GetElementIndex());
            break;
        default:
            res = field->GetValue(element->GlobalCoordinates(point));
        }

        return res;
    }

    ValueScalar3DCongruentPtr CreateValueScalar3DCongruent(IMeshPtr mesh)
    {
        auto res = ValueScalar3DCongruent::Create();

        res->SetMesh(mesh);

        return res;
    }
    ValueScalar3DCongruentPtr CreateValueScalar3DCongruent(IMeshPtr mesh, String name, String key)
    {
        auto res = ValueScalar3DCongruent::Create();

        res->SetName(name);
        res->SetKey(key);
        res->SetMesh(mesh);

        return res;
    }
    ValueScalar3DCongruentPtr ValueScalar3DCongruent::Create()
    {
        class MakeSharedEnabler : public ValueScalar3DCongruent
        {
        };

        auto res = std::make_shared<MakeSharedEnabler>();

        return res;
    }
    Type ValueScalar3DCongruent::GetType() const
    {
        return type_;
    }
    const String& ValueScalar3DCongruent::GetName() const
    {
        return name_;
    }
    const String& ValueScalar3DCongruent::GetKey() const
    {
        return key_;
    }
    Scalar ValueScalar3DCongruent::GetValue(Scalar x, Scalar y, Scalar z) const
    {   
        return GetValue(x, y, z, elementIndex_);
    }
    Scalar ValueScalar3DCongruent::GetValue(Scalar x, Scalar y, Scalar z, ElementIndex elementIndex) const
    {
        Vector point(3);

        point(0) = x;
        point(1) = y;
        point(2) = z;

        return GetValue(point, elementIndex);
    }
    Scalar ValueScalar3DCongruent::GetValue(const Vector& point) const
    {
        return GetValue(point, elementIndex_);
    }
    Scalar ValueScalar3DCongruent::GetValue(const Vector& point, ElementIndex elementIndex) const
    {
        return mesh_->GetElements()[elementIndex]->u(point)(0);
    }
    IMeshPtr ValueScalar3DCongruent::GetMesh() const
    {
        return mesh_;
    }
    void ValueScalar3DCongruent::SetName(const String& name)
    {
        name_ = name;
    }
    void ValueScalar3DCongruent::SetKey(const String& key)
    {
        key_ = key;
    }
    void ValueScalar3DCongruent::SetMesh(IMeshPtr mesh)
    {
        mesh_ = mesh;
    }
    void ValueScalar3DCongruent::SetElementIndex(ElementIndex elementIndex)
    {
        elementIndex_ = elementIndex;
    }
}