QT += testlib
QT -= gui widgets

CONFIG += c++17 console
CONFIG -= app_bundle

TARGET = tst_expressionparser
TEMPLATE = app

INCLUDEPATH += ../include

SOURCES += \
    tst_expressionparser.cpp \
    ../src/expressionparser.cpp
