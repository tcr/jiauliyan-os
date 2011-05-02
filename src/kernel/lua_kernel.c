#include <stddef.h>
#include <ctype.h>
#include <stream.h>
#include <stdlib.h>
#include <serial.h>
#include <vga.h>

#include "../../lua-5.1/src/lua.h"
#include "../../lua-5.1/src/lauxlib.h"

/*
 * trim
 * based on http://lua-users.org/lists/lua-l/2009-12/msg00951.html from Sean Conner
 */

int lua_trim(lua_State *L)
{
	const char *front;
	const char *end;
	size_t      size;

	front = luaL_checklstring(L,1,&size);
	end   = &front[size - 1];

	for ( ; size && isspace(*front) ; size-- , front++)
	;
	for ( ; size && isspace(*end) ; size-- , end--)
	;

	lua_pushlstring(L,front,(size_t)(end - front) + 1);
	return 1;
}

/*
 * serial functions
 */

int lua_serial_send(lua_State *L)
{
	const char *str;
	size_t      len;
	size_t		i;

	str = luaL_checklstring(L,1,&len);
	for (i = 0; i < len; i++) {
		serialout->put(serialout, str[i]);
	}

	return 0;
}

int lua_serial_receive_all(lua_State *L)
{
	long int len = serialin->avail(serialin);
	unsigned char *str = malloc(len);
	long int i;
	
	for (i = 0; i < len; i++) {
		str[i] = serialin->get(serialin);
	}
	
	lua_pushlstring(L, (char *) str, (size_t) len);
	return 1;
}

/*
 * vga functions
 */

int lua_vga_setfg(lua_State *L)
{
	int color = (int) lua_tonumber(L, -1);
	if (color >= 0x0 && color <= 0xF) {
		vga_setfg(color);
	}
	return 0;
}

int lua_vga_setbg(lua_State *L)
{
	int color = (int) lua_tonumber(L, -1);
	if (color >= 0x0 && color <= 0xF) {
		vga_setbg(color);
	}
	return 0;
}

int lua_vga_placechar(lua_State *L)
{
	const char *str;
	size_t      len;

	str = luaL_checklstring(L, 1, &len);
	int x = (int) lua_tonumber(L, 2);
	int y = (int) lua_tonumber(L, 3);
	vga_placechar(str[0], x, y);
	return 0;
}

/*
 * initializer
 */

static void lua_setfield_number(lua_State *L, const char *name, double number)
{
	lua_pushstring(L, name);
    lua_pushnumber(L, number);
	lua_settable(L, -3);
}

static void lua_setfield_function(lua_State *L, const char *name, void *func)
{
	lua_pushstring(L, name);
    lua_pushcfunction(L, func);
	lua_settable(L, -3);
}

int luaopen_kernel(lua_State *L)
{
	lua_register(L,"trim",lua_trim);
	
	/* serial binding */
    lua_newtable(L);
	lua_setfield_function(L, "send", lua_serial_send);
	lua_setfield_function(L, "receive_all", lua_serial_receive_all);
	lua_setglobal(L, "serial");
	
	/* vga binding */
    lua_newtable(L);
	lua_setfield_function(L, "placechar", lua_vga_placechar);
	lua_setfield_function(L, "setfg", lua_vga_setfg);
	lua_setfield_function(L, "setbg", lua_vga_setbg);
	lua_setfield_number(L, "BLACK", BLACK);
	lua_setfield_number(L, "BLUE", BLUE);
	lua_setfield_number(L, "GREEN", GREEN);
	lua_setfield_number(L, "CYAN", CYAN);
	lua_setfield_number(L, "RED", RED);
	lua_setfield_number(L, "MAGENTA", MAGENTA);
	lua_setfield_number(L, "BROWN", BROWN);
	lua_setfield_number(L, "LIGHT_GREY", LIGHT_GREY);
	lua_setfield_number(L, "DARK_GREY", DARK_GREY);
	lua_setfield_number(L, "LIGHT_BLUE", LIGHT_BLUE);
	lua_setfield_number(L, "LIGHT_GREEN", LIGHT_GREEN);
	lua_setfield_number(L, "LIGHT_CYAN", LIGHT_CYAN);
	lua_setfield_number(L, "LIGHT_RED", LIGHT_RED);
	lua_setfield_number(L, "LIGHT_MAGENTA", LIGHT_MAGENTA);
	lua_setfield_number(L, "LIGHT_BROWN", LIGHT_BROWN);
	lua_setfield_number(L, "WHITE", WHITE);
	lua_setglobal(L, "vga");
	
	return 0;
}