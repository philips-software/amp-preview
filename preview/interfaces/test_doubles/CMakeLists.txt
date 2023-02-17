add_library(preview.interfaces.test_doubles EXCLUDE_FROM_ALL INTERFACE)

target_link_libraries(preview.interfaces.test_doubles INTERFACE
    gmock
    preview.interfaces
)

target_sources(preview.interfaces.test_doubles PRIVATE
    BitmapPainterMock.hpp
    BufferedDisplayMock.hpp
    CanvasMock.hpp
    DirectDisplayMock.hpp
    DoubleBufferDisplayMock.hpp
    ButtonMock.hpp
    ViewMock.hpp
    ViewPainterMock.hpp
)
