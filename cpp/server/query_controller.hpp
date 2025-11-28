#pragma once

#include "controller.hpp"
#include "defines.hpp"
#include "player_events.hpp"

#include <memory>

namespace msrv {

class Router;

class QueryController : public ControllerBase
{
public:
    QueryController(Request* request, Player* player, EventDispatcher* dispatcher);
    ~QueryController();

    ResponsePtr getUpdates();

    static void defineRoutes(
        Router* router, WorkQueue* workQueue, Player* player, EventDispatcher* dispatcher);

private:
    void listenForEvents();

    static Json eventsToJson();
    Json stateToJson();

    Player* player_;
    EventDispatcher* dispatcher_;

    std::unique_ptr<EventListener> listener_;
    ColumnsQueryPtr queueQuery_;
};

}
