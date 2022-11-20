#ifndef C_FOUNDSET_H
#define C_FOUNDSET_H

#include "Task.h"

class C_FOUNDSET : public Task {
public:
    void run() {
        emit response(buffer);
    }
};

#endif // C_FOUNDSET_H

