list(APPEND MODULES enum_serializer json_struct)
include(waffle)
waffle_add_executable(wafflec "${MODULES}")
