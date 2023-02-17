include(${CMAKE_CURRENT_LIST_DIR}/toolchain-arm-gcc-m7.cmake)

add_link_options(-mfpu=fpv5-d16 -mfloat-abi=hard)
add_compile_options(-mfpu=fpv5-d16 -mfloat-abi=hard)
