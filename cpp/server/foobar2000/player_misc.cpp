#include "project_info.hpp"
#include "player.hpp"

namespace msrv {
namespace player_foobar2000 {

PlayerImpl::PlayerImpl()
    : playbackControl_(playback_control_v3::get()),
      titleFormatCompiler_(titleformat_compiler::get())
{
    auto callback = [this](PlayerEvents ev) { emitEvents(ev); };
    playerEventAdapter_.setCallback(callback);
}

PlayerImpl::~PlayerImpl()
{
}

const char* PlayerImpl::name()
{
    return MSRV_PLAYER_FOOBAR2000;
}

std::unique_ptr<WorkQueue> PlayerImpl::createWorkQueue()
{
    return std::make_unique<Fb2kWorkQueue>();
}

ColumnsQueryPtr PlayerImpl::createColumnsQuery(const std::vector<std::string>& columns)
{
    return std::make_unique<ColumnsQueryImpl>(compileColumns(columns));
}

TitleFormatVector PlayerImpl::compileColumns(const std::vector<std::string>& columns)
{
    TitleFormatVector compiledColumns;
    compiledColumns.reserve(columns.size());

    for (auto& column : columns)
    {
        service_ptr_t<titleformat_object> compiledColumn;

        if (!titleFormatCompiler_->compile(compiledColumn, column.c_str()))
            throw InvalidRequestException("invalid format expression: " + column);

        compiledColumns.emplace_back(std::move(compiledColumn));
    }

    return compiledColumns;
}

}

}
