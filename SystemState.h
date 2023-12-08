//SystemState.h
#ifndef SYSTEMSTATE_H
#define SYSTEMSTATE_H

#include <atomic>

class SystemState {
public:
    SystemState();

    std::atomic<bool> isDoorOpenOrOpening{false};
    std::atomic<bool> programRunning{true};
    std::atomic<int> failedPasswordAttempts{0};
    std::atomic<bool> isSystemLocked{false};

    void doorIsOpen();
    void doorIsClosed();
};

#endif // SYSTEMSTATE_H

