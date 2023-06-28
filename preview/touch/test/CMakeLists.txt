add_executable(preview.touch.test)
emil_build_for(preview.touch.test BOOL PREVIEW_BUILD_TESTS)
emil_add_test(preview.touch.test)

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
