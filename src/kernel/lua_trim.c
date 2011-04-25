/* trim.c - based on http://lua-users.org/lists/lua-l/2009-12/msg00951.html
            from Sean Conner */
#include <stddef.h>
#include <ctype.h>
#include <stream.h>
#include <stdlib.h>
#include <serial.h>
#include "../../lua-5.1/src/lua.h"
#include "../../lua-5.1/src/lauxlib.h"

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

int luaopen_trim(lua_State *L)
{
	lua_register(L,"trim",lua_trim);
	lua_register(L,"serial_send",lua_serial_send);
	lua_register(L,"serial_receive_all",lua_serial_receive_all);
	return 0;
}