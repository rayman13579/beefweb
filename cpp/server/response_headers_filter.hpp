#pragma once
#include "request_filter.hpp"

namespace msrv {
class ResponseHeadersFilter : public RequestFilter
{
public:
    explicit ResponseHeadersFilter();
    ~ResponseHeadersFilter() override;
    virtual void endRequest(Request* request) override;
};

}
