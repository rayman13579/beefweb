#pragma once

#include "core_types.hpp"
#include "defines.hpp"

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <boost/thread/future.hpp>

namespace msrv {

class WorkQueue;

class Player;

enum class PlaybackState
{
    STOPPED,
    PLAYING,
    PAUSED,
};

enum class PlayerEvents : int
{
    NONE = 0,
    PLAYER_CHANGED = 1 << 0
};

MSRV_ENUM_FLAGS(PlayerEvents, int);

struct ActiveItemInfo {
    std::string artist;
    std::string title;
    double position;
    double duration;
};

struct PlayerState {
    PlaybackState playbackState = PlaybackState::STOPPED;
    ActiveItemInfo activeItem;
};

class ColumnsQuery
{
public:
    ColumnsQuery() = default;
    virtual ~ColumnsQuery() = default;

    MSRV_NO_COPY_AND_ASSIGN(ColumnsQuery);
};

using PlayerStatePtr = std::unique_ptr<PlayerState>;
using ColumnsQueryPtr = std::unique_ptr<ColumnsQuery>;
using PlayerEventsCallback = std::function<void(PlayerEvents)>;

class Player
{
public:
    Player() = default;
    virtual ~Player() = default;

    virtual const char* name() = 0;
    virtual std::unique_ptr<WorkQueue> createWorkQueue() = 0;

    // Player control and query API

    virtual PlayerStatePtr queryPlayerState() = 0;
    virtual void playCurrent() = 0;
    virtual void playRandom() = 0;
    virtual void playNext() = 0;
    virtual void playPrevious() = 0;
    virtual void stop() = 0;
    virtual void pause() = 0;
    virtual void togglePause() = 0;
    virtual void playOrPause() = 0;

    virtual void seekAbsolute(double offsetSeconds) = 0;
    virtual void seekRelative(double offsetSeconds) = 0;

    virtual ColumnsQueryPtr createColumnsQuery() = 0;

    // Events API

    void onEvents(PlayerEventsCallback callback)
    {
        eventsCallback_ = std::move(callback);
    }

protected:
    void emitEvents(PlayerEvents events)
    {
        if (events != PlayerEvents::NONE && eventsCallback_)
            eventsCallback_(events);
    }

private:
    PlayerEventsCallback eventsCallback_;

    MSRV_NO_COPY_AND_ASSIGN(Player);
};

}
