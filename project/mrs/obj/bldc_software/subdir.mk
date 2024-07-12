################################################################################
# MRS Version: 1.9.1
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
E:/SmartCar/blcd����/CH32V203C8T6_BLDC_ESC_1/bldc_software/control.c \
E:/SmartCar/blcd����/CH32V203C8T6_BLDC_ESC_1/bldc_software/motor.c 

OBJS += \
./bldc_software/control.o \
./bldc_software/motor.o 

C_DEPS += \
./bldc_software/control.d \
./bldc_software/motor.d 


# Each subdirectory must supply rules for building sources it contributes
bldc_software/control.o: E:/SmartCar/blcd����/CH32V203C8T6_BLDC_ESC_1/bldc_software/control.c
	@	@	riscv-none-elf-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\addition_hardware" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\addition_software" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\bldc_config" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\bldc_hardware" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\bldc_software" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\project\user" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Core" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Debug" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Ld" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\inc" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\src" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Startup" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
bldc_software/motor.o: E:/SmartCar/blcd����/CH32V203C8T6_BLDC_ESC_1/bldc_software/motor.c
	@	@	riscv-none-elf-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\addition_hardware" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\addition_software" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\bldc_config" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\bldc_hardware" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\bldc_software" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\project\user" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Core" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Debug" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Ld" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\inc" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\src" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Startup" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
