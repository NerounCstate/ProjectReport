#include "string_utils.h"

/*******************************************************************************
* 函 数 名       : format_time_to_string
* 函数功能		 : 将分钟和秒钟格式化为"MM:SS"字符串
* 输    入       : time_str：输出字符串指针
                   minutes：分钟数
                   seconds：秒钟数
                   add_status：是否添加倒计时状态指示符(1=是)
* 输    出    	 : 无
*******************************************************************************/
void format_time_to_string(u8 *time_str, u16 minutes, u16 seconds, u8 add_status)
{
    u8 index = 0;
    
    // 处理分钟的十位
    if(minutes >= 10)
    {
        time_str[index++] = (minutes/10) + '0';
    }
    else
    {
        time_str[index++] = '0';  // 前导零
    }
    
    // 处理分钟的个位
    time_str[index++] = (minutes%10) + '0';
    
    // 添加分隔符
    time_str[index++] = ':';
    
    // 处理秒钟的十位
    if(seconds >= 10)
    {
        time_str[index++] = (seconds/10) + '0';
    }
    else
    {
        time_str[index++] = '0';  // 前导零
    }
    
    // 处理秒钟的个位
    time_str[index++] = (seconds%10) + '0';
    
    // 如果需要添加状态指示
    if(add_status)
    {
        time_str[index++] = ' ';
        time_str[index++] = 'C';
        time_str[index++] = 'N';
        time_str[index++] = 'T';
    }
    
    // 字符串结束符
    time_str[index] = '\0';
}

/*******************************************************************************
* 函 数 名       : int_to_string
* 函数功能		 : 将整数转换为指定位数的字符串
* 输    入       : str：输出字符串指针
                   value：需要转换的整数
                   digits：转换后的位数（如果不足则添加前导零）
* 输    出    	 : 无
*******************************************************************************/
void int_to_string(u8 *str, u16 value, u8 digits)
{
    u8 i;
    u8 temp_str[10];
    u8 index = 0;
    
    // 先将数字转换到临时数组
    if(value == 0)
    {
        temp_str[index++] = '0';
    }
    else
    {
        while(value > 0)
        {
            temp_str[index++] = (value % 10) + '0';
            value /= 10;
        }
    }
    
    // 处理前导零
    u8 output_index = 0;
    if(index < digits)
    {
        for(i = 0; i < (digits - index); i++)
        {
            str[output_index++] = '0';
        }
    }
    
    // 反序输出（因为是倒序转换的）
    for(i = index; i > 0; i--)
    {
        str[output_index++] = temp_str[i-1];
    }
    
    str[output_index] = '\0';
}

/*******************************************************************************
* 函 数 名       : float_to_string
* 函数功能		 : 将浮点数转换为字符串
* 输    入       : str：输出字符串指针
                   value：浮点数值
                   decimal_places：小数位数
* 输    出    	 : 无
*******************************************************************************/
void float_to_string(u8 *str, float value, u8 decimal_places)
{
    u8 index = 0;
    int integer_part;
    int decimal_part;
    
    // 处理负数
    if(value < 0) {
        str[index++] = '-';
        value = -value;
    }
    
    // 分离整数部分和小数部分
    integer_part = (int)value;
    decimal_part = (int)((value - integer_part) * 10 + 0.5);  // 保留一位小数
    
    // 处理整数部分
    if(integer_part >= 100)
    {
        str[index++] = (integer_part/100) + '0';
        integer_part %= 100;
    }
    
    if(integer_part >= 10 || index > 0)
    {
        str[index++] = (integer_part/10) + '0';
        integer_part %= 10;
    }
    
    str[index++] = integer_part + '0';
    
    // 小数点
    if(decimal_places > 0)
    {
        str[index++] = '.';
        
        // 小数位
        for(u8 i = 0; i < decimal_places; i++)
        {
            if(i == 0)
            {
                str[index++] = decimal_part + '0';
            }
        }
    }
    
    // 字符串结束符
    str[index] = '\0';
}
