#-------------------------------------------------
#
# Project created by QtCreator 2016-06-03T21:14:41
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES += Main.cpp\
    Client.cpp \
    GameWindow.cpp \
    IPDialog.cpp \
    MainWindow.cpp

HEADERS  += \
    Client.h \
    GameWindow.h \
    IPDialog.h \
    MainWindow.h \
    Task.h \
    packets/C_ADDGAME.h \
    packets/C_JOINGAME.h \
    packets/C_LOBBYDATA.h \
    packets/C_GAMEOVER.h \
    packets/C_SETNAME.h \
    QLabel2.h \
    packets/C_GAMECARDS.h \
    packets/C_FOUNDSET.h \
    packets/C_DONESELECTING.h \
    packets/C_MESSAGE.h \
    packets/C_GAMEOVER.h

FORMS += \
    GameWindow.ui \
    IPDialog.ui \
    MainWindow.ui

QMAKE_CXXFLAGS += -std=c++11

RESOURCES += \
    resources.qrc
