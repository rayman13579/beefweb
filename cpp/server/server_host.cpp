#include "server_host.hpp"
#include "query_controller.hpp"
#include "response_headers_filter.hpp"
#include "log.hpp"

namespace msrv {

ServerHost::ServerHost(Player* player)
    : player_(player), utilityQueue_(8, MSRV_THREAD_NAME("io"))
{
    playerWorkQueue_ = player_->createWorkQueue();
    player_->onEvents([this]() { handlePlayerEvents(); });
    serverThread_ = std::make_unique<ServerThread>();
}

ServerHost::~ServerHost()
{
    player_->onEvents(PlayerEventsCallback());
}

void ServerHost::handlePlayerEvents()
{
    dispatcher_.dispatch();
    serverThread_->dispatchEvents();
}

void ServerHost::reconfigure()
{
    auto config = std::make_unique<ServerConfig>();

    auto router = &config->router;
    auto filters = &config->filters;

    filters->add(std::make_unique<ExecuteHandlerFilter>());
    filters->add(std::make_unique<ResponseHeadersFilter>());
    auto playerQueue = playerWorkQueue_.get();

    QueryController::defineRoutes(router, playerQueue, player_, &dispatcher_);

    serverThread_->restart(std::move(config));
}

}
