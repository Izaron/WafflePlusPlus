#include "module.h"

#include <clang/AST/Comment.h>
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
        const auto* comment = Ctx_.AstContext.getCommentForDecl(decl, nullptr);
        if (!comment) {
            return true;
        }

        for (const auto* block : comment->getBlocks()) {
            const auto* blockCommand = clang::dyn_cast_or_null<clang::comments::BlockCommandComment>(block);
            if (!blockCommand) {
                continue;
            }
            llvm::errs() << "COMMAND NAME: " << blockCommand->getCommandName(Ctx_.AstContext.getCommentCommandTraits()) << "\n";

            const auto* paragraph = blockCommand->getParagraph();
            if (!paragraph) {
                continue;
            }
            for (const auto* child = paragraph->child_begin(); child != paragraph->child_end(); ++child) {
                const auto* textComment = clang::dyn_cast_or_null<clang::comments::TextComment>(*child);
                if (!textComment) {
                    continue;
                }
                llvm::errs() << "GOT TEXT: " << textComment->getText() << "\n";
            }
        }
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
