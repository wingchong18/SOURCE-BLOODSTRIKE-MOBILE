#include <iostream>
#include <thread>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <vector>
#include <mutex>
#include <map>
#include <functional>
#include <chrono>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <linux/input.h>
#include <dirent.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <random>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <arm_neon.h>

#include "../xorstr.hpp"
#include "Oxorany/oxorany.h"
#include "draw.h"
#include "ANativeWindowCreator.h"
#include "memoryImage/Logo.h"
#include "include.h"
#include "kernelReadWrite/wanbai.h"
#include "Variabel.h"
#include "ImGui/imgui_additional.h"
#include "Offsets.hpp"
#include "UI_Functions.h"
std::mutex mtx;
// --- GLOBALS ---
uintptr_t MessiahBase = 0;
long int LocalPlayer = 0;
pid_t pid = 0;
bool isFiring = false;
bool isADS = false;
char PlayerName[100] = {0};
int AimCount = 0;
int MaxPlayerCount = 0;
int Gmin = -1;
float matrix[16] = {0};
float px = 0, py = 0;
extern uint64_t g_LayoutCheckSum;
std::vector<std::thread> threadPool;

namespace sdk {
    bool esp_enabled = false;
    bool esp_box = false;
    bool esp_line = false;
    bool esp_distance = false;
    bool esp_bone = false;
    int esp_line_pos = 0; // 0: Bottom, 1: Center, 2: Top

    bool aimbot_enabled = false;
    bool aimbot_draw_fov = true;
    bool aimbot_draw_line = true;
    bool aimbot_always_on = false;
    bool aimbot_use_trigger = true;
    float aimbot_fov = 150.0f;
    float aimbot_smooth = 5.0f;
    int aimbot_target = 0;
    int aimbot_mode = 0; // 0: Crosshair, 1: Distance
    bool aimbot_ignore_prone = true;

    float fire_button_x = 1800.0f;
    float fire_button_y = 600.0f;
    float fire_button_radius = 150.0f;
    bool show_trigger_area = true;

    float color_esp_box[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
    float color_esp_line[4] = { 1.0f, 1.0f, 1.0f, 0.5f };
    float color_esp_bone[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float color_aimbot_line[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
    float color_aimbot_fov[4] = { 1.0f, 1.0f, 1.0f, 0.5f };
}

bool g_hasTarget = false;
bool g_isFiring = false;

struct Matrix3x4 { float m[3][4]; };

bool ProjectWorldToScreen(uint64_t camera, Vector3A world, Vector2A& out) {
    if (!camera) return false;

    float cam_rel_x = driver->read<float>(camera + 124);
    float cam_rel_y = driver->read<float>(camera + 128);
    float cam_rel_z = driver->read<float>(camera + 132);

    float relX = world.X - cam_rel_x;
    float relY = world.Y - cam_rel_y;
    float relZ = world.Z - cam_rel_z;

    float m11 = driver->read<float>(camera + 772);
    float m12 = driver->read<float>(camera + 784);
    float m13 = driver->read<float>(camera + 796);

    float m21 = driver->read<float>(camera + 776);
    float m22 = driver->read<float>(camera + 788);
    float m23 = driver->read<float>(camera + 800);

    float m31 = driver->read<float>(camera + 780);
    float m32 = driver->read<float>(camera + 792);
    float m33 = driver->read<float>(camera + 804);

    float px_proj = relX * m11 + relY * m12 + relZ * m13;
    float py_proj = relX * m21 + relY * m22 + relZ * m23;
    float pzOrig = relX * m31 + relY * m32 + relZ * m33;

    if (pzOrig >= -0.01f) return false;

    float pz = -pzOrig;

    float fov = driver->read<float>(camera + 824);
    if (fov < 5.0f || fov > 170.0f) fov = 90.0f;
    float f = 1.0f / tanf((fov * 0.017453292f) * 0.5f);

    float screenW = (float)driver->read<uint16_t>(camera + 752);
    float screenH = (float)driver->read<uint16_t>(camera + 754);

    if (screenW == 0 || screenH == 0) {
        screenW = (float)screen_x;
        screenH = (float)screen_y;
    }

    float invZ = 1.0f / (pz > 0.000001f ? pz : 0.000001f);

    out.X = ((px_proj * invZ) * f * screenH + screenW) * 0.5f;
    out.Y = (screenH - ((py_proj * invZ) * f * screenH)) * 0.5f;

    return true;
}

Vector3A TransformBonePosition(const Matrix3x4& world, const Matrix3x4& bone) {
    Vector3A out;
    out.X = (world.m[0][0] * bone.m[2][1]) + (world.m[0][3] * bone.m[2][2]) + (world.m[1][2] * bone.m[2][3]) + world.m[2][1];
    out.Y = (world.m[0][1] * bone.m[2][1]) + (world.m[1][0] * bone.m[2][2]) + (world.m[1][3] * bone.m[2][3]) + world.m[2][2];
    out.Z = (world.m[0][2] * bone.m[2][1]) + (world.m[1][1] * bone.m[2][2]) + (world.m[2][0] * bone.m[2][3]) + world.m[2][3];
    return out;
}

void DrawOutlineText(ImDrawList* drawList, ImVec2 pos, ImU32 color, const char* text) {
    ImU32 outline = IM_COL32(0, 0, 0, 255);
    drawList->AddText(ImVec2(pos.x + 1, pos.y + 1), outline, text);
    drawList->AddText(ImVec2(pos.x - 1, pos.y - 1), outline, text);
    drawList->AddText(pos, color, text);
}

Vector3A GetBoneFromIndex(uint64_t actor, uint64_t iEntity, int index) {
    uint64_t animationCore = driver->read<uint64_t>(actor + Offsets::AnimationCore);
    if (!animationCore) return Vector3A(0, 0, 0);
    uint64_t firstBone = driver->read<uint64_t>(animationCore + Offsets::BoneArray);
    if (!firstBone) return Vector3A(0, 0, 0);
    int boneCount = driver->read<int>(animationCore + 0x68);
    if (index < 0 || index >= boneCount) return Vector3A(0, 0, 0);

    uint64_t boneEntry = firstBone + 0xA0 * index;
    Matrix3x4 world = driver->read<Matrix3x4>(iEntity + Offsets::WorldMatrix);
    Matrix3x4 boneMatrix = driver->read<Matrix3x4>(boneEntry + 0x30);
    return TransformBonePosition(world, boneMatrix);
}

void DrawSkeleton(uint64_t actor, uint64_t iEntity, uint64_t camera, ImDrawList* drawList) {
    static BoneIndex::Bones bones;
    auto getScreenPos = [&](int index) -> Vector2A {
        Vector3A worldPos = GetBoneFromIndex(actor, iEntity, index);
        Vector2A screenPos;
        if (ProjectWorldToScreen(camera, worldPos, screenPos)) return screenPos;
        return Vector2A(-1, -1);
    };

    auto drawLine = [&](Vector2A a, Vector2A b) {
        if (a.X > 0 && b.X > 0) drawList->AddLine(ImVec2(a.X, a.Y), ImVec2(b.X, b.Y), ImGui::GetColorU32(*(ImVec4*)sdk::color_esp_bone), 1.5f);
    };

    Vector2A head = getScreenPos(bones.head);
    Vector2A neck = getScreenPos(bones.neck);
    Vector2A spine = getScreenPos(bones.spine_02);
    Vector2A hips = getScreenPos(bones.hips);
    Vector2A lSh = getScreenPos(bones.upperarm_l);
    Vector2A rSh = getScreenPos(bones.upperarm_r);
    Vector2A lElb = getScreenPos(bones.lowerarm_l);
    Vector2A rElb = getScreenPos(bones.lowerarm_r);
    Vector2A lWr = getScreenPos(bones.hand_l);
    Vector2A rWr = getScreenPos(bones.hand_r);
    Vector2A lTh = getScreenPos(bones.thigh_l);
    Vector2A rTh = getScreenPos(bones.thigh_r);
    Vector2A lKn = getScreenPos(bones.calf_l);
    Vector2A rKn = getScreenPos(bones.calf_r);
    Vector2A lAn = getScreenPos(bones.foot_l);
    Vector2A rAn = getScreenPos(bones.foot_r);

    drawLine(head, neck); drawLine(neck, spine); drawLine(spine, hips);
    drawLine(neck, lSh); drawLine(lSh, lElb); drawLine(lElb, lWr);
    drawLine(neck, rSh); drawLine(rSh, rElb); drawLine(rElb, rWr);
    drawLine(hips, lTh); drawLine(lTh, lKn); drawLine(lKn, lAn);
    drawLine(hips, rTh); drawLine(rTh, rKn); drawLine(rKn, rAn);
}

void Touch_Down(int slot, int x, int y);
void Touch_Move(int slot, int x, int y);
void Touch_Up(int slot);

struct AimStruct {
    Vector3A ObjAim;
    float ScreenDistance;
    float WodDistance;
};
AimStruct Aim[100];

int findminat() {
    std::lock_guard<std::mutex> lock(mtx);
    float min = sdk::aimbot_fov;
    int minAt = -1;
    float DistanceMin = 10000.0f;
    for (int i = 0; i < MaxPlayerCount; i++) {
        if (i >= 100) break;
        if (Aim[i].ScreenDistance > sdk::aimbot_fov) continue;
        if (sdk::aimbot_mode == 0) {
            if (Aim[i].ScreenDistance < min) { min = Aim[i].ScreenDistance; minAt = i; }
        } else {
            if (Aim[i].WodDistance < DistanceMin) { DistanceMin = Aim[i].WodDistance; minAt = i; }
        }
    }
    Gmin = minAt;
    return minAt;
}

void AimBotAuto() {
    float tx = 0, ty = 0;
    bool isDown = false;


    float xzb = 650.0f;
    float yzb = 1500.0f;

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (!sdk::aimbot_enabled || !g_isFiring || MessiahBase == 0) {
            if (isDown) { Touch_Up(6); isDown = false; }
            continue;
        }

        int targetIndex = findminat();
        if (targetIndex == -1) {
            if (isDown) { Touch_Up(6); isDown = false; }
            continue;
        }

        uint64_t clientEngine = driver->read<uint64_t>(MessiahBase + Offsets::GClientEngine);
        uint64_t gameplay = clientEngine ? driver->read<uint64_t>(clientEngine + Offsets::IGamePlay) : 0;
        uint64_t clientPlayer = gameplay ? driver->read<uint64_t>(gameplay + Offsets::IClientPlayer) : 0;
        uint64_t camera = clientPlayer ? driver->read<uint64_t>(clientPlayer + Offsets::ICamera) : 0;
        if (!camera) continue;

        Vector3A targetPos;
        {
            std::lock_guard<std::mutex> lock(mtx);
            targetPos = Aim[targetIndex].ObjAim;
        }

        Vector2A screenTarget;
        if (ProjectWorldToScreen(camera, targetPos, screenTarget)) {

            float ScreenX = (screen_x < screen_y) ? (float)screen_x : (float)screen_y;
            float ScreenY = (screen_x > screen_y) ? (float)screen_x : (float)screen_y;

            float ScrXH = ScreenX / 2.0f;
            float ScrYH = ScreenY / 2.0f;


            float zm_y = screenTarget.X;
            float zm_x = ScreenX - screenTarget.Y;

            float smooth = sdk::aimbot_smooth > 1.0f ? sdk::aimbot_smooth : 5.0f;

            if (!isDown) {
                tx = xzb; ty = yzb;
                Touch_Down(6, (int)tx, (int)ty);
                isDown = true;
            }

            float TargetX = 0, TargetY = 0;
            if (zm_x > ScrXH) TargetX = (zm_x - ScrXH) / smooth;
            else if (zm_x < ScrXH) TargetX = (zm_x - ScrXH) / smooth;

            if (zm_y > ScrYH) TargetY = (zm_y - ScrYH) / smooth;
            else if (zm_y < ScrYH) TargetY = (zm_y - ScrYH) / smooth;

            float MaxStep = 7.0f;
            if (TargetX > MaxStep) TargetX = MaxStep;
            if (TargetX < -MaxStep) TargetX = -MaxStep;
            if (TargetY > MaxStep) TargetY = MaxStep;
            if (TargetY < -MaxStep) TargetY = -MaxStep;

            if (fabs(TargetX) < 0.1f) TargetX = 0;
            if (fabs(TargetY) < 0.1f) TargetY = 0;

            tx += TargetX;
            ty += TargetY;

            // Reset touch point to prevent "stuck" or "camera bug"
            if (tx >= xzb + 200 || tx <= xzb - 200 || ty >= yzb + 200 || ty <= yzb - 200) {
                Touch_Up(6);
                tx = xzb; ty = yzb;
                Touch_Down(6, (int)tx, (int)ty);
            }

            Touch_Move(6, (int)tx, (int)ty);
        } else {
            if (isDown) { Touch_Up(6); isDown = false; }
        }
    }
}

void DrawPlayer(ImDrawList* drawList) {
    if (g_LayoutCheckSum != ((uint64_t)(screen_x + screen_y) ^ oxorany(0xDEADBEEF))) {
        return;
    }
    if (pid <= 0 || MessiahBase == 0) return;
    g_hasTarget = false;
    if (!sdk::esp_enabled && !sdk::aimbot_enabled) {
        g_isFiring = false;
        return;
    }

    uint64_t clientEngine = driver->read<uint64_t>(MessiahBase + Offsets::GClientEngine);
    if (!clientEngine) return;
    uint64_t gameplay = driver->read<uint64_t>(clientEngine + Offsets::IGamePlay);
    uint64_t clientPlayer = driver->read<uint64_t>(gameplay + Offsets::IClientPlayer);
    uint64_t camera = driver->read<uint64_t>(clientPlayer + Offsets::ICamera);
    if (!camera) return;

    uint64_t localIEntity = driver->read<uint64_t>(clientPlayer + Offsets::LocalIEntity);
    Vector3A localPos(0, 0, 0);
    uint64_t localActor = 0;
    if (localIEntity) {
        localPos = driver->read<Vector3A>(localIEntity + Offsets::IEntityPosition);
        uint64_t pStruct = driver->read<uint64_t>(localIEntity + Offsets::PStruct);
        uint64_t localActorComp = pStruct ? driver->read<uint64_t>(pStruct + Offsets::LocalActorComponent) : 0;
        localActor = localActorComp ? driver->read<uint64_t>(localActorComp + Offsets::Actor) : 0;
    }

    // Trigger logic
    static bool is_trigger_active = false;
    ImVec2 mousePos = ImGui::GetIO().MousePos;
    float tdx = mousePos.x - sdk::fire_button_x;
    float tdy = mousePos.y - sdk::fire_button_y;
    if (ImGui::IsMouseDown(0)) {
        if (!is_trigger_active) {
            if (sqrtf(tdx*tdx + tdy*tdy) <= sdk::fire_button_radius) is_trigger_active = true;
        }
    } else { is_trigger_active = false; }

    g_isFiring = sdk::aimbot_enabled && (sdk::aimbot_always_on || (sdk::aimbot_use_trigger && is_trigger_active));

    if (sdk::aimbot_use_trigger && sdk::show_trigger_area) {
        drawList->AddCircle(ImVec2(sdk::fire_button_x, sdk::fire_button_y), sdk::fire_button_radius, GetColorU35(c::accent_color, 0.5f), 64, 3.0f);
    }

    if (sdk::aimbot_enabled && sdk::aimbot_draw_fov) {
        drawList->AddCircle(ImVec2(screen_x / 2, screen_y / 2), sdk::aimbot_fov, ImGui::GetColorU32(*(ImVec4*)sdk::color_aimbot_fov), 64, 1.0f);
    }

    uint64_t actorBase = driver->read<uint64_t>(MessiahBase + Offsets::ActorBase);
    if (!actorBase) return;
    uint64_t currentNodeAddress = driver->read<uint64_t>(actorBase + 0x10);
    std::vector<uint64_t> visitedNodes;
    visitedNodes.reserve(400);

    int localAimCount = 0;
    int enemyCount = 0;
    for (int i = 0; i < 400; i++) {
        if (!currentNodeAddress || std::find(visitedNodes.begin(), visitedNodes.end(), currentNodeAddress) != visitedNodes.end()) break;
        visitedNodes.push_back(currentNodeAddress);
        uint64_t nextAddress = driver->read<uint64_t>(currentNodeAddress);
        uint64_t actor = driver->read<uint64_t>(currentNodeAddress + 0x18);
        if (actor && actor != localActor) {
            int actorType = driver->read<int>(actor + 0x8);
            if (actorType == 3) {
                bool isInWorld = driver->read<bool>(actor + Offsets::InWorld);
                if (isInWorld) {
                    uint64_t actorProps = driver->read<uint64_t>(actor + Offsets::ActorProps);
                    uint64_t actorComp = actorProps ? driver->read<uint64_t>(actorProps + Offsets::ActorComponent) : 0;
                    uint64_t iEntity = actorComp ? driver->read<uint64_t>(actorComp + Offsets::IEntity) : 0;
                    if (iEntity) {
                        Vector3A rootPos = driver->read<Vector3A>(iEntity + Offsets::IEntityPosition);
                        float distance = get_3D_Distance(rootPos.X, rootPos.Y, rootPos.Z, localPos.X, localPos.Y, localPos.Z);
                        if (distance > 0.5f && distance < 1000.0f) {
                            enemyCount++;
                            int targetBone = 7;
                            if (sdk::aimbot_target == 1) targetBone = 6;
                            else if (sdk::aimbot_target == 2) targetBone = 5;
                            Vector3A headPos = GetBoneFromIndex(actor, iEntity, targetBone);
                            Vector2A screenRoot, screenHead;
                            if (ProjectWorldToScreen(camera, rootPos, screenRoot) && ProjectWorldToScreen(camera, headPos, screenHead)) {
                                if (sdk::esp_enabled) {
                                    if (sdk::esp_box) {
                                        float h = fabsf(screenRoot.Y - screenHead.Y);
                                        if (h < 5.0f) h = 15.0f;
                                        float w = h * 0.6f;
                                        drawList->AddRect(ImVec2(screenHead.X - w/2, screenHead.Y), ImVec2(screenHead.X + w/2, screenRoot.Y), ImGui::GetColorU32(*(ImVec4*)sdk::color_esp_box), 0, 0, 1.5f);
                                    }
                                    if (sdk::esp_line) {
                                        float start_y = (sdk::esp_line_pos == 0) ? screen_y : (sdk::esp_line_pos == 1 ? screen_y / 2 : 0);
                                        drawList->AddLine(ImVec2(screen_x / 2, start_y), ImVec2(screenRoot.X, screenRoot.Y), ImGui::GetColorU32(*(ImVec4*)sdk::color_esp_line));
                                    }
                                    if (sdk::esp_distance) {
                                        char distStr[32]; snprintf(distStr, sizeof(distStr), "[%dm]", (int)(distance / 5.0f));
                                        DrawOutlineText(drawList, ImVec2(screenRoot.X - 15, screenRoot.Y + 5), IM_COL32(255, 255, 0, 255), distStr);
                                    }
                                    if (sdk::esp_bone) DrawSkeleton(actor, iEntity, camera, drawList);
                                }
                                if (sdk::aimbot_enabled) {
                                    float sdx = screenHead.X - (screen_x / 2);
                                    float sdy = screenHead.Y - (screen_y / 2);
                                    float crossDist = sqrtf(sdx*sdx + sdy*sdy);
                                    if (crossDist <= sdk::aimbot_fov * 2.0f && localAimCount < 100) {
                                        std::lock_guard<std::mutex> lock(mtx);
                                        Aim[localAimCount].ObjAim = headPos;
                                        Aim[localAimCount].ScreenDistance = crossDist;
                                        Aim[localAimCount].WodDistance = distance;
                                        localAimCount++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        currentNodeAddress = nextAddress;
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        AimCount = localAimCount;
        MaxPlayerCount = localAimCount;
    }
    findminat();
    int currentTarget = Gmin;
    if (sdk::aimbot_enabled && sdk::aimbot_draw_line && currentTarget >= 0 && currentTarget < MaxPlayerCount) {
        Vector2A screenTarget;
        if (ProjectWorldToScreen(camera, Aim[currentTarget].ObjAim, screenTarget)) {
            drawList->AddLine(ImVec2(screen_x / 2, screen_y / 2), ImVec2(screenTarget.X, screenTarget.Y), ImGui::GetColorU32(*(ImVec4*)sdk::color_aimbot_line), 1.0f);
        }
    }

    if (enemyCount > 0) {
        char countStr[64];
        snprintf(countStr, sizeof(countStr), "ENEMIES AROUND: %d", enemyCount);
        ImVec2 textSize = ImGui::CalcTextSize(countStr);
        DrawOutlineText(drawList, ImVec2((screen_x - textSize.x) / 2, 50), IM_COL32(255, 255, 255, 255), countStr);
    }
}

void DrawGradientBar(float x, float y, float w, float h)
{
    ImDrawList* draw = ImGui::GetForegroundDrawList();
    ImU32 colLeft  = IM_COL32(0, 0, 0, 180);
    ImU32 colRight = IM_COL32(0, 180, 255, 180);

    draw->AddRectFilledMultiColor(
            ImVec2(x, y),
            ImVec2(x + w, y + h),
            colLeft,
            colRight,
            colRight,
            colLeft
    );
}

template <typename T>
inline T Read(uintptr_t address) {
    return driver->read<T>(address);
}

ImFont *flamee = nullptr;
ImFont *Japan = nullptr;
ImFont *font1 = nullptr;
ImFont *font2 = nullptr;
ImFont *ubu_0 = nullptr;
ImFont *pubg_font = nullptr;

static bool 已挂载 = false;
bool kang;

static bool 菜单 = true;
static bool IsBall = true;
bool isSetSize = false;
int 数量;
bool screenshotScheduled = false;
double screenshotStartTime = 0.0;
double tooltipStartTime = 0.0;
bool showOverlay = false;

void takeScreenshot() {
    std::random_device rd;
    std::uniform_int_distribution<int> dist(100000, 999999);
    int random_number = dist(rd);
    std::string file_name = "game_" + std::to_string(random_number) + ".png";
    std::thread([file_name]() {
        std::string command = "screencap -p /sdcard/" + file_name;
        system(command.c_str());
    }).detach();
}

void ImGuiMenustyle();
void NumIoLoad(const char *name);

ANativeWindow *native_window;
android::ANativeWindowCreator::DisplayInfo displayInfo{0};
int FPS;
int 帧数 = 60;
timer FPS限制;
float FPF显示;
Screen full_screen;
int Orientation = 0;
int screen_x = 0, screen_y = 0;
int init_screen_x = 0, init_screen_y = 0;
bool g_Initialized = false;

string exec(string command) {
    char buffer[128];
    string result = "";
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return "popen failed!";
    while (fgets(buffer, 128, pipe) != nullptr) result += buffer;
    pclose(pipe);
    return result;
}

int init_Vulkan(int _screen_x, int _screen_y, bool log) {
    InitVulkan();
    SetupVulkan();
    ::native_window = android::ANativeWindowCreator::Create("AImGui", _screen_x, _screen_y, false);
    SetupVulkanWindow(::native_window, (int) _screen_x, (int) _screen_y);
    return 1;
}

void ImGui_init() {
    if (g_Initialized) return;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;
    ImGui_ImplAndroid_Init(native_window);
    static const ImWchar icons_ranges[] = {0xf000, 0xf3ff, 0};
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = 2.5;
    icons_config.OversampleV = 2.5;
    ImFontConfig cfg;
    cfg.FontBuilderFlags = 0;
    ImFontConfig font_config;

    io.Fonts->AddFontFromMemoryTTF((void *)SabFont, (int)SabFont_size, 30.f, &font_config, io.Fonts->GetGlyphRangesDefault());
    io.Fonts->AddFontFromMemoryTTF(fa_solid_900_ttf, fa_solid_900_ttf_len, 24.0f, &icons_config, icons_ranges);
    flamee = io.Fonts->AddFontFromMemoryTTF((void *)Yaser_data, Mod_size, 28.0f, &icons_config, io.Fonts->GetGlyphRangesCyrillic());
    Japan = io.Fonts->AddFontFromMemoryTTF((void *)Darmen_data, Darmen_size, 28.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
    ubu_0 = io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char *>(ubuntu_0), ubuntu_0_size, 28, NULL, io.Fonts->GetGlyphRangesCyrillic());
    pubg_font = io.Fonts->AddFontFromMemoryTTF((void *)default_f, sizeof default_f, 28, NULL, io.Fonts->GetGlyphRangesCyrillic());
    font::lexend_general_bold = io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char *>(lexend_bold), lexend_bold_size, 28.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::lexend_bold = io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char *>(lexend_regular), lexend_regular_size, 28.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::lexend_regular = io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char *>(lexend_regular), lexend_regular_size, 28.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::icomoon = io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char *>(icomoon), icomoon_size, 30.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::icomoon_tabs = io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char *>(icomoon), icomoon_size, 35.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::icomoon_widget = io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char *>(icomoon_widget), icomoon_widget_size, 35.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::icomoon_widget2 = io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char *>(icomoon), icomoon_size, 35.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    ImGui::GetStyle().ScaleAllSizes(2.4f);

    UploadFonts();
    颜色初始化();

    FloatBall = ImAgeHeadFile_Vk(icon, sizeof(icon));


    ::g_Initialized = true;
}

void drawEnd() {
    ImGui::Render();
    FrameRender(ImGui::GetDrawData());
    FramePresent();
}

void shutdown() {
    if (!g_Initialized) return;
    DeviceWait();
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();
    CleanupVulkanWindow();
    CleanupVulkan();
    android::ANativeWindowCreator::Destroy(native_window);
    ::g_Initialized = false;
}

int DrawInt() {
    pid = driver->GetPid((char*)"com.netease.newspike");
    driver->init_pid(pid);
    memset(matrix, 0, 64);
    if (screen_x > screen_y) { py = screen_y/2.0f; px = screen_x/2.0f; }
    else { py = screen_x/2.0f; px = screen_y/2.0f; }
    MessiahBase = driver->getModuleBase((char*)"libGame.so");
    return 0;
}

void screen_config(){
    std::string window_size = exec("wm size");
    sscanf(window_size.c_str(),"Physical size: %dx%d",&screen_x, &screen_y);
    full_screen.ScreenX = screen_x;
    full_screen.ScreenY = screen_y;

    std::thread *orithread = new std::thread([&] {
        while(true){
            Orientation = atoi(exec("dumpsys display | grep 'mCurrentOrientation' | cut -d'=' -f2").c_str());
            if(Orientation == 0 || Orientation == 2){ screen_x = full_screen.ScreenX; screen_y = full_screen.ScreenY; }
            if(Orientation == 1 || Orientation == 3){ screen_x = full_screen.ScreenY; screen_y = full_screen.ScreenX; }
            if (pid <= 0 || MessiahBase <= 0) DrawInt();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    });
    orithread->detach();
}

int 数据() {
    DIR *dir = opendir("/dev/input/");
    if (dir == NULL) return -1;
    struct dirent *ptr = NULL;
    int count = 0;
    while ((ptr = readdir(dir)) != NULL) { if (strstr(ptr->d_name, "event")) count++; }
    closedir(dir);
    return count ? count : -1;
}

void 处理输入事件(struct input_event ev) {
    if (ev.type == EV_KEY && ev.value == 1) {
        if (ev.code == KEY_VOLUMEUP) { IsBall = true; MemuSwitch = true; }
        else if (ev.code == KEY_VOLUMEDOWN) { IsBall = false; }
    }
}

int 音量() {
    int EventCount = 数据();
    if (EventCount < 0) return -1;
    int *fdArray = (int *)malloc(EventCount * sizeof(int));
    fd_set fds;
    struct timeval tv;
    int maxfd = 0;
    for (int i = 0; i < EventCount; i++) {
        char temp[128]; sprintf(temp, "/dev/input/event%d", i);
        fdArray[i] = open(temp, O_RDONLY | O_NONBLOCK);
        if(fdArray[i] > maxfd) maxfd = fdArray[i];
    }
    struct input_event ev;
    while (1) {
        FD_ZERO(&fds);
        for (int i = 0; i < EventCount; i++) FD_SET(fdArray[i], &fds);
        tv.tv_sec = 5; tv.tv_usec = 0;
        int ret = select(maxfd + 1, &fds, NULL, NULL, &tv);
        if (ret == -1) break;
        else if (ret == 0) continue;
        else {
            for (int i = 0; i < EventCount; i++) {
                if (FD_ISSET(fdArray[i], &fds)) {
                    memset(&ev, 0, sizeof(ev));
                    if (read(fdArray[i], &ev, sizeof(ev)) == sizeof(ev)) 处理输入事件(ev);
                }
                usleep(5000);
            }
        }
        usleep(5000);
    }
    for (int i = 0; i < EventCount; i++) if (fdArray[i] >= 0) close(fdArray[i]);
    free(fdArray);
    return 0;
}


void DrawLogo(ImVec2 center, float size)
{
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
    draw_list->AddImage(FloatBall, ImVec2(center.x - size, center.y - size), ImVec2(center.x + size, center.y + size));
}

bool Tabb,interface3,isTe;


Vector2A WorldToScreenLocal(Vector3A pos, float matrix[16]) {
    float viewW = matrix[3] * pos.X + matrix[7] * pos.Y + matrix[11] * pos.Z + matrix[15];
    if (viewW < 0.01f) return {0, 0};

    float x = px + (matrix[0] * pos.X + matrix[4] * pos.Y + matrix[8] * pos.Z + matrix[12]) / viewW * px;
    float y = py - (matrix[1] * pos.X + matrix[5] * pos.Y + matrix[9] * pos.Z + matrix[13]) / viewW * py;

    return {x, y};
}

extern void DecryptPOS(ImDrawList* dl, float sw, float sh);

void tick() {

    DecryptPOS(ImGui::GetBackgroundDrawList(), (float)screen_x, (float)screen_y);
    DrawPlayer(ImGui::GetForegroundDrawList());

    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle &style = ImGui::GetStyle();


    style.WindowPadding = ImVec2(0, 0);
    style.ItemSpacing = ImVec2(20, 15);
    style.WindowBorderSize = 0;
    style.ChildRounding = 10.f;
    style.FrameRounding = 8.f;

    static bool is_menu_open = false;
    static float menu_anim = 0.f;
    menu_anim = ImLerp(menu_anim, is_menu_open ? 1.f : 0.f, io.DeltaTime * 10.f);

    static ImVec2 logo_pos = ImVec2(83, 93);
    static bool dragging = false;
    static ImVec2 drag_offset;
    float size = 80.0f;
    DrawLogo(logo_pos, size);
    ImVec2 min = ImVec2(logo_pos.x - size, logo_pos.y - size);
    ImVec2 max = ImVec2(logo_pos.x + size, logo_pos.y + size);
    ImVec2 mouse = ImGui::GetMousePos();

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        if (mouse.x >= min.x && mouse.x <= max.x && mouse.y >= min.y && mouse.y <= max.y) {
            dragging = true;
            drag_offset = ImVec2(mouse.x - logo_pos.x, mouse.y - logo_pos.y);
        }
    }
    if (dragging) {
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) { logo_pos = ImVec2(mouse.x - drag_offset.x, mouse.y - drag_offset.y); }
        else {
            dragging = false;
            if (mouse.x >= min.x && mouse.x <= max.x && mouse.y >= min.y && mouse.y <= max.y) is_menu_open = !is_menu_open;
        }
    }

    if (menu_anim < 0.01f && !is_menu_open) return;

    static int page = 0;
    static int active_tab = 0;
    static float tab_alpha = 0.f;

    if (is_menu_open || menu_anim > 0.01f) {
        ImGui::SetNextWindowSize(c::bg::size);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, menu_anim);
        ImGui::Begin("STARCOOL KERNEL", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        ImVec2 pos = ImGui::GetWindowPos();
        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Background
        draw->AddRectFilled(pos, pos + c::bg::size, GetColorU35(c::bg::background), c::bg::rounding);
        // Sidebar background
        draw->AddRectFilled(pos, pos + ImVec2(130, c::bg::size.y), GetColorU35(c::bg::border, 0.5f), c::bg::rounding, ImDrawFlags_RoundCornersLeft);

        // Logo in sidebar
        ImGui::SetCursorPos(ImVec2(25, 25));
        ImGui::Image(FloatBall, ImVec2(80, 80));

        // Separator
        draw->AddLine(pos + ImVec2(0, 130), pos + ImVec2(130, 130), GetColorU35(ImVec4(1,1,1,0.1f)));

        // Tabs
        ImGui::SetCursorPos(ImVec2(15, 150));
        ImGui::BeginGroup();
        if (e_elements::Tabis(page == 0, ImGui::GetID("VISUAL"), "b", ImVec2(100, 80))) page = 0;
        ImGui::Spacing();
        if (e_elements::Tabis(page == 1, ImGui::GetID("AIMBOT"), "c", ImVec2(100, 80))) page = 1;
        ImGui::Spacing();
        if (e_elements::Tabis(page == 2, ImGui::GetID("INFO"), "f", ImVec2(100, 80))) page = 2;
        ImGui::EndGroup();

        // Container
        ImGui::SetCursorPos(ImVec2(150, 30));

        // Tab switching animation logic
        if (active_tab != page) {
            tab_alpha = ImLerp(tab_alpha, 0.f, io.DeltaTime * 15.f);
            if (tab_alpha < 0.05f) active_tab = page;
        } else {
            tab_alpha = ImLerp(tab_alpha, 1.f, io.DeltaTime * 15.f);
        }

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * menu_anim);
        ImGui::BeginChild("##container", ImVec2(c::bg::size.x - 180, c::bg::size.y - 60), false, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        float child_w = (ImGui::GetContentRegionAvail().x - 20) / 2;

        if (active_tab == 0) {
            ImGui::BeginGroup();
            e_elements::BeginChilds("Player ESP", ImVec2(child_w, 530));
            ImGui::PushStyleColor(ImGuiCol_Text, c::text::text_active);
            e_elements::Checkbox("Enable ESP", &sdk::esp_enabled);
            e_elements::Checkbox("Box", &sdk::esp_box);
            e_elements::Checkbox("Line", &sdk::esp_line);
            e_elements::Checkbox("Distance", &sdk::esp_distance);
            e_elements::Checkbox("Skeleton", &sdk::esp_bone);


            ImGui::PopStyleColor();
            e_elements::EndChilds();
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            e_elements::BeginChilds("ESP Colors", ImVec2(child_w, 530));
            ImGui::PushStyleColor(ImGuiCol_Text, c::text::text_active);
            ImGui::ColorEdit4("Box Color", sdk::color_esp_box, ImGuiColorEditFlags_NoInputs);
            ImGui::ColorEdit4("Line Color", sdk::color_esp_line, ImGuiColorEditFlags_NoInputs);
            ImGui::ColorEdit4("Bone Color", sdk::color_esp_bone, ImGuiColorEditFlags_NoInputs);
            ImGui::Spacing();
            ImGui::Text("Line Position:");
            if (e_elements::RadioButton("Top", sdk::esp_line_pos == 2)) sdk::esp_line_pos = 2;
            ImGui::SameLine();
            if (e_elements::RadioButton("Center", sdk::esp_line_pos == 1)) sdk::esp_line_pos = 1;
            ImGui::SameLine();
            if (e_elements::RadioButton("Bot", sdk::esp_line_pos == 0)) sdk::esp_line_pos = 0;

            ImGui::PopStyleColor();
            e_elements::EndChilds();
            ImGui::EndGroup();
        }
        else if (active_tab == 1) {
            ImGui::BeginGroup();
            e_elements::BeginChilds("Aim Toush", ImVec2(child_w, 530));
            ImGui::PushStyleColor(ImGuiCol_Text, c::text::text_active);
            e_elements::Checkbox("Enable Aimbot", &sdk::aimbot_enabled);
            e_elements::Checkbox("Always On", &sdk::aimbot_always_on);
            e_elements::Checkbox("Right Fire Trigger", &sdk::aimbot_use_trigger);
            e_elements::Checkbox("Draw FOV", &sdk::aimbot_draw_fov);
            e_elements::Checkbox("Draw Aimline", &sdk::aimbot_draw_line);

            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
            ImGui::Text("Priority:");

            if (e_elements::RadioButton("Crosshair", sdk::aimbot_mode == 0)) sdk::aimbot_mode = 0;
            ImGui::SameLine();
            if (e_elements::RadioButton("Distance", sdk::aimbot_mode == 1)) sdk::aimbot_mode = 1;
            ImGui::PopStyleColor();
            e_elements::EndChilds();
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            e_elements::BeginChilds("Settings", ImVec2(child_w, 530));
            ImGui::PushStyleColor(ImGuiCol_Text, c::text::text_active);
            e_elements::SliderFloat("FOV", &sdk::aimbot_fov, 10.0f, 800.0f);
            e_elements::SliderFloat("Smoothnes", &sdk::aimbot_smooth, 1.0f, 30.0f);
            e_elements::Checkbox("Show Fire Zone", &sdk::show_trigger_area);
            e_elements::SliderFloat("Trigger Radius", &sdk::fire_button_radius, 10.0f, 500.0f);
            e_elements::SliderFloat("Trigger X", &sdk::fire_button_x, 0.0f, 2500.0f);
            e_elements::SliderFloat("Trigger Y", &sdk::fire_button_y, 0.0f, 1500.0f);
            ImGui::PopStyleColor();
            e_elements::EndChilds();
            ImGui::EndGroup();
        }
        else if (active_tab == 2) {
            ImGui::BeginGroup();
            e_elements::BeginChilds("Information", ImVec2(child_w, 530));
            ImGui::PushStyleColor(ImGuiCol_Text, c::text::text_active);
            ImGui::Text("Device Res: %dx%d", screen_x, screen_y);
            ImGui::Text("Process PID: %d", pid);
            ImGui::Text("Aimbot Status: %s", g_isFiring ? "FIRING" : "IDLE");
            ImGui::Text("Current FPS: %.1f", FPF显示);
            ImGui::Spacing(); ImGui::Spacing();
            if (ImGui::Button("Exit Cheat", ImVec2(-1, 80))) exit(0);
            ImGui::PopStyleColor();
            e_elements::EndChilds();
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            e_elements::BeginChilds("System", ImVec2(child_w, 530));
            ImGui::PushStyleColor(ImGuiCol_Text, c::text::text_active);

            e_elements::SliderInt("Limit FPS", &帧数, 30, 120);
            FPS = 帧数; // Sync with main.cpp FPS variable
            ImGui::Spacing();


            ImGui::PopStyleColor();
            e_elements::EndChilds();
            ImGui::EndGroup();
        }

        ImGui::EndChild();
        ImGui::PopStyleVar();
        ImGui::End();
        ImGui::PopStyleVar();
    }
}
