cmake_minimum_required(VERSION 3.3)

set(PROJECT_NAME Aditboard)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

include(pico_sdk_import.cmake)
include(external/FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(${PROJECT_NAME} C CXX ASM)

pico_sdk_init()

set(TARGET_NAME ${PROJECT_NAME})

add_executable(${TARGET_NAME}
	src/main.c
)

pico_generate_pio_header(${TARGET_NAME} ${CMAKE_CURRENT_LIST_DIR}/src/uart_rx.pio)

#pico_enable_stdio_uart(${TARGET_NAME} 1)
pico_enable_stdio_usb(${TARGET_NAME} 1)

target_compile_definitions(${TARGET_NAME} PRIVATE
)

target_include_directories(${TARGET_NAME} PRIVATE
	#${CMAKE_CURRENT_LIST_DIR}
	include/
)

target_link_libraries(${TARGET_NAME} PRIVATE
	pico_stdlib
	hardware_gpio
	hardware_pio
	pico_multicore

	FreeRTOS-Kernel
	FreeRTOS-Kernel-Heap4
)

pico_add_extra_outputs(${TARGET_NAME})
