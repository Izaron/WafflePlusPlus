list(APPEND MODULES enum_serializer json_dump)
include(waffle)
waffle_add_executable(wafflec "${MODULES}")
