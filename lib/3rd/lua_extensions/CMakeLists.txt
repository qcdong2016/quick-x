
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
	socket/buffer.c
	socket/compat.c
	socket/except.c
	socket/inet.c
	socket/io.c
	socket/luasocket.c
	socket/mime.c
	socket/options.c
	socket/select.c
	# socket/serial.c
	socket/tcp.c
	socket/timeout.c
	socket/udp.c
	# socket/unix.c
	# socket/unixdgram.c
	# socket/unixstream.c
	# socket/usocket.c
	# socket/wsocket.c
	socket/socket_scripts.c

	lua_extensions.c
)

if (MSVC)
	list(APPEND SOURCE_FILES
		socket/wsocket.c
	)
else()
	list(APPEND SOURCE_FILES
		socket/unix.c
		socket/usocket.c
		# socket/serial.c
	)
endif()

set(INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})

library(lua_extensions)
