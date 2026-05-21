cmake_minimum_required(VERSION 3.16)
project(RastreadorSalud VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Sql Charts)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SQLITE3 REQUIRED sqlite3)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
    include
    ${SQLITE3_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    Qt5::Sql
    Qt5::Charts
    ${SQLITE3_LIBRARIES}
)