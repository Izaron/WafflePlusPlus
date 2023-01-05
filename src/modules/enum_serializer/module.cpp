#include "module.h"

#include <lib/comment/comment.h>

#include <clang/AST/RecursiveASTVisitor.h>

using namespace Waffle;

namespace {

constexpr std::string_view MODULE_NAME = "enum_serializer";

constexpr std::string_view COMMAND_SERIALIZABLE = "serializable";
constexpr std::string_view COMMAND_STRING_VALUE = "stringvalue";

class EnumVisitor : public clang::RecursiveASTVisitor<EnumVisitor> {
public:
    explicit EnumVisitor(Context& ctx)
        : Ctx_{ctx}
    {}

    bool VisitEnumDecl(clang::EnumDecl* decl) {
        auto commentData = ParseCommentData(Ctx_.AstContext, *decl);
        const auto* serializableCommand = commentData->FindByName(COMMAND_SERIALIZABLE);
        if (!serializableCommand) {
            return true;
        }
        decl->dump();
        return true;
    }

private:
    Context& Ctx_;
};

} // namespace

std::string_view EnumSerializerModule::Name() const {
    return MODULE_NAME;
}

std::span<const std::string_view> EnumSerializerModule::Commands() const {
    static const std::vector<std::string_view> COMMANDS = {
        COMMAND_SERIALIZABLE,
        COMMAND_STRING_VALUE,
    };
    return COMMANDS;
}

void EnumSerializerModule::Do(Context& ctx) const {
    EnumVisitor visitor{ctx};
    visitor.TraverseDecl(ctx.AstContext.getTranslationUnitDecl());

    auto& printer = ctx.FileManager.GetOrCreateFilePrinter("temp.cpp");
    printer << "// GENERATED FILE\n";
    printer << "// DO NOT CHANGE\n";
    printer << "int kek = 1337;\n";
}

REGISTER_MODULE(EnumSerializerModule);
