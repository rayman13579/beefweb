#include "player.hpp"
#include "project_info.hpp"

namespace msrv {
namespace player_foobar2000 {

namespace {

double clampVolume(double value)
{
    return std::max(std::min(value, 0.0), static_cast<double>(playback_control::volume_mute));
}

}

std::vector<std::string> PlayerImpl::evaluatePlaybackColumns(const TitleFormatVector& compiledColumns)
{
    std::vector<std::string> result;
    result.reserve(compiledColumns.size());

    pfc::string8 buffer;

    for (auto& compiledColumn : compiledColumns)
    {
        auto ret = playbackControl_->playback_format_title(
            nullptr,
            buffer,
            compiledColumn,
            nullptr,
            playback_control::display_level_all);

        if (!ret)
        {
            result.clear();
            return result;
        }

        result.emplace_back(buffer.get_ptr(), buffer.get_length());
    }

    return result;
}

PlaybackState PlayerImpl::getPlaybackState()
{
    if (playbackControl_->is_paused())
        return PlaybackState::PAUSED;

    if (playbackControl_->is_playing())
        return PlaybackState::PLAYING;

    return PlaybackState::STOPPED;
}

void PlayerImpl::queryActiveItem(ActiveItemInfo* info, ColumnsQuery* query)
{
    info->position = playbackControl_->playback_get_position();
    info->duration = playbackControl_->playback_get_length_ex();

    if (auto queryImpl = dynamic_cast<ColumnsQueryImpl*>(query))
    {
        info->columns = evaluatePlaybackColumns(queryImpl->columns);
    }
}

PlayerStatePtr PlayerImpl::queryPlayerState(ColumnsQuery* activeItemQuery)
{

    auto state = std::make_unique<PlayerState>();

    state->playbackState = getPlaybackState();
    queryActiveItem(&state->activeItem, activeItemQuery);

    return state;
}

void PlayerImpl::playCurrent()
{
    playbackControl_->play_or_unpause();
}

void PlayerImpl::playRandom()
{
    playbackControl_->start(playback_control::track_command_rand);
}

void PlayerImpl::playNext()
{
    playbackControl_->next();
}

void PlayerImpl::playPrevious()
{
    playbackControl_->previous();
}

void PlayerImpl::stop()
{
    playbackControl_->stop();
}

void PlayerImpl::pause()
{
    playbackControl_->pause(true);
}

void PlayerImpl::playOrPause()
{
    playbackControl_->play_or_pause();
}

void PlayerImpl::togglePause()
{
    playbackControl_->toggle_pause();
}

void PlayerImpl::seekAbsolute(double offsetSeconds)
{
    playbackControl_->playback_seek(offsetSeconds);
}

void PlayerImpl::seekRelative(double offsetSeconds)
{
    playbackControl_->playback_seek_delta(offsetSeconds);
}

}

}
