cmake_minimum_required(VERSION 3.6)
project(can107normal C ASM)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


add_definitions(-DSTM32F10X_CL)
add_definitions(-DUSE_STDPERIPH_DRIVER)

include_directories(Lib/cmsis)
include_directories(Lib/inc)
include_directories(User/SysTick)
include_directories(User)

set(SOURCE_FILES
#    Lib/cmsis/core_cm3.c
    Lib/src/misc.c
#    Lib/src/stm32f10x_adc.c
#    Lib/src/stm32f10x_bkp.c
    Lib/src/stm32f10x_can.c
#    Lib/src/stm32f10x_cec.c
#    Lib/src/stm32f10x_crc.c
#    Lib/src/stm32f10x_dac.c
#    Lib/src/stm32f10x_dbgmcu.c
#    Lib/src/stm32f10x_dma.c
#    Lib/src/stm32f10x_exti.c
#    Lib/src/stm32f10x_flash.c
#    Lib/src/stm32f10x_fsmc.c
    Lib/src/stm32f10x_gpio.c
#    Lib/src/stm32f10x_i2c.c
#    Lib/src/stm32f10x_iwdg.c
#    Lib/src/stm32f10x_pwr.c
    Lib/src/stm32f10x_rcc.c
#    Lib/src/stm32f10x_rtc.c
#    Lib/src/stm32f10x_sdio.c
#    Lib/src/stm32f10x_spi.c
    Lib/src/stm32f10x_tim.c
    Lib/src/stm32f10x_usart.c
#    Lib/src/stm32f10x_wwdg.c
    User/SysTick/stm32f10x_systick.c
    User/SysTick/systick.c
    User/stm32f10x_it.c
    User/system_stm32f10x.c
    User/usart.c
        User/syscalls.c
        User/main.c
        startup/startup_stm32f107xc.s
        )


#add_definitions(-DDEBUG )


add_executable(${PROJECT_NAME}.elf ${SOURCE_FILES} ${LINKER_SCRIPT})


set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.map")
set(HEX_FILE ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.bin)
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE} \nBuilding ${BIN_FILE}")