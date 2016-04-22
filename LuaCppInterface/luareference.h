#ifndef LUAREFERENCE_H
#define LUAREFERENCE_H

#include <memory>
#include "luatype.h"
#include "lua.hpp"

class LuaTable;
class LuaReference
{
  enum { INVALID_REF = -1 };
  int ref = INVALID_REF;

protected:
	std::shared_ptr<lua_State> state;
  LuaReference();

  static LuaReference EMPTY_REF;

public:
	LuaReference(std::shared_ptr<lua_State> state, int index);
	LuaReference(const LuaReference& other);
	LuaReference& operator=(const LuaReference& other);
	virtual ~LuaReference();

	// push the object to the stack you are working on
	void PushToStack(lua_State* currentState) const;
	LuaType::Value GetType() const;
	std::shared_ptr<lua_State> GetState();
	LuaTable GetMetaTable() const;

  bool IsValid() const { return ref != INVALID_REF; }
};

#endif // LUAREFERENCE_H
