#include "collector.h"

#include <optional>

#include <lib/comment/comment.h>

#include <clang/AST/RecursiveASTVisitor.h>

namespace Waffle::RestController {

namespace {

constexpr std::string_view COMMAND_REST_CONTROLLER = "restcontroller";
constexpr std::string_view COMMAND_GET_MAPPING = "getmapping";
constexpr std::string_view COMMAND_POST_MAPPING = "postmapping";
constexpr std::string_view COMMAND_DELETE_MAPPING = "deletemapping";

class Collector : public clang::RecursiveASTVisitor<Collector> {
public:
    explicit Collector(clang::ASTContext& ctx) : Ctx_{ctx} {}

    bool VisitCXXRecordDecl(clang::CXXRecordDecl* decl) {
        if (ParseCommentData(Ctx_, *decl)->FindByName(COMMAND_REST_CONTROLLER)) {
            auto& data = Datas_.emplace_back();
            data.Decl = decl;

            for (const auto methodDecl : decl->methods()) {
                if (const auto httpMethod = TryGetHttpMethod(*methodDecl)) {
                    data.Mapping.emplace(methodDecl, *httpMethod);
                }
            }
        }
        return true;
    }

    StructDatas&& GetDatas() && {
        return std::move(Datas_);
    }

private:
    std::optional<std::string_view> TryGetHttpMethod(const clang::CXXMethodDecl& decl) {
        static const std::unordered_map<std::string_view, std::string_view> COMMAND_TO_METHOD = {
            {COMMAND_GET_MAPPING, "GET"},
            {COMMAND_POST_MAPPING, "POST"},
            {COMMAND_DELETE_MAPPING, "DELETE"},
        };
        for (const auto& [command, method] : COMMAND_TO_METHOD) {
            if (ParseCommentData(Ctx_, decl)->FindByName(command)) {
                return method;
            }
        }
        return std::nullopt;
    }

private:
    clang::ASTContext& Ctx_;
    StructDatas Datas_;
};

} // namespace

StructDatas Collect(clang::ASTContext& ctx) {
    Collector collector{ctx};
    collector.TraverseDecl(ctx.getTranslationUnitDecl());
    return std::move(collector).GetDatas();
}

std::vector<std::string_view> Commands() {
    return {
        COMMAND_REST_CONTROLLER,
        COMMAND_GET_MAPPING,
        COMMAND_POST_MAPPING,
        COMMAND_DELETE_MAPPING,
    };
}

} // namespace Waffle::RestController
