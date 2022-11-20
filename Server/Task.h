#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QRunnable>
#include <QDebug>

class Task : public QObject, public QRunnable {
    Q_OBJECT
public:
    void setBuffer(const QByteArray &buffer) {
        this->buffer = buffer.right(buffer.size() - 1);
    }
protected:
    QByteArray buffer;
    virtual void run() = 0;
signals:
    void response(QByteArray);
};

#endif
