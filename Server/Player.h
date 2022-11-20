#ifndef PLAYER_H
#define PLAYER_H

#include "Connection.h"
#include <QObject>
#include <QTcpSocket>

class Player : public QObject {
    Q_OBJECT
public:
    Player(int) {}
    explicit Player(QObject* parent = 0, QTcpSocket* socket = 0);
    Connection* connection;
    QString getName();
    char setName(QString);
};

#endif
