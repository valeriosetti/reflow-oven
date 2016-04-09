STM_CUBE = ./STM32Cube_FW_F4_V1.11.0/Drivers/STM32F4xx_HAL_Driver
STM42F4_DISCO_BSP = ./STM32Cube_FW_F4_V1.11.0/Drivers/BSP/STM32F4-Discovery

STM_CUBE_SRCS = $(wildcard $(STM_CUBE)/Src/*.c)		\
				$(wildcard $(STM42F4_DISCO_BSP)/stm32f4_discovery.c)


STM_CUBE_INCS = -I$(STM_CUBE)/Inc		\
				-I$(STM42F4_DISCO_BSP)


STM_CUBE_LINKER = ./linker/STM32F407VG_FLASH.ld
