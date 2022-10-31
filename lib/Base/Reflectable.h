#pragma once

#include <cassert>
#include "refl.hpp"

// create a class to hold runtime type information
class TypeInfo {
public:

    // instances can be obtained only through calls to Get()
    template<typename T>
    static const TypeInfo &Get() {
        // here we create the singleton instance for this particular type
        static const TypeInfo ti(refl::reflect<T>());
        return ti;
    }

    const std::string &Name() const {
        return name_;
    }

private:

    // were only storing the name for demonstration purposes,
    // but this can be extended to hold other properties as well
    std::string name_;

    // given a type_descriptor, we construct a TypeInfo
    // with all the metadata we care about (currently only name)
    template<typename T, typename... Fields>
    TypeInfo(refl::type_descriptor<T> td)
            : name_(td.name) {
    }

};

class Reflectable {
public:
    virtual const TypeInfo &GetTypeInfo() const = 0;
};

// define a convenience macro to autoimplement GetTypeInfo()
#define GLASS_REFLECTABLE() \
    virtual const TypeInfo& GetTypeInfo() const override \
    { \
        return TypeInfo::Get<::refl::trait::remove_qualifiers_t<decltype(*this)>>(); \
    }


