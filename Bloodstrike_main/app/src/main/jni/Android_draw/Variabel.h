#include <string>
#include <iostream>
#include <string.h>
#include <cstdio>
#include <malloc.h>
#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <sys/system_properties.h>

void ImGuiLayout1();
void ImGuiLayout2();
void ImGuiLayout3();
void ImGuiLayout4();
void ImGuiLayout5();
int MenuTab = 1;
int leftShoulder;
int rightShoulder;
int leftElbow;
int rightElbow;
int leftWrist;
int rightWrist;
int leftThigh;
int rightThigh;
int leftKnee;
int rightKnee;
int leftAnkle;
int rightAnkle;
char *BoxName;
bool MemuSwitch = true;
bool BallSwitch = true;
ImGuiWindow *Window = nullptr;
void DrawLogo(ImVec2 center, float size);
bool IsLoGin = true, IsDown = false, IsWin = true;
ImVec2 ImagePos = {0, 0};
#ifndef 变量全局_H
#define 变量全局_H
#include <string>
extern char 全局变量[100];
#endif
ImVec2 aimsuspensionPos;// NOLINT

bool isSetAim = false;
bool isAimDown = false;

ImTextureID FloatBall;
ImTextureID offButton;
ImTextureID onButton;

static bool driverConnected;

int MyWeapon;//自身武器

//射线
enum LINE {
line1 = 0,
line2 = 1,
line3 = 2
};
static LINE LINE;
//方框
enum BOX {
box1 = 0,
box2 = 1,
box3 = 2
};
static BOX BOX;
//自瞄部位
enum AimTriggerMode {
Fire = 0,
ADS = 1,
FireAndADS = 2,
Always = 3
};

/*enum 自瞄瞄准部位 {
头部 = 1,
胸部 = 2,
盆骨 = 3
};*/





struct 颜色数据 {
ImVec4 设计颜色;
};
颜色数据 颜色[100];
ImVec4 设计颜色[100];
int 拼接(int a,int b) {
int t=9;
while(t<b)t=t*10+9;
t=(t+1)*a+b;
return t;
}
ImVec4 随机(){
int 红;
int 绿;
int 蓝;
红=(rand()%255+0);
绿=(rand()%255+0);
蓝=(rand()%255+0);
if(sizeof(红)==1||sizeof(红)==0)
红=拼接(红,红);
if(sizeof(绿)==1||sizeof(绿)==0)
绿=拼接(绿,绿);
if(sizeof(蓝)==1||sizeof(蓝)==0)
蓝=拼接(蓝,蓝);
return {(float)红/255.0f,(float)绿/255.0f,(float)蓝/255.0f,174/255.0f};
}
void 颜色初始化(){
srand((unsigned)time(NULL)); /*随机数*/
for(int 数量=0;数量<100;数量++){
设计颜色[数量]=随机();
}
}
ImVec4 获取颜色(int 序号){
return 设计颜色[序号-1];
}
ImVec4 队伍颜色(int 队伍){
if(队伍<=100 && 队伍>0)
return 获取颜色(队伍);
else
return {0/255.f,255/255.f,0/255.f,50/255.f};
}
std::string tou(int 头) {
    std::string StateName = "Unknown";
    if (头 == 0) {
        StateName = "Bare Head"; 
    }
    if (头 == 502001 || 头 == 502004 || 头 == 502101) {
        StateName = "One Head"; 
    }
    if (头 == 502002 || 头 == 502005 || 头 == 502102) {
        StateName = "Two Head"; 
    }
    if (头 == 502003 || 头 == 502006 || 头 == 502103) {
        StateName = "Three Head"; 
    }
    if (头 == 502104 || 头 == 502107 || 头 == 502110) {
        StateName = "Four Head"; 
    }
    if (头 == 502105 || 头 == 502108 || 头 == 502111) {
        StateName = "Five Head"; 
    }
    if (头 == 502106 || 头 == 502109 || 头 == 502112) {
        StateName = "Six Head"; 
    }
    if (头 == 9804017 || 头 == 9804018 || 头 == 9804019) {
        StateName = "Seven Head"; 
    }
    return StateName;
}

std::string jia(int 甲) {
    std::string StateName = "Unknown";
    if (甲 == 0) {
        StateName = "Bare Armor"; 
    }
    if (甲 == 503001 || 甲 == 503004 || 甲 == 503101) {
        StateName = "One Armor"; 
    }
    if (甲 == 503002 || 甲 == 503005 || 甲 == 503102) {
        StateName = "Two Armor"; 
    }
    if (甲 == 503003 || 甲 == 503006 || 甲 == 503103) {
        StateName = "Three Armor"; 
    }
    if (甲 == 503104 || 甲 == 503107 || 甲 == 503110) {
        StateName = "Four Armor"; 
    }
    if (甲 == 503105 || 甲 == 503108 || 甲 == 503111) {
        StateName = "Five Armor"; 
    }
    if (甲 == 503106 || 甲 == 503109 || 甲 == 503112) {
        StateName = "Six Armor"; 
    }
    return StateName;
}

std::string bao(int 包) {
    std::string StateName = "Unknown";
    if (包 == 0) {
        StateName = "Bare Bag"; 
    }
    if (包 == 501001 || 包 == 501004 || 包 == 501101) {
        StateName = "One Bag"; 
    }
    if (包 == 501002 || 包 == 501005 || 包 == 501102) {
        StateName = "Two Bag"; 
    }
    if (包 == 501003 || 包 == 501006 || 包 == 501103) {
        StateName = "Three Bag"; 
    }
    if (包 == 501104 || 包 == 501107 || 包 == 501110) {
        StateName = "Four Bag"; 
    }
    if (包 == 501105 || 包 == 501108 || 包 == 501111) {
        StateName = "Five Bag"; 
    }
    if (包 == 501106 || 包 == 501109 || 包 == 501112) {
        StateName = "Six Bag"; 
    }
    return StateName;
}


bool 箱子判断(char *gname, char **name) {
	
	 
    if (strstr(gname, "BP_DC_HighLevel_TE_ItemBox_Lua_Lv1_C") ||strstr(gname, "BP_DC_HighLevel_TE_ItemBox_Lua_Lv1_2_C ")||strstr(gname, "BP_DC_TE_ItemBox_Lua_Lv1_C")||strstr(gname, "BP_DC_TE_ItemBox_Lua_Lv1_2_C")!= nullptr) 
    {
        *name = "Low Supply Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TE_ItemBox_Lua_Lv2_C") ||strstr(gname, "BP_DC_HighLevel_TE_ItemBox_Lua_Lv2_2_C ")||strstr(gname, "BP_DC_TE_ItemBox_Lua_Lv2_C")||strstr(gname, "BP_DC_TE_ItemBox_Lua_Lv2_2_C")!= nullptr)    
    {
        *name = "Medium Supply Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TE_ItemBox_Lua_Lv3_C") ||strstr(gname, "BP_DC_HighLevel_TE_ItemBox_Lua_Lv3_2_C ")||strstr(gname, "BP_DC_TE_ItemBox_Lua_Lv3_C")||strstr(gname, "BP_DC_TE_ItemBox_Lua_Lv3_2_C")!= nullptr)    {
        *name = "High Supply Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TE_ItemBox_Lua_Lv4_C") ||strstr(gname, "BP_DC_HighLevel_TE_ItemBox_Lua_Lv4_2_C ")||strstr(gname, "BP_DC_TE_ItemBox_Lua_Lv4_C")||strstr(gname, "BP_DC_TE_ItemBox_Lua_Lv4_2_C")!= nullptr)
    {
        *name = "Special Supply Box";
        return true;
    }
    
        if (strstr(gname, "BP_DC_HighLevel_TE_MedicineBox_Lua_Lv1_C") ||strstr(gname, "BP_DC_HighLevel_TE_MedicineBox_Lua_Lv1_2_C ")||strstr(gname, "BP_DC_TE_MedicineBox_Lua_Lv1_C")||strstr(gname, "BP_DC_TE_MedicineBox_Lua_Lv1_2_C")!= nullptr) 
    {
        *name = "Low Medkit";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TE_MedicineBox_Lua_Lv2_C") ||strstr(gname, "BP_DC_HighLevel_TE_MedicineBox_Lua_Lv2_2_C ")||strstr(gname, "BP_DC_TE_MedicineBox_Lua_Lv2_C")||strstr(gname, "BP_DC_TE_MedicineBox_Lua_Lv2_2_C")!= nullptr)    
    {
        *name = "Medium Medkit";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TE_MedicineBox_Lua_Lv3_C") ||strstr(gname, "BP_DC_HighLevel_TE_MedicineBox_Lua_Lv3_2_C ")||strstr(gname, "BP_DC_TE_MedicineBox_Lua_Lv3_C")||strstr(gname, "BP_DC_TE_MedicineBox_Lua_Lv3_2_C")!= nullptr)    {
        *name = "High Medkit";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TE_MedicineBox_Lua_Lv4_C") ||strstr(gname, "BP_DC_HighLevel_TE_MedicineBox_Lua_Lv4_2_C ")||strstr(gname, "BP_DC_TE_MedicineBox_Lua_Lv4_C")||strstr(gname, "BP_DC_TE_MedicineBox_Lua_Lv4_2_C")!= nullptr)
    {
        *name = "Special Medkit";
        return true;
    }

        if (strstr(gname, "BP_DC_HighLevel_TE_WeaponBox_Lua_Lv1_C") ||strstr(gname, "BP_DC_HighLevel_TE_WeaponBox_Lua_Lv1_2_C ")||strstr(gname, "BP_DC_TE_WeaponBox_Lua_Lv1_C")||strstr(gname, "BP_DC_TE_WeaponBox_Lua_Lv1_2_C")!= nullptr) 
    {
        *name = "Low Weapon Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TE_WeaponBox_Lua_Lv2_C") ||strstr(gname, "BP_DC_HighLevel_TE_WeaponBox_Lua_Lv2_2_C ")||strstr(gname, "BP_DC_TE_WeaponBox_Lua_Lv2_C")||strstr(gname, "BP_DC_TE_WeaponBox_Lua_Lv2_2_C")!= nullptr)    
    {
        *name = "Medium Weapon Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TE_WeaponBox_Lua_Lv3_C") ||strstr(gname, "BP_DC_HighLevel_TE_WeaponBox_Lua_Lv3_2_C ")||strstr(gname, "BP_DC_TE_WeaponBox_Lua_Lv3_C")||strstr(gname, "BP_DC_TE_WeaponBox_Lua_Lv3_2_C")!= nullptr)    {
        *name = "High Weapon Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TE_WeaponBox_Lua_Lv4_C") ||strstr(gname, "BP_DC_HighLevel_TE_WeaponBox_Lua_Lv4_2_C ")||strstr(gname, "BP_DC_TE_WeaponBox_Lua_Lv4_C")||strstr(gname, "BP_DC_TE_WeaponBox_Lua_Lv4_2_C")!= nullptr)
    {
        *name = "Special Weapon Box";
        return true;
    }

        if (strstr(gname, "BP_DC_HighLevel_TE_FileBag_Lua_Lv1_C") ||strstr(gname, "BP_DC_HighLevel_TE_FileBag_Lua_Lv1_2_C ")||strstr(gname, "BP_DC_TE_FileBag_Lua_Lv1_C")||strstr(gname, "BP_DC_TE_FileBag_Lua_Lv1_2_C")!= nullptr) 
    {
        *name = "Low Briefcase";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TE_FileBag_Lua_Lv2_C") ||strstr(gname, "BP_DC_HighLevel_TE_FileBag_Lua_Lv2_2_C ")||strstr(gname, "BP_DC_TE_FileBag_Lua_Lv2_C")||strstr(gname, "BP_DC_TE_FileBag_Lua_Lv2_2_C")!= nullptr)    
    {
        *name = "Medium Briefcase";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TE_FileBag_Lua_Lv3_C") ||strstr(gname, "BP_DC_HighLevel_TE_FileBag_Lua_Lv3_2_C ")||strstr(gname, "BP_DC_TE_FileBag_Lua_Lv3_C")||strstr(gname, "BP_DC_TE_FileBag_Lua_Lv3_2_C")!= nullptr)    {
        *name = "High Briefcase";
        return true;
    }
    
        if (strstr(gname, "BP_DC_HighLevel_TE_FileBox_Lua_Lv1_C") ||strstr(gname, "BP_DC_HighLevel_TE_FileBox_Lua_Lv1_2_C ")||strstr(gname, "BP_DC_TE_FileBox_Lua_Lv1_C")||strstr(gname, "BP_DC_TE_FileBox_Lua_Lv1_2_C")!= nullptr) 
    {
        *name = "Low Document Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TE_FileBox_Lua_Lv2_C") ||strstr(gname, "BP_DC_HighLevel_TE_FileBox_Lua_Lv2_2_C ")||strstr(gname, "BP_DC_TE_FileBox_Lua_Lv2_C")||strstr(gname, "BP_DC_TE_FileBox_Lua_Lv2_2_C")!= nullptr)    
    {
        *name = "Medium Document Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TE_FileBox_Lua_Lv3_C") ||strstr(gname, "BP_DC_HighLevel_TE_FileBox_Lua_Lv3_2_C ")||strstr(gname, "BP_DC_TE_FileBox_Lua_Lv3_C")||strstr(gname, "BP_DC_TE_FileBox_Lua_Lv3_2_C")!= nullptr)    {
        *name = "High Document Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TE_FileBox_Lua_Lv4_C") ||strstr(gname, "BP_DC_HighLevel_TE_FileBox_Lua_Lv4_2_C ")||strstr(gname, "BP_DC_TE_FileBox_Lua_Lv4_C")||strstr(gname, "BP_DC_TE_FileBox_Lua_Lv4_2_C")!= nullptr)
    {
        *name = "Special Document Box";
        return true;
    }
	    if (strstr(gname, "BP_DC_HighLevel_TD_ItemBox_Lua_Lv1_C") ||strstr(gname, "BP_DC_HighLevel_TD_ItemBox_Lua_Lv1_2_C ")||strstr(gname, "BP_DC_TD_ItemBox_Lua_Lv1_C")||strstr(gname, "BP_DC_TD_ItemBox_Lua_Lv1_2_C")!= nullptr) 
    {
        *name = "Low Supply Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TD_ItemBox_Lua_Lv2_C") ||strstr(gname, "BP_DC_HighLevel_TD_ItemBox_Lua_Lv2_2_C ")||strstr(gname, "BP_DC_TD_ItemBox_Lua_Lv2_C")||strstr(gname, "BP_DC_TD_ItemBox_Lua_Lv2_2_C")!= nullptr)    
    {
        *name = "Medium Supply Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TD_ItemBox_Lua_Lv3_C") ||strstr(gname, "BP_DC_HighLevel_TD_ItemBox_Lua_Lv3_2_C ")||strstr(gname, "BP_DC_TD_ItemBox_Lua_Lv3_C")||strstr(gname, "BP_DC_TD_ItemBox_Lua_Lv3_2_C")!= nullptr)    {
        *name = "High Supply Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TD_ItemBox_Lua_Lv4_C") ||strstr(gname, "BP_DC_HighLevel_TD_ItemBox_Lua_Lv4_2_C ")||strstr(gname, "BP_DC_TD_ItemBox_Lua_Lv4_C")||strstr(gname, "BP_DC_TD_ItemBox_Lua_Lv4_2_C")!= nullptr)
    {
        *name = "Special Supply Box";
        return true;
    }
    
        if (strstr(gname, "BP_DC_HighLevel_TD_MedicineBox_Lua_Lv1_C") ||strstr(gname, "BP_DC_HighLevel_TD_MedicineBox_Lua_Lv1_2_C ")||strstr(gname, "BP_DC_TD_MedicineBox_Lua_Lv1_C")||strstr(gname, "BP_DC_TD_MedicineBox_Lua_Lv1_2_C")!= nullptr) 
    {
        *name = "Low Medkit";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TD_MedicineBox_Lua_Lv2_C") ||strstr(gname, "BP_DC_HighLevel_TD_MedicineBox_Lua_Lv2_2_C ")||strstr(gname, "BP_DC_TD_MedicineBox_Lua_Lv2_C")||strstr(gname, "BP_DC_TD_MedicineBox_Lua_Lv2_2_C")!= nullptr)    
    {
        *name = "Medium Medkit";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TD_MedicineBox_Lua_Lv3_C") ||strstr(gname, "BP_DC_HighLevel_TD_MedicineBox_Lua_Lv3_2_C ")||strstr(gname, "BP_DC_TD_MedicineBox_Lua_Lv3_C")||strstr(gname, "BP_DC_TD_MedicineBox_Lua_Lv3_2_C")!= nullptr)    {
        *name = "High Medkit";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TD_MedicineBox_Lua_Lv4_C") ||strstr(gname, "BP_DC_HighLevel_TD_MedicineBox_Lua_Lv4_2_C ")||strstr(gname, "BP_DC_TD_MedicineBox_Lua_Lv4_C")||strstr(gname, "BP_DC_TD_MedicineBox_Lua_Lv4_2_C")!= nullptr)
    {
        *name = "Special Medkit";
        return true;
    }

        if (strstr(gname, "BP_DC_HighLevel_TD_WeaponBox_Lua_Lv1_C") ||strstr(gname, "BP_DC_HighLevel_TD_WeaponBox_Lua_Lv1_2_C ")||strstr(gname, "BP_DC_TD_WeaponBox_Lua_Lv1_C")||strstr(gname, "BP_DC_TD_WeaponBox_Lua_Lv1_2_C")!= nullptr) 
    {
        *name = "Low Weapon Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TD_WeaponBox_Lua_Lv2_C") ||strstr(gname, "BP_DC_HighLevel_TD_WeaponBox_Lua_Lv2_2_C ")||strstr(gname, "BP_DC_TD_WeaponBox_Lua_Lv2_C")||strstr(gname, "BP_DC_TD_WeaponBox_Lua_Lv2_2_C")!= nullptr)    
    {
        *name = "Medium Weapon Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TD_WeaponBox_Lua_Lv3_C") ||strstr(gname, "BP_DC_HighLevel_TD_WeaponBox_Lua_Lv3_2_C ")||strstr(gname, "BP_DC_TD_WeaponBox_Lua_Lv3_C")||strstr(gname, "BP_DC_TD_WeaponBox_Lua_Lv3_2_C")!= nullptr)    {
        *name = "High Weapon Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TD_WeaponBox_Lua_Lv4_C") ||strstr(gname, "BP_DC_HighLevel_TD_WeaponBox_Lua_Lv4_2_C ")||strstr(gname, "BP_DC_TD_WeaponBox_Lua_Lv4_C")||strstr(gname, "BP_DC_TD_WeaponBox_Lua_Lv4_2_C")!= nullptr)
    {
        *name = "Special Weapon Box";
        return true;
    }

        if (strstr(gname, "BP_DC_HighLevel_TD_FileBag_Lua_Lv1_C") ||strstr(gname, "BP_DC_HighLevel_TD_FileBag_Lua_Lv1_2_C ")||strstr(gname, "BP_DC_TD_FileBag_Lua_Lv1_C")||strstr(gname, "BP_DC_TD_FileBag_Lua_Lv1_2_C")!= nullptr) 
    {
        *name = "Low Briefcase";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TD_FileBag_Lua_Lv2_C") ||strstr(gname, "BP_DC_HighLevel_TD_FileBag_Lua_Lv2_2_C ")||strstr(gname, "BP_DC_TD_FileBag_Lua_Lv2_C")||strstr(gname, "BP_DC_TD_FileBag_Lua_Lv2_2_C")!= nullptr)    
    {
        *name = "Medium Briefcase";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TD_FileBag_Lua_Lv3_C") ||strstr(gname, "BP_DC_HighLevel_TD_FileBag_Lua_Lv3_2_C ")||strstr(gname, "BP_DC_TD_FileBag_Lua_Lv3_C")||strstr(gname, "BP_DC_TD_FileBag_Lua_Lv3_2_C")!= nullptr)    {
        *name = "High Briefcase";
        return true;
    }
    
        if (strstr(gname, "BP_DC_HighLevel_TD_FileBox_Lua_Lv1_C") ||strstr(gname, "BP_DC_HighLevel_TD_FileBox_Lua_Lv1_2_C ")||strstr(gname, "BP_DC_TD_FileBox_Lua_Lv1_C")||strstr(gname, "BP_DC_TD_FileBox_Lua_Lv1_2_C")!= nullptr) 
    {
        *name = "Low Document Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TD_FileBox_Lua_Lv2_C") ||strstr(gname, "BP_DC_HighLevel_TD_FileBox_Lua_Lv2_2_C ")||strstr(gname, "BP_DC_TD_FileBox_Lua_Lv2_C")||strstr(gname, "BP_DC_TD_FileBox_Lua_Lv2_2_C")!= nullptr)    
    {
        *name = "Medium Document Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TD_FileBox_Lua_Lv3_C") ||strstr(gname, "BP_DC_HighLevel_TD_FileBox_Lua_Lv3_2_C ")||strstr(gname, "BP_DC_TD_FileBox_Lua_Lv3_C")||strstr(gname, "BP_DC_TD_FileBox_Lua_Lv3_2_C")!= nullptr)    {
        *name = "High Document Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TD_FileBox_Lua_Lv4_C") ||strstr(gname, "BP_DC_HighLevel_TD_FileBox_Lua_Lv4_2_C ")||strstr(gname, "BP_DC_TD_FileBox_Lua_Lv4_C")||strstr(gname, "BP_DC_TD_FileBox_Lua_Lv4_2_C")!= nullptr)
    {
        *name = "Special Document Box";
        return true;
    }
	    if (strstr(gname, "BP_DC_HighLevel_T_ItemBox_Lua_Lv1_C") ||strstr(gname, "BP_DC_HighLevel_T_ItemBox_Lua_Lv1_2_C ")||strstr(gname, "BP_DC_T_ItemBox_Lua_Lv1_C")||strstr(gname, "BP_DC_T_ItemBox_Lua_Lv1_2_C")!= nullptr) 
    {
        *name = "Low Supply Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_T_ItemBox_Lua_Lv2_C") ||strstr(gname, "BP_DC_HighLevel_T_ItemBox_Lua_Lv2_2_C ")||strstr(gname, "BP_DC_T_ItemBox_Lua_Lv2_C")||strstr(gname, "BP_DC_T_ItemBox_Lua_Lv2_2_C")!= nullptr)    
    {
        *name = "Medium Supply Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_T_ItemBox_Lua_Lv3_C") ||strstr(gname, "BP_DC_HighLevel_T_ItemBox_Lua_Lv3_2_C ")||strstr(gname, "BP_DC_T_ItemBox_Lua_Lv3_C")||strstr(gname, "BP_DC_T_ItemBox_Lua_Lv3_2_C")!= nullptr)    {
        *name = "High Supply Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_T_ItemBox_Lua_Lv4_C") ||strstr(gname, "BP_DC_HighLevel_T_ItemBox_Lua_Lv4_2_C ")||strstr(gname, "BP_DC_T_ItemBox_Lua_Lv4_C")||strstr(gname, "BP_DC_T_ItemBox_Lua_Lv4_2_C")!= nullptr)
    {
        *name = "Special Supply Box";
        return true;
    }
    
        if (strstr(gname, "BP_DC_HighLevel_T_MedicineBox_Lua_Lv1_C") ||strstr(gname, "BP_DC_HighLevel_T_MedicineBox_Lua_Lv1_2_C ")||strstr(gname, "BP_DC_T_MedicineBox_Lua_Lv1_C")||strstr(gname, "BP_DC_T_MedicineBox_Lua_Lv1_2_C")!= nullptr) 
    {
        *name = "Low Medkit";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_T_MedicineBox_Lua_Lv2_C") ||strstr(gname, "BP_DC_HighLevel_T_MedicineBox_Lua_Lv2_2_C ")||strstr(gname, "BP_DC_T_MedicineBox_Lua_Lv2_C")||strstr(gname, "BP_DC_T_MedicineBox_Lua_Lv2_2_C")!= nullptr)    
    {
        *name = "Medium Medkit";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_T_MedicineBox_Lua_Lv3_C") ||strstr(gname, "BP_DC_HighLevel_T_MedicineBox_Lua_Lv3_2_C ")||strstr(gname, "BP_DC_T_MedicineBox_Lua_Lv3_C")||strstr(gname, "BP_DC_T_MedicineBox_Lua_Lv3_2_C")!= nullptr)    {
        *name = "High Medkit";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_T_MedicineBox_Lua_Lv4_C") ||strstr(gname, "BP_DC_HighLevel_T_MedicineBox_Lua_Lv4_2_C ")||strstr(gname, "BP_DC_T_MedicineBox_Lua_Lv4_C")||strstr(gname, "BP_DC_T_MedicineBox_Lua_Lv4_2_C")!= nullptr)
    {
        *name = "Special Medkit";
        return true;
    }

        if (strstr(gname, "BP_DC_HighLevel_T_WeaponBox_Lua_Lv1_C") ||strstr(gname, "BP_DC_HighLevel_T_WeaponBox_Lua_Lv1_2_C ")||strstr(gname, "BP_DC_T_WeaponBox_Lua_Lv1_C")||strstr(gname, "BP_DC_T_WeaponBox_Lua_Lv1_2_C")!= nullptr) 
    {
        *name = "Low Weapon Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_T_WeaponBox_Lua_Lv2_C") ||strstr(gname, "BP_DC_HighLevel_T_WeaponBox_Lua_Lv2_2_C ")||strstr(gname, "BP_DC_T_WeaponBox_Lua_Lv2_C")||strstr(gname, "BP_DC_T_WeaponBox_Lua_Lv2_2_C")!= nullptr)    
    {
        *name = "Medium Weapon Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_T_WeaponBox_Lua_Lv3_C") ||strstr(gname, "BP_DC_HighLevel_T_WeaponBox_Lua_Lv3_2_C ")||strstr(gname, "BP_DC_T_WeaponBox_Lua_Lv3_C")||strstr(gname, "BP_DC_T_WeaponBox_Lua_Lv3_2_C")!= nullptr)    {
        *name = "High Weapon Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_T_WeaponBox_Lua_Lv4_C") ||strstr(gname, "BP_DC_HighLevel_T_WeaponBox_Lua_Lv4_2_C ")||strstr(gname, "BP_DC_T_WeaponBox_Lua_Lv4_C")||strstr(gname, "BP_DC_T_WeaponBox_Lua_Lv4_2_C")!= nullptr)
    {
        *name = "Special Weapon Box";
        return true;
    }

        if (strstr(gname, "BP_DC_HighLevel_T_FileBag_Lua_Lv1_C") ||strstr(gname, "BP_DC_HighLevel_T_FileBag_Lua_Lv1_2_C ")||strstr(gname, "BP_DC_T_FileBag_Lua_Lv1_C")||strstr(gname, "BP_DC_T_FileBag_Lua_Lv1_2_C")!= nullptr) 
    {
        *name = "Low Briefcase";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_T_FileBag_Lua_Lv2_C") ||strstr(gname, "BP_DC_HighLevel_T_FileBag_Lua_Lv2_2_C ")||strstr(gname, "BP_DC_T_FileBag_Lua_Lv2_C")||strstr(gname, "BP_DC_T_FileBag_Lua_Lv2_2_C")!= nullptr)    
    {
        *name = "Medium Briefcase";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_T_FileBag_Lua_Lv3_C") ||strstr(gname, "BP_DC_HighLevel_T_FileBag_Lua_Lv3_2_C ")||strstr(gname, "BP_DC_T_FileBag_Lua_Lv3_C")||strstr(gname, "BP_DC_T_FileBag_Lua_Lv3_2_C")!= nullptr)    {
        *name = "High Briefcase";
        return true;
    }
    
        if (strstr(gname, "BP_DC_HighLevel_T_FileBox_Lua_Lv1_C") ||strstr(gname, "BP_DC_HighLevel_T_FileBox_Lua_Lv1_2_C ")||strstr(gname, "BP_DC_T_FileBox_Lua_Lv1_C")||strstr(gname, "BP_DC_T_FileBox_Lua_Lv1_2_C")!= nullptr) 
    {
        *name = "Low Document Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_T_FileBox_Lua_Lv2_C") ||strstr(gname, "BP_DC_HighLevel_T_FileBox_Lua_Lv2_2_C ")||strstr(gname, "BP_DC_T_FileBox_Lua_Lv2_C")||strstr(gname, "BP_DC_T_FileBox_Lua_Lv2_2_C")!= nullptr)    
    {
        *name = "Medium Document Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_T_FileBox_Lua_Lv3_C") ||strstr(gname, "BP_DC_HighLevel_T_FileBox_Lua_Lv3_2_C ")||strstr(gname, "BP_DC_T_FileBox_Lua_Lv3_C")||strstr(gname, "BP_DC_T_FileBox_Lua_Lv3_2_C")!= nullptr)    {
        *name = "High Document Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_T_FileBox_Lua_Lv4_C") ||strstr(gname, "BP_DC_HighLevel_T_FileBox_Lua_Lv4_2_C ")||strstr(gname, "BP_DC_T_FileBox_Lua_Lv4_C")||strstr(gname, "BP_DC_T_FileBox_Lua_Lv4_2_C")!= nullptr)
    {
        *name = "Special Document Box";
        return true;
    }
	    if (strstr(gname, "BP_DC_HighLevel_P_ItemBox_Lua_Lv1_C") ||strstr(gname, "BP_DC_HighLevel_P_ItemBox_Lua_Lv1_2_C ")||strstr(gname, "BP_DC_P_ItemBox_Lua_Lv1_C")||strstr(gname, "BP_DC_P_ItemBox_Lua_Lv1_2_C")!= nullptr) 
    {
        *name = "Low Supply Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_P_ItemBox_Lua_Lv2_C") ||strstr(gname, "BP_DC_HighLevel_P_ItemBox_Lua_Lv2_2_C ")||strstr(gname, "BP_DC_P_ItemBox_Lua_Lv2_C")||strstr(gname, "BP_DC_P_ItemBox_Lua_Lv2_2_C")!= nullptr)    
    {
        *name = "Medium Supply Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_P_ItemBox_Lua_Lv3_C") ||strstr(gname, "BP_DC_HighLevel_P_ItemBox_Lua_Lv3_2_C ")||strstr(gname, "BP_DC_P_ItemBox_Lua_Lv3_C")||strstr(gname, "BP_DC_P_ItemBox_Lua_Lv3_2_C")!= nullptr)    {
        *name = "High Supply Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_P_ItemBox_Lua_Lv4_C") ||strstr(gname, "BP_DC_HighLevel_P_ItemBox_Lua_Lv4_2_C ")||strstr(gname, "BP_DC_P_ItemBox_Lua_Lv4_C")||strstr(gname, "BP_DC_P_ItemBox_Lua_Lv4_2_C")!= nullptr)
    {
        *name = "Special Supply Box";
        return true;
    }
    
        if (strstr(gname, "BP_DC_HighLevel_P_MedicineBox_Lua_Lv1_C") ||strstr(gname, "BP_DC_HighLevel_P_MedicineBox_Lua_Lv1_2_C ")||strstr(gname, "BP_DC_P_MedicineBox_Lua_Lv1_C")||strstr(gname, "BP_DC_P_MedicineBox_Lua_Lv1_2_C")!= nullptr) 
    {
        *name = "Low Medkit";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_P_MedicineBox_Lua_Lv2_C") ||strstr(gname, "BP_DC_HighLevel_P_MedicineBox_Lua_Lv2_2_C ")||strstr(gname, "BP_DC_P_MedicineBox_Lua_Lv2_C")||strstr(gname, "BP_DC_P_MedicineBox_Lua_Lv2_2_C")!= nullptr)    
    {
        *name = "Medium Medkit";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_P_MedicineBox_Lua_Lv3_C") ||strstr(gname, "BP_DC_HighLevel_P_MedicineBox_Lua_Lv3_2_C ")||strstr(gname, "BP_DC_P_MedicineBox_Lua_Lv3_C")||strstr(gname, "BP_DC_P_MedicineBox_Lua_Lv3_2_C")!= nullptr)    {
        *name = "High Medkit";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_P_MedicineBox_Lua_Lv4_C") ||strstr(gname, "BP_DC_HighLevel_P_MedicineBox_Lua_Lv4_2_C ")||strstr(gname, "BP_DC_P_MedicineBox_Lua_Lv4_C")||strstr(gname, "BP_DC_P_MedicineBox_Lua_Lv4_2_C")!= nullptr)
    {
        *name = "Special Medkit";
        return true;
    }

        if (strstr(gname, "BP_DC_HighLevel_P_WeaponBox_Lua_Lv1_C") ||strstr(gname, "BP_DC_HighLevel_P_WeaponBox_Lua_Lv1_2_C ")||strstr(gname, "BP_DC_P_WeaponBox_Lua_Lv1_C")||strstr(gname, "BP_DC_P_WeaponBox_Lua_Lv1_2_C")!= nullptr) 
    {
        *name = "Low Weapon Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_P_WeaponBox_Lua_Lv2_C") ||strstr(gname, "BP_DC_HighLevel_P_WeaponBox_Lua_Lv2_2_C ")||strstr(gname, "BP_DC_P_WeaponBox_Lua_Lv2_C")||strstr(gname, "BP_DC_P_WeaponBox_Lua_Lv2_2_C")!= nullptr)    
    {
        *name = "Medium Weapon Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_P_WeaponBox_Lua_Lv3_C") ||strstr(gname, "BP_DC_HighLevel_P_WeaponBox_Lua_Lv3_2_C ")||strstr(gname, "BP_DC_P_WeaponBox_Lua_Lv3_C")||strstr(gname, "BP_DC_P_WeaponBox_Lua_Lv3_2_C")!= nullptr)    {
        *name = "High Weapon Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_P_WeaponBox_Lua_Lv4_C") ||strstr(gname, "BP_DC_HighLevel_P_WeaponBox_Lua_Lv4_2_C ")||strstr(gname, "BP_DC_P_WeaponBox_Lua_Lv4_C")||strstr(gname, "BP_DC_P_WeaponBox_Lua_Lv4_2_C")!= nullptr)
    {
        *name = "Special Weapon Box";
        return true;
    }

        if (strstr(gname, "BP_DC_HighLevel_P_FileBag_Lua_Lv1_C") ||strstr(gname, "BP_DC_HighLevel_P_FileBag_Lua_Lv1_2_C ")||strstr(gname, "BP_DC_P_FileBag_Lua_Lv1_C")||strstr(gname, "BP_DC_P_FileBag_Lua_Lv1_2_C")!= nullptr) 
    {
        *name = "Low Briefcase";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_P_FileBag_Lua_Lv2_C") ||strstr(gname, "BP_DC_HighLevel_P_FileBag_Lua_Lv2_2_C ")||strstr(gname, "BP_DC_P_FileBag_Lua_Lv2_C")||strstr(gname, "BP_DC_P_FileBag_Lua_Lv2_2_C")!= nullptr)    
    {
        *name = "Medium Briefcase";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_P_FileBag_Lua_Lv3_C") ||strstr(gname, "BP_DC_HighLevel_P_FileBag_Lua_Lv3_2_C ")||strstr(gname, "BP_DC_P_FileBag_Lua_Lv3_C")||strstr(gname, "BP_DC_P_FileBag_Lua_Lv3_2_C")!= nullptr)    {
        *name = "High Briefcase";
        return true;
    }
    
        if (strstr(gname, "BP_DC_HighLevel_P_FileBox_Lua_Lv1_C") ||strstr(gname, "BP_DC_HighLevel_P_FileBox_Lua_Lv1_2_C ")||strstr(gname, "BP_DC_P_FileBox_Lua_Lv1_C")||strstr(gname, "BP_DC_P_FileBox_Lua_Lv1_2_C")!= nullptr) 
    {
        *name = "Low Document Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_P_FileBox_Lua_Lv2_C") ||strstr(gname, "BP_DC_HighLevel_P_FileBox_Lua_Lv2_2_C ")||strstr(gname, "BP_DC_P_FileBox_Lua_Lv2_C")||strstr(gname, "BP_DC_P_FileBox_Lua_Lv2_2_C")!= nullptr)    
    {
        *name = "Medium Document Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_P_FileBox_Lua_Lv3_C") ||strstr(gname, "BP_DC_HighLevel_P_FileBox_Lua_Lv3_2_C ")||strstr(gname, "BP_DC_P_FileBox_Lua_Lv3_C")||strstr(gname, "BP_DC_P_FileBox_Lua_Lv3_2_C")!= nullptr)    {
        *name = "High Document Box";
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_P_FileBox_Lua_Lv4_C") ||strstr(gname, "BP_DC_HighLevel_P_FileBox_Lua_Lv4_2_C ")||strstr(gname, "BP_DC_P_FileBox_Lua_Lv4_C")||strstr(gname, "BP_DC_P_FileBox_Lua_Lv4_2_C")!= nullptr)
    {
        *name = "Special Document Box";
        return true;
    }
   
    return false;
}
