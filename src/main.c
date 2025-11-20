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
#include "led8.h"

#define KEY3 P3_2

// 外部全局变量
extern u16 minutes;
extern u16 seconds;
extern u8 countdown_active;

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
        delay_10us(1000);  // 再消抖
        current_mode = MODE_START_PAGE;
        countdown_active = 0;  // 停止倒计时
    }
}

void main(void)
{
    u8 key_val = 0;
    //全部初始化一下
    lcd1602_init();
    ds18b20_init();
    exit_init();
    timer0_init();  // 初始化定时器中断
    
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
    u8 key_val = 0;
    u8 last_key = 0;  // 上次按键值
    
    // 显示固定的界面
    lcd1602_show_string(0, 0, "Press Key 3     ");
    lcd1602_show_string(0, 1, "To Play Music   ");
    
    // 进入时立即在数码管显示3（使用全局段码表）
    extern u8 gsmg_code[];  // 引用全局数码管段码表
    P0 = gsmg_code[3];
    
    // 清空之前可能残留的按键信息
    delay_ms(100);
    key_matrix_flip_scan();
    
    while(current_mode == MODE_HARUHIKAGE)
    {
        key_val = key_matrix_flip_scan();
        
        // 检测矩阵按键3启动倒计时和播放（从0变为3）
        if (key_val == 3 && last_key == 0)
        {
            lcd1602_show_string(0, 1, "Playing:        ");
            lcd1602_show_string(0, 1, " Haruhikage    ");
            haruhikage();
            
            // 播放完毕后重置为3显示
            if (current_mode == MODE_HARUHIKAGE)
            {
                P0 = gsmg_code[3];
                last_key = 0;  // 重置按键状态
                // 清空可能残留的按键
                delay_ms(100);
                key_matrix_flip_scan();
            }
        }
        else if (key_val != last_key)
        {
            last_key = key_val;  // 更新最后按键状态
        }
        
        delay_ms(50);
    }
}
// 按键3功能：倒计时
void mode_3_display(void)
{
    u8 time_str[10];
    u8 last_seconds = 0;
    u8 cnt_visible = 0;  // CNT是否显示
    static u8 beep_played = 1;  // 初始化为1，防止进入时蜂鸣
    
    // 每次进入功能3时重置倒计时状态
    minutes = 0;
    seconds = 0;
    countdown_active = 0;
    beep_played = 1;  // 进入时设为1，防止蜂鸣
    
    lcd1602_show_string(0, 0, "CountDown Timer ");
    lcd1602_show_string(0, 1, "00:00           "); 
    
    last_seconds = seconds;
    
    while(current_mode == MODE_3)
    {
        u8 key_val = key_matrix_flip_scan();
        
        // 根据矩阵按键控制时间（只在未倒计时时允许调整）
        if (!countdown_active)
        {
            switch(key_val)
            {
                case 1:  // 增加分钟
                    if (minutes < 59) minutes++;
                    break;
                case 2:  // 增加秒钟
                    if (seconds < 59) seconds++;
                    break;
                case 5:  // 减少分钟
                    if (minutes > 0) minutes--;
                    break;
                case 6:  // 减少秒钟
                    if (seconds > 0) seconds--;
                    break;
                case 3:  // 启动倒计时
                    if (minutes > 0 || seconds > 0) {
                        countdown_active = 1;
                        cnt_visible = 1;
                    }
                    break;
                default:
                    break;
            }
        }
        else
        {
            // 倒计时中，按键3可以停止
            if (key_val == 3) {
                countdown_active = 0;
            }
            
            // 检查秒数是否改变，用于切换CNT显示
            if (seconds != last_seconds)
            {
                cnt_visible = !cnt_visible;  // 每秒切换一次
                last_seconds = seconds;
            }
        }
        
        // 格式化时间显示
        time_str[0] = (minutes / 10) + '0';
        time_str[1] = (minutes % 10) + '0';
        time_str[2] = ':';
        time_str[3] = (seconds / 10) + '0';
        time_str[4] = (seconds % 10) + '0';
        
        // 倒计时激活且CNT可见时显示CNT
        if (countdown_active && cnt_visible)
        {
            time_str[5] = ' ';
            time_str[6] = 'C';
            time_str[7] = 'N';
            time_str[8] = 'T';
        }
        else
        {
            time_str[5] = ' ';
            time_str[6] = ' ';
            time_str[7] = ' ';
            time_str[8] = ' ';
        }
        
        time_str[9] = '\0';
        
        // 显示时间和CNT
        lcd1602_show_string(0, 1, time_str);
        
        // 倒计时完成时蜂鸣
        if (minutes == 0 && seconds == 0 && !countdown_active)
        {
            if (!beep_played)
            {
                play_mi_sound();
                beep_played = 1;
            }
        }
        else
        {
            beep_played = 0;  // 重置蜂鸣标志
        }
        
        delay_ms(50);  // 50ms更新一次显示
    }
}

// 按键4功能：显示信息 + LED流水灯
void mode_4_display(void)
{
    u8 led_pos = 0;
    u16 loop_count = 0;
    
    while(current_mode == MODE_4)
    {
        // 显示第一页信息：设计者
        lcd1602_show_string(0, 0, "Designed by     ");
        lcd1602_show_string(0, 1, "Xuanning Zhang  ");
        
        // LED流水灯循环 - 2秒内
        for(loop_count = 0; loop_count < 200; loop_count++)
        {
            if (current_mode != MODE_4) return;  // 检查是否退出
            
            // 控制LED流水灯效果
            led_pos = (loop_count / 25) % 8;  // 每25次循环切换一个LED
            
            // 关闭所有LED
            LED1 = LED2 = LED3 = LED4 = 1;
            LED5 = LED6 = LED7 = LED8 = 1;
            
            // 点亮当前位置的LED
            switch(led_pos)
            {
                case 0: LED1 = 0; break;
                case 1: LED2 = 0; break;
                case 2: LED3 = 0; break;
                case 3: LED4 = 0; break;
                case 4: LED5 = 0; break;
                case 5: LED6 = 0; break;
                case 6: LED7 = 0; break;
                case 7: LED8 = 0; break;
            }
            
            delay_ms(10);
        }
        
        // 关闭所有LED
        LED1 = LED2 = LED3 = LED4 = 1;
        LED5 = LED6 = LED7 = LED8 = 1;
        
        // 显示第二页信息：编辑环境
        lcd1602_show_string(0, 0, "Editing on      ");
        lcd1602_show_string(0, 1, "Mac Book Pro M1 ");
        
        // LED流水灯循环 - 2秒内（反向）
        for(loop_count = 0; loop_count < 200; loop_count++)
        {
            if (current_mode != MODE_4) return;  // 检查是否退出
            
            // 控制LED流水灯效果（反向）
            led_pos = 7 - ((loop_count / 25) % 8);  // 反向流动
            
            // 关闭所有LED
            LED1 = LED2 = LED3 = LED4 = 1;
            LED5 = LED6 = LED7 = LED8 = 1;
            
            // 点亮当前位置的LED
            switch(led_pos)
            {
                case 0: LED1 = 0; break;
                case 1: LED2 = 0; break;
                case 2: LED3 = 0; break;
                case 3: LED4 = 0; break;
                case 4: LED5 = 0; break;
                case 5: LED6 = 0; break;
                case 6: LED7 = 0; break;
                case 7: LED8 = 0; break;
            }
            
            delay_ms(10);
        }
        
        // 关闭所有LED
        LED1 = LED2 = LED3 = LED4 = 1;
        LED5 = LED6 = LED7 = LED8 = 1;
        
        // 显示第三页信息：AI助手
        lcd1602_show_string(0, 0, "Assistance by   ");
        lcd1602_show_string(0, 1, "Claude Haiku 4.5");
        
        // LED流水灯循环 - 2秒内（正向）
        for(loop_count = 0; loop_count < 200; loop_count++)
        {
            if (current_mode != MODE_4) return;  // 检查是否退出
            
            // 控制LED流水灯效果
            led_pos = (loop_count / 25) % 8;  // 正向流动
            
            // 关闭所有LED
            LED1 = LED2 = LED3 = LED4 = 1;
            LED5 = LED6 = LED7 = LED8 = 1;
            
            // 点亮当前位置的LED
            switch(led_pos)
            {
                case 0: LED1 = 0; break;
                case 1: LED2 = 0; break;
                case 2: LED3 = 0; break;
                case 3: LED4 = 0; break;
                case 4: LED5 = 0; break;
                case 5: LED6 = 0; break;
                case 6: LED7 = 0; break;
                case 7: LED8 = 0; break;
            }
            
            delay_ms(10);
        }
        
        // 关闭所有LED
        LED1 = LED2 = LED3 = LED4 = 1;
        LED5 = LED6 = LED7 = LED8 = 1;
    }
}