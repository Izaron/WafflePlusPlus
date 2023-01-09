list(APPEND MODULES enum_serializer json_dump data_class google_mock)
include(waffle)
waffle_add_executable(wafflec "${MODULES}")
