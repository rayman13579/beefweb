#include "content_type_map.hpp"
#include "http.hpp"
#include <stdlib.h>

namespace msrv {

namespace {

const uint8_t JPEG_HEADER[] = {0xFF, 0xD8, 0xFF};

const uint8_t PNG_HEADER[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

const uint8_t GIF_HEADER[] = {0x47, 0x49, 0x46, 0x38};

const uint8_t BMP_HEADER[] = {0x42, 0x4D};

inline bool matchHeader(
    const std::vector<uint8_t>& fileData,
    const uint8_t* header,
    size_t size)
{
    return fileData.size() >= size
        && memcmp(fileData.data(), header, size) == 0;
}

}

ContentTypeMap::ContentTypeMap()
    : defaultType_(ContentType::APPLICATION_OCTET_STREAM)
{
    add(ContentType::TEXT_HTML_UTF8, "htm", "html");
    add(ContentType::TEXT_PLAIN_UTF8, "txt");
    add(ContentType::APPLICATION_JAVASCRIPT, "js");
    add(ContentType::APPLICATION_JSON, "json");
}

ContentTypeMap::~ContentTypeMap() = default;

const std::string& ContentTypeMap::byHeader(const std::vector<uint8_t>& header) const
{
    return defaultType_;
}

void ContentTypeMap::add(const std::string& contentType, const std::string& ext)
{
    mapping_.emplace(pathFromUtf8("." + ext).native(), contentType);
}

void ContentTypeMap::add(const std::string& contentType, const std::string& ext, const std::string& ext2)
{
    add(contentType, ext);
    add(contentType, ext2);
}

}
