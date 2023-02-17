add_executable(preview.touch.test)
add_test(NAME preview.touch.test COMMAND preview.touch.test)

target_link_libraries(preview.touch.test PUBLIC
    preview.interfaces.test_doubles
    preview.touch
    infra.timer_test_helper
    gtest
    gmock
    gmock_main
)

target_sources(preview.touch.test PRIVATE
    TestTouchButton.cpp
    TestTouchHorizontalLayout.cpp
    TestTouchVerticalLayout.cpp
    TouchRecipientMock.hpp
    TestTouchScreen.cpp
)
