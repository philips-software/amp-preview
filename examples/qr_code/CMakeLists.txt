if (EMIL_BUILD_WIN OR TARGET_MCU STREQUAL stm32f746)
    add_executable(examples.qr_code)
else()
    add_library(examples.qr_code INTERFACE) # Other platforms have no sources
endif()
emil_build_for(examples.qr_code HOST Windows TARGET_MCU stm32f746 PREREQUISITE_BOOL PREVIEW_BUILD_EXAMPLES)

set_target_properties(examples.qr_code PROPERTIES WIN32_EXECUTABLE $<BOOL:${EMIL_BUILD_WIN}>)
if (EMIL_BUILD_WIN)
    target_compile_definitions(examples.qr_code PUBLIC NOMINMAX)
endif()

target_include_directories(examples.qr_code PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_sources(examples.qr_code PRIVATE
    $<$<BOOL:${EMIL_BUILD_WIN}>:MainWin.cpp>
    $<$<STREQUAL:${TARGET_MCU},stm32f746>:MainStm32f746.cpp>
)

target_link_libraries(examples.qr_code PUBLIC
    $<$<BOOL:${EMIL_BUILD_WIN}>:hal.generic>
    $<$<BOOL:${EMIL_BUILD_WIN}>:preview.sdl>
    $<$<STREQUAL:${TARGET_MCU},stm32f746>:hal_st.stm32fxxx>
    $<$<STREQUAL:${TARGET_MCU},stm32f746>:preview.stm32fxxx>
    preview.touch
    preview.views
)

if(TARGET_MCU STREQUAL stm32f746)
    halst_target_default_linker_scripts(examples.qr_code)
    halst_target_default_init(examples.qr_code)
endif()
