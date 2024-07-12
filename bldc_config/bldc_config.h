#ifndef _bldc_config_h_
#define _bldc_config_h_

// 开环启动电压
#define BLDC_START_VOLTAGE          (1.3f )         // 1.0 - 2.0 单位V

#define BLDC_MIN_VOLTAGE            (10500)         // 最低电压值，单位mv

#define BLDC_COMMUTATION_FAILED_MAX (2000 )         // 换相错误最大次数 大于这个次数后认为电机堵转

#define BLDC_MAX_DUTY               (3000)          // 输出频率为48kHZ，PWM的最大占空比，需要自行根据PWM初始化去计算
                                                    // 计算公式：SystemCoreClock / 48000 = 3000
                                                    // SystemCoreClock = 144000000

#define BLDC_BRAKE_DUTY             (100)           // 刹车时的占空比.

#define BLDC_MIN_DUTY               (150)           // PWM的最小占空比，根据输入油门大小计算出来的占空比小于此定义则电机停转 至少要大于150

#define BLDC_POLES                  (7  )           // 电机极对数

#define BLDC_OPEN_LOOP_WRITE        (10 )           // 开环等待换向次数

#define BLDC_CLOSE_LOOP_WRITE       (30 )           // 闭环等待换向次数

#define BLDC_STALL_TIME_OUT         (200)           // 堵转超时检测，单位ms

#define BLDC_PWM_DEADTIME           (30)            // PWM死区设置

#define BLDC_BEEP_ENABLE            (1  )           // 1:使能上电电机鸣叫功能 0：禁用

#define BLDC_BEEP_VOLUME            (100)           // 电机鸣叫声音大小 0-100

#define BLDC_RESTART_DELAY          (100)           // 启动失败，重启延时，单位ms

#define BLDC_RESTART_MAX            (20)            // 最大重启次数

#define BLDC_SPEED_INCREMENTAL      (5 )            // 单次加速最大值 1-15，可修改这个提高加速响应

#endif










