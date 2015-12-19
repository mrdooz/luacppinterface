#ifndef LUATYPE_H
#define LUATYPE_H

#include <lua.hpp>

namespace LuaType
{
	enum Value
	{
		none = LUA_TNONE,
		nil = LUA_TNIL,
		string = LUA_TSTRING,
		number = LUA_TNUMBER,
		thread = LUA_TTHREAD,
		boolean = LUA_TBOOLEAN,
		function = LUA_TFUNCTION,
		userdata = LUA_TUSERDATA,
		lightuserdata = LUA_TLIGHTUSERDATA,
		table = LUA_TTABLE,
		integer = LUA_NUMTAGS + 1,
		uinteger = LUA_NUMTAGS + 2,
	};
}

#endif // LUATYPE_H
