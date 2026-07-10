#pragma once
#include <cstdint>

namespace Offsets {
    inline uintptr_t GClientEngine = 0x7D541C0; //FROM IDAA
    inline uintptr_t ActorBase = 0x7FB9840; //FROM IDAA
    inline uintptr_t IGamePlay = 0x40; //FROM IDAA
    inline uintptr_t IClientPlayer = 0x58; //FROM IDAA
    inline uintptr_t ICamera = 0x228; //FROM IDAA
    inline uintptr_t LocalIEntity = 0x260; //FROM IDAA
    inline uintptr_t PStruct = 0x40; //FROM IDAA
    inline uintptr_t LocalActorComponent = 0x10; //FROM IDAA
    inline uintptr_t ActorComponent = 0x18; //FROM IDAA
    inline uintptr_t Actor = 0xB8; //FROM IDAA
    inline uintptr_t ActorProps = 0x238; 
    inline uintptr_t InWorld = 0x173;
    inline uintptr_t CharCtrlComponent = 0x18;
    inline uintptr_t PhysicsSpaceComponent = 0xA8;
    inline uintptr_t IEntity = 0x40;
    inline uintptr_t IEntityPosition = 0x7C;
    inline uintptr_t AnimationCore = 0x18;
    inline uintptr_t BoneArray = 0x10;
    inline uintptr_t WorldMatrix = 0x58;
    inline uintptr_t BoneInformation = 0x88;
    inline uintptr_t BoneName = 0x58;

}

namespace BoneIndex {
    struct Bones {
        int head = 7;
        int neck = 6;
        int spine_02 = 5;
        int hips = 2;
        int upperarm_l = 9;
        int lowerarm_l = 10;
        int hand_l = 12;
        int upperarm_r = 29;
        int lowerarm_r = 30;
        int hand_r = 32;
        int thigh_l = 48;
        int calf_l = 49;
        int foot_l = 54;
        int thigh_r = 50;
        int calf_r = 51;
        int foot_r = 52;
        int Root = 0;
    };
}
