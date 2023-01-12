// Generated by the Waffle++ code generator. DO NOT EDIT!
// source: structs.h

#include <memory>
#include <optional>

#include "structs.h"

namespace Waffle {

// ------------- declare poly_obj ------------- //
template<typename T>
class poly_obj;

template<>
class poly_obj<model::Robot> {
public:
    template<typename Object>
    poly_obj(Object&& object)
        : object_ptr_{std::make_unique<object_impl<Object>>(std::move(object))}
    {}

    void Forward(double distance) { return object_ptr_->Forward(distance); };
    void Turn(double degrees) { return object_ptr_->Turn(degrees); };
    void GoTo(double x, double y) { return object_ptr_->GoTo(x, y); };
    double GetX() const { return object_ptr_->GetX(); };
    double GetY() const { return object_ptr_->GetY(); };

private:
    struct object_interface {
        virtual ~object_interface() = default;

        virtual void Forward(double distance) = 0;
        virtual void Turn(double degrees) = 0;
        virtual void GoTo(double x, double y) = 0;
        virtual double GetX() const = 0;
        virtual double GetY() const = 0;
    };

    template<typename Object>
    struct object_impl : object_interface {
        object_impl(Object&& object) : object_{std::move(object)} {}
        Object object_;

        void Forward(double distance) override { return object_.Forward(distance); };
        void Turn(double degrees) override { return object_.Turn(degrees); };
        void GoTo(double x, double y) override { return object_.GoTo(x, y); };
        double GetX() const override { return object_.GetX(); };
        double GetY() const override { return object_.GetY(); };
    };

    std::unique_ptr<object_interface> object_ptr_;
};

template<>
class poly_obj<model::Stringer> {
public:
    template<typename Object>
    poly_obj(Object&& object)
        : object_ptr_{std::make_unique<object_impl<Object>>(std::move(object))}
    {}

    std::string String() const { return object_ptr_->String(); };

private:
    struct object_interface {
        virtual ~object_interface() = default;

        virtual std::string String() const = 0;
    };

    template<typename Object>
    struct object_impl : object_interface {
        object_impl(Object&& object) : object_{std::move(object)} {}
        Object object_;

        std::string String() const override { return object_.String(); };
    };

    std::unique_ptr<object_interface> object_ptr_;
};

// ------------- declare poly_ref ------------- //
template<typename T>
class poly_ref;

template<>
class poly_ref<model::Robot> {
public:
    template<typename Object>
    poly_ref(Object& object)
        : object_ptr_{std::make_unique<object_impl<Object>>(object)}
    {}

    void Forward(double distance) { return object_ptr_->Forward(distance); };
    void Turn(double degrees) { return object_ptr_->Turn(degrees); };
    void GoTo(double x, double y) { return object_ptr_->GoTo(x, y); };
    double GetX() const { return object_ptr_->GetX(); };
    double GetY() const { return object_ptr_->GetY(); };

private:
    struct object_interface {
        virtual ~object_interface() = default;

        virtual void Forward(double distance) = 0;
        virtual void Turn(double degrees) = 0;
        virtual void GoTo(double x, double y) = 0;
        virtual double GetX() const = 0;
        virtual double GetY() const = 0;
    };

    template<typename Object>
    struct object_impl : object_interface {
        object_impl(Object& object) : object_{object} {}
        Object& object_;

        void Forward(double distance) override { return object_.Forward(distance); };
        void Turn(double degrees) override { return object_.Turn(degrees); };
        void GoTo(double x, double y) override { return object_.GoTo(x, y); };
        double GetX() const override { return object_.GetX(); };
        double GetY() const override { return object_.GetY(); };
    };

    std::unique_ptr<object_interface> object_ptr_;
};

template<>
class poly_ref<model::Stringer> {
public:
    template<typename Object>
    poly_ref(Object& object)
        : object_ptr_{std::make_unique<object_impl<Object>>(object)}
    {}

    std::string String() const { return object_ptr_->String(); };

private:
    struct object_interface {
        virtual ~object_interface() = default;

        virtual std::string String() const = 0;
    };

    template<typename Object>
    struct object_impl : object_interface {
        object_impl(Object& object) : object_{object} {}
        Object& object_;

        std::string String() const override { return object_.String(); };
    };

    std::unique_ptr<object_interface> object_ptr_;
};

// ------------- declare const_poly_ref ------------- //
template<typename T>
class const_poly_ref;

template<>
class const_poly_ref<model::Robot> {
public:
    template<typename Object>
    const_poly_ref(const Object& object)
        : object_ptr_{std::make_unique<object_impl<Object>>(object)}
    {}

    double GetX() const { return object_ptr_->GetX(); };
    double GetY() const { return object_ptr_->GetY(); };

private:
    struct object_interface {
        virtual ~object_interface() = default;

        virtual double GetX() const = 0;
        virtual double GetY() const = 0;
    };

    template<typename Object>
    struct object_impl : object_interface {
        object_impl(const Object& object) : object_{object} {}
        const Object& object_;

        double GetX() const override { return object_.GetX(); };
        double GetY() const override { return object_.GetY(); };
    };

    const std::unique_ptr<const object_interface> object_ptr_;
};

template<>
class const_poly_ref<model::Stringer> {
public:
    template<typename Object>
    const_poly_ref(const Object& object)
        : object_ptr_{std::make_unique<object_impl<Object>>(object)}
    {}

    std::string String() const { return object_ptr_->String(); };

private:
    struct object_interface {
        virtual ~object_interface() = default;

        virtual std::string String() const = 0;
    };

    template<typename Object>
    struct object_impl : object_interface {
        object_impl(const Object& object) : object_{object} {}
        const Object& object_;

        std::string String() const override { return object_.String(); };
    };

    const std::unique_ptr<const object_interface> object_ptr_;
};

// ------------- declare poly_ptr ------------- //
template<typename T>
class poly_ptr {
public:
    poly_ptr() = default;
    poly_ptr(std::nullptr_t) {};

    template<typename Object>
    poly_ptr(Object* object) {
        if (object != nullptr) {
            ref_.emplace(*object);
        }
    }

    poly_ref<T>& operator*() {
        return ref_.value();
    }

    poly_ref<T>* operator->() {
        return &ref_.value();
    }

private:
    std::optional<poly_ref<T>> ref_;
};

// ------------- declare const_poly_ptr ------------- //
template<typename T>
class const_poly_ptr {
public:
    const_poly_ptr() = default;
    const_poly_ptr(std::nullptr_t) {};

    template<typename Object>
    const_poly_ptr(const Object* object) {
        if (object != nullptr) {
            ref_.emplace(*object);
        }
    }

    const_poly_ref<T>& operator*() {
        return ref_.value();
    }

    const_poly_ref<T>* operator->() {
        return &ref_.value();
    }

private:
    std::optional<const_poly_ref<T>> ref_;
};

} // namespace Waffle
