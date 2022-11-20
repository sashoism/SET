#ifndef S_CARDSELECTED_H
#define S_CARDSELECTED_H

#include "Task.h"

class S_CARDSELECTED : public Task {
    Q_OBJECT
public:
    void run() {
        QByteArrayList data = buffer.split(';');
        emit _response(data.at(0).toInt(), data.at(1).toInt());
    }
signals:
    void _response(int, int);
};

#endif // S_CARDSELECTED_H

