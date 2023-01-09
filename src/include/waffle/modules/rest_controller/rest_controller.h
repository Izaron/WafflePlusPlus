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
    int Code;
    std::string Body;
};

class IRestController {
public:
    virtual HttpResponse ProcessRequest(const HttpRequest& httpRequest) = 0;
};

template<typename Handler>
class RestController : public IRestController {
public:
    RestController(std::shared_ptr<Handler> handler);
    HttpResponse ProcessRequest(const HttpRequest& httpRequest) override;
private:
    class Impl;
    std::unique_ptr<Impl> Impl_;
};

} // namespace Waffle
