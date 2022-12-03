QT += core gui widgets
CONFIG += c++20

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    Main.cpp \
    Task.cpp \
    MainWindow.cpp

HEADERS += \
    MainWindow.hpp \
    Task.hpp

FORMS += \
    MainWindow.ui \
    Task.ui
