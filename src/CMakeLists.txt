add_executable(${NAME}
        main.cpp
        agent.cpp
        blink_agent.cpp
        )

# Don't forget to link the libraries you need!
target_link_libraries(${NAME}
        pico_stdlib
        pico_explorer
        pico_graphics
        st7789
        button
        motor
        analog
        encoder
        FreeRTOS-Kernel-Heap4
        )

target_include_directories(${NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )

target_compile_definitions(${NAME} PRIVATE
        configNUM_CORES=2
        )

# create map/bin/hex file etc.
pico_add_extra_outputs(${NAME})

# enable usb output, disable uart output
pico_enable_stdio_usb(${NAME} 0)
pico_enable_stdio_uart(${NAME} 1)