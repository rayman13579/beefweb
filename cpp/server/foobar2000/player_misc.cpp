#include "project_info.hpp"
#include "player.hpp"

namespace msrv {
namespace player_foobar2000 {

PlayerImpl::PlayerImpl()
    : playbackControl_(playback_control_v3::get()),
      titleFormatCompiler_(titleformat_compiler::get())
{
    auto callback = [this]() { emitEvents(); };
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

ColumnsQueryPtr PlayerImpl::createColumnsQuery()
{
    return std::make_unique<ColumnsQueryImpl>(compileColumns());
}

TitleFormatVector PlayerImpl::compileColumns()
{
    std::vector<std::string> columns = {"%artist%", "%title%"};
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
