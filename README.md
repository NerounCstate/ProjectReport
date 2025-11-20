# STC89C52RC 综合应用系统

## 项目概述

这是一个基于 **STC89C52RC 单片机**的综合应用项目，集成了多个常用模块和功能：

- ✅ **温度传感**：基于DS18B20传感器的实时温度显示
- ✅ **LCD显示**：1602液晶屏幕显示
- ✅ **音乐播放**：播放春日野穹 (Haruhikage) 主题曲
- ✅ **倒计时功能**：可配置分钟和秒钟的倒计时器
- ✅ **矩阵按键**：4x4矩阵按键控制系统
- ✅ **LED指示**：8个LED灯状态指示

## 项目结构

```
ReportWork/
├── platformio.ini              # PlatformIO配置文件
├── include/                    # 包含目录
│   ├── intrins.h              # 内置函数库
│   └── README
├── lib/                        # 库文件和头文件
│   ├── beep.h                 # 蜂鸣器定义
│   ├── config.h               # 系统配置常数（新增）
│   ├── ds18b20.h              # DS18B20温度传感器
│   ├── haruhikage.h           # 音乐播放模块
│   ├── key.h                  # 按键定义和模式常数
│   ├── key_matrix.h           # 矩阵按键驱动
│   ├── lcd1602.h              # LCD显示驱动
│   ├── led8.h                 # LED定义
│   ├── public.h               # 公共类型定义
│   ├── smg.h                  # 数码管定义
│   ├── string_utils.h         # 字符串工具库（新增）
│   ├── timer.h                # 定时器模块
│   └── README
└── src/                        # 源代码
    ├── ds18b20.c              # DS18B20驱动实现
    ├── Haruhikage.c           # 音乐播放实现
    ├── key_matrix.c           # 矩阵按键驱动实现
    ├── lcd1602.c              # LCD驱动实现
    ├── main.c                 # 主程序
    ├── public.c               # 公共函数
    ├── string_utils.c         # 字符串工具库（新增）
    └── timer.c                # 定时器实现
```

## 系统工作流程

### 主界面
```
┌─────────────────────────────┐
│  Press Key3 Back            │
│  Matrix_SKey 1-4            │
└─────────────────────────────┘
```

使用矩阵按键选择功能：
- **按键1**：进入温度显示模式
- **按键2**：进入音乐播放模式
- **按键3**：进入倒计时模式
- **按键4**：显示关于信息
- **按键Key3**：返回主界面

### 功能模块

#### 1. 温度显示模式 (MODE_TEMPERATURE)
- 显示当前温度值
- 实时更新频率：约1秒钟一次
- 支持显示负温度和小数点

#### 2. 音乐播放模式 (MODE_HARUHIKAGE)
- 3秒倒计时后自动播放音乐
- 可实时中断返回主界面
- LED灯配合音乐闪烁

#### 3. 倒计时模式 (MODE_3)
- 按键1：增加分钟
- 按键2：增加秒钟
- 按键5：减少分钟
- 按键6：减少秒钟
- 按键3：启动/停止倒计时
- 倒计时结束时播放提示音

#### 4. 关于模式 (MODE_4)
- 显示项目信息和开发者信息

## 硬件管脚配置

### LCD1602 接口
| 功能 | 管脚 | 说明 |
|------|------|------|
| RS   | P2.6 | 数据命令选择 |
| RW   | P2.5 | 读写选择 |
| EN   | P2.4 | 使能信号 |
| D0-D7| P0   | 8位数据总线 |

### DS18B20 温度传感器
| 功能 | 管脚 |
|------|------|
| DQ   | P3.7 |

### 按键接口
| 功能 | 管脚 |
|------|------|
| 矩阵按键 | P1 |
| KEY3 | P3.2 |

### LED 指示
| LED  | 管脚 |
|------|------|
| LED1 | P2.0 |
| LED2 | P2.1 |
| LED3 | P2.2 |
| LED4 | P2.3 |
| LED5 | P2.4 |
| LED6 | P1.0 |
| LED7 | P1.1 |
| LED8 | P1.2 |

### 音频输出
| 功能 | 管脚 |
|------|------|
| BEEP | P2.7 |

### 数码管
| 功能 | 管脚 |
|------|------|
| SMG  | P0   |

## 主要优化改进

### 1. 代码结构优化
- ✅ 消除代码重复：统一音符函数到 `Haruhikage.c`
- ✅ 创建字符串工具库：统一时间和浮点数格式化
- ✅ 添加配置头文件：集中管理系统常数

### 2. 硬件管脚优化
- ✅ 修复LED和BEEP管脚冲突
- ✅ 将LCD_E重命名为LCD_EN避免枚举冲突
- ✅ 重新分配LED6、LED7、LED8到P1口

### 3. 驱动优化
- ✅ 改进DS18B20通信超时处理
- ✅ 优化LCD显示性能
- ✅ 改进矩阵按键扫描算法

### 4. 代码规范
- ✅ 规范化变量命名和格式
- ✅ 统一代码风格和缩进
- ✅ 添加详细的函数文档注释

## 编译和烧录

### 使用 PlatformIO

```bash
# 编译项目
platformio run -e STC89C52RC

# 烧录到设备
platformio run -e STC89C52RC --target upload

# 清除编译产物
platformio run -e STC89C52RC --target clean
```

### 编译选项说明
- `-Os`：优化代码大小，适合嵌入式系统
- `-Iinclude`：包含include目录
- `-Ilib`：包含lib目录

## 函数接口文档

### 字符串工具库 (string_utils.h)

#### format_time_to_string()
```c
void format_time_to_string(u8 *time_str, u16 minutes, u16 seconds, u8 add_status);
```
功能：将分钟和秒钟格式化为"MM:SS"字符串  
参数：
- `time_str`：输出字符串指针
- `minutes`：分钟数（0-59）
- `seconds`：秒钟数（0-59）
- `add_status`：是否添加倒计时状态指示符(1=是)

#### float_to_string()
```c
void float_to_string(u8 *str, float value, u8 decimal_places);
```
功能：将浮点数转换为字符串  
参数：
- `str`：输出字符串指针
- `value`：浮点数值
- `decimal_places`：小数位数

### 音乐模块 (haruhikage.h)

音符播放函数都使用统一的常数定义：
```c
#define NOTE_DOU   176  // B音
#define NOTE_REI   157  // C#音
#define NOTE_MI    140  // D音
#define NOTE_FA    132  // E音
#define NOTE_SOL   118  // F#音
#define NOTE_LA    108  // G音
#define NOTE_SI    93   // A音
```

#### play()
```c
void play(void);
```
功能：播放完整的春日野穹主题曲

#### haruhikage()
```c
void haruhikage(void);
```
功能：包含倒计时和音乐播放的主函数

## 注意事项

1. **晶振频率**：默认配置为 11.0592 MHz
   - 如使用其他晶振频率，需修改 `config.h` 中的 `XTAL_FREQ` 和 `DELAY_MS_LOOP_COUNT`

2. **LCD初始化**：
   - 默认配置为8位数据总线
   - 可通过修改 `lcd1602.h` 中的 `LCD1602_4OR8_DATA_INTERFACE` 切换4位模式

3. **温度显示范围**：
   - DS18B20支持-55℃ 到 +125℃
   - 本项目显示精度为0.1℃

4. **倒计时时间**：
   - 最长可设置59分59秒
   - 定时器精度约±1ms

## 开发说明

### 添加新功能

1. 创建新的模式常数（在 `key.h` 中）
2. 编写对应的模块代码
3. 在 `main.c` 的主循环中添加 case 分支
4. 更新文档

### 代码提交规范

- 保持统一的代码风格
- 添加详细的函数文档注释
- 避免全局变量，使用 `extern` 声明外部变量
- 使用意义明确的变量名

## 常见问题

**Q: 温度显示不正常？**  
A: 检查DS18B20的DQ管脚连接，确保有4.7k欧的上拉电阻

**Q: 音乐播放中断？**  
A: 确保定时器0配置正确，检查中断优先级设置

**Q: LED不闪烁？**  
A: 验证LED管脚配置和供电连接

## 许可证

本项目开源提供，可自由使用和修改。

## 作者

- 原始设计者：Xuanning Zhang
- 代码优化：基于PlatformIO框架的全面改进

---

**最后更新**：2024年11月  
**支持平台**：PlatformIO + STC89C52RC
