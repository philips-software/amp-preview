cmake_minimum_required(VERSION 3.24)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(PREVIEW_STANDALONE On)
endif()

option(CMAKE_COMPILE_WARNINGS_AS_ERROR "Enable warnings-as-error" On)
option(PREVIEW_BUILD_TESTS "Enable building the tests" Off)
option(PREVIEW_BUILD_EXAMPLES "Enable building the examples" Off)
option(PREVIEW_INCLUDE_SDL "Include SDL support" Off)

if (PREVIEW_BUILD_TESTS)
    # CTest cannot be included before the first project() statement, but Embedded Infastructure
    # Library needs to see that test utilities need to be built.
    set(BUILD_TESTING On)
endif()

if (PREVIEW_STANDALONE)
    include(FetchContent)

    FetchContent_Declare(
        emil
        GIT_REPOSITORY https://github.com/philips-software/amp-embedded-infra-lib
        GIT_TAG        4239adfcec976c47fbdc2b8ada6c6577d7a3c2b9 # v7.1.0
    )

    FetchContent_MakeAvailable(emil)

    if (TARGET_MCU_VENDOR STREQUAL st)
        FetchContent_Declare(
            halst
            GIT_REPOSITORY https://github.com/philips-software/amp-hal-st
            GIT_TAG        1e684ac1c00b7466928b390f07d2d69c6ae4e1fc # v4.0.0
        )

        FetchContent_MakeAvailable(halst)
    endif()
endif()

project(preview LANGUAGES C CXX VERSION 1.2.0) # x-release-please-version

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)

set_directory_properties(PROPERTY USE_FOLDERS On)

# When building amp-preview by itself do not exclude any targets from all
# Libraries will only be excluded from all when amp-preview is a consumed as a dependency.
if (PREVIEW_STANDALONE)
    set(PREVIEW_EXCLUDE_FROM_ALL "")
else()
    set(PREVIEW_EXCLUDE_FROM_ALL "EXCLUDE_FROM_ALL")
endif()

if (PREVIEW_BUILD_TESTS)
    include(CTest)
    emil_enable_testing()
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # Suppress warnings about parameter passing for argument of type '...' changed in GCC 7.1
    add_compile_options(-Wno-psabi)
endif()

emil_exclude_directory_from_clang_format(.)

add_subdirectory(preview)
add_subdirectory(examples)

emil_clangformat_directories(preview DIRECTORIES .)

if (PREVIEW_STANDALONE)
    emil_folderize_all_targets()
endif()
