add_executable(preview.interfaces.test)
add_test(NAME preview.interfaces.test COMMAND preview.interfaces.test)

target_link_libraries(preview.interfaces.test PUBLIC
    preview.interfaces.test_doubles
    hal.interfaces_test_doubles
    gtest
    gmock
    gmock_main
)

target_sources(preview.interfaces.test PRIVATE
    TestGeometry.cpp
    TestWindowBitmapCanvas.cpp
    TestViewPainterBufferedDisplay.cpp
    TestViewPainterDirectDisplay.cpp
    TestViewPainterDoubleBufferDisplay.cpp
    TestViewRepainter.cpp
    TestBitmap.cpp
)
