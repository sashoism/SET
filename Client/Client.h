#ifndef CLIENT_H
#define CLIENT_H

#define SERVER_PORT 2187
#define REJOIN_ROOM_CD_SEC 20

#include "IPDialog.h"
#include "MainWindow.h"
#include "GameWindow.h"
#include "Task.h"
#include <QTcpSocket>
#include <QThreadPool>
#include <QTimer>

class Client : public QObject {
    Q_OBJECT
public:
    IPDialog* d;
    MainWindow* w;
    GameWindow* g;
    Task** runnables;
    static const int runnablesCount = 10;
    QTcpSocket* socket;
    QString name;
    static Client* getInstance();
    void start(QString, qint16);
    void startGame();
    QString ipDialog();
    QTimer* cooldownTimer;
public slots:
    void connected();
    void error(QAbstractSocket::SocketError);
    void disconnected();
    void readyRead();
    void write(QByteArray);
private:
    static Client* instance;
    Client();
signals:
    void signalDisconnected();
};

#endif // CLIENT_H
