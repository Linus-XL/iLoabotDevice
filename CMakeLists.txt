cmake_minimum_required(VERSION 3.16)
project(iLoabotDevice LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED COMPONENTS system)

set(ILOABOT_CORE_SOURCES
    src/device_factory.cpp
    src/devices/robot.cpp
    src/devices/agv.cpp
    src/devices/suction_cup.cpp
    src/devices/battery.cpp
    src/devices/charger.cpp
    src/devices/light.cpp
    src/devices/plc.cpp
    src/comm/socket.cpp
    src/comm/serial_port.cpp
    src/comm/tcp_client.cpp
    src/comm/tcp_server.cpp
)

add_library(iLoabotDevice SHARED
    ${ILOABOT_CORE_SOURCES}
)

target_include_directories(iLoabotDevice PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(iLoabotDevice PUBLIC Boost::system)

add_executable(iLoabotDeviceDemo
    src/main.cpp
)

target_link_libraries(iLoabotDeviceDemo PRIVATE iLoabotDevice)
