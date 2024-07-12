################################################################################
# MRS Version: 1.9.1
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include wch_sdk/Startup/subdir.mk
-include wch_sdk/Peripheral/src/subdir.mk
-include wch_sdk/Debug/subdir.mk
-include wch_sdk/Core/subdir.mk
-include user/subdir.mk
-include bldc_software/subdir.mk
-include bldc_hardware/subdir.mk
-include addition_software/subdir.mk
-include addition_hardware/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
CH32V203C8T6_ESC_1.hexAndbin \

SECONDARY_LIST += \
CH32V203C8T6_ESC_1.lst \

SECONDARY_SIZE += \
CH32V203C8T6_ESC_1.siz \


# 所有目标
all: CH32V203C8T6_ESC_1.elf secondary-outputs

# 工具调用
CH32V203C8T6_ESC_1.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-elf-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -T "E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"CH32V203C8T6_ESC_1.map" --specs=nano.specs --specs=nosys.specs -Wl,--print-memory-usage -o "CH32V203C8T6_ESC_1.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
CH32V203C8T6_ESC_1.hexAndbin: CH32V203C8T6_ESC_1.elf
	@	riscv-none-elf-objcopy -O ihex "CH32V203C8T6_ESC_1.elf"  "CH32V203C8T6_ESC_1.hex" && riscv-none-elf-objcopy -O binary "CH32V203C8T6_ESC_1.elf"  "CH32V203C8T6_ESC_1.bin"
	@	@
CH32V203C8T6_ESC_1.lst: CH32V203C8T6_ESC_1.elf
	@	riscv-none-elf-objdump --all-headers --demangle --disassemble -M xw "CH32V203C8T6_ESC_1.elf" > "CH32V203C8T6_ESC_1.lst"
	@	@
CH32V203C8T6_ESC_1.siz: CH32V203C8T6_ESC_1.elf
	@	riscv-none-elf-size --format=berkeley "CH32V203C8T6_ESC_1.elf"
	@	@
	$(MAKE) --no-print-directory post-build

# 其他目标
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) CH32V203C8T6_ESC_1.elf
	-@
post-build:
	-riscv-none-elf-objcopy -O ihex "CH32V203C8T6_ESC_1.elf" "CH32V203C8T6_ESC_1.hex" && riscv-none-elf-objcopy -O binary "CH32V203C8T6_ESC_1.elf" "CH32V203C8T6_ESC_1.bin"
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents post-build

-include ../makefile.targets
