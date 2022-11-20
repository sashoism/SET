#ifndef S_PGAMEOVER_H
#define S_PGAMEOVER_H

#include "Task.h"

class S_PGAMEOVER : public Task {
public:
    S_PGAMEOVER() {}
protected:
    void run() {
        QByteArray r(1, char(3));
        emit response(r);
    }
};

#endif
