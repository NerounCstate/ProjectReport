# 快速参考指南

## 🎛️ 系统模式说明

| 模式 | 对应按键 | 功能说明 |
|------|--------|--------|
| **MODE_START_PAGE** (0) | 初始 | 主菜单显示 |
| **MODE_TEMPERATURE** (1) | 按键1 | 温度实时显示(DS18B20) |
| **MODE_HARUHIKAGE** (2) | 按键2 | 音乐播放(春日野穹) |
| **MODE_3** (3) | 按键3 | 倒计时功能 |
| **MODE_4** (4) | 按键4 | 关于信息显示 |

---

## 🎶 音符频率参考

所有音符延时值以10us为单位

| 音符 | 常量名 | 延时值 | 频率近似 |
|------|--------|--------|---------|
| B音 | NOTE_DOU | 176 | ~284Hz |
| C#音 | NOTE_REI | 157 | ~318Hz |
| D音 | NOTE_MI | 140 | ~357Hz |
| E音 | NOTE_FA | 132 | ~378Hz |
| F#音 | NOTE_SOL | 118 | ~423Hz |
| G音 | NOTE_LA | 108 | ~463Hz |
| A音 | NOTE_SI | 93 | ~537Hz |
| 高音1 | NOTE_HI1 | 87 | ~575Hz |
| 高音2 | NOTE_HI2 | 88 | ~568Hz |

---

## ⏱️ 节拍长度定义

| 常量 | 值 | 说明 |
|------|-----|------|
| BEAT_LONG | 600 | 长音符(四分音符) |
| BEAT_SHORT | 220 | 短音符(八分音符) |
| BEAT_EXTRA | 80 | 极短音符 |
| BEAT_VERY_SHORT | 100 | 很短音符 |
| BEAT_PAUSE | 500 | 音符间停顿 |

---

## 🔌 硬件管脚配置

### LCD1602管脚
```c
LCD1602_RS   = P2_6  // 寄存器选择(0=命令, 1=数据)
LCD1602_RW   = P2_5  // 读写选择(0=写, 1=读)
LCD1602_E    = P2_7  // 使能脚(上升沿有效)
LCD1602_DATA = P0    // 8位数据端口
```

### LED管脚(8个LED)
```c
LED1 = P2_0  LED5 = P2_4
LED2 = P2_1  LED6 = P2_5
LED3 = P2_2  LED7 = P2_6
LED4 = P2_3  LED8 = P2_7
```

### 其他器件
```c
BEEP      = P2_5  // 蜂鸣器
DS18B20   = P3_7  // 温度传感器
KEY_MATRIX = P1   // 矩阵按键(0x00~0xFF)
SMG_DP    = P0    // 数码管显示
```

### 外部中断
```c
INT0 (KEY3) = P3_2  // 返回上一级按键
```

---

## 📊 常用数据转换

### 时间格式化示例
```c
unsigned char time_str[16];

// 将10分30秒格式化为"10:30"
format_time_to_string(time_str, 10, 30, 0);
// 结果: "10:30"

// 加入倒计时状态指示
format_time_to_string(time_str, 5, 45, 1);
// 结果: "05:45 CNT"
```

### 浮点数显示示例
```c
unsigned char temp_str[16];

// 将26.5°C格式化为显示字符串
float_to_string(temp_str, 26.5, 1);
// 结果: "26.5"
```

---

## ⚡ 延迟函数参考

| 函数 | 参数单位 | 参数值 | 实际延时 |
|------|--------|--------|---------|
| delay_10us(n) | 10微秒 | 1 | ~10us |
| delay_10us(100) | 10微秒 | 100 | ~1ms |
| delay_ms(n) | 毫秒 | 1 | ~1ms |
| delay_ms(1000) | 毫秒 | 1000 | ~1秒 |

---

## 🎹 播放音乐的基本步骤

1. **进入MODE_HARUHIKAGE**
   ```c
   current_mode = MODE_HARUHIKAGE;
   ```

2. **调用haruhikage()函数**
   - 显示3秒倒计时
   - 自动播放音乐

3. **音乐播放控制**
   - 音乐会检查`current_mode`
   - 如果中途改变模式，音乐会停止

---

## 🌡️ 温度读取的工作流程

1. **初始化**
   ```c
   ds18b20_init();  // 检测传感器
   ```

2. **读取温度**
   ```c
   float temp = ds18b20_read_temperature();
   // 返回值范围: -55.0 ~ +125.0°C
   ```

3. **格式化显示**
   ```c
   float_to_string(buf, temp, 1);  // 一位小数
   ```

---

## ⌨️ 矩阵按键扫描结果

4×4矩阵按键扫描返回值(1-16):

```
行\列  1  2  3  4
  1   1  2  3  4
  2   5  6  7  8
  3   9 10 11 12
  4  13 14 15 16
```

### 按键扫描函数
```c
u8 key = key_matrix_flip_scan();
// 返回0表示无按键按下
// 返回1-16表示对应按键
```

---

## 📋 主要函数调用流程

### 启动流程
```
main()
  ├─ lcd1602_init()       // 初始化LCD
  ├─ ds18b20_init()       // 初始化温度传感器
  ├─ exit_init()          // 初始化外部中断
  └─ 播放启动音效 (dou, rei, mi)
```

### 模式切换流程
```
main() 循环
  ├─ 检测矩阵按键
  ├─ 根据按键更改current_mode
  └─ switch(current_mode)执行不同功能
     ├─ MODE_TEMPERATURE → temperature_display()
     ├─ MODE_HARUHIKAGE → haruhikage_playing()
     ├─ MODE_3 → mode_3_display()
     └─ MODE_4 → mode_4_display()
```

### 返回主菜单
```
按下KEY3 (P3_2外部中断)
  └─ current_mode = MODE_START_PAGE
```

---

## 🔍 调试技巧

### 查看字符串输出
```c
// 在LCD显示调试信息
lcd1602_show_string(0, 0, "Debug Info:");
lcd1602_show_string(0, 1, "value=123   ");
```

### 使用LED指示
```c
// 点亮LED表示程序进度
LED1 = 0;  // 点亮(低电平有效)
LED1 = 1;  // 熄灭

// 快速闪烁表示运行中
for(int i=0; i<10; i++) {
    LED1 = 0; delay_ms(100);
    LED1 = 1; delay_ms(100);
}
```

### 蜂鸣器测试
```c
// 短促鸣叫
BEEP = 0; delay_ms(100);
BEEP = 1; delay_ms(100);
```

---

## 📚 文件结构说明

```
ReportWork/
├── src/
│   ├── main.c              // 主程序（模式管理、UI）
│   ├── lcd1602.c           // LCD驱动
│   ├── ds18b20.c           // 温度传感器驱动
│   ├── timer.c             // 定时器和倒计时
│   ├── key_matrix.c        // 矩阵按键扫描
│   ├── public.c            // 基础延迟函数
│   ├── Haruhikage.c        // 音乐播放模块
│   └── string_utils.c      // 字符串格式化工具
├── lib/
│   ├── public.h            // 基础类型定义
│   ├── lcd1602.h           // LCD接口定义
│   ├── ds18b20.h           // 温度传感器接口
│   ├── timer.h             // 定时器接口
│   ├── key_matrix.h        // 按键矩阵接口
│   ├── key.h               // 模式常量定义
│   ├── beep.h              // 蜂鸣器定义
│   ├── led8.h              // LED定义
│   ├── smg.h               // 数码管定义
│   ├── haruhikage.h        // 音乐接口（常量+函数）
│   └── string_utils.h      // 字符串工具接口
├── include/
│   └── intrins.h           // 8051指令集
└── platformio.ini          // 构建配置
```

---

**最后更新**: 2025年11月20日
**版本**: 1.0
