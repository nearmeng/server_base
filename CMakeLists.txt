cmake_minimum_required (VERSION 3.20)

project(server_base)

include (FetchContent)
FetchContent_Declare(cmake_tool GIT_REPOSITORY https://github.com/nearmeng/cmake_tools.git GIT_TAG master)
FetchContent_MakeAvailable(cmake_tool)

fetch_content(common https://github.com/nearmeng/common.git master)
fetch_content(router_client https://github.com/nearmeng/router_client.git master)
fetch_content(db_proxy_client https://github.com/nearmeng/db_proxy_client.git master)
fetch_content(conn_proto https://github.com/nearmeng/conn_proto.git master)
fetch_content(cs_proto_common https://github.com/nearmeng/cs_proto_common.git master)

init_project()

pre_toluapp(src/script)

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
