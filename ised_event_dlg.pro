#-------------------------------------------------
#
# Project created by QtCreator 2013-07-22T11:18:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ised_event_dlg
TEMPLATE = app


SOURCES += main.cpp\
        event_dlg.cpp \
    events_list_model.cpp \
    event.cpp \
    complex_event.cpp \
    simple_event.cpp \
    xml_parser_adapter.cpp \
    simple_event_page_widget.cpp \
    event_pages_updating_visitor.cpp \
    ised_stream_parser.cpp \
    pattern_qualifier.cpp \
    imageviewer.cpp \
    simple_event_page_visitor.cpp

HEADERS  += event_dlg.h \
    events_list_model.h \
    event.h \
    complex_event.h \
    simple_event.h \
    xml_parser_adapter.h \
    event_serializing_visitor.hpp \
    event_visitor.h \
    event_pages_updating_visitor.h \
    simple_event_page_widget.h \
    null_deleter.h \
    ised_stream_parser.h \
    pattern_qualifier.h \
    imageviewer.h \
    page_visitor.h \
    simple_event_page_visitor.h \
    event_page_widget.h

FORMS    +=

OTHER_FILES +=

RESOURCES += \
    ised_res.qrc

win32:INCLUDEPATH += d:/CPP/boost/boost_1_53_0/boost_1_53_0
win32:LIBS += -Ld:/CPP/boost/boost_1_53_0/boost_1_53_0/stage/lib
