CMSIS_LIB = ./STM32Cube_FW_F4_V1.11.0/Drivers

CMSIS_INCS = 	-I$(CMSIS_LIB)/CMSIS/Include	\
				-I$(CMSIS_LIB)/CMSIS/Device/ST/STM32F4xx/Include
				
CMSIS_SRCS = 	$(wildcard $(CMSIS_LIB)/CMSIS/Device/ST/STM32F4xx/Source/Templates/*.c)

CMSIS_ASMS = 	$(CMSIS_LIB)/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f407xx.s
