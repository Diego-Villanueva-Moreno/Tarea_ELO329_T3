# Configuración específica para Qt 6
QT += core gui widgets

# Forzar el estándar de C++ moderno exigido por Qt 6
CONFIG += c++17

TEMPLATE = app
TARGET = stage1

# Archivos de cabecera (.h) de tu código actual
HEADERS += \
    Equipo.h \
    Cellular.h \
    CellularView.h \
    Territory.h \
    territoryview.h

# Archivos de origen (.cpp)
SOURCES += \
    main.cpp