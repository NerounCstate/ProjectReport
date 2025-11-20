#include<8052.h>
#include "public.h"
#include "led8.h"
#include "beep.h"
#include "smg.h"
#include "key.h"
#include "haruhikage.h"

// 全局变量，用于控制音乐播放
extern u8 current_mode;

// 定义音符函数 - 使用头文件中定义的常量
void dou(int i) { // B音
    while(i-- && current_mode == MODE_HARUHIKAGE) {
        BEEP = !BEEP;
        delay_10us(NOTE_DOU);
        LED1 = 0;
    }
    LED1 = 1;
    BEEP = 0;
}

void rei(int i) { // C#音
    while(i-- && current_mode == MODE_HARUHIKAGE) {
        BEEP = !BEEP;
        delay_10us(NOTE_REI);
        LED2 = 0;
    }
    LED2 = 1;
    BEEP = 0;
}

void mi(int i) { // D音
    while(i-- && current_mode == MODE_HARUHIKAGE) {
        BEEP = !BEEP;
        delay_10us(NOTE_MI);
        LED3 = 0;
    }
    LED3 = 1;
    BEEP = 0;
}

void fa(int i) { // E音
    while(i-- && current_mode == MODE_HARUHIKAGE) {
        BEEP = !BEEP;
        delay_10us(NOTE_FA);
        LED4 = 0;
    }
    LED4 = 1;
    BEEP = 0;
}

void sol(int i) { // F#音
    while(i-- && current_mode == MODE_HARUHIKAGE) {
        BEEP = !BEEP;
        delay_10us(NOTE_SOL);
        LED5 = 0;
    }
    LED5 = 1;
    BEEP = 0;
}

void la(int i) { // G音
    while(i-- && current_mode == MODE_HARUHIKAGE) {
        BEEP = !BEEP;
        delay_10us(NOTE_LA);
        // LED6已避免使用（与蜂鸣器共享管脚的历史遗留）
    }
    BEEP = 0;
}

void si(int i) { // A音
    while(i-- && current_mode == MODE_HARUHIKAGE) {
        BEEP = !BEEP;
        delay_10us(NOTE_SI);
        LED7 = 0;
    }
    LED7 = 1;
    BEEP = 0;
}

void hi1(int i) { // 高音1
    while(i-- && current_mode == MODE_HARUHIKAGE) {
        BEEP = !BEEP;
        delay_10us(NOTE_HI1);
        LED7 = 0;
    }
    LED7 = 1;
    BEEP = 0;
}

void hi2(int i) { // 高音2
    while(i-- && current_mode == MODE_HARUHIKAGE) {
        BEEP = !BEEP;
        delay_10us(NOTE_HI2);
        LED7 = 0;
    }
    LED7 = 1;
    BEEP = 0;
}

// 播放歌曲函数
void play(void)
{
    // 前奏
    for(int j=0; j<4 && current_mode == MODE_HARUHIKAGE; j++){
        mi(380);
        delay_10us(BEAT_PAUSE);
        rei(180);
        delay_10us(BEAT_PAUSE);
        dou(380);
        delay_10us(BEAT_PAUSE);
        rei(180);
        delay_10us(BEAT_PAUSE);
        mi(380);
        delay_10us(BEAT_PAUSE);
        fa(100);
        delay_10us(BEAT_PAUSE);
        mi(100);
        delay_10us(BEAT_PAUSE);
        rei(380);
        delay_10us(200000);
    }
    delay_10us(5000);
    
    // 第一段歌词部分
    dou(BEAT_EXTRA); delay_10us(BEAT_PAUSE);
    rei(BEAT_EXTRA); delay_10us(BEAT_PAUSE);
    mi(BEAT_SHORT); delay_10us(BEAT_PAUSE);
    mi(BEAT_SHORT); delay_10us(BEAT_PAUSE);
    rei(BEAT_SHORT); delay_10us(BEAT_PAUSE);
    fa(BEAT_SHORT); delay_10us(BEAT_PAUSE);
    mi(BEAT_SHORT); delay_10us(BEAT_PAUSE);
    rei(BEAT_SHORT); delay_10us(BEAT_PAUSE);
    rei(BEAT_SHORT); delay_10us(BEAT_PAUSE);
    fa(BEAT_SHORT); delay_10us(BEAT_PAUSE);
    dou(BEAT_SHORT); delay_10us(BEAT_PAUSE);
    fa(BEAT_SHORT); delay_10us(BEAT_PAUSE);
    mi(BEAT_SHORT); delay_10us(BEAT_PAUSE);
    rei(BEAT_SHORT); delay_10us(BEAT_PAUSE);
    rei(260); delay_10us(BEAT_PAUSE);
    dou(BEAT_EXTRA); delay_10us(BEAT_PAUSE);
    rei(BEAT_EXTRA); delay_10us(BEAT_PAUSE);
    mi(BEAT_LONG); delay_10us(50000);
    
    // 间奏部分
    mi(BEAT_SHORT); delay_10us(BEAT_PAUSE);
    sol(BEAT_SHORT); delay_10us(BEAT_PAUSE);
    hi1(BEAT_SHORT); delay_10us(BEAT_PAUSE);
    si(BEAT_LONG); delay_10us(BEAT_PAUSE);
    hi1(BEAT_SHORT); delay_10us(BEAT_PAUSE);
    si(BEAT_LONG); delay_10us(BEAT_PAUSE);
    hi1(180); delay_10us(BEAT_PAUSE);
    si(180); delay_10us(BEAT_PAUSE);
    la(180); delay_10us(BEAT_PAUSE);
    sol(350); delay_10us(5000);
    
    // 第二段歌词部分
    sol(200); delay_10us(BEAT_PAUSE);
    rei(200); delay_10us(BEAT_PAUSE);
    fa(200); delay_10us(BEAT_PAUSE);
    fa(200); delay_10us(BEAT_PAUSE);
    mi(200); delay_10us(BEAT_PAUSE);
    mi(200); delay_10us(BEAT_PAUSE);
    mi(250); delay_10us(10000);
    dou(200); delay_10us(BEAT_PAUSE);
    fa(180); delay_10us(BEAT_PAUSE);
    mi(200); delay_10us(BEAT_PAUSE);
    rei(200); delay_10us(BEAT_PAUSE);
    mi(380); delay_10us(BEAT_PAUSE);
    sol(200); delay_10us(BEAT_PAUSE);
    dou(240); delay_10us(BEAT_PAUSE);
}

// haruhikage主函数 - 倒计时后播放音乐
void haruhikage(void)
{
    u8 smg_code[4] = {0x3f, 0x06, 0x5b, 0x4f};
    
    // 3秒倒计时显示
    for(int i = 3; i >= 0; i--){
        SMG_DP = smg_code[i];
        delay_10us(50000);
    }
    
    // 播放音乐
    if(current_mode == MODE_HARUHIKAGE) {
        play();
    }
}