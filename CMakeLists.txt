cmake_minimum_required (VERSION 2.6)
project (TrainBoom)

include (CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

set (SRC_DIR "src")
include_directories (${SRC_DIR})
include_directories (include)

set (CPP_FILES "sha1.cpp")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")  # 添加cmake_module_path
find_package(Pistache)    # 在新的cmake_module_path下寻找pistache
include_directories("${Pistache_INCLUDE_DIRS}")

add_executable (TrainBoom ${SRC_DIR}/main.cpp ${SRC_DIR}/${CPP_FILES})
target_link_libraries(TrainBoom ${Pistache_LIBRARIES})

install (TARGETS TrainBoom DESTINATION bin)
