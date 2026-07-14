cmake_minimum_required(VERSION 3.21)
project(Weather VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Network for the concrete service; Quick for the UI.
find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Network)
qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appWeather
    main.cpp
)

# KISS: one window, components not reused elsewhere -> one module, no static-lib
# sub-modules. All C++ QML types are registered declaratively via SOURCES.
qt_add_qml_module(appWeather
    URI Weather
    VERSION 1.0
    QML_FILES
        Main.qml
    SOURCES
        weatherservice.h
        openmeteoservice.h openmeteoservice.cpp
        dailyforecast.h
        forecastmodel.h forecastmodel.cpp
        weathercontroller.h weathercontroller.cpp
)

target_link_libraries(appWeather PRIVATE Qt6::Quick Qt6::Network)
