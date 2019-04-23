cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)
project(smr)

set (APP_NAME smr)
set (SRC_DIR ${PROJECT_SOURCE_DIR})

# DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL
set( CMAKE_BUILD_TYPE DEBUG)

include_directories(SRC_DIR)

list(APPEND SRC ${SRC_DIR}/main.cpp)
list(APPEND SRC ${SRC_DIR}/serialization/json/jsonSerializer.cpp)
list(APPEND SRC ${SRC_DIR}/serialization/serializerFactory.cpp)
list(APPEND SRC ${SRC_DIR}/serialization/json/jsonDecoder.cpp)
list(APPEND SRC ${SRC_DIR}/serialization/json/jsonEncoder.cpp)
list(APPEND SRC ${SRC_DIR}/serialization/serializationUtils.cpp)

add_executable (${APP_NAME} ${SRC})