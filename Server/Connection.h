#ifndef CONNECTION_H
#define CONNECTION_H

#include "Task.h"
#include <QObject>
#include <QTcpSocket>

class Player;
class Connection : public QObject {
    Q_OBJECT
public:
    const qintptr& socketDescriptor;
    Task** runnables;
    explicit Connection(QObject* parent = 0, QTcpSocket* socket = 0);
public slots:
    void readyRead();
    void write(QByteArray);
    void disconnected();
private:
    QTcpSocket* socket;
    qintptr _socketDescriptor;
};

#endif
