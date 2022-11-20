QT += core
QT -= gui
QT += network

TARGET = Server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += Main.cpp \
    Connection.cpp \
    Game.cpp \
    Player.cpp \
    Player2.cpp \
    Server.cpp

HEADERS += \
    Connection.h \
    Game.h \
    Player.h \
    Player2.h \
    Server.h \
    Task.h \
    Utility.h \
    packets/S_ADDGAME.h \
    packets/S_CARDSELECTED.h \
    packets/S_JOINGAME.h \
    packets/S_LOBBYDATA.h \
    packets/S_SETNAME.h \
    packets/S_LEAVEGAME.h \
    packets/S_FOUNDSET.h \
    packets/S_CARDSELECTED.h \
    Card.h \
    packets/S_ONJOIN.h

QMAKE_CXXFLAGS += -std=c++11
