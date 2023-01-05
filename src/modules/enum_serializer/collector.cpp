#include "collector.h"

#include <lib/comment/comment.h>
#include <lib/string_util/string_util.h>

#include <clang/AST/RecursiveASTVisitor.h>

namespace Waffle::EnumSerializer {

namespace {

constexpr std::string_view COMMAND_SERIALIZABLE = "serializable";
constexpr std::string_view COMMAND_STRING_VALUE = "stringvalue";

class EnumDataCollector : public clang::RecursiveASTVisitor<EnumDataCollector> {
public:
    explicit EnumDataCollector(clang::ASTContext& ctx) : Ctx_{ctx} {}

    bool VisitEnumDecl(clang::EnumDecl* decl) {
        if (ParseCommentData(Ctx_, *decl)->FindByName(COMMAND_SERIALIZABLE)) {
            AddSerializedEnum(decl);
        }
        return true;
    }

    EnumDatas&& GetDatas() && {
        return std::move(Datas_);
    }

private:
    void AddSerializedEnum(const clang::EnumDecl* decl) {
        auto& data = Datas_.emplace_back();
        data.Decl = decl;
        for (const auto* enumConstantDecl : decl->enumerators()) {
            data.Constants.emplace_back(enumConstantDecl, CalculateStringValues(enumConstantDecl));
        }
    }

    std::vector<std::string> CalculateStringValues(const clang::EnumConstantDecl* decl) {
        std::vector<std::string> result;
        auto commentData = ParseCommentData(Ctx_, *decl);
        if (const auto command = commentData->FindByName(COMMAND_STRING_VALUE)) {
            for (const auto part : StringUtil::SplitBySpace(command->Text)) {
                result.emplace_back(part);
            }
        }
        if (result.empty()) {
            result.emplace_back(decl->getNameAsString());
        }
        return result;
    }

private:
    clang::ASTContext& Ctx_;
    EnumDatas Datas_;
};

} // namespace

EnumDatas CollectEnumDatas(clang::ASTContext& ctx) {
    EnumDataCollector collector{ctx};
    collector.TraverseDecl(ctx.getTranslationUnitDecl());
    return std::move(collector).GetDatas();
}

std::span<const std::string_view> Commands() {
    static const std::vector<std::string_view> COMMANDS = {
        COMMAND_SERIALIZABLE,
        COMMAND_STRING_VALUE,
    };
    return COMMANDS;
}

} // namespace Waffle::EnumSerializer
