#include "lcd1602.h"
#include <8051.h>
#include "intrins.h"
#include "ds18b20.h"
#include "key.h"
#include "key_matrix.h"
#include "public.h"
#include "timer.h"
#include "beep.h"
#include "haruhikage.h"
#include "string_utils.h"
#include "string_utils.h"

#define KEY3 P3_2
// 函数声明
void temperature_display(void);
void haruhikage_main(void);
void mode_3_display(void);
void mode_4_display(void);
void haruhikage_playing(void);
void time_control(void);

// 全局变量
u8 current_mode = MODE_START_PAGE;

// 中断初始化
void exit_init(void) {
    IT0 = 1;  // 外部中断0下降沿触发
    EX0 = 1;  // 使能外部中断0
    EA = 1;   // 使能总中断
}

// 外部中断0(KEY3)
void INT1_ISR() __interrupt(0) {
    delay_10us(1000); 
    if (KEY3 == 0) {
        while (KEY3 == 0);  
        current_mode = MODE_START_PAGE;
    }
}

void main(void)
{
    u8 key_val = 0;
    //全部初始化一下
    lcd1602_init();
    ds18b20_init();
    exit_init();
    
    dou(380); delay_ms(50);
    rei(180); delay_ms(50);
    mi(380); delay_ms(50);
    while(1)
    {
        // 如果在初始界面，检测矩阵按键
        if (current_mode == MODE_START_PAGE) {
            key_val = key_matrix_flip_scan();
            if (key_val != 0) {
                switch(key_val) {
                    case 1:
                        current_mode = MODE_TEMPERATURE;
                        break;
                    case 2:
                        current_mode = MODE_HARUHIKAGE;
                        break;
                    case 3:
                        current_mode = MODE_3;
                        break;
                    case 4:
                        current_mode = MODE_4;
                        break;
                    default:
                        break;
                }
            }
        }
        
        // 进入界面
        switch(current_mode)
        {
            case MODE_START_PAGE:
                lcd1602_show_string(0, 0, "Press Key3 Back ");
                lcd1602_show_string(0, 1, "Matrix_SKey 1-4 ");
                break;
                
            case MODE_TEMPERATURE:
                
                lcd1602_show_string(0, 0, "Temperature:    "); 
                temperature_display();
                break;
                
            case MODE_HARUHIKAGE:
                lcd1602_show_string(0, 0, "Press Key1 To   ");
                lcd1602_show_string(0, 1, "Play: Haruhikage");
                haruhikage_playing();
                break;
                
            case MODE_3:
                mode_3_display();
                
                break;
                
            case MODE_4:
                mode_4_display();
                break;
        }
        
        delay_ms(20);
    }
}

void temperature_display(void)
{
    u16 counter = 0;
    float temp_value = 0.0;
    u8 temp_buf[16];
    
    ds18b20_init();  
    
    // 主循环
    while(current_mode == MODE_TEMPERATURE)
    {
        counter++;
        // 每50次循环读取一次温度（约1秒）
        if(counter >= 50)
        {
            counter = 0;
            
            // 读取温度
            temp_value = ds18b20_read_temperature();
            
            // 使用统一的浮点数转换函数
            float_to_string(temp_buf, temp_value, 1);
            
            // 在字符串末尾添加摄氏度符号
            u8 i = 0;
            while(temp_buf[i] != '\0') i++;
            temp_buf[i] = 'C';
            temp_buf[i+1] = '\0';
            
            // 清空显示并更新
            lcd1602_show_string(0, 1, "                "); 
            lcd1602_show_string(0, 1, temp_buf);
        }
        
        delay_ms(20); 
    } 
}
void haruhikage_playing(void){
    haruhikage();  // 跳转到Haruhikage.c
}
// 按键3功能预留
void mode_3_display(void)
{
    lcd1602_show_string(0, 0, "Time Setting:   ");
    lcd1602_show_string(0, 1, "00:00");
    
    while(current_mode == MODE_3)
    {
        // 处理时间控制（按键1增加分钟，2增加秒钟；5减少分钟，6减少秒钟）
        time_control();
        
        // 功能3
        delay_ms(100);
    }
}

// 按键4功能预留
void mode_4_display(void)
{
    lcd1602_show_string(0, 0, "Designed by     ");
    lcd1602_show_string(0, 1, "Xuanning Zhang  ");
    delay_ms(20);
    lcd1602_show_string(0, 0, "Editing on      ");
    lcd1602_show_string(0, 1, "Mac Book Pro M1 ");
    
    while(current_mode == MODE_4)
    {
        // 功能4
        delay_ms(100);
    }
}