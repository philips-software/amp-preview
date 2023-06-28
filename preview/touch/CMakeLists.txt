add_library(preview.touch ${PREVIEW_EXCLUDE_FROM_ALL} STATIC)

target_include_directories(preview.touch PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(preview.touch PUBLIC
    preview.interfaces
    preview.views
)

target_sources(preview.touch PRIVATE
    TouchAligned.cpp
    TouchAligned.hpp
    TouchButton.cpp
    TouchButton.hpp
    TouchFrame.hpp
    TouchFt5x06.cpp
    TouchFt5x06.hpp
    TouchHorizontalLayout.cpp
    TouchHorizontalLayout.hpp
    TouchPanel.cpp
    TouchPanel.hpp
    TouchRecipient.hpp
    TouchScreen.cpp
    TouchScreen.hpp
    TouchScreenToTouchRecipientInteractor.cpp
    TouchScreenToTouchRecipientInteractor.hpp
    TouchSpinInteger.cpp
    TouchSpinInteger.hpp
    TouchSubView.cpp
    TouchSubView.hpp
    TouchTitledFrame.hpp
    TouchVerticalLayout.cpp
    TouchVerticalLayout.hpp
    TouchViewMultiple.cpp
    TouchViewMultiple.hpp
    TouchViewSelector.hpp
    TouchViewSelector.cpp
)

add_subdirectory(test)
