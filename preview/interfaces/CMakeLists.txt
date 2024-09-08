add_library(preview.interfaces ${PREVIEW_EXCLUDE_FROM_ALL} STATIC)

target_include_directories(preview.interfaces PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(preview.interfaces PUBLIC
    infra.timer
    infra.util
    hal.interfaces
    services.tracer
)

target_sources(preview.interfaces PRIVATE
    Bitmap.cpp
    Bitmap.hpp
    BitmapCanvas.cpp
    BitmapCanvas.hpp
    BitmapPainter.cpp
    BitmapPainter.hpp
    BufferedDisplay.hpp
    BufferedDisplayIli9341.cpp
    BufferedDisplayIli9341.hpp
    BufferedDisplaySsd1306.cpp
    BufferedDisplaySsd1306.hpp
    Button.hpp
    Canvas.hpp
    Colour.cpp
    Colour.hpp
    DirectDisplay.hpp
    DoubleBufferDisplay.hpp
    Font.cpp
    Font.hpp
    Geometry.cpp
    Geometry.hpp
    MultiBufferDisplay.hpp
    QRCode.cpp
    QRCode.hpp
    View.cpp
    View.hpp
    ViewOverlay.cpp
    ViewOverlay.hpp
    ViewPainter.hpp
    ViewPainterBufferedDisplay.cpp
    ViewPainterBufferedDisplay.hpp
    ViewPainterDirectDisplay.cpp
    ViewPainterDirectDisplay.hpp
    ViewPainterDoubleBufferDisplay.cpp
    ViewPainterDoubleBufferDisplay.hpp
    ViewRepainter.cpp
    ViewRepainter.hpp
    WindowBitmapCanvas.cpp
    WindowBitmapCanvas.hpp
)

add_subdirectory(test_doubles)
add_subdirectory(test)
