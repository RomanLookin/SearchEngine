TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt c++11

SOURCES += \
        main.cpp \
    converterjson.cpp \
    invertedindex.cpp \
    searchserver.cpp

HEADERS += \
    converterjson.h \
    invertedindex.h \
    searchserver.h

QT += testlib
