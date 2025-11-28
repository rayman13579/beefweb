#include "player_events.hpp"

#include <assert.h>

namespace msrv {

std::unique_ptr<EventListener> EventDispatcher::createListener()
{
    std::unique_ptr<EventListener> listener(new EventListener());

    {
        std::lock_guard<std::mutex> lock(mutex_);
        listeners_.insert(listener.get());
        listener->owner_ = this;
    }

    return listener;
}

void EventDispatcher::dispatch()
{
    std::lock_guard<std::mutex> lock(mutex_);

    for (auto listener : listeners_)
    {
        listener->pendingEvent_.store(true);
    }
}

EventListener::EventListener()
    : owner_(nullptr), pendingEvent_(false)
{
}

EventListener::~EventListener()
{
    if (!owner_)
        return;

    std::lock_guard<std::mutex> lock(owner_->mutex_);

    auto it = owner_->listeners_.find(this);
    assert(it != owner_->listeners_.end());
    owner_->listeners_.erase(it);
}

void EventListener::readEvents()
{
    pendingEvent_.exchange(false);
}

}
