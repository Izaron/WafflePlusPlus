list(APPEND MODULES enum_serializer dump_struct)
include(waffle)
waffle_add_executable(wafflec "${MODULES}")
