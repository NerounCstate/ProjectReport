#ifndef _HARUHIKAGE_H
#define _HARUHIKAGE_H

#include "public.h"

// 音符延时常量定义（单位：10us）
#define NOTE_DOU   176  // B音
#define NOTE_REI   157  // C#音
#define NOTE_MI    140  // D音
#define NOTE_FA    132  // E音
#define NOTE_SOL   118  // F#音
#define NOTE_LA    108  // G音
#define NOTE_SI    93   // A音
#define NOTE_HI1   87   // 高音1
#define NOTE_HI2   88   // 高音2

// 音符时长定义
#define BEAT_LONG   600
#define BEAT_SHORT  220
#define BEAT_EXTRA  80
#define BEAT_VERY_SHORT  100
#define BEAT_PAUSE  500

// 音符函数声明
void dou(int i);   // B音
void rei(int i);   // C#音
void mi(int i);    // D音
void fa(int i);    // E音
void sol(int i);   // F#音
void la(int i);    // G音
void si(int i);    // A音
void hi1(int i);   // 高音1
void hi2(int i);   // 高音2
void play(void);   // 播放完整的歌曲
void haruhikage(void);  // haruhikage主函数

#endif