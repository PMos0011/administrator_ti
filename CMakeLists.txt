cmake_minimum_required(VERSION 2.8.9)
project(TI)

include_directories(src/include)
file(GLOB SOURCES "src/*.cpp")

link_libraries( -lzip)
link_libraries( -lstdc++fs)

add_executable(Administrator_TI.c ${SOURCES})


find_path( MYSQL_INCLUDE_DIR
		NAMES "mysql.h"
		PATHS "/usr/include/mysql"
			  "/usr/local/include/mysql"
			  "/usr/mysql/include/mysql" )
	
	find_library( MYSQL_LIBRARY
		NAMES "mysqlclient" "mysqlclient_r"
		PATHS "/lib/mysql"
			  "/lib64/mysql"
			  "/usr/lib/mysql"
			  "/usr/lib64/mysql"
			  "/usr/local/lib/mysql"
			  "/usr/local/lib64/mysql"
			  "/usr/mysql/lib/mysql"
			  "/usr/mysql/lib64/mysql" )


target_link_libraries (Administrator_TI.c ${MYSQL_LIBRARY})

