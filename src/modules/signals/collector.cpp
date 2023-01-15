#include "collector.h"

#include <lib/comment/comment.h>
#include <lib/string_util/string_util.h>

#include <clang/AST/RecursiveASTVisitor.h>

namespace Waffle::Signals {

namespace {

constexpr std::string_view COMMAND_SIGNAL = "signal";

class Collector : public clang::RecursiveASTVisitor<Collector> {
public:
    explicit Collector(clang::ASTContext& ctx) : Ctx_{ctx} {}

    bool VisitCXXRecordDecl(clang::CXXRecordDecl* decl) {
        if (!decl->isThisDeclarationADefinition()) {
            return true;
        }
        if (!IsDerivedFromSignalBase(*decl)) {
            return true;
        }

        auto& data = Datas_.emplace_back();
        data.Decl = decl;
        for (const auto& method : decl->methods()) {
            if (ParseCommentData(Ctx_, *method)->FindByName(COMMAND_SIGNAL)) {
                data.Signals.emplace_back(method);
            }
        }
        return true;
    }

    StructDatas&& GetDatas() && {
        return std::move(Datas_);
    }

private:
    bool IsDerivedFromSignalBase(clang::CXXRecordDecl& decl) {
        for (const auto& base : decl.bases()) {
            const auto* type = base.getType().getTypePtr()->getUnqualifiedDesugaredType();
            if (GetTypeName(*type) == "Waffle::SignalBase") {
                return true;
            }
        }
        return false;
    }

    std::string GetTypeName(const clang::Type& type) {
        if (const auto* typeDecl = type.getAsTagDecl()) {
            return StringUtil::QualifiedName(*typeDecl);
        }
        return "";
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
    return {COMMAND_SIGNAL};
}

} // namespace Waffle::Signals
