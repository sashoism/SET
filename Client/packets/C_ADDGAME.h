#include "Task.h"

class C_ADDGAME : public Task {
public:
    void run() {
        emit response(QByteArray(1, buffer.at(0)));
    }
};
