#-------------------------------------------------
#
# Project created by QtCreator 2017-07-27T20:57:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtEmployment
TEMPLATE = app

CONFIG += c++14


SOURCES += src/main.cpp\
        src/gui/main_window.cpp \
    src/kernel/employee/employee_base.cpp \
    src/kernel/ssn_handle.cpp \
    src/kernel/employee/employee_monthly.cpp \
    src/kernel/employee/employee_hourly.cpp \
    src/kernel/employee/employee_salesman.cpp \
    src/kernel/employee/employee_list.cpp \
    src/gui/employee_list_model.cpp \
    src/gui/add_emp_window.cpp \
    src/kernel/employee/employee_utils.cpp \
    src/gui/table_dynamic_search.cpp \
    src/gui/search_proxy_model.cpp \
    src/gui/line_edit_w_hint.cpp

HEADERS  += src/gui/main_window.h \
    src/kernel/employee/employee_base.h \
    src/kernel/ssn_handle.h \
    src/kernel/employee/employee_monthly.h \
    src/kernel/employee/employee_hourly.h \
    src/kernel/employee/employee_salesman.h \
    src/common/enum_helper.h \
    src/common/enum_range.h \
    src/common/enum_misc.h \
    src/kernel/debug_utils.h \
    src/kernel/employee/employee_list.h \
    src/kernel/employee/employee_list_abstract.h \
    src/kernel/employee/employee_list_generic.h \
    src/containers/enum_vector.h \
    src/gui/employee_list_model.h \
    src/common/utils.h \
    src/gui/add_emp_window.h \
    src/kernel/employee/employee_utils.h \
    src/containers/enum_bitset.h \
    src/gui/table_dynamic_search.h \
    src/gui/search_proxy_model.h \
    src/gui/line_edit_w_hint.h

INCLUDEPATH += src

#QMAKE_CXXFLAGS += -std=c++14

RESOURCES += \
    resources.qrc
