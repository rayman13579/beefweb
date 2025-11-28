#pragma once

#include "common.hpp"
#include "../log.hpp"
#include "../work_queue.hpp"
#include "../player_api.hpp"

namespace msrv::player_foobar2000 {

namespace prefs_pages {
extern const GUID main;
extern const GUID permissions;
}

boost::optional<GUID> tryParseGuid(const char* str);
boost::optional<std::pair<GUID, GUID>> tryParseDoubleGuid(const char* str);
std::string doubleGuidToString(const GUID& guid1, const GUID& guid2);

using TitleFormatVector = std::vector<service_ptr_t<titleformat_object>>;

class Fb2kLogger : public Logger
{
public:
    Fb2kLogger();

    void log(LogLevel, const char*, va_list va) override;

private:
    std::string prefix_;
};

class Fb2kWorkQueue : public ExternalWorkQueue
{
protected:
    void schedule(WorkCallback callback) override;
};

class PlayerEventAdapter final : play_callback
{
public:
    PlayerEventAdapter()
    {
        constexpr auto flags = flag_on_playback_all & ~(flag_on_playback_dynamic_info | flag_on_playback_time) | flag_on_volume_change;
        play_callback_manager::get()->register_callback(this, flags, false);
    }

    ~PlayerEventAdapter()
    {
        play_callback_manager::get()->unregister_callback(this);
    }

    void setCallback(PlayerEventsCallback callback)
    {
        callback_ = std::move(callback);
    }

private:
    void on_playback_starting(play_control::t_track_command p_command, bool p_paused) override
    {
        notify();
    }

    void on_playback_new_track(metadb_handle_ptr p_track) override
    {
        notify();
    }

    void on_playback_stop(play_control::t_stop_reason p_reason) override
    {
        notify();
    }

    void on_playback_seek(double p_time) override
    {
        notify();
    }

    void on_playback_pause(bool p_state) override
    {
        notify();
    }

    void on_playback_edited(metadb_handle_ptr p_track) override
    {
        notify();
    }

    void on_playback_dynamic_info(const file_info& p_info) override
    {
        /* ignore */
    }

    void on_playback_dynamic_info_track(const file_info& p_info) override
    {
        notify();
    }

    void on_playback_time(double p_time) override
    {
        //TODO can I use this instead of countin myself?
        /* ignore */
    }

    void on_volume_change(float p_new_val) override
    {
        notify();
    }

    void notify()
    {
        if (callback_)
            callback_();
    }

    PlayerEventsCallback callback_;

    MSRV_NO_COPY_AND_ASSIGN(PlayerEventAdapter);
};


}
