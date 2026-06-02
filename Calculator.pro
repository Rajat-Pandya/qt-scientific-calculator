QT       += core gui widgets

CONFIG += c++17

# Project name
TARGET = Calculator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated.
DEFINES += QT_DEPRECATED_WARNINGS

# Include paths
INCLUDEPATH += include/

# Source files
SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/expressionparser.cpp

# Header files
HEADERS += \
    include/mainwindow.h \
    include/expressionparser.h

# UI files
FORMS += \
    ui/mainwindow.ui

# Resources (stylesheets, icons, etc.)
RESOURCES += \
    resources/calculator.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Windows deployment
win32 {
    CONFIG += console
    CONFIG -= app_bundle
}

# macOS deployment
macx {
    # Add any macOS-specific configurations here
}

# Linux deployment
unix:!macx {
    # Add any Linux-specific configurations here
}
