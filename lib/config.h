#ifndef __CONFIG_H__
#define __CONFIG_H__

/*
 * 系统配置头文件
 * 集中管理所有常用的系统配置和定义
 */

#include "8052.h"

// 系统晶振频率配置（单位：Hz）
#define XTAL_FREQ   11059200UL  // 11.0592MHz

// 延时函数参数（基于上述晶振频率）
#define DELAY_MS_LOOP_COUNT  110  // 延时1ms需要的循环次数

// ==================== 功能模式定义 ====================
#define MODE_START_PAGE    0      // 初始界面
#define MODE_TEMPERATURE   1      // 温度显示
#define MODE_HARUHIKAGE    2      // 音乐播放
#define MODE_3             3      // 模式3
#define MODE_4             4      // 模式4

// ==================== 硬件资源配置 ====================

// LCD1602配置
#define LCD_ROWS           2      // LCD行数
#define LCD_COLUMNS        16     // LCD列数
#define LCD_DATA_WIDTH     8      // 8位数据总线

// 按键配置
#define KEY_DEBOUNCE_TIME  1000   // 按键消抖时间（10us单位）
#define MATRIX_ROWS        4      // 矩阵按键行数
#define MATRIX_COLS        4      // 矩阵按键列数

// 温度传感器配置
#define TEMP_READ_INTERVAL 50     // 温度读取间隔（20ms循环单位）
#define TEMP_DECIMAL_PLACES 1     // 温度显示小数位数

// ==================== 时间设置 ====================
#define MAX_MINUTES        59      // 最大分钟数
#define MAX_SECONDS        59      // 最大秒钟数

#endif
