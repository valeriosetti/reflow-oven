USB_LIB = ./STM32Cube_FW_F4_V1.11.0/Middlewares/ST/STM32_USB_Device_Library

USB_LIB_SRC = 	$(wildcard $(USB_LIB)/Core/Src/*.c)		\
				$(wildcard $(USB_LIB)/Class/CDC/Src/*.c)
				

USB_LIB_INCS = 	-I$(USB_LIB)/Core/Inc	\
				-I$(USB_LIB)/Class/CDC/Inc

