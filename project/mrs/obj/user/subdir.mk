################################################################################
# MRS Version: 1.9.1
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
E:/SmartCar/blcd代码/CH32V203C8T6_BLDC_ESC_1/project/user/ch32v20x_it.c \
E:/SmartCar/blcd代码/CH32V203C8T6_BLDC_ESC_1/project/user/main.c \
E:/SmartCar/blcd代码/CH32V203C8T6_BLDC_ESC_1/project/user/system_ch32v20x.c 

OBJS += \
./user/ch32v20x_it.o \
./user/main.o \
./user/system_ch32v20x.o 

C_DEPS += \
./user/ch32v20x_it.d \
./user/main.d \
./user/system_ch32v20x.d 


# Each subdirectory must supply rules for building sources it contributes
user/ch32v20x_it.o: E:/SmartCar/blcd代码/CH32V203C8T6_BLDC_ESC_1/project/user/ch32v20x_it.c
	@	@	riscv-none-elf-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\addition_hardware" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\addition_software" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\bldc_config" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\bldc_hardware" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\bldc_software" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\project\user" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Core" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Debug" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Ld" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\inc" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\src" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Startup" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
user/main.o: E:/SmartCar/blcd代码/CH32V203C8T6_BLDC_ESC_1/project/user/main.c
	@	@	riscv-none-elf-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\addition_hardware" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\addition_software" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\bldc_config" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\bldc_hardware" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\bldc_software" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\project\user" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Core" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Debug" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Ld" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\inc" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\src" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Startup" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
user/system_ch32v20x.o: E:/SmartCar/blcd代码/CH32V203C8T6_BLDC_ESC_1/project/user/system_ch32v20x.c
	@	@	riscv-none-elf-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\addition_hardware" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\addition_software" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\bldc_config" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\bldc_hardware" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\bldc_software" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\project\user" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Core" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Debug" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Ld" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\inc" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\src" -isystem"E:\SmartCar\blcd代码\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Startup" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

