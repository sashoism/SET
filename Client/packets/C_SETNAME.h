#include "Task.h"

class C_SETNAME : public Task {
public:
    void run() {
        emit response(QByteArray(1, buffer.at(0)));
    }
};
