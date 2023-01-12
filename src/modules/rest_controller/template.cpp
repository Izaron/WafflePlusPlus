// Generated by the Waffle++ code generator. DO NOT EDIT!
// source: {{ source_file }}

#include <waffle/modules/json_dump/json_dump.h>
#include <waffle/modules/rest_controller/rest_controller.h>

#include <lib/string_util/string_util.h>

#include "{{ source_file }}"

namespace Waffle {

namespace Impl {

using PathParts = std::vector<std::string_view>;

inline constexpr std::string_view DELIMS = "/?=&";

inline bool PatternMatches(std::string_view pattern, std::string_view requestPath) {
    PathParts patternParts = StringUtil::SplitByDelims(pattern, DELIMS);
    PathParts requestPathParts = StringUtil::SplitByDelims(requestPath, DELIMS);

    if (patternParts.size() != requestPathParts.size()) {
        return false;
    }

    for (size_t i = 0; i < patternParts.size(); ++i) {
        if (patternParts[i].front() == '{' && patternParts[i].back() == '}') {
            // this is a placeholder like '{id}'
            continue;
        }
        if (patternParts[i] != requestPathParts[i]) {
            return false;
        }
    }

    return true;
}

template<typename T>
T FindPlaceholderValue(std::string_view pattern, std::string_view requestPath, std::string_view placeholder);

template<>
inline std::string_view FindPlaceholderValue<std::string_view>(std::string_view pattern, std::string_view requestPath, std::string_view placeholder) {
    // assuming that `PatternMatches(pattern, requestPath) == true`
    PathParts patternParts = StringUtil::SplitByDelims(pattern, DELIMS);
    PathParts requestPathParts = StringUtil::SplitByDelims(requestPath, DELIMS);

    const auto iter = std::find(patternParts.begin(), patternParts.end(), placeholder);
    return requestPathParts[std::distance(patternParts.begin(), iter)];
}

template<>
inline size_t FindPlaceholderValue<size_t>(std::string_view pattern, std::string_view requestPath, std::string_view placeholder) {
    std::string_view str = FindPlaceholderValue<std::string_view>(pattern, requestPath, placeholder);
    size_t result;
    std::sscanf(str.data(), "%zu", &result);
    return result;
}

template<>
inline double FindPlaceholderValue<double>(std::string_view pattern, std::string_view requestPath, std::string_view placeholder) {
    std::string_view str = FindPlaceholderValue<std::string_view>(pattern, requestPath, placeholder);
    double result;
    std::sscanf(str.data(), "%lf", &result);
    return result;
}

} // namespace Impl

## for struct in structs
template<>
HttpResponse ProcessRequest({{ struct.name }}& handler, const HttpRequest& request) {
    try {
        HttpResponse response;
        response.StatusCode = 200;
## for method in struct.methods
        if (request.Method == "{{ method.http_method }}" && Impl::PatternMatches("{{ method.mapping }}", request.Path)) {
## for param in method.params
{% if param.kind == "requestbody" %}
            {{ param.type }} arg{{ loop.index1 }} = FromJson<{{ param.type }}>(nlohmann::json::parse(request.Body));
{% else if param.kind == "pathvariable" %}
            {{ param.type }} arg{{ loop.index1 }} = Impl::FindPlaceholderValue<{{ param.type }}>("{{ method.mapping }}", request.Path, "{" "{{ param.name }}" "}");
{% endif %}
## endfor
{% if method.return_type == "void" %}
            handler.{{ method.name }}({{ list_args(method.params) }});
{% else %}
            auto result = handler.{{ method.name }}({{ list_args(method.params) }});
            response.Body = ToJson(result).dump(/*indent=*/4);
{% endif %}
            return response;
        }
## endfor
        throw std::runtime_error("Can't handle " + request.Method + " request with path \"" + request.Path + "\"");
    } catch (const std::exception& ex) {
        nlohmann::json bodyJson;
        bodyJson["reason"] = ex.what();

        HttpResponse response;
        response.StatusCode = 500;
        response.Body = bodyJson.dump(/*indent=*/4);
        return response;
    }
}

## endfor
} // namespace Waffle
