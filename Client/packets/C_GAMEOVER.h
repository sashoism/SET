#include "Task.h"

class C_GAMEOVER : public Task {
    Q_OBJECT
public:
    void run() {
        emit _response();
    }
signals:
    void _response();
};
