
#include <lua.h>
#include <lauxlib.h>

#include "stdhup.h"

static int
luahup_init(lua_State *L)
{
	if (stdhup_init()) {
		lua_pushboolean(L, 0);
		lua_pushstring(L, "Failed stdhup init");
		return 2;
	}

	lua_pushboolean(L, 1);
	return 1;
}

int luaopen_luahup(lua_State *L)
{
	lua_pushcfunction(L, luahup_init);
	return 1;
}
