#pragma once

#include "defines.hpp"
#include "player_api.hpp"

#include <atomic>
#include <unordered_set>
#include <bitset>
#include <mutex>
#include <memory>

namespace msrv {

class EventListener;

class EventDispatcher
{
public:
    EventDispatcher() = default;

    std::unique_ptr<EventListener> createListener();
    void dispatch();

private:
    friend class EventListener;

    std::mutex mutex_;
    std::unordered_set<EventListener*> listeners_;

    MSRV_NO_COPY_AND_ASSIGN(EventDispatcher);
};

class EventListener
{
public:
    ~EventListener();
    void readEvents();

private:
    friend class EventDispatcher;

    explicit EventListener();

    EventDispatcher* owner_;
    std::atomic_bool pendingEvent_;

    MSRV_NO_COPY_AND_ASSIGN(EventListener);
};

}
