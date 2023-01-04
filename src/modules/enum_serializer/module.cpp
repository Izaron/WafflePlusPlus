#include "module.h"

#include <clang/AST/RecursiveASTVisitor.h>

using namespace Waffle;

namespace {

class EnumVisitor : public clang::RecursiveASTVisitor<EnumVisitor> {
public:
    explicit EnumVisitor(Context& ctx)
        : Ctx_{ctx}
    {}

    bool VisitEnumDecl(clang::EnumDecl* decl) {
        decl->dump();
        return true;
    }

private:
    Context& Ctx_;
};

} // namespace

std::string_view EnumSerializerModule::Name() const {
    return "enum_serializer";
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
