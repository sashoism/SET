#ifndef SERVER_H
#define SERVER_H

#include "Utility.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QThreadPool>

class Player; class Game;
class Server : public QTcpServer {
    Q_OBJECT
public:
    static const int runnablesCount = 8;
    static Server* getInstance();
    void start(quint16);
    QByteArray gameList();
public slots:
    void broadcastGameList() { emit broadcast(gameList()); }
    void createGame(Player*);
    void joinGame(Player*, Game*);
    void restartTimer(QByteArray);
protected:
    void incomingConnection(qintptr socketDescriptor);
private:
    Server() { QThreadPool::globalInstance()->setMaxThreadCount(50); }
    static Server* instance;
    QTimer* broadcastTimer;
signals:
    void broadcast(QByteArray);
};

#endif
