#pragma once

#include "player_api.hpp"
#include "json.hpp"

namespace msrv {

void to_json(Json& json, const PlaybackState& value);
void to_json(Json& json, const ActiveItemInfo& value);
void to_json(Json& json, const PlayerState& value);

}
