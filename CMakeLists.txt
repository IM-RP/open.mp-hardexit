cmake_minimum_required(VERSION 3.19)
project(HardExit LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# open.mp SDK (header-only interface library)
FetchContent_Declare(
	OMP_SDK
	GIT_REPOSITORY https://github.com/openmultiplayer/open.mp-sdk.git
	GIT_TAG        master
	GIT_SHALLOW    TRUE
)

# Pawn runtime (we only need the amx headers)
FetchContent_Declare(
	pawn
	GIT_REPOSITORY https://github.com/pawn-lang/pawn.git
	GIT_TAG        master
	GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(OMP_SDK pawn)

# Build the component as a shared library
add_library(${PROJECT_NAME} SHARED main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE OMP-SDK)

# pawn.hpp expects <amx/amx.h>, which lives at pawn/source/amx/amx.h
# So we add pawn/source/ as an include path.
target_include_directories(${PROJECT_NAME} PRIVATE
	${pawn_SOURCE_DIR}/source
)

# Prevent amx.h from redefining standard integer types
target_compile_definitions(${PROJECT_NAME} PRIVATE
	HAVE_STDINT_H
	PAWN_CELL_SIZE=32
)

# open.mp server is 32-bit
if(UNIX AND NOT APPLE)
	target_compile_options(${PROJECT_NAME} PRIVATE -m32 -fvisibility=hidden)
	target_link_options(${PROJECT_NAME} PRIVATE -m32)
elseif(NOT MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE -fvisibility=hidden)
endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY PREFIX "")
