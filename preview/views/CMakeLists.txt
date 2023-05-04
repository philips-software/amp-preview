add_library(preview.views ${PREVIEW_EXCLUDE_FROM_ALL} STATIC)
install(TARGETS preview.views EXPORT previewTargets)

target_include_directories(preview.views PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(preview.views PUBLIC
    preview.interfaces
    preview.fonts
)

target_sources(preview.views PRIVATE
    HorizontalLayout.cpp
    HorizontalLayout.hpp
    HorizontalShifter.cpp
    HorizontalShifter.hpp
    RestrictPaint.hpp
    VerticalLayout.cpp
    VerticalLayout.hpp
    VerticalShifter.cpp
    VerticalShifter.hpp
    ViewAligned.cpp
    ViewAligned.hpp
    ViewBitmap.cpp
    ViewBitmap.hpp
    ViewButtonPanel.cpp
    ViewButtonPanel.hpp
    ViewCurrentTime.cpp
    ViewCurrentTime.hpp
    ViewDisableableText.cpp
    ViewDisableableText.hpp
    ViewFadingText.cpp
    ViewFadingText.hpp
    ViewFramedText.cpp
    ViewFramedText.hpp
    ViewFramedTextButton.cpp
    ViewFramedTextButton.hpp
    ViewFramedTextButtonWithPopOut.cpp
    ViewFramedTextButtonWithPopOut.hpp
    ViewFrame.cpp
    ViewFrame.hpp
    ViewIcon.cpp
    ViewIcon.hpp
    ViewMultiple.cpp
    ViewMultiple.hpp
    ViewOffsetRegion.cpp
    ViewOffsetRegion.hpp
    ViewPanel.cpp
    ViewPanel.hpp
    ViewRotating.cpp
    ViewRotating.hpp
    ViewScrolling.cpp
    ViewScrolling.hpp
    ViewSelector.cpp
    ViewSelector.hpp
    ViewText.cpp
    ViewText.hpp
    ViewTitledFrame.cpp
    ViewTitledFrame.hpp
)

add_subdirectory(test)
