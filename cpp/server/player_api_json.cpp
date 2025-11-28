#include "player_api_json.hpp"
#include <stdexcept>

namespace msrv {

namespace {

const char INVALID_PLAYBACK_STATE[] = "Invalid playback state";

}

void to_json(Json& json, const PlaybackState& value)
{
    switch (value)
    {
    case PlaybackState::PAUSED:
        json = "paused";
        break;

    case PlaybackState::PLAYING:
        json = "playing";
        break;

    case PlaybackState::STOPPED:
        json = "stopped";
        break;

    default:
        throw std::invalid_argument(INVALID_PLAYBACK_STATE);
    }
}

void to_json(Json& json, const ActiveItemInfo& value)
{
    json["artist"] = value.artist;
    json["title"] = value.title;
    json["position"] = value.position;
    json["duration"] = value.duration;
}

void to_json(Json& json, const PlayerState& value)
{
    json["playbackState"] = value.playbackState;
    json["activeItem"] = value.activeItem;
}

}
