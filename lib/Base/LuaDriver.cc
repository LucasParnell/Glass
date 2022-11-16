//
// Created by lucas on 24/10/2022.
//

#include "LuaDriver.h"


static int wrap_exceptions(lua_State *L, lua_CFunction f) {
    try {
        return f(L);  // Call wrapped function and return result.
    } catch (const char *s) {  // Catch and convert exceptions.
        lua_pushstring(L, s);
    } catch (std::exception &e) {
        lua_pushstring(L, e.what());
    } catch (...) {
        lua_pushliteral(L, "caught (...)");
    }
    return lua_error(L);  // Rethrow as a Lua error.
}

void Base::LuaDriver::BeginState() {
    luaState = luaL_newstate();

    lua_pushlightuserdata(luaState, (void *) wrap_exceptions);
    luaJIT_setmode(luaState, -1, LUAJIT_MODE_WRAPCFUNC | LUAJIT_MODE_ON);
    luaL_openlibs(luaState);

    lua_pop(luaState, 1);
    luaL_dostring(luaState, "print('This is lua code')");
}
