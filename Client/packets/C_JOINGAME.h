#include "Task.h"

class C_JOINGAME : public Task {
public:
    void run() {
        emit response(QByteArray(1, buffer.at(0)));
    }
};
