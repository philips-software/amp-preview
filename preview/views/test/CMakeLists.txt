add_executable(preview.views.test)
emil_build_for(preview.views.test BOOL PREVIEW_BUILD_TESTS)
emil_add_test(preview.views.test)

target_link_libraries(preview.views.test PUBLIC
    preview.interfaces.test_doubles
    preview.views
    gtest
    gmock
    gmock_main
)

target_sources(preview.views.test PRIVATE
    TestHorizontalLayout.cpp
    TestRestrictPaint.cpp
    TestVerticalLayout.cpp
    TestViewBitmap.cpp
    TestViewFrame.cpp
    TestViewFramedText.cpp
    TestViewOffsetRegion.cpp
    TestViewPanel.cpp
    TestViewSelector.cpp
    TestViewText.cpp
    TestViewTitledFrame.cpp
)
