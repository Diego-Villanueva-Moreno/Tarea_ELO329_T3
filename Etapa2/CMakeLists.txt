cmake_minimum_required(VERSION 3.16)

project(stage2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(PROJECT_SOURCES
        main.cpp
        Equipo.cpp
        Territory.cpp
        Equipo.h
        Cellular.h
        CellularView.h
        Territory.h
        territoryview.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(stage2
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        EloTelTag.h
        Tablet.h
        EloTelTagView.h
        TabletView.h
        ETNube.h
        Equipo.cpp
        Territory.cpp
    )
else()
    if(ANDROID)
        add_library(stage2 SHARED ${PROJECT_SOURCES})
    else()
        add_executable(stage2 ${PROJECT_SOURCES})
    endif()
endif()

target_link_libraries(stage2 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(stage2 PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS stage2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(stage2)
endif()