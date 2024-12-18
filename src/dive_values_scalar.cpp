#include "dive_values_scalar.hpp"

namespace dive
{
    namespace values
    {
        ValueScalarPtr CreateValueScalar(Scalar value)
        {
            auto res = ValueScalar::Create();

            res->SetValue(value);

            return res;
        }
        ValueScalarPtr CreateValueScalar(Scalar value, String name, String key)
        {
            auto res = ValueScalar::Create();

            res->SetName(name);
            res->SetKey(key);
            res->SetValue(value);

            return res;
        }
        ValueScalarPtr ValueScalar::ValueScalar::Create()
        {
            class MakeSharedEnabler : public ValueScalar
            {
            };

            auto res = std::make_shared<MakeSharedEnabler>();

            return res;
        }
        Type ValueScalar::GetType() const
        {
            return type_;
        }
        const String& ValueScalar::GetName() const
        {
            return name_;
        }
        const String& ValueScalar::GetKey() const
        {
            return key_;
        }
        Scalar ValueScalar::GetValue() const
        {
            return value_;
        }
        void ValueScalar::SetName(const String& name)
        {
            name_ = name;
        }
        void ValueScalar::SetKey(const String& key)
        {
            key_ = key;
        }
        void ValueScalar::SetValue(Scalar value)
        {
            value_ = value;
        }

        ValueScalar1DPtr CreateValueScalar1D(Scalar value)
        {
            auto res = ValueScalar1D::Create();

            res->SetValue(value);

            return res;
        }
        ValueScalar1DPtr CreateValueScalar1D(Scalar value, String name, String key)
        {
            auto res = ValueScalar1D::Create();

            res->SetName(name);
            res->SetKey(key);
            res->SetValue(value);

            return res;
        }
        ValueScalar1DPtr ValueScalar1D::ValueScalar1D::Create()
        {
            class MakeSharedEnabler : public ValueScalar1D
            {
            };

            auto res = std::make_shared<MakeSharedEnabler>();

            return res;
        }
        Type ValueScalar1D::GetType() const
        {
            return type_;
        }
        const String& ValueScalar1D::GetName() const
        {
            return name_;
        }
        const String& ValueScalar1D::GetKey() const
        {
            return key_;
        }
        Scalar ValueScalar1D::GetValue(Scalar x) const
        {
            return value_;
        }
        Scalar ValueScalar1D::GetValue(const Vector& point) const
        {
            return value_;
        }
        void ValueScalar1D::SetName(const String& name)
        {
            name_ = name;
        }
        void ValueScalar1D::SetKey(const String& key)
        {
            key_ = key;
        }
        void ValueScalar1D::SetValue(Scalar value)
        {
            value_ = value;
        }

        ValueScalar2DPtr CreateValueScalar2D(Scalar value)
        {
            auto res = ValueScalar2D::Create();

            res->SetValue(value);

            return res;
        }
        ValueScalar2DPtr CreateValueScalar2D(Scalar value, String name, String key)
        {
            auto res = ValueScalar2D::Create();

            res->SetName(name);
            res->SetKey(key);
            res->SetValue(value);

            return res;
        }
        ValueScalar2DPtr ValueScalar2D::ValueScalar2D::Create()
        {
            class MakeSharedEnabler : public ValueScalar2D
            {
            };

            auto res = std::make_shared<MakeSharedEnabler>();

            return res;
        }
        Type ValueScalar2D::GetType() const
        {
            return type_;
        }
        const String& ValueScalar2D::GetName() const
        {
            return name_;
        }
        const String& ValueScalar2D::GetKey() const
        {
            return key_;
        }
        Scalar ValueScalar2D::GetValue(Scalar x, Scalar y) const
        {
            return value_;
        }
        Scalar ValueScalar2D::GetValue(const Vector& point) const
        {
            return value_;
        }
        void ValueScalar2D::SetName(const String& name)
        {
            name_ = name;
        }
        void ValueScalar2D::SetKey(const String& key)
        {
            key_ = key;
        }
        void ValueScalar2D::SetValue(Scalar value)
        {
            value_ = value;
        }

        ValueScalar3DPtr CreateValueScalar3D(Scalar value)
        {
            auto res = ValueScalar3D::Create();

            res->SetValue(value);

            return res;
        }
        ValueScalar3DPtr CreateValueScalar3D(Scalar value, String name, String key)
        {
            auto res = ValueScalar3D::Create();

            res->SetName(name);
            res->SetKey(key);
            res->SetValue(value);

            return res;
        }
        ValueScalar3DPtr ValueScalar3D::ValueScalar3D::Create()
        {
            class MakeSharedEnabler : public ValueScalar3D
            {
            };

            auto res = std::make_shared<MakeSharedEnabler>();

            return res;
        }
        Type ValueScalar3D::GetType() const
        {
            return type_;
        }
        const String& ValueScalar3D::GetName() const
        {
            return name_;
        }
        const String& ValueScalar3D::GetKey() const
        {
            return key_;
        }
        Scalar ValueScalar3D::GetValue(Scalar x, Scalar y, Scalar z) const
        {
            return value_;
        }
        Scalar ValueScalar3D::GetValue(const Vector& point) const
        {
            return value_;
        }
        void ValueScalar3D::SetName(const String& name)
        {
            name_ = name;
        }
        void ValueScalar3D::SetKey(const String& key)
        {
            key_ = key;
        }
        void ValueScalar3D::SetValue(Scalar value)
        {
            value_ = value;
        }
    }
}
