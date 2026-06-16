cmake_minimum_required(VERSION 3.16)
project(iLoabotDevice LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost CONFIG REQUIRED COMPONENTS system)

set(ILOABOT_CORE_SOURCES
    src/product_factory.cpp
    src/c_api.cpp
    src/factories/robot_factory.cpp
    src/factories/agv_factory.cpp
    src/factories/suction_cup_factory.cpp
    src/factories/battery_factory.cpp
    src/factories/charger_factory.cpp
    src/factories/light_factory.cpp
    src/factories/plc_factory.cpp
    src/devices/robot.cpp
    src/devices/robot002.cpp
    src/devices/agv.cpp
    src/devices/agv002.cpp
    src/devices/suction_cup.cpp
    src/devices/suction_cup002.cpp
    src/devices/battery.cpp
    src/devices/battery002.cpp
    src/devices/charger.cpp
    src/devices/charger002.cpp
    src/devices/light.cpp
    src/devices/light002.cpp
    src/devices/plc.cpp
    src/devices/plc002.cpp
    src/devices/iloabot_device.cpp
    src/comm/socket.cpp
    src/comm/serial_port.cpp
    src/comm/tcp_client.cpp
    src/comm/tcp_server.cpp
)

add_library(iLoabotDevice SHARED
    ${ILOABOT_CORE_SOURCES}
)

if(MINGW)
    target_link_options(iLoabotDevice PRIVATE -Wl,--export-all-symbols)
endif()

target_include_directories(iLoabotDevice PUBLIC ${CMAKE_SOURCE_DIR}/include) 
target_link_libraries(iLoabotDevice PUBLIC Boost::system)
target_link_libraries(iLoabotDevice PUBLIC ws2_32 mswsock)
add_executable(iLoabotDeviceDemo
    src/main.cpp
)

target_link_libraries(iLoabotDeviceDemo PRIVATE iLoabotDevice)
target_link_libraries(iLoabotDeviceDemo PRIVATE advapi32)
