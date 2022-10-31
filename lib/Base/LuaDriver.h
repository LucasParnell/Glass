#pragma once

#include <exception>
#include "LuaJIT/lua.hpp"
#include "glass_export.h"

namespace Base {
class LuaDriver {
    public:
        GLASS_EXPORT void BeginState();


    private:
        lua_State *luaState;


};
}