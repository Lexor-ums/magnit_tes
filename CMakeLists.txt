# CMakeList.txt: проект CMake для ClientServer; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)
macro(get_WIN32_WINNT version)
    if (WIN32 AND CMAKE_SYSTEM_VERSION)
        set(ver ${CMAKE_SYSTEM_VERSION})
        string(REPLACE "." "" ver ${ver})
        string(REGEX REPLACE "([0-9])" "0\\1" ver ${ver})

        set(${version} "0x${ver}")
    endif()
endmacro()

get_WIN32_WINNT(ver)
project ("ClientServer")

set(CMAKE_CXX_STANDARD 20)
set(Boost_USE_STATIC_LIBS ON)
set(BOOST_ROOT C:/boost_1_75_0)
set(BOOST_INCLUDEDIR ${BOOST_ROOT}/boost)
set(BOOST_LIBRARYDIR  ${BOOST_ROOT}/stage/lib)
find_package(Boost 1.75.0 REQUIRED COMPONENTS date_time regex thread)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Добавьте источник в исполняемый файл этого проекта.
include_directories(${Boost_INCLUDE_DIRS} src)
add_subdirectory(src)
# TODO: Добавьте тесты и целевые объекты, если это необходимо.
