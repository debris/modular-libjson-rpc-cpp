cmake_minimum_required(VERSION 3.0.0)

project(modular_server)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")
find_package (json_rpc_cpp 0.4)
find_package (Jsoncpp 0.60)

find_program(JSONRPCSTUB jsonrpcstub)

macro (create_module SPEC NAME)

	execute_process(COMMAND
		${JSONRPCSTUB} ${SPEC}.json --cpp-server=${NAME} --cpp-server-file=${NAME}.h
		WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/example
		OUTPUT_FILE ${NAME}.h
	)

	execute_process(COMMAND
		${CMAKE_CURRENT_LIST_DIR}/mod.sh ${NAME}.h ${NAME}
		WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/example
		INPUT_FILE ${NAME}.h
		OUTPUT_FILE ${NAME}Module.h
	)

endmacro()

create_module(moduleA AInterface)
create_module(moduleB BInterface)
create_module(moduleC CInterface)

aux_source_directory(. SRC_LIST)
file(GLOB HEADERS "*.h")

set(EXECUTABLE modular_server)

add_executable(${EXECUTABLE} ${SRC_LIST} ${HEADERS})

# let cmake induce c++ standard
target_compile_features(${EXECUTABLE} PRIVATE cxx_range_for)

target_include_directories(${EXECUTABLE} SYSTEM PUBLIC ${JSON_RPC_CPP_INCLUDE_DIRS})
target_link_libraries(${EXECUTABLE} ${JSON_RPC_CPP_SERVER_LIBRARIES})

target_include_directories(${EXECUTABLE} SYSTEM BEFORE PUBLIC ${JSONCPP_INCLUDE_DIRS})
target_link_libraries(${EXECUTABLE} ${JSONCPP_LIBRARIES})

