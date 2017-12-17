STM_CUBE = ./STM32Cube_drivers_1.3.0

STM_CUBE_SRCS = $(wildcard $(STM_CUBE)/STM32F4xx_HAL_Driver/Src/*.c)


STM_CUBE_INCS = -I$(STM_CUBE)/STM32F4xx_HAL_Driver/Inc


STM_CUBE_LINKER = ./linker/STM32F407VG_FLASH.ld
