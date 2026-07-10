#ifndef NATIVESURFACE_DRAW_H
#define NATIVESURFACE_DRAW_H

#include <iostream>
#include <thread>
#include <chrono>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <android/native_window.h>

#include "ANativeWindowCreator.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_android.h"
#include "backends/imgui_impl_vulkan.h"
#include "Android_vulkan/VulkanUtils.h"
#include "ImGui/SabFont.h"
#include "Font/Custom.h"
#include "Font/flamee.h"
#include "Font/Japan.h"
#include "Font/lexend_bold.h"
#include "Font/Pubg_Font.h"
#include "Font/ubuntu_medium.h"
#include "ImGui/IconsFontAwesome7.h"
#include "ImGui/font_awesome.h"
#include "stb_image.h"
#include "GUI_Custom.h"
#include "timer.h"
using namespace std;
using namespace std::chrono_literals;

extern ANativeWindow *native_window;
struct Screen {
    float ScreenX;
    float ScreenY;
};
extern int FPS;
extern timer FPS限制;
extern float FPF显示;
extern float NumIo[50];
extern Screen full_screen;
extern int Orientation;
extern int screen_x, screen_y;
extern int init_screen_x, init_screen_y;
extern bool g_Initialized;
string exec(string command);
int init_Vulkan(int _screen_x, int _screen_y, bool log = false);
void screen_config();
void shutdown();
void tick();
void ImGui_init();
void drawEnd();
void DrawPlayer(ImDrawList *Draw);
//void AimBotAuto(ImDrawList *Draw);
//void DrawPlayer();
void AimBotAuto();
void GetTouch();
void *AimCalcu();
void DrawInit();
int DrawInt();
int 数据();
int 音量();
#endif
