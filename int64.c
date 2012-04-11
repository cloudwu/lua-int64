#include <lua.h>
#include <lauxlib.h>
#include <stdint.h>
#include <math.h>

static int64_t
_int64(lua_State *L, int index) {
	int type = lua_type(L,index);
	int64_t n = 0;
	switch(type) {
	case LUA_TNUMBER: {
		lua_Number d = lua_tonumber(L,index);
		n = (int64_t)d;
		break;
	}
	case LUA_TSTRING: {
		size_t len = 0;
		uint8_t * str = (uint8_t *)lua_tolstring(L, index, &len);
		int i = 0;
		for (i=0;i<(int)len;i++) {
			n = (n << 8) | str[i];
		}
		break;
	}
	case LUA_TLIGHTUSERDATA: {
		void * p = lua_touserdata(L,index);
		n = (intptr_t)p;
		break;
	}
	default:
		return luaL_error(L, "argument 1 error type %s",  lua_typename(L,1));
	}
	return n;
}

static void
_pushint64(lua_State *L, int64_t n) {
	void * p = (void *)(intptr_t)n;
	lua_pushlightuserdata(L,p);
}

static int
int64_add(lua_State *L) {
	int64_t a = _int64(L,1);
	int64_t b = _int64(L,2);
	_pushint64(L, a+b);
	
	return 1;
}

static int
int64_sub(lua_State *L) {
	int64_t a = _int64(L,1);
	int64_t b = _int64(L,2);
	_pushint64(L, a-b);
	
	return 1;
}

static int
int64_mul(lua_State *L) {
	int64_t a = _int64(L,1);
	int64_t b = _int64(L,2);
	_pushint64(L, a * b);
	
	return 1;
}

static int
int64_div(lua_State *L) {
	int64_t a = _int64(L,1);
	int64_t b = _int64(L,2);
	if (b == 0) {
		return luaL_error(L, "div by zero");
	}
	_pushint64(L, a / b);
	
	return 1;
}

static int
int64_mod(lua_State *L) {
	int64_t a = _int64(L,1);
	int64_t b = _int64(L,2);
	if (b == 0) {
		return luaL_error(L, "mod by zero");
	}
	_pushint64(L, a % b);
	
	return 1;
}

static int64_t
_pow64(int64_t a, int64_t b) {
	if (b == 1) {
		return a;
	}
	int64_t a2 = a * a;
	if (b % 2 == 1) {
		return _pow64(a2, b/2) * a;
	} else {
		return _pow64(a2, b/2);
	}
}

static int
int64_pow(lua_State *L) {
	int64_t a = _int64(L,1);
	int64_t b = _int64(L,2);
	int64_t p;
	if (b > 0) {
		p = _pow64(a,b);
	} else if (b == 0) {
		p = 1;
	} else {
		return luaL_error(L, "pow by nagtive number %d",(int)b);
	} 
	_pushint64(L, p);

	return 1;
}

static int
int64_unm(lua_State *L) {
	int64_t a = _int64(L,1);
	_pushint64(L, -a);
	return 1;
}

static int
int64_new(lua_State *L) {
	if (lua_gettop(L) == 0) {
		lua_pushlightuserdata(L,NULL);
		return 1;
	}
	int64_t n = _int64(L,1);
	_pushint64(L,n);
	return 1;
}

static int
int64_eq(lua_State *L) {
	int64_t a = _int64(L,1);
	int64_t b = _int64(L,2);
	printf("%s %s\n",lua_typename(L,1),lua_typename(L,2));
	printf("%ld %ld\n",a,b);
	lua_pushboolean(L,a == b);
	return 1;
}

static int
int64_lt(lua_State *L) {
	int64_t a = _int64(L,1);
	int64_t b = _int64(L,2);
	lua_pushboolean(L,a < b);
	return 1;
}

static int
int64_le(lua_State *L) {
	int64_t a = _int64(L,1);
	int64_t b = _int64(L,2);
	lua_pushboolean(L,a <= b);
	return 1;
}

static int
int64_len(lua_State *L) {
	int64_t a = _int64(L,1);
	lua_pushnumber(L,(lua_Number)a);
	return 1;
}

static void
make_mt(lua_State *L) {
	luaL_Reg lib[] = {
		{ "__add", int64_add },
		{ "__sub", int64_sub },
		{ "__mul", int64_mul },
		{ "__div", int64_div },
		{ "__mod", int64_mod },
		{ "__unm", int64_unm },
		{ "__pow", int64_pow },
		{ "__eq", int64_eq },
		{ "__lt", int64_lt },
		{ "__le", int64_le },
		{ "__len", int64_len },
		{ NULL, NULL },
	};
	luaL_newlib(L,lib);
}

int
luaopen_int64(lua_State *L) {
	if (sizeof(intptr_t)!=sizeof(int64_t)) {
		return luaL_error(L, "Only support 64bit architecture");
	}
	lua_pushlightuserdata(L,NULL);
	make_mt(L);
	lua_setmetatable(L,-2);
	lua_pop(L,1);

	lua_pushcfunction(L, int64_new);	
	return 1;
}

