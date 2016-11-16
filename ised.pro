#-------------------------------------------------
#
# Project created by QtCreator 2013-07-30T11:49:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

unix:QMAKE_CXXFLAGS += -std=c++0x

TARGET = ised
TEMPLATE = app


SOURCES +=\
        ised_win.cpp \
    simple_event_analizer.cpp\
    event_analizer.cpp \
    ised_main.cpp \
    xml_parser_adapter.cpp \
    simple_event_page_widget.cpp \
    simple_event_page_visitor.cpp \
    simple_event.cpp \
    pattern_qualifier.cpp \
    ised_stream_parser.cpp \
    imageviewer.cpp \
    events_list_model.cpp \
    event_pages_updating_visitor.cpp \
    event_dlg.cpp \
    event.cpp \
    complex_event.cpp

HEADERS  += ised_win.h \
    simple_event_analizer.h \
    event_analizer.h \
    images_comparator.h \
    xml_parser_adapter.h \
    simple_event_page_widget.h \
    simple_event_page_visitor.h \
    simple_event.h \
    pattern_qualifier.h \
    page_visitor.h \
    null_deleter.h \
    ised_stream_parser.h \
    imageviewer.h \
    events_list_model.h \
    event_visitor.h \
    event_serializing_visitor.hpp \
    event_pages_updating_visitor.h \
    event_page_widget.h \
    event_dlg.h \
    event.h \
    complex_event.h \
    logger.h \
    simple_event_analizer_wrapper.h

win32:INCLUDEPATH += D:/cpp/boost_1_53_0

RESOURCES += \
    ised_res.qrc
