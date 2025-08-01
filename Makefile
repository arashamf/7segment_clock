##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [4.4.0-B60] date: [Fri Jul 25 14:37:36 MSK 2025] 
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET = belt


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
timer64/time64.c \
Core/Src/main.c \
Core/Src/gpio.c \
Core/Src/i2c.c \
Core/Src/tim.c \
Core/Src/usart.c \
Core/Src/stm32l1xx_it.c \
IO_drivers/ring_buffer.c \
IO_drivers/LED_display.c \
IO_drivers/rtc.c \
IO_drivers/time_protocol.c \
Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_ll_rcc.c \
Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_ll_utils.c \
Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_ll_exti.c \
Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_ll_gpio.c \
Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_ll_i2c.c \
Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_ll_dma.c \
Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_ll_tim.c \
Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_ll_usart.c \
Core/Src/system_stm32l1xx.c \
Core/Src/sysmem.c \
Core/Src/syscalls.c \
Core/Src/freertos.c \
Core/Src/stm32l1xx_hal_msp.c \
Core/Src/stm32l1xx_hal_timebase_tim.c \
Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal.c \
Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_rcc.c \
Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_rcc_ex.c \
Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_flash.c \
Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_flash_ex.c \
Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_flash_ramfunc.c \
Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_gpio.c \
Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_dma.c \
Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_pwr.c \
Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_pwr_ex.c \
Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_cortex.c \
Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_exti.c \
Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_tim.c \
Drivers/STM32L1xx_HAL_Driver/Src/stm32l1xx_hal_tim_ex.c \
Middlewares/Third_Party/FreeRTOS/Source/croutine.c \
Middlewares/Third_Party/FreeRTOS/Source/event_groups.c \
Middlewares/Third_Party/FreeRTOS/Source/list.c \
Middlewares/Third_Party/FreeRTOS/Source/queue.c \
Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c \
Middlewares/Third_Party/FreeRTOS/Source/tasks.c \
Middlewares/Third_Party/FreeRTOS/Source/timers.c \
Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c \
Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c \
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3/port.c

# ASM sources
ASM_SOURCES =  \
startup_stm32l151xb.s

# ASM sources
ASMM_SOURCES = 


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi


# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_FULL_LL_DRIVER \
-DSTM32L151xB \
-DUSE_HAL_DRIVER


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-Itimer64 \
-IIO_drivers \
-ICore/Inc \
-IDrivers/STM32L1xx_HAL_Driver/Inc \
-IDrivers/CMSIS/Device/ST/STM32L1xx/Include \
-IDrivers/CMSIS/Include \
-IDrivers/STM32L1xx_HAL_Driver/Inc/Legacy \
-IMiddlewares/Third_Party/FreeRTOS/Source/include \
-IMiddlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS \
-IMiddlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32L151C8Tx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASMM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASMM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@
$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

#######################################
# prog
#######################################
CMSIS_DAP = 1
MK = stm32l1

ifeq ($(CMSIS_DAP), 1)
	OCD_INTER = cmsis-dap
else
	OCD_INTER = stlink
endif
prog: $(BUILD_DIR)/$(TARGET).elf
	openocd -f  interface/$(OCD_INTER).cfg -f target/$(MK).cfg -c "program build/$(TARGET).elf verify exit reset"
# *** EOF ***
