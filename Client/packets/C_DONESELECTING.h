#ifndef C_DONESELECTING_H
#define C_DONESELECTING_H

#include "Task.h"

class C_DONESELECTING : public Task {
    Q_OBJECT
public:
    void run() {
        QByteArrayList list = buffer.split(';');
        bool successful = list.at(0).at(0);
        QString name(list.at(1));
        emit _response(successful, name);
    }
signals:
    void _response(bool, QString);
};

#endif // C_DONESELECTING_H

