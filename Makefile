# Include external libraries
include ./add_project.mk
#include ./add_stm_cube_1.3.0.mk
include ./add_stm_cube_1.11.0.mk
include ./add_cmsis.mk
include ./add_USB_library.mk

# Add source filesv
SRCS = $(PROJECT_SRCS)
SRCS += $(STM_CUBE_SRCS)
SRCS += $(CMSIS_SRCS)
SRCS += $(USB_LIB_SRC)

# and headers files here
INCS = $(PROJECT_INCS)
INCS += $(STM_CUBE_INCS)
INCS += $(CMSIS_INCS)
INCS += $(USB_LIB_INCS)

# Add assembly files here
ASMS = $(CMSIS_ASMS)

# Binaries will be generated with this name (.elf, .bin, .hex, etc)
PROJ_NAME = reflow_oven
DEVICE_TYPE = STM32F407xx
OUT_PATH = ./build
OBJ_PATH = ./build/objs

# 
C_OBJS = $(addprefix $(OBJ_PATH)/, $(notdir $(SRCS:.c=.o)))
ASM_OBJS = $(addprefix $(OBJ_PATH)/, $(notdir $(ASMS:.s=.o)))

VPATH = $(dir $(SRCS)) \
		$(dir $(ASMS))

#######################################################################################

CC=arm-none-eabi-gcc
AS=arm-none-eabi-as
SIZE=arm-none-eabi-size
OBJCOPY=arm-none-eabi-objcopy
LD=arm-none-eabi-ld

# compiler options
CFLAGS  = -g -O0 -T$(STM_CUBE_LINKER) -nostartfiles -Wl,-Map=$(OUT_PATH)/$(PROJ_NAME).map,--cref 
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -D$(DEVICE_TYPE)
CFLAGS += -DUSE_HAL_DRIVER
CFLAGS += $(INCS)
CFLAGS += -MD -MP -MF .dep/$(@F).d
# Removed options = -Wall  -mthumb-interwork --specs=nosys.specs 

###############################################################################

all : $(OUT_PATH)/$(PROJ_NAME).elf
	@echo "Creating HEX and BIN files"
	@$(OBJCOPY) -O ihex $(OUT_PATH)/$(PROJ_NAME).elf $(OUT_PATH)/$(PROJ_NAME).hex
	@$(OBJCOPY) -O binary $(OUT_PATH)/$(PROJ_NAME).elf $(OUT_PATH)/$(PROJ_NAME).bin
	@$(SIZE) -A -x $(OUT_PATH)/$(PROJ_NAME).elf

$(OUT_PATH)/$(PROJ_NAME).elf : $(C_OBJS) $(ASM_OBJS)
#	@echo $(CFLAGS)
	@echo "Assembling objects"
	@$(CC) $(CFLAGS) $^ -o $@ 

$(C_OBJS) : $(OBJ_PATH)/%.o : %.c
#	@echo $(INCS)
	@echo "Compiling"   $<
	@$(CC) -c $(CFLAGS) $< -o $@
	
$(ASM_OBJS) : $(OBJ_PATH)/%.o : %.s
	@echo "Compiling " $<
	@$(CC) -c $(CFLAGS) $< -o $@

clean: 
	rm -f $(OBJ_PATH)/*.o
	rm -f $(OUT_PATH)/$(PROJ_NAME).*
	rm -f .dep/*

-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)
