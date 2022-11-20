#ifndef S_LEAVEGAME_H
#define S_LEAVEGAME_H

#include "Task.h"

class S_LEAVEGAME : public Task {
    Q_OBJECT
protected:
    void run() {
        emit _response();
    }
signals:
    void _response();
};

#endif
