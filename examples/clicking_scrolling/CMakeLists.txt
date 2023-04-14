add_executable(examples.clicking_scrolling)

set_target_properties(examples.clicking_scrolling PROPERTIES EXCLUDE_FROM_ALL $<NOT:$<OR:$<BOOL:${EMIL_BUILD_WIN}>,$<STREQUAL:${TARGET_MCU},stm32f746>>>)
set_target_properties(examples.clicking_scrolling PROPERTIES WIN32_EXECUTABLE $<BOOL:${EMIL_BUILD_WIN}>)

target_include_directories(examples.clicking_scrolling PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_sources(examples.clicking_scrolling PRIVATE
    $<$<BOOL:${EMIL_BUILD_WIN}>:MainWin.cpp>
    $<$<STREQUAL:${TARGET_MCU},stm32f746>:MainStm32f746.cpp>
    TouchViewClickingScrolling.cpp
    TouchViewClickingScrolling.hpp
)

target_link_libraries(examples.clicking_scrolling PUBLIC
    $<$<BOOL:${EMIL_BUILD_WIN}>:hal.generic>
    $<$<BOOL:${EMIL_BUILD_WIN}>:preview.sdl>
    $<$<STREQUAL:${TARGET_MCU},stm32f746>:hal_st.stm32fxxx>
    $<$<STREQUAL:${TARGET_MCU},stm32f746>:preview.stm32fxxx>
    preview.touch
    preview.views
)

if(TARGET_MCU STREQUAL stm32f746)
    halst_target_default_linker_scripts(examples.clicking_scrolling)
    halst_target_default_init(examples.clicking_scrolling)
endif()
