#include "response_headers_filter.hpp"
#include "request.hpp"
#include "response.hpp"

namespace msrv {

ResponseHeadersFilter::ResponseHeadersFilter()
{
}

ResponseHeadersFilter::~ResponseHeadersFilter() = default;

void ResponseHeadersFilter::endRequest(Request* request)
{
    if (request->response)
    {
        const std::unordered_map<std::string, std::string> CORS = { {"Access-Control-Allow-Origin", "*"} };
        request->response->addHeaders(CORS);
    }
}

}
