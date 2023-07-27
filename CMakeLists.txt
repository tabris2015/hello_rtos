cmake_minimum_required(VERSION 3.12)

# Change your executable name to something creative!
set(NAME hello_rtos) # <-- Name your project/executable here!
#set(PICO_BOARD pico_w)

include(pimoroni_pico_import.cmake)
include(pico_sdk_import.cmake)

# Gooey boilerplate
project(${NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()


#include Libraries
SET(FREERTOS_KERNEL_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/FreeRTOS-Kernel" CACHE STRING "Course Common Lib")
SET(FREERTOS_CONFIG_FILE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/port/FreeRTOS-Kernel" CACHE STRING "Local Config")
include_directories("${FREERTOS_CONFIG_FILE_DIRECTORY}")
include(FreeRTOS_Kernel_import.cmake)

add_subdirectory(src)

# include libraries
include(common/pimoroni_bus)
include(drivers/button/button)
include(drivers/pwm/pwm)
include(drivers/motor/motor)
include(drivers/analog/analog)
include(drivers/encoder/encoder)
#include(libraries/pico_explorer/pico_explorer)
include(libraries/bitmap_fonts/bitmap_fonts)
include(libraries/hershey_fonts/hershey_fonts)
include(libraries/pico_explorer/pico_explorer)

# Set up files for the release packages
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.uf2
        ${CMAKE_CURRENT_LIST_DIR}/README.md
        DESTINATION .
        )

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP" "TGZ")
include(CPack)
