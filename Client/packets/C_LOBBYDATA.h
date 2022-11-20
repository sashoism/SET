#include "Task.h"

class C_LOBBYDATA : public Task {
public:
    void run() {
        emit response(buffer);
    }
};
