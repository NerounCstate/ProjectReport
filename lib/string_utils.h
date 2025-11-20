#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__

#include "public.h"

/*
 * 时间格式化函数
 * 将分钟和秒钟格式化为"MM:SS"字符串
 */
void format_time_to_string(u8 *time_str, u16 minutes, u16 seconds, u8 add_status);

/*
 * 整数转字符串函数
 * 将整数转换为指定位数的字符串，支持前导零
 */
void int_to_string(u8 *str, u16 value, u8 digits);

/*
 * 浮点数转字符串函数
 * 将浮点数转换为字符串，支持指定小数位数
 */
void float_to_string(u8 *str, float value, u8 decimal_places);

#endif
