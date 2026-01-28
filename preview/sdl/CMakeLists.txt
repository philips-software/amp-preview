if (EMIL_BUILD_WIN AND PREVIEW_INCLUDE_SDL)
    include(FetchContent)

    FetchContent_Declare(
        sdl
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG        a962f40bbba175e9716557a25d5d7965f134a3d3 # v3.4.0
    )

    FetchContent_MakeAvailable(sdl)

    emil_exclude_directory_from_clang_format(.)

    add_library(preview.sdl ${PREVIEW_EXCLUDE_FROM_ALL} STATIC)

    target_include_directories(preview.sdl PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    )

    target_link_libraries(preview.sdl PUBLIC
        preview.interfaces
        SDL3::SDL3-static
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
