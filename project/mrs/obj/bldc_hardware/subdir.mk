################################################################################
# MRS Version: 1.9.1
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
E:/SmartCar/blcd代码/CH32V203C8T6_BLDC_ESC_1/bldc_hardware/operational_amplifier.c \
E:/SmartCar/blcd代码/CH32V203C8T6_BLDC_ESC_1/bldc_hardware/pwm_input.c \
E:/SmartCar/blcd代码/CH32V203C8T6_BLDC_ESC_1/bldc_hardware/pwm_output.c 

OBJS += \
./bldc_hardware/operational_amplifier.o \
./bldc_hardware/pwm_input.o \
./bldc_hardware/pwm_output.o 

C_DEPS += \
./bldc_hardware/operational_amplifier.d \
./bldc_hardware/pwm_input.d \
./bldc_hardware/pwm_output.d 


# Each subdirectory must supply rules for building sources it contributes
bldc_hardware/operational_amplifier.o: E:/SmartCar/blcd代码/CH32V203C8T6_BLDC_ESC_1/bldc_hardware/operational_amplifier.c
	@	@	riscv-none-elf-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\addition_hardware" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\addition_software" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\bldc_config" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\bldc_hardware" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\bldc_software" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\project\user" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Core" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Debug" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Ld" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\inc" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\src" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Startup" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
bldc_hardware/pwm_input.o: E:/SmartCar/blcd代码/CH32V203C8T6_BLDC_ESC_1/bldc_hardware/pwm_input.c
	@	@	riscv-none-elf-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\addition_hardware" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\addition_software" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\bldc_config" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\bldc_hardware" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\bldc_software" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\project\user" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Core" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Debug" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Ld" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\inc" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\src" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Startup" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
bldc_hardware/pwm_output.o: E:/SmartCar/blcd代码/CH32V203C8T6_BLDC_ESC_1/bldc_hardware/pwm_output.c
	@	@	riscv-none-elf-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\addition_hardware" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\addition_software" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\bldc_config" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\bldc_hardware" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\bldc_software" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\project\user" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Core" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Debug" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Ld" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\inc" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\src" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Startup" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

