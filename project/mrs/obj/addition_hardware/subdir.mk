################################################################################
# MRS Version: 1.9.1
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
E:/SmartCar/blcd����/CH32V203C8T6_BLDC_ESC_1/addition_hardware/battery.c \
E:/SmartCar/blcd����/CH32V203C8T6_BLDC_ESC_1/addition_hardware/delay.c \
E:/SmartCar/blcd����/CH32V203C8T6_BLDC_ESC_1/addition_hardware/iwdg.c \
E:/SmartCar/blcd����/CH32V203C8T6_BLDC_ESC_1/addition_hardware/led.c \
E:/SmartCar/blcd����/CH32V203C8T6_BLDC_ESC_1/addition_hardware/usart.c 

OBJS += \
./addition_hardware/battery.o \
./addition_hardware/delay.o \
./addition_hardware/iwdg.o \
./addition_hardware/led.o \
./addition_hardware/usart.o 

C_DEPS += \
./addition_hardware/battery.d \
./addition_hardware/delay.d \
./addition_hardware/iwdg.d \
./addition_hardware/led.d \
./addition_hardware/usart.d 


# Each subdirectory must supply rules for building sources it contributes
addition_hardware/battery.o: E:/SmartCar/blcd����/CH32V203C8T6_BLDC_ESC_1/addition_hardware/battery.c
	@	@	riscv-none-elf-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\addition_hardware" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\addition_software" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\bldc_config" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\bldc_hardware" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\bldc_software" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\project\user" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Core" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Debug" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Ld" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\inc" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\src" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Startup" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
addition_hardware/delay.o: E:/SmartCar/blcd����/CH32V203C8T6_BLDC_ESC_1/addition_hardware/delay.c
	@	@	riscv-none-elf-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\addition_hardware" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\addition_software" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\bldc_config" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\bldc_hardware" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\bldc_software" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\project\user" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Core" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Debug" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Ld" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\inc" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\src" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Startup" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
addition_hardware/iwdg.o: E:/SmartCar/blcd����/CH32V203C8T6_BLDC_ESC_1/addition_hardware/iwdg.c
	@	@	riscv-none-elf-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\addition_hardware" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\addition_software" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\bldc_config" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\bldc_hardware" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\bldc_software" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\project\user" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Core" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Debug" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Ld" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\inc" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\src" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Startup" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
addition_hardware/led.o: E:/SmartCar/blcd����/CH32V203C8T6_BLDC_ESC_1/addition_hardware/led.c
	@	@	riscv-none-elf-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\addition_hardware" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\addition_software" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\bldc_config" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\bldc_hardware" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\bldc_software" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\project\user" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Core" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Debug" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Ld" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\inc" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\src" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Startup" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
addition_hardware/usart.o: E:/SmartCar/blcd����/CH32V203C8T6_BLDC_ESC_1/addition_hardware/usart.c
	@	@	riscv-none-elf-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\addition_hardware" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\addition_software" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\bldc_config" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\bldc_hardware" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\bldc_software" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\project\user" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Core" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Debug" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Ld" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\inc" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Peripheral\src" -isystem"E:\SmartCar\blcd����\CH32V203C8T6_BLDC_ESC_1\wch_sdk\Startup" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
