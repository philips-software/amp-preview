add_library(preview.stm32fxxx STATIC)
install(TARGETS preview.stm32fxxx EXPORT previewTargets)

set_target_properties(preview.stm32fxxx PROPERTIES EXCLUDE_FROM_ALL $<NOT:$<AND:$<STREQUAL:${TARGET_MCU_FAMILY},stm32f7xx>,$<BOOL:${PREVIEW_STANDALONE}>>>)

target_include_directories(preview.stm32fxxx PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(preview.stm32fxxx PUBLIC
    preview.interfaces
    hal_st.stm32fxxx
)

target_sources(preview.stm32fxxx PRIVATE
    BitmapPainterStm.cpp
    BitmapPainterStm.hpp
    LcdStm.cpp
    LcdStm.hpp
)
