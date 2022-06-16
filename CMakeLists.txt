cmake_minimum_required(VERSION 3.16)
project(dng
        VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-${PROJECT_VERSION})

# Setup Lua Libraries
# To determine which files look at the lua/makefile to see how the targets are built
set(LUA_SRC ${PROJECT_SOURCE_DIR}/thirdparty/lua)
add_library(lua STATIC
        ${LUA_SRC}/lapi.c
        ${LUA_SRC}/lcode.c
        ${LUA_SRC}/lctype.c
        ${LUA_SRC}/ldebug.c
        ${LUA_SRC}/ldo.c
        ${LUA_SRC}/ldump.c
        ${LUA_SRC}/lfunc.c
        ${LUA_SRC}/lgc.c
        ${LUA_SRC}/llex.c
        ${LUA_SRC}/lmem.c
        ${LUA_SRC}/lobject.c
        ${LUA_SRC}/lopcodes.c
        ${LUA_SRC}/lparser.c
        ${LUA_SRC}/lstate.c
        ${LUA_SRC}/lstring.c
        ${LUA_SRC}/ltable.c
        ${LUA_SRC}/ltm.c
        ${LUA_SRC}/lundump.c
        ${LUA_SRC}/lvm.c
        ${LUA_SRC}/lzio.c
        ${LUA_SRC}/ltests.c)
add_library(lauxlib STATIC ${LUA_SRC}/lauxlib.c)
add_library(lualib STATIC
        ${LUA_SRC}/lbaselib.c
        ${LUA_SRC}/lcorolib.c
        ${LUA_SRC}/ldblib.c
        ${LUA_SRC}/linit.c
        ${LUA_SRC}/liolib.c
        ${LUA_SRC}/lmathlib.c
        ${LUA_SRC}/loadlib.c
        ${LUA_SRC}/loslib.c
        ${LUA_SRC}/lstrlib.c
        ${LUA_SRC}/ltablib.c
        ${LUA_SRC}/lutf8lib.c)
target_compile_definitions(lualib PRIVATE -DLUA_USE_POSIX=1) # to properly use mkstemp
target_include_directories(lua PUBLIC lua)
target_include_directories(lauxlib PUBLIC lua)
target_include_directories(lualib PUBLIC lua)
target_link_libraries(lauxlib lua)
set(LUA_LIBRARIES lua lualib lauxlib)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty/lua)

# System specific includes
if (UNIX)
    set(RES_LIB src/linux/res.h)
endif ()

add_executable(${PROJECT_NAME} src/main.cpp src/Level.cpp src/Level.h src/CApi.h src/Scene.h src/LuaApi.h src/MessageBox.h ${RES_LIB})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LUA_LIBRARIES})

# configure and link SFML
set(BUILD_SHARED_LIBS FALSE)
set(SFML_LIBRARIES sfml-system sfml-window sfml-graphics)
if (WIN32)
    set(SFML_USE_STATIC_STD_LIBS TRUE)
endif ()
set(SFML_USE_SYSTEM_DEPS ON)
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS ON)
add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/SFML EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PUBLIC ${SFML_LIBRARIES} ${LUA_LIBRARIES})


# Install Directives

if (UNIX)
    set(SFML_INSTALL_PKGCONFIG_FILES FALSE)
endif ()
install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION .)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/maps DESTINATION .)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dnglib DESTINATION .)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE DESTINATION .)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/thirdparty-licenses.txt DESTINATION .)


# Package Directives
set(CPACK_GEN TGZ)
if (WIN32)
    set(CPACK_GEN ZIP)
endif ()

set(CPACK_GENERATOR ${CPACK_GEN})
set(CPACK_SOURCE_GENERATOR ${CPACK_GEN})
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE)
include(CPack)