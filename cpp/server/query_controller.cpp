#include "query_controller.hpp"
#include "core_types_json.hpp"
#include "core_types_parsers.hpp"
#include "player_api_json.hpp"
#include "router.hpp"

namespace msrv {

namespace {

constexpr char PLAYER_KEY[] = "player";
constexpr char MUTED_KEY[] = "muted";

}

QueryController::QueryController(
    Request* request, Player* player, EventDispatcher* dispatcher)
    : ControllerBase(request), player_(player), dispatcher_(dispatcher), voicemeeterClient_(new VoicemeeterClient())
{
}

QueryController::~QueryController() = default;

ResponsePtr QueryController::getUpdates()
{
    listenForEvents();

    return Response::eventStream([this] {
        listener_->readEvents();
        return stateToJson();
    });
}

void QueryController::listenForEvents()
{
    listener_ = dispatcher_->createListener();
}

Json QueryController::stateToJson()
{
    Json obj = Json::object();
    Json state(*player_->queryPlayerState());
    Json muted(voicemeeterClient_->getMuted());
    obj[PLAYER_KEY] = state;
    obj[MUTED_KEY] = muted == 1.0f ? true : false;

    return obj;
}

void QueryController::defineRoutes(
    Router* router, WorkQueue* workQueue, Player* player, EventDispatcher* dispatcher)
{
    auto routes = router->defineRoutes<QueryController>();

    routes.createWith([=](Request* request) { return new QueryController(request, player, dispatcher); });
    routes.useWorkQueue(workQueue);

    routes.get("foobar2000", &QueryController::getUpdates);
}

}
