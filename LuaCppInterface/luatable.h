#ifndef LUATABLE_H
#define LUATABLE_H

#include <cassert>
#include <string>
#include <functional>
#include <memory>
#include "luareference.h"

template<typename SIG>
class LuaFunction;
class LuaFunctionBase;

template<typename T>
struct pusher;

template<typename T>
struct popper;

class LuaTable : public LuaReference
{
  static LuaTable EMPTY_TABLE;
  LuaTable() : LuaReference() {}

public:
	LuaTable(std::shared_ptr<lua_State> state, int index);

	template<typename OBJ>
	void Set(std::string key, const OBJ& value)
	{
		PushToStack(state.get());
		lua_pushlstring(state.get(), key.c_str(), key.size());
		pusher<OBJ>::push(state, value);
		lua_settable(state.get(), -3);
		lua_pop(state.get(), 1);
	}

	template<typename OBJ>
	void Set(int key, const OBJ& value)
	{
		PushToStack(state.get());
		lua_pushinteger(state.get(), key);
		pusher<OBJ>::push(state, value);
		lua_settable(state.get(), -3);
		lua_pop(state.get(), 1);
	}

  template<typename OBJ>
  OBJ GetEmpty() const
  {
    return OBJ();
  }

  template<>
  LuaTable& GetEmpty() const
  {
    return LuaTable::EMPTY_TABLE;
  }
  
  template<typename OBJ>
  OBJ Get(std::string key, bool* outRes = nullptr) const
  {
    PushToStack(state.get());
    lua_pushlstring(state.get(), key.c_str(), key.size());

    int luaRes = lua_gettable(state.get(), -2);
    bool keyExists = !lua_isnil(state.get(), -1);
    if (outRes)
    {
      *outRes = keyExists;
      if (!keyExists)
      {
        popper<OBJ>::pop(state);
        lua_pop(state.get(), 1);
        return GetEmpty<OBJ>();
      }
    }

    OBJ res = popper<OBJ>::pop(state);
    lua_pop(state.get(), 1);
    return res;
  }

	template<typename OBJ>
	OBJ Get(int key, bool* outRes = nullptr) const
	{
		PushToStack(state.get());
		lua_pushinteger(state.get(), key);

    int luaRes = lua_gettable(state.get(), -2);
    bool keyExists = !lua_isnil(state.get(), -1);
    if (outRes)
    {
      *outRes = keyExists;
      if (!keyExists)
      {
        lua_pop(state.get(), 1);
        return GetEmpty<OBJ>();
      }
    }

		OBJ res = popper<OBJ>::pop(state);
		lua_pop(state.get(), 1);
		return res;
	}

	// Get the type of value at a specified key
	LuaType::Value GetTypeOfValueAt(std::string key) const;
	LuaType::Value GetTypeOfValueAt(int key) const;

	// Get all the keys in the LuaTable
	void ForAllStringKeys(std::function<void(std::string, LuaType::Value)> stringKeys) const;
	void ForAllIntegerKeys(std::function<void(int, LuaType::Value)> integerKeys) const;

};

#endif // LUATABLE_H
