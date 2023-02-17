add_library(preview.fonts EXCLUDE_FROM_ALL STATIC)
install(TARGETS preview.fonts EXPORT previewTargets)

target_include_directories(preview.fonts PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(preview.fonts PUBLIC
    preview.interfaces
    infra.util
)

target_sources(preview.fonts PRIVATE
    FreeSans9pt7b.cpp
    FreeSans12pt7b.cpp
    FreeSans18pt7b.cpp
    FreeSans24pt7b.cpp
    Fonts.hpp
)
