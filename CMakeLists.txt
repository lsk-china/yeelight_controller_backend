cmake_minimum_required(VERSION 3.26)
project(yeelight_backend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Network
        DBus
        REQUIRED)
find_package(RapidJSON)

add_executable(yeelight_backend main.cpp
        MiioDevice.h
        YeeLight.cpp
        YeeLight.h
        YeeLightDBusService.cpp
        YeeLightDBusService.h
        MiCloudClient.cpp
        MiCloudClient.h
)
link_directories(lib)
target_link_libraries(yeelight_backend
        Qt5::Core
        Qt5::Network
        Qt5::DBus
        miio
)

