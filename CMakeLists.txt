cmake_minimum_required (VERSION 3.20)

project(server_base)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#set(CMAKE_VERBOSE_MAKEFILE on)

include (precompile)
include (project)
include (fetch_content)
include (toluapp)

fetch_content(common https://github.com/nearmeng/common.git master)
fetch_content(router_client https://github.com/nearmeng/router_client.git master)
fetch_content(db_proxy_client https://github.com/nearmeng/db_proxy_client.git master)
fetch_content(conn_proto https://github.com/nearmeng/conn_proto.git master)
fetch_content(cs_proto_common https://github.com/nearmeng/cs_proto_common.git master)

init_project()

pre_toluapp(${BASE_DIR}/src/script)

pre_project(server_base STATIC)

add_project()

post_project()

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries (${TARGET_NAME}
    mango::common
    3rd::tconnd
    mango::router_client
    mango::db_proxy_client
	mango::conn_proto
	mango::cs_proto_common
)
