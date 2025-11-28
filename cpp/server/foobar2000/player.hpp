#pragma once

#include "common.hpp"
#include "../player_api.hpp"
#include "utils.hpp"

namespace msrv::player_foobar2000 {

class ColumnsQueryImpl final : public ColumnsQuery
{
public:
    ColumnsQueryImpl(TitleFormatVector columnsVal)
        : columns(std::move(columnsVal))
    {
    }

    ~ColumnsQueryImpl() = default;

    TitleFormatVector columns;
};

class PlayerImpl final : public Player
{
public:
    PlayerImpl();
    ~PlayerImpl() override;

    const char* name() override;
    std::unique_ptr<WorkQueue> createWorkQueue() override;

    PlayerStatePtr queryPlayerState(ColumnsQuery* activeItemQuery = nullptr) override;

    void playCurrent() override;
    void playRandom() override;
    void playNext() override;
    void playPrevious() override;
    void stop() override;
    void pause() override;
    void togglePause() override;
    void playOrPause() override;

    void seekAbsolute(double offsetSeconds) override;
    void seekRelative(double offsetSeconds) override;

    ColumnsQueryPtr createColumnsQuery(const std::vector<std::string>& columns) override;

private:
    PlaybackState getPlaybackState();
    void queryActiveItem(ActiveItemInfo* info, ColumnsQuery* query);

    TitleFormatVector compileColumns(const std::vector<std::string>& columns);

    std::vector<std::string> evaluatePlaybackColumns(
        const TitleFormatVector& compiledColumns);

    service_ptr_t<playback_control_v3> playbackControl_;
    service_ptr_t<titleformat_compiler> titleFormatCompiler_;

    PlayerEventAdapter playerEventAdapter_;

    MSRV_NO_COPY_AND_ASSIGN(PlayerImpl);
};

}
