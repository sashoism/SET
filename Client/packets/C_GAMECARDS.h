#ifndef C_GAMECARDS_H
#define C_GAMECARDS_H

#include "Task.h"

class C_GAMECARDS : public Task {
public:
    void run() {
        emit response(buffer);
    }
};

#endif // C_GAMECARDS_H

