QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += ../SE/invertedindex.h ../SE/searchserver.h

SOURCES +=  tst_test_se.cpp ../SE/invertedindex.cpp ../SE/searchserver.cpp
