if (EMIL_BUILD_WIN AND PREVIEW_INCLUDE_SDL)
    include(FetchContent)

    FetchContent_Declare(
        sdl
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG        f070c83a6059c604cbd098680ddaee391b0a7341 # v2.26.2
    )

    FetchContent_MakeAvailable(sdl)

    emil_exclude_directory_from_clang_format(.)

    add_library(preview.sdl ${PREVIEW_EXCLUDE_FROM_ALL} STATIC)
    install(TARGETS preview.sdl EXPORT previewTargets)

    target_include_directories(preview.sdl PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    )

    target_link_libraries(preview.sdl PUBLIC
        preview.interfaces
        SDL2::SDL2-static
    )

    target_sources(preview.sdl PRIVATE
        DirectDisplaySdl.cpp
        DirectDisplaySdl.hpp
        LowPowerStrategySdl.cpp
        LowPowerStrategySdl.hpp
        SdlMouse.hpp
        SdlTouchInteractor.cpp
        SdlTouchInteractor.hpp
    )
endif()
