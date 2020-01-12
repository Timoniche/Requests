#ifndef TRANSAS_STOPPER_H
#define TRANSAS_STOPPER_H

#include "../inc/ProjectPCH.h"

class Stopper final
{
private:
    std::unique_ptr<std::atomic<bool>> _suspended = std::make_unique<std::atomic<bool>>(false);

public:
    Stopper() = default;

    inline void suspend() noexcept
    {
        *_suspended = true;
    }

    inline void resume() noexcept
    {
        *_suspended = false;
    }

    inline bool is_suspended() const
    {
        return *_suspended;
    }
};

#endif //TRANSAS_STOPPER_H
