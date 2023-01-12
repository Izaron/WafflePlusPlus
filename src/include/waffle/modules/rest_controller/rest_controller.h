#pragma once

#include <memory>
#include <string>

namespace Waffle {

struct HttpRequest {
    std::string Method;
    std::string Path;
    std::string Body;
};

struct HttpResponse {
    int StatusCode;
    std::string Body;
};

template<typename Handler>
[[nodiscard]] HttpResponse ProcessRequest(Handler& handler, const HttpRequest& httpRequest);

} // namespace Waffle
