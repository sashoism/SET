#ifndef C_MESSAGE_H
#define C_MESSAGE_H

#include "Task.h"

class C_MESSAGE : public Task {
    Q_OBJECT
public:
    void run() {
        emit _response(QString(buffer));
    }
signals:
    void _response(QString);
};

#endif // C_MESSAGE_H

