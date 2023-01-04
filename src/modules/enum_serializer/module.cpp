#include "module.h"

using namespace Waffle;

std::string_view EnumSerializerModule::Name() const {
    return "enum_serializer";
}

void EnumSerializerModule::Do(Context& context) const {
    auto& printer = context.FileManager.GetOrCreateFilePrinter("temp.cpp");
    printer << "// GENERATED FILE\n";
    printer << "// DO NOT CHANGE\n";
    printer << "int kek = 1337;\n";
}

REGISTER_MODULE(EnumSerializerModule);
