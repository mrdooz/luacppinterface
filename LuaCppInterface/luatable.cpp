#include "luacoroutine.h"
#include "luatable.h"
#include "luatypetemplates.h"
#include "luafunction.h"

static LuaType::Value CheckForInteger(lua_State* state, LuaType::Value valueType)
{
	if (valueType == LuaType::number)
	{
		if (lua_isinteger(state, -1))
		{
			valueType = LuaType::integer;
		}
	}
	return valueType;
}

LuaTable::LuaTable(std::shared_ptr<lua_State> state, int index) : LuaReference(state, index)
{
	assert(GetType() == LuaType::table);
}

LuaType::Value LuaTable::GetTypeOfValueAt(std::string key) const
{
	PushToStack(state.get());
	lua_pushlstring(state.get(), key.c_str(), key.size());
	lua_gettable(state.get(), -2);
	LuaType::Value res = (LuaType::Value)lua_type(state.get(), -1);
	lua_pop(state.get(), 2);
	return res;
}

LuaType::Value LuaTable::GetTypeOfValueAt(int key) const
{
	PushToStack(state.get());
	lua_pushinteger(state.get(), key);
	lua_gettable(state.get(), -2);
	LuaType::Value res = (LuaType::Value)lua_type(state.get(), -1);
	lua_pop(state.get(), 2);
	return res;
}

void LuaTable::ForAllStringKeys(std::function<void(std::string, LuaType::Value)> stringKeys) const
{
	PushToStack(state.get());
	lua_pushnil(state.get());  /* first key */

	while (lua_next(state.get(), -2) != 0)
	{
		if ((LuaType::Value)lua_type(state.get(), -2) == LuaType::string)
		{
			const char* key = lua_tostring(state.get(), -2);
			auto valueType = (LuaType::Value)lua_type(state.get(), -1);
			valueType = CheckForInteger(state.get(), valueType);
			stringKeys(key, valueType);
		}
		lua_pop(state.get(), 1);
	}
	lua_pop(state.get(), 1);
}

void LuaTable::ForAllIntegerKeys(std::function<void(int, LuaType::Value)> integerKeys) const
{
	PushToStack(state.get());
	lua_pushnil(state.get());  /* first key */

	while (lua_next(state.get(), -2) != 0)
	{
		if ((LuaType::Value)lua_type(state.get(), -2) == LuaType::number)
		{
			int key = lua_tointeger(state.get(), -2);
			auto valueType = (LuaType::Value)lua_type(state.get(), -1);
			valueType = CheckForInteger(state.get(), valueType);
			integerKeys(key, valueType);
		}
		lua_pop(state.get(), 1);
	}
	lua_pop(state.get(), 1);
}
