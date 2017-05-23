
set(SOURCE_FILES
	cjson/fpconv.c
	cjson/lua_cjson.c
	cjson/strbuf.c

	debugger/debugger.c

	filesystem/lfs.c

	lpack/lpack.c

	lsqlite3/lsqlite3.c
	lsqlite3/sqlite3.c

	snapshot/snapshot.c

	zlib/lua_zlib.c

	socket/auxiliar.c 
	socket/luasocket_buffer.c 
	socket/except.c 
	socket/inet.c 
	socket/luasocket_io.c 
	socket/luasocket.c 
	socket/mime.c 
	socket/options.c 
	socket/select.c 
	socket/serial.c 
	socket/socket_scripts.c 
	socket/tcp.c 
	socket/timeout.c 
	socket/udp.c 
	socket/unix.c 
	socket/usocket.c 

	lua_extensions.c
)

set(INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})

library(lua_extensions)