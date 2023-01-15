list(APPEND MODULES
    data_class
    enum_serializer
    google_mock
    json_dump
    poly_ptr
    rest_controller
    signals)
include(waffle)
waffle_add_executable(wafflec "${MODULES}")
