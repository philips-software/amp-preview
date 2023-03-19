cmake_minimum_required(VERSION 3.24)

project(preview LANGUAGES C CXX VERSION 1.0.0) # x-release-please-version

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(PREVIEW_STANDALONE On)
endif()

# Before Embedded Infrastructure Library can be included, we must
# first decide whether or not to build tests.
if (CMAKE_SYSTEM_NAME STREQUAL "Windows" OR CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(PREVIEW_HOST_BUILD On)
endif()

# When building Preview by itself the examples and tests will be built
# by default. When consuming Preview as a dependency the tests and
# examples are disabled. This behavior can be overridden by below
# options.
if (PREVIEW_STANDALONE AND PREVIEW_HOST_BUILD)
    set(PREVIEW_DEFAULTOPT On)
else()
    set(PREVIEW_DEFAULTOPT Off)
endif()

option(CMAKE_COMPILE_WARNINGS_AS_ERROR "Enable warnings-as-error" On)
option(PREVIEW_BUILD_TESTS "Enable building the tests" ${PREVIEW_DEFAULTOPT})
option(PREVIEW_BUILD_EXAMPLES "Enable building the examples" ${PREVIEW_DEFAULTOPT})
option(PREVIEW_INCLUDE_SDL "Include SDL support" ${PREVIEW_DEFAULTOPT})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)

set_directory_properties(PROPERTY USE_FOLDERS On)

if (PREVIEW_BUILD_TESTS)
    include(CTest)
endif()

if (PREVIEW_STANDALONE)
    include(FetchContent)

    FetchContent_Declare(
        emil
        GIT_REPOSITORY https://github.com/philips-software/amp-embedded-infra-lib
        GIT_TAG        modern-cmake
    )

    FetchContent_MakeAvailable(emil)

    if (TARGET_MCU_VENDOR STREQUAL st)
        FetchContent_Declare(
            halst
            GIT_REPOSITORY https://github.com/philips-software/amp-hal-st
            GIT_TAG        main
        )

        FetchContent_MakeAvailable(halst)
    endif()
endif()

if (PREVIEW_BUILD_TESTS)
    emil_enable_testing()
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # Suppress warnings about parameter passing for argument of type '...' changed in GCC 7.1
    add_compile_options(-Wno-psabi)
endif()

emil_exclude_directory_from_clang_format(.)

add_subdirectory(preview)
if (PREVIEW_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

emil_clangformat_directories(preview DIRECTORIES .)

if (PREVIEW_STANDALONE)
    emil_folderize_all_targets()
endif()
