#include "Client.h"
#include "packets/C_LOBBYDATA.h"
#include "packets/C_SETNAME.h"
#include "packets/C_ADDGAME.h"
#include "packets/C_GAMEOVER.h"
#include "packets/C_JOINGAME.h"
#include <QApplication>
#include <QMessageBox>

Client* Client::instance = nullptr;
Client* Client::getInstance() {
    if (instance == nullptr) {
        instance = new Client;
    }
    return instance;
}

Client::Client() {
    d = new IPDialog;
    cooldownTimer = new QTimer(this);
    cooldownTimer->setSingleShot(true);
    cooldownTimer->setInterval(REJOIN_ROOM_CD_SEC * 1000);
}

void Client::start(QString IP, qint16 port) {
    connect(this, &Client::signalDisconnected, QApplication::instance(), &QApplication::quit);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &Client::connected);
    connect(socket, &QTcpSocket::readyRead, this, &Client::readyRead);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    connect(socket, &QTcpSocket::aboutToClose, this, &Client::disconnected);
    socket->connectToHost(IP, port);
    if (!socket->waitForConnected(1000)) {
        #ifdef Q_OS_ANDROID
            QMessageBox::critical(d, "Failed to connect to the server.", "");
        #else
            QMessageBox::critical(d, " ", "Failed to connect to the server.\nThe game will now shut down.");
        #endif
        QApplication::exit(0);
    }
}

void Client::startGame() {
    g = new GameWindow;
    connect(g, SIGNAL(joinCooldown()), cooldownTimer, SLOT(start()));
}

void Client::connected() {
    w = new MainWindow; w->show();
    runnables = new Task*[runnablesCount]; for (int i = 0; i < runnablesCount; i ++) runnables[i] = nullptr;
    runnables[0] = new C_LOBBYDATA(); runnables[0]->setAutoDelete(false);
    runnables[1] = new C_SETNAME(); runnables[1]->setAutoDelete(false);
    runnables[2] = new C_JOINGAME(); runnables[2]->setAutoDelete(false);
    runnables[3] = new C_ADDGAME(); runnables[3]->setAutoDelete(false);
    runnables[9] = new C_GAMEOVER(); runnables[9]->setAutoDelete(false);
    connect(runnables[0], &Task::response, w, &MainWindow::received_lobbyData);
    connect(runnables[1], &Task::response, w, &MainWindow::setName_response);
    connect(runnables[2], &Task::response, w, &MainWindow::joinGame_response);
    connect(runnables[3], &Task::response, w, &MainWindow::addGame_response);
    connect(static_cast<C_GAMEOVER*>(runnables[9]), &C_GAMEOVER::_response, w, &MainWindow::pGameOver_response);
    write(QByteArray(1, char(0)));
}

void Client::error(QAbstractSocket::SocketError) {
    socket->close();
}

void Client::disconnected() {
    for (int i = 0; i < runnablesCount; i ++) {
        if (runnables[i] != nullptr)
        runnables[i]->deleteLater();
    }
    emit signalDisconnected();
}

void Client::readyRead() {
    QList<QByteArray> buffer = socket->readAll().split('\x1f');
    for (int i = 0; i < buffer.size(); i ++) {
        if (!buffer.at(i).isEmpty()) {
            unsigned int firstByte = (unsigned int) buffer.at(i).at(0);\
            if (firstByte >= runnablesCount) {
                socket->disconnectFromHost();
                return;
            }
            if (runnables[firstByte] == nullptr) return;
            runnables[firstByte]->setBuffer(buffer.at(i));
            QThreadPool::globalInstance()->start(runnables[firstByte]);
        }
    }
}

void Client::write(QByteArray buffer) {
    buffer.append('\x1f');
    socket->write(buffer);
    socket->flush();
}
