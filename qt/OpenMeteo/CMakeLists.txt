cmake_minimum_required(VERSION 3.16)

project(OpenMeteo VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Widgets is required because QtCharts pulls in the widgets stack.
# QuickControls2 provides QQuickStyle, used to select the Material style.
find_package(Qt6 6.8 REQUIRED COMPONENTS Quick Qml Network Charts Widgets QuickControls2)

qt_standard_project_setup()

qt_add_executable(OpenMeteo
    main.cpp
    WeatherService.h
    WeatherService.cpp
)

# Theme.qml is a QML singleton: one source of truth for the app's colors,
# fed into the Material style on the root window (see Main.qml).
set_source_files_properties(Theme.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(OpenMeteo
    URI App
    VERSION 1.0
    QML_FILES
        Main.qml
        TileMap.qml
        DateField.qml
        Theme.qml
)

target_link_libraries(OpenMeteo PRIVATE
    Qt6::Quick
    Qt6::Qml
    Qt6::Network
    Qt6::Charts
    Qt6::Widgets
    Qt6::QuickControls2
)
