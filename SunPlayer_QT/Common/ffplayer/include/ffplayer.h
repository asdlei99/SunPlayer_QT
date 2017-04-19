﻿#ifndef __FFPLAYER_FFPLAYER_H__
#define __FFPLAYER_FFPLAYER_H__

// 包含头文件
#include "stdefine.h"

#ifdef __cplusplus
extern "C" {
#endif

// 常量定义
// message
#define MSG_FFPLAYER    (WM_APP + 1)
#define PLAY_PROGRESS   (('R' << 24) | ('U' << 16) | ('N' << 8))
#define PLAY_COMPLETED  (('E' << 24) | ('N' << 16) | ('D' << 8))

// adev render type
enum {
    ADEV_RENDER_TYPE_WAVEOUT,
};

// vdev render type
enum {
    VDEV_RENDER_TYPE_GDI,
    VDEV_RENDER_TYPE_D3D,
};

// render mode
enum {
    VIDEO_MODE_LETTERBOX,
    VIDEO_MODE_STRETCHED,
    VIDEO_MODE_MAX_NUM,
};

// visual effect
enum {
    VISUAL_EFFECT_DISABLE ,
    VISUAL_EFFECT_WAVEFORM,
    VISUAL_EFFECT_SPECTRUM,
    VISUAL_EFFECT_MAX_NUM,
};

// param
enum {
    //++ public
    // duration & position
    PARAM_MEDIA_DURATION = 0x1000,
    PARAM_MEDIA_POSITION,

    // media detail info
    PARAM_VIDEO_WIDTH,
    PARAM_VIDEO_HEIGHT,

    // video display mode
    PARAM_VIDEO_MODE,

    // audio volume control
    PARAM_AUDIO_VOLUME,

    // playback speed control
    PARAM_PLAY_SPEED,

    // visual effect mode
    PARAM_VISUAL_EFFECT,

    // audio/video sync diff
    PARAM_AVSYNC_TIME_DIFF,

    // player event callback
    PARAM_PLAYER_CALLBACK,
    //-- public

    //++ for adev
    PARAM_ADEV_RENDER_TYPE = 0x2000,
    //-- for adev

    //++ for vdev
    PARAM_VDEV_RENDER_TYPE = 0x3000,
    PARAM_VDEV_PIXEL_FORMAT,
    PARAM_VDEV_FRAME_RATE,
    PARAM_VDEV_SURFACE_WIDTH,
    PARAM_VDEV_SURFACE_HEIGHT,
    //-- for vdev
};

// player event callback
typedef void (*PFN_PLAYER_CALLBACK)(__int32 msg, __int64 param);

// 函数声明
void* player_open    (char *file, void *win, int adevtype, int vdevtype);
void  player_close   (void *hplayer);
void  player_play    (void *hplayer);
void  player_pause   (void *hplayer);
void  player_seek    (void *hplayer, LONGLONG ms);
void  player_setrect (void *hplayer, int type, int x, int y, int w, int h); // type: 0 - video rect, 1 - visual effect rect
void  player_setparam(void *hplayer, DWORD id, void *param);
void  player_getparam(void *hplayer, DWORD id, void *param);

// 函数说明
/*
player_open     创建一个 player 对象
    file        - 文件路径（可以是网络流媒体的 URL）
    win         - Win32 的窗口句柄
    adevtype    - 音频回放的设备类型，目前仅支持 ADEV_RENDER_TYPE_WAVEOUT
    vdevtype    - 视频回放的设备类型，目前支持 VDEV_RENDER_TYPE_GDI 和 VDEV_RENDER_TYPE_D3D
    返回值      - void* 指针类型，指向 player 对象

player_close    关闭播放器
    hplayer     - 指向 player_open 返回的 player 对象

player_play     开始播放
    hplayer     - 指向 player_open 返回的 player 对象

player_pause    暂停播放
    hplayer     - 指向 player_open 返回的 player 对象

player_seek     跳转到指定位置
    hplayer     - 指向 player_open 返回的 player 对象
    ms          - 指定位置，以毫秒为单位

player_setrect  设置显示区域，有两种显示区域，视频显示区和视觉效果显示区
    hplayer     - 指向 player_open 返回的 player 对象
    type        - 指定区域类型
    x,y,w,h     - 指定显示区域

player_setparam 设置参数
    hplayer     - 指向 player_open 返回的 player 对象
    id          - 参数 id
    param       - 参数指针

player_getparam 获取参数
    hplayer     - 指向 player_open 返回的 player 对象
    id          - 参数 id
    param       - 参数指针
 */

// 参数说明
/*
PARAM_MEDIA_DURATION 和 PARAM_MEDIA_POSITION
用于获取多媒体文件的总长度和当前播放位置（毫秒为单位）
LONGLONG total = 1, pos = 0;
player_getparam(g_hplayer, PARAM_MEDIA_DURATION, &total);
player_getparam(g_hplayer, PARAM_MEDIA_POSITION, &pos  );

PARAM_VIDEO_WIDTH 和 PARAM_VIDEO_HEIGHT
用于获取多媒体文件的视频宽度和高度（像素为单位）
int vw = 0, vh = 0;
player_getparam(g_hplayer, PARAM_VIDEO_WIDTH , &vw);
player_getparam(g_hplayer, PARAM_VIDEO_HEIGHT, &vh);

PARAM_VIDEO_MODE
用于获取和设置视频显示方式，有两种方式可选：
    1. VIDEO_MODE_LETTERBOX - 按比例缩放到显示区域
    2. VIDEO_MODE_STRETCHED - 拉伸到显示区域
（注：视频显示区域由 player_setrect 进行设定）
int mode = 0;
player_getparam(g_hplayer, PARAM_VIDEO_MODE, &mode);
mode = VIDEO_MODE_STRETCHED;
player_setparam(g_hplayer, PARAM_VIDEO_MODE, &mode);

PARAM_AUDIO_VOLUME
用于设置播放音量，不同于系统音量，ffplayer 内部具有一个 -30dB 到 +12dB 的软件音量控制单元
音量范围：[-182, 73]，-182 对应 -30dB，73 对应 +12dB
特殊值  ：0 对应 0dB 增益，-255 对应静音，+255 对应最大增益
int volume = -0;
player_setparam(g_hplayer, PARAM_AUDIO_VOLUME, &volume);

PARAM_PLAY_SPEED
用于设置播放速度，ffplayer 支持变速播放
int speed = 150;
player_setparam(g_hplayer, PARAM_PLAY_SPEED, &speed);
参数 speed 为百分比速度，150 表示以 150% 进行播放
速度没有上限和下限，设置为 0 没有意义，内部会处理为 1%
播放速度的实际上限，由处理器的处理能力决定，超过处理器能力，播放会出现卡顿现象

PARAM_VISUAL_EFFECT
用于指定视觉效果的类型，ffplayer 支持视觉效果，主要是对音频进行视觉效果的呈现
int mode = 0;
player_getparam(g_hplayer, PARAM_VISUAL_EFFECT, &mode);
mode = VISUAL_EFFECT_WAVEFORM;
player_setparam(g_hplayer, PARAM_VISUAL_EFFECT, &mode);
目前总共有三种视觉效果：
    1. VISUAL_EFFECT_DISABLE  - 关闭
    2. VISUAL_EFFECT_WAVEFORM - 波形
    3. VISUAL_EFFECT_SPECTRUM - 频谱
（注：视觉效果区域由 player_setrect 进行设定）

PARAM_AVSYNC_TIME_DIFF
用于设置 audio 和 video 的时间同步差值（毫秒为单位）
int diff = 100;
player_setparam(g_hplayer, PARAM_AVSYNC_TIME_DIFF, &diff);
设置为 100 后，音频将比视频快 100ms，设置为 -100 则慢 100ms

PARAM_PLAYER_CALLBACK
用于设置播放器事件回调函数，回调函数的原型定义如下：
typedef void (*PFN_PLAYER_CALLBACK)(__int32 msg, __int64 param);
回调时的参数定义如下：
    msg   - PLAY_PROGRESS 播放进行中，PLAY_COMPLETED 播放完成
    param - 当前播放进度，以毫秒为单位
（注：建议使用 windows 的消息机制来处理播放器事件，更加安全稳定。
      只要不使用 PARAM_PLAYER_CALLBACK 的接口，来注册回调函数，
      ffplayer 会发送 MSG_FFPLAYER 这个消息通知应用程序播放已经
      完成，而播放进度的显示，建议在窗口中使用定时器机制来查询并
      显示）

PARAM_VDEV_RENDER_TYPE
用于设置视频渲染方式，目前有 VDEV_RENDER_TYPE_GDI 和 VDEV_RENDER_TYPE_D3D 两种可选
int mode = 0;
player_getparam(g_hplayer, PARAM_VDEV_RENDER_TYPE, &mode);
mode = VDEV_RENDER_TYPE_D3D;
player_setparam(g_hplayer, PARAM_VDEV_RENDER_TYPE, &mode);


所有的参数，都是可以 get 的，但并不是所有的参数都可以 set，因为有些参数是只读的。
 */

#ifdef __cplusplus
}
#endif

#endif




