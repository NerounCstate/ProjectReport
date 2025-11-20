#include "timer.h"
#include "key_matrix.h"
#include "lcd1602.h"
#include "beep.h"
#include "string_utils.h"

// 全局变量，用于存储时间
u16 minutes = 0;
u16 seconds = 0;
u8 countdown_active = 0;  // 倒计时是否激活的标志
u8 countdown_finished = 0; // 倒计时完成标志

/*******************************************************************************
* 函 数 名         : timer0_init
* 函数功能		   : 定时器0初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void timer0_init(void)
{
	TMOD |= 0x01;  // 设置定时器0为工作方式1（16位定时器）
	TH0 = 0xFC;    // 初始值高8位
	TL0 = 0x18;    // 初始值低8位，定时1ms（假设晶振为11.0592MHz）
	ET0 = 1;       // 使能定时器0中断
	EA = 1;        // 使能总中断
	TR0 = 1;       // 启动定时器0
}

/*******************************************************************************
* 函 数 名         : timer0_isr
* 函数功能		   : 定时器0中断服务函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void timer0_isr(void) __interrupt(1)
{
	static u16 count = 0;
	
	// 重新装载初值，保持1ms定时
	TH0 = 0xFC;
	TL0 = 0x18;
	
	count++;
	// 每隔一定时间执行特定任务
	if(count >= 1000)  // 约1秒
	{
		count = 0;
		// 如果倒计时激活且时间大于0，则减少时间
		if(countdown_active && (minutes > 0 || seconds > 0))
		{
			if(seconds > 0)
			{
				seconds--;
			}
			else if(minutes > 0)
			{
				minutes--;
				seconds = 59;
			}
		}
		// 如果倒计时结束（时间为00:00）且之前是激活状态
		else if(countdown_active && minutes == 0 && seconds == 0)
		{
			// 停止倒计时
			countdown_active = 0;
			countdown_finished = 1; // 设置倒计时完成标志
		}
	}
}

/*******************************************************************************
* 函 数 名         : time_control
* 函数功能		   : 通过矩阵按键控制时间（分钟和秒钟的增减）
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void time_control(void)
{
	u8 key_val = 0;
	
	// 扫描矩阵按键
	key_val = key_matrix_flip_scan();
	
	// 根据按键值处理时间增减
	switch(key_val)
	{
		case 1:  // 按键1：增加分钟
			if(!countdown_active)  // 只有在非倒计时状态下才能调整时间
			{
				minutes++;
				if(minutes > 59) minutes = 0;  // 限制分钟范围0-59
			}
			break;
			
		case 2:  // 按键2：增加秒钟
			if(!countdown_active)  // 只有在非倒计时状态下才能调整时间
			{
				seconds++;
				if(seconds > 59) seconds = 0;  // 限制秒钟范围0-59
			}
			break;
			
		case 5:  // 按键5：减少分钟
			if(!countdown_active)  // 只有在非倒计时状态下才能调整时间
			{
				if(minutes > 0) 
					minutes--;
				else 
					minutes = 59;
			}
			break;
			
		case 6:  // 按键6：减少秒钟
			if(!countdown_active)  // 只有在非倒计时状态下才能调整时间
			{
				if(seconds > 0) 
					seconds--;
				else 
					seconds = 59;
			}
			break;
			
		case 3:  // 按键3：启动/停止倒计时
			countdown_active = !countdown_active;
			break;
			
		default:
			break;
	}
	
	// 显示时间
	u8 time_str[16];
	
	// 使用格式化函数代替手动构建字符串
	format_time_to_string(time_str, minutes, seconds, countdown_active);
	
	// 在LCD上显示时间
	lcd1602_show_string(0, 1, time_str);
	
	// 如果倒计时结束，播放三声短暂的mi音
	static u8 played_sound = 0;
	if(!countdown_active && minutes == 0 && seconds == 0)
	{
		if(!played_sound)
		{
			// 播放三声短暂的mi音
			for(int i = 0; i < 3; i++)
			{
				lcd1602_show_string(0, 1, "It's Time!      ");
				play_mi_sound();
				delay_ms(500);  // 声音之间间隔500ms
			}
			played_sound = 1;
		}
	}
	else
	{
		// 重置声音播放标志，以便下次倒计时结束时可以再次播放
		played_sound = 0;
	}
}

/*******************************************************************************
* 函 数 名         : play_mi_sound
* 函数功能		   : 播放短暂的mi音
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void play_mi_sound(void)
{
	// 播放大约200ms的mi音（假设mi音频率对应特定延时）
	for(int i = 0; i < 200; i++)
	{
		BEEP = !BEEP;
		delay_10us(140);  // mi音的延时
	}
	BEEP = 0;  // 关闭蜂鸣器
	delay_ms(100);  // 音符之间的短暂停顿
}

/*******************************************************************************
* 函 数 名         : get_minutes
* 函数功能		   : 获取当前分钟数
* 输    入         : 无
* 输    出         : 当前分钟数
*******************************************************************************/
u16 get_minutes(void)
{
	return minutes;
}

/*******************************************************************************
* 函 数 名         : get_seconds
* 函数功能		   : 获取当前秒钟数
* 输    入         : 无
* 输    出         : 当前秒钟数
*******************************************************************************/
u16 get_seconds(void)
{
	return seconds;
}

/*******************************************************************************
* 函 数 名         : set_time
* 函数功能		   : 设置时间
* 输    入         : m: 分钟, s: 秒钟
* 输    出         : 无
*******************************************************************************/
void set_time(u16 m, u16 s)
{
	if(m <= 59 && !countdown_active) minutes = m;  // 只有在非倒计时状态下才能设置时间
	if(s <= 59 && !countdown_active) seconds = s;
}

/*******************************************************************************
* 函 数 名         : is_countdown_active
* 函数功能		   : 检查倒计时是否激活
* 输    入         : 无
* 输    出         : 倒计时状态（1=激活，0=未激活）
*******************************************************************************/
u8 is_countdown_active(void)
{
	return countdown_active;
}

/*******************************************************************************
* 函 数 名         : is_countdown_finished
* 函数功能		   : 检查倒计时是否已完成
* 输    入         : 无
* 输    出         : 倒计时完成状态（1=已完成，0=未完成）
*******************************************************************************/
u8 is_countdown_finished(void)
{
	return countdown_finished;
}

/*******************************************************************************
* 函 数 名         : reset_countdown_finished
* 函数功能		   : 重置倒计时完成标志
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void reset_countdown_finished(void)
{
	countdown_finished = 0;
}
