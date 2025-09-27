#include "dive_values_vector_congruent.hpp"
#include "dive_elements.hpp"

namespace values
{
    Vector GetValueVector3D(IVector3DPtr field, const Vector& point, IElementPtr element)
    {
        Vector res;

        if (field->GetType() == values::value_vector3D_congruent)
        {
            res = std::static_pointer_cast<values::ValueVector3DCongruent>(field)->GetValue(point, element->GetElementIndex());
        }
        else if (field->GetType() == values::value_vector3D_scalars)
        {
            res = field->GetValue(point);
        }
        else
        {
            res = field->GetValue(element->GlobalCoordinates(point));
        }

        return res;
    }

    ValueVector3DCongruentPtr CreateValueVector3DCongruent(IMeshPtr mesh)
    {
        auto res = ValueVector3DCongruent::Create();

        res->SetMesh(mesh);

        return res;
    }
    ValueVector3DCongruentPtr CreateValueVector3DCongruent(IMeshPtr mesh, String name, String key)
    {
        auto res = ValueVector3DCongruent::Create();

        res->SetName(name);
        res->SetKey(key);
        res->SetMesh(mesh);

        return res;
    }
    ValueVector3DCongruentPtr ValueVector3DCongruent::Create()
    {
        class MakeSharedEnabler : public ValueVector3DCongruent
        {
        };

        auto res = std::make_shared<MakeSharedEnabler>();

        return res;
    }
    Type ValueVector3DCongruent::GetType() const
    {
        return type_;
    }
    const String& ValueVector3DCongruent::GetName() const
    {
        return name_;
    }
    const String& ValueVector3DCongruent::GetKey() const
    {
        return key_;
    }
    Vector ValueVector3DCongruent::GetValue(Scalar x, Scalar y, Scalar z) const
    {   
        return GetValue(x, y, z, elementIndex_);
    }
    Vector ValueVector3DCongruent::GetValue(Scalar x, Scalar y, Scalar z, ElementIndex elementIndex) const
    {
        Vector point(3);

        point(0) = x;
        point(1) = y;
        point(2) = z;

        return GetValue(point, elementIndex);
    }
    Vector ValueVector3DCongruent::GetValue(const Vector& point) const
    {
        return GetValue(point, elementIndex_);
    }
    Vector ValueVector3DCongruent::GetValue(const Vector& point, ElementIndex elementIndex) const
    {
        const auto& element = std::dynamic_pointer_cast<dive::elements::IElementMapped>(mesh_->GetElements()[elementIndex]);

        return Vector(element->u(point), 0);
    }
    IMeshPtr ValueVector3DCongruent::GetMesh() const
    {
        return mesh_;
    }
    void ValueVector3DCongruent::SetName(const String& name)
    {
        name_ = name;
    }
    void ValueVector3DCongruent::SetKey(const String& key)
    {
        key_ = key;
    }
    void ValueVector3DCongruent::SetMesh(IMeshPtr mesh)
    {
        mesh_ = mesh;
    }
    void ValueVector3DCongruent::SetElementIndex(ElementIndex elementIndex)
    {
        elementIndex_ = elementIndex;
    }
}