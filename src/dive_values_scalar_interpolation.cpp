#include "dive_values_scalar_interpolation.hpp"

namespace dive
{
    namespace values
    {
        ValueScalar3DInterpolationPtr CreateValueScalar3DInterpolation(Nodes nodes, Type interpolationType)
        {
            auto res = ValueScalar3DInterpolation::Create();

            res->SetNodes(nodes, interpolationType);

            return res;
        }
        ValueScalar3DInterpolationPtr CreateValueScalar3DInterpolation(Nodes nodes, Type interpolationType, String name, String key)
        {
            auto res = ValueScalar3DInterpolation::Create();

            res->SetName(name);
            res->SetKey(key);
            res->SetNodes(nodes, interpolationType);

            return res;
        }
        ValueScalar3DInterpolationPtr ValueScalar3DInterpolation::Create()
        {
            class MakeSharedEnabler : public ValueScalar3DInterpolation
            {
            };

            auto res = std::make_shared<MakeSharedEnabler>();

            return res;
        }
        Type ValueScalar3DInterpolation::GetType() const
        {
            return type_;
        }
        const String& ValueScalar3DInterpolation::GetName() const
        {
            return name_;
        }
        const String& ValueScalar3DInterpolation::GetKey() const
        {
            return key_;
        }
        Scalar ValueScalar3DInterpolation::GetValue(Scalar x, Scalar y, Scalar z) const
        {
            auto node = nodes::CreateNode(x, y, z);

            interpolation_->GetValue(node);

            return node->GetValue()(0);
        }
        Scalar ValueScalar3DInterpolation::GetValue(const Vector& point) const
        {
            auto node = nodes::CreateNode(point);

            interpolation_->GetValue(node);

            return node->GetValue()(0);
        }
        IInterpolationPtr ValueScalar3DInterpolation::GetInterpolation() const
        {
            return interpolation_;
        }
        void ValueScalar3DInterpolation::SetName(const String& name)
        {
            name_ = name;
        }
        void ValueScalar3DInterpolation::SetKey(const String& key)
        {
            key_ = key;
        }
        void ValueScalar3DInterpolation::SetNodes(Nodes nodes, Type interpolationType)
        {
            switch (interpolationType)
            {
            case interpolation::interpolation_idw:
                interpolation_ = interpolation::CreateInterpolationIDW();
                break;
            case interpolation::interpolation_nn:
                interpolation_ = interpolation::CreateInterpolationNN();
                break;
            case interpolation::interpolation_rbf:
                interpolation_ = interpolation::CreateInterpolationRBF();
                break;
            case interpolation::interpolation_fixed:
            default:
                interpolation_ = interpolation::CreateInterpolationFixed();
                break;
            }

            if (interpolation_ != nullptr)
            {
                interpolation_->SetNodes(nodes);
            }
            else
            {
                logger::Error(valuesLogger, "Not possible to set nodes or invalid interpolation method");
            }
        }
    }
}