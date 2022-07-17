/*
 * File: z_en_mkk.c
 * Overlay: ovl_En_Mkk
 * Description: Black and White Boe (Name origin: Makkurokurosuke)
 */

#include "z_en_mkk.h"
#include "objects/object_mkk/object_mkk.h"

#define FLAGS (ACTOR_FLAG_1 | ACTOR_FLAG_4)

#define THIS ((EnMkk*)thisx)

void EnMkk_Init(Actor* thisx, PlayState* play);
void EnMkk_Destroy(Actor* thisx, PlayState* play);
void EnMkk_Update(Actor* thisx, PlayState* play);
void EnMkk_Draw(Actor* thisx, PlayState* play);

void func_80A4E100(EnMkk* this, PlayState* play);
void func_80A4E1F0(EnMkk* this, PlayState* play);
void func_80A4E2E8(EnMkk* this, PlayState* play);
void func_80A4E60C(EnMkk* this, PlayState* play);
void func_80A4E72C(EnMkk* this, PlayState* play);
void func_80A4EE48(EnMkk* this, PlayState* play);
void func_80A4EF74(EnMkk* this, PlayState* play);
void func_80A4E0CC(EnMkk* this);
void func_80A4E190(EnMkk* this);
void func_80A4E2B8(EnMkk* this);
void func_80A4E58C(EnMkk* this);
void func_80A4EDF0(EnMkk* this);
void func_80A4EEF4(EnMkk* this);
void func_80A4F16C(EnMkk* this, PlayState* play);
void func_80A4F4C8(EnMkk* this, PlayState* play);
void func_80A4E67C(EnMkk* this);
void func_80A4E84C(EnMkk* this);
void func_80A4EBBC(EnMkk* this, PlayState* play);

const ActorInit En_Mkk_InitVars = {
    ACTOR_EN_MKK,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_MKK,
    sizeof(EnMkk),
    (ActorFunc)EnMkk_Init,
    (ActorFunc)EnMkk_Destroy,
    (ActorFunc)EnMkk_Update,
    (ActorFunc)EnMkk_Draw,
};

static ColliderSphereInit sSphereInit = {
    {
        COLTYPE_HIT3,
        AT_NONE | AT_TYPE_ENEMY,
        AC_NONE | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_SPHERE,
    },
    {
        ELEMTYPE_UNK0,
        { 0xF7CFFFFF, 0x00, 0x04 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_HARD,
        BUMP_ON,
        OCELEM_ON,
    },
    { 1, { { 0, 0, 0 }, 15 }, 100 },
};

struct EnMkkDlists {
    /* 0x00 */ Gfx* unk0;
    /* 0x04 */ Gfx* unk4;
    /* 0x08 */ Gfx* unk8;
    /* 0x0C */ Gfx* unkC;
};

static CollisionCheckInfoInit sColChkInfoInit = { 1, 15, 30, 10 };

static DamageTable sDamageTable = {
    /* Deku Nut       */ DMG_ENTRY(1, 0x0),
    /* Deku Stick     */ DMG_ENTRY(1, 0x0),
    /* Horse trample  */ DMG_ENTRY(1, 0x0),
    /* Explosives     */ DMG_ENTRY(1, 0x0),
    /* Zora boomerang */ DMG_ENTRY(1, 0x0),
    /* Normal arrow   */ DMG_ENTRY(1, 0x0),
    /* UNK_DMG_0x06   */ DMG_ENTRY(0, 0x0),
    /* Hookshot       */ DMG_ENTRY(1, 0x0),
    /* Goron punch    */ DMG_ENTRY(1, 0x0),
    /* Sword          */ DMG_ENTRY(1, 0x0),
    /* Goron pound    */ DMG_ENTRY(1, 0x0),
    /* Fire arrow     */ DMG_ENTRY(2, 0x0),
    /* Ice arrow      */ DMG_ENTRY(2, 0x0),
    /* Light arrow    */ DMG_ENTRY(2, 0x0),
    /* Goron spikes   */ DMG_ENTRY(1, 0x0),
    /* Deku spin      */ DMG_ENTRY(1, 0x0),
    /* Deku bubble    */ DMG_ENTRY(1, 0x0),
    /* Deku launch    */ DMG_ENTRY(2, 0x0),
    /* UNK_DMG_0x12   */ DMG_ENTRY(1, 0x0),
    /* Zora barrier   */ DMG_ENTRY(1, 0x0),
    /* Normal shield  */ DMG_ENTRY(0, 0x0),
    /* Light ray      */ DMG_ENTRY(0, 0x0),
    /* Thrown object  */ DMG_ENTRY(1, 0x0),
    /* Zora punch     */ DMG_ENTRY(1, 0x0),
    /* Spin attack    */ DMG_ENTRY(1, 0x0),
    /* Sword beam     */ DMG_ENTRY(0, 0x0),
    /* Normal Roll    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1B   */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1C   */ DMG_ENTRY(0, 0x0),
    /* Unblockable    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1E   */ DMG_ENTRY(0, 0x0),
    /* Powder Keg     */ DMG_ENTRY(1, 0x0),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32_DIV1000(gravity, -500, ICHAIN_CONTINUE),
    ICHAIN_F32(terminalVelocity, -5, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 3000, ICHAIN_STOP),
};

static Color_RGBA8 D_80A4F780 = { 250, 250, 250, 255 }; //prim

static Color_RGBA8 D_80A4F784 = { 180, 180, 180, 255 }; //env

static Vec3f D_80A4F788 = { 0.0f, 0.45f, 0.0f };

static Color_RGBA8 sEffPrimColors[] = {
    { 50, 50, 50, 255 },
    { 255, 255, 255, 255 }
};

static Color_RGBA8 sEffEnvColors[] = {
    { 200, 200, 200, 255 },
    { 255, 255, 255, 255 }
};
static struct EnMkkDlists sDLists[2] = {
    { object_mkk_DL_000030, object_mkk_DL_0000B0, object_mkk_DL_0000C8, object_mkk_DL_000140 },
    { object_mkk_DL_0001F0, object_mkk_DL_000278, object_mkk_DL_000290, object_mkk_DL_000310 },
};

static Color_RGBA8 D_80A4F7C4[] = {
    { 255, 255, 255, 255 }, { 128, 128, 128, 255 }, { 0, 0, 0, 255 },
    { 0, 0, 0, 255 },       { 128, 128, 128, 255 }, { 255, 255, 255, 255 },
};

void EnMkk_Init(Actor* thisx, PlayState* play) {
    EnMkk* this = (EnMkk*)thisx;
    s32 params8;
    s32 params2;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    Collider_InitAndSetSphere(play, &this->collider, &this->actor, &sSphereInit);
    this->collider.dim.worldSphere.radius = sSphereInit.dim.modelSphere.radius;
    CollisionCheck_SetInfo(&this->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);
    ActorShape_Init(&this->actor.shape, 1000.0f, NULL, 0.0f);

    this->unk148 = 0;
    this->unk149 = 0;
    this->unk152 = (this->actor.shape.rot.x >= 0) ? this->actor.shape.rot.x : this->actor.shape.rot.x * -0x50;

    this->unk14C = CLAMP(this->actor.shape.rot.z, 0, 16);

    this->actor.shape.rot.z = 0;
    this->actor.world.rot.z = 0;
    this->actor.shape.rot.x = 0;
    this->actor.world.rot.x = 0;

    Math_Vec3f_Copy(&this->unk154, &this->actor.world.pos);
    Math_Vec3f_Copy(&this->unk160, &this->actor.world.pos);
    this->unk14B = (this->actor.params & 4) ? 8 : 0;

    params8 = (this->actor.params >> 8) & 0xFF;
    params2 = this->actor.params & 2;
    this->actor.params &= 1;
    if (this->actor.params == 1) {
        this->actor.hintId = 0x3C;
    } else {
        this->actor.hintId = 0x2C;
    }
    if ((params8 == 0) || (params8 == 255)) {
        func_80A4E0CC(this);
        this->unk178 = 30000.0f;
        if (params2 > 0) {
            this->unk14B |= 4;
        }
    } else {
        this->unk178 = params8 * 40.0f * 0.1f;
        func_80A4EDF0(this);
        this->unk14E = 0;
    }
}

void EnMkk_Destroy(Actor* thisx, PlayState* play) {
    EnMkk* this = THIS;
    Collider_DestroySphere(play, &this->collider);
}

void func_80A4E0CC(EnMkk* this) {
    this->unk14A = 0;
    this->unk14B |= 3;
    this->actor.flags &= -2;
    this->actionFunc = func_80A4E100;
}

void func_80A4E100(EnMkk* this, PlayState* play) {
    s32 phi_v0;

    if ((this->unk14B & 4) != 0) {
        phi_v0 = this->unk14A + 15;
    } else {
        phi_v0 = this->unk14A + 5;
    }
    if (phi_v0 >= 0xFF) {
        this->unk148 = 3;
        this->collider.base.acFlags |= 1;
        this->unk14A = 255;
        this->actor.flags |= 1;
        this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
        this->unk14B = this->unk14B & 0xFFFB;
        func_80A4E190(this);
    } else {
        this->unk14A = (u8)phi_v0;
    }
}

void func_80A4E190(EnMkk* this) {
    this->unk14E = (s32)(Rand_ZeroOne() * 20.0f) + 0xA;
    if (this->unk149 != 0) {
        this->unk149--;
    }
    this->actionFunc = func_80A4E1F0;
}

void func_80A4E1F0(EnMkk* this, PlayState* play) {
    this->unk14E--;
    if (this->unk14E == 0) {
        func_80A4E2B8(this);
    }
}

void func_80A4E22C(EnMkk* this, PlayState* play) {
    Vec3f pos;

    pos.x = this->actor.world.pos.x;
    pos.y = this->actor.world.pos.y + 15.0f;
    pos.z = this->actor.world.pos.z;
    func_800B0DE0(play, &pos, &gZeroVec3f, &D_80A4F788, &D_80A4F780, &D_80A4F784, 0x15E, 0x14);
}

void func_80A4E2B8(EnMkk* this) {
    this->unk14E = 30;
    this->unk150 = this->actor.shape.rot.y;
    this->unk14B = this->unk14B & 0xFFFE;
    this->actionFunc = func_80A4E2E8;
}

void func_80A4E2E8(EnMkk* this, PlayState* play) {
    Player* sp24 = GET_PLAYER(play);
    s32 sp20;

    this->unk14E--;
    if ((this->actor.params == 1) && (this->actor.bgCheckFlags & 1) && (this->actor.speedXZ > 2.5f) &&
        (!(play->gameplayFrames % 3))) {
        func_80A4E22C(this, play);
    }
    if (this->unk14E > 0) {
        Math_StepToF(&this->actor.speedXZ, 5.0f, 0.7f);
        sp20 = 0;
    } else {
        sp20 = Math_StepToF(&this->actor.speedXZ, 0.0f, 0.7f);
    }
    if (((sp24->stateFlags3 & 0x100)) || (Player_GetMask(play) == 0x10)) {
        Math_ScaledStepToS(&this->unk150, Actor_YawToPoint(&this->actor, &this->actor.home.pos), 0x400);
    } else if ((sp24->stateFlags2 & 0x80) || (sp24->actor.freezeTimer > 0)) {
        Math_ScaledStepToS(&this->unk150, this->actor.yawTowardsPlayer + 0x8000, 0x400);
    } else {
        Math_ScaledStepToS(&this->unk150, this->actor.yawTowardsPlayer, 0x400);
    }
    this->actor.shape.rot.y =
        (s32)(sin_rad(this->unk14E * 0.41887903f) * (614.4f * this->actor.speedXZ)) + this->unk150;
    func_800B9010(&this->actor, 0x30BC);
    if (sp20 != 0) {
        this->unk14B &= 0xFFFD;
        func_80A4E190(this);
    } else {
        if ((this->unk149 == 0) && ((sp24->stateFlags3 & 0x100) == 0) && (Player_GetMask(play) != 0x10) &&
            ((this->actor.bgCheckFlags & 1) != 0) && (Actor_IsFacingPlayer(&this->actor, 0x1800) != 0) &&
            (this->actor.xzDistToPlayer < 120.0f) && (fabsf(this->actor.playerHeightRel) < 100.0f)) {
            func_80A4E58C(this);
        }
    }
}

void func_80A4E58C(EnMkk* this) {
    this->unk14B = this->unk14B | 1;
    this->actor.speedXZ = 3.0f;
    this->actor.velocity.y = 5.0f;
    Actor_PlaySfxAtPos(this, 0x38BD);
    this->collider.base.atFlags = this->collider.base.atFlags | 1;
    Math_ScaledStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0x800);
    this->actionFunc = func_80A4E60C;
}

void func_80A4E60C(EnMkk* this, PlayState* play) {
    if ((this->collider.base.atFlags & 2)) {
        this->collider.base.atFlags = (this->collider.base.atFlags) & 0xFFFC;
    }
    if ((this->actor.velocity.y < 0.0f) && ((this->actor.bgCheckFlags & 1))) {
        this->unk149 = 2;
        this->collider.base.atFlags &= 0xFFFE;
        func_80A4E2B8(this);
    }
}

void func_80A4E67C(EnMkk* this) {
    this->unk14B |= 1;
    this->actor.flags &= ~1;
    this->collider.base.acFlags &= -2;
    this->actor.flags = this->actor.flags | 0x10;
    Actor_PlaySfxAtPos(&this->actor, 0x3876);
    this->unk14A = 254;
    func_800BE568(&this->actor, &this->collider);
    this->actor.speedXZ = 7.0f;
    this->actor.shape.rot.y = this->actor.world.rot.y;
    this->actor.velocity.y = 5.0f;
    this->actor.gravity = -1.3f;
    this->actor.bgCheckFlags &= 0xFFFE;
    this->actionFunc = func_80A4E72C;
}

void func_80A4E72C(EnMkk* this, PlayState* play) {
    Vec3f temp;

    if ((this->actor.bgCheckFlags & 1)) {
        if (this->actor.velocity.y > -1.0f) {
            temp.x = this->actor.world.pos.x;
            temp.y = this->actor.world.pos.y + 15.0f;
            temp.z = this->actor.world.pos.z;
            EffectSsDeadDb_Spawn(play, &temp, &gZeroVec3f, &gZeroVec3f, &sEffPrimColors[this->actor.params],
                                 &sEffEnvColors[this->actor.params], 0x46, 4, 0xC);
            Actor_PlaySfxAtPos(&this->actor, 0x3878);
            if (this->unk14C != 0) {
                Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, (this->unk14C * 0x10));
            }
            func_80A4EEF4(this);
        } else {
            this->actor.velocity.y = (f32)this->actor.velocity.y * -0.8f;
            this->actor.bgCheckFlags = this->actor.bgCheckFlags & 0xFFFE;
        }
    }
}

void func_80A4E84C(EnMkk* this) {
    if ((this->unk14B & 3) != 0) {
        Vec3f sp34;
        f32 temp_fv0 = Math_Vec3f_DistXYZ(&this->actor.world.pos, &this->actor.prevPos);

        Math_Vec3f_Copy(&sp34, &this->unk154);
        if (temp_fv0 < 0.001f) {
            Math_Vec3f_Copy(&this->unk154, &this->actor.prevPos);
        } else {
            temp_fv0 = 1.0f / temp_fv0;

            this->unk154.x =
                ((this->actor.prevPos.x - this->actor.world.pos.x) * temp_fv0 * 10.0f) + this->actor.world.pos.x;
            this->unk154.y =
                ((this->actor.prevPos.y - this->actor.world.pos.y) * temp_fv0 * 10.0f) + this->actor.world.pos.y;
            this->unk154.z =
                ((this->actor.prevPos.z - this->actor.world.pos.z) * temp_fv0 * 10.0f) + this->actor.world.pos.z;
        }
        temp_fv0 = Math_Vec3f_DistXYZ(&this->unk154, &sp34);
        if (temp_fv0 < 0.001f) {
            Math_Vec3f_Copy(&this->unk160, &sp34);
        } else {
            temp_fv0 = 1.0f / temp_fv0;

            this->unk160.x = ((sp34.x - this->unk154.x) * temp_fv0 * 12.0f) + this->unk154.x;
            this->unk160.y = ((sp34.y - this->unk154.y) * temp_fv0 * 12.0f) + this->unk154.y;
            this->unk160.z = ((sp34.z - this->unk154.z) * temp_fv0 * 12.0f) + this->unk154.z;
        }
    } else {
        this->unk160.y = this->unk154.y;
        this->unk154.y = this->actor.world.pos.y;
        this->unk154.x = this->actor.world.pos.x -
                         10.0f * Math_SinS(this->actor.shape.rot.y +
                                           (s32)(1228.8f * this->actor.speedXZ * sin_rad(this->unk14E * 0.62831855f)));
        this->unk154.z = this->actor.world.pos.z -
                         10.0f * Math_CosS(this->actor.shape.rot.y +
                                           (s32)(1228.8f * this->actor.speedXZ * sin_rad(this->unk14E * 0.62831855f)));
        this->unk160.x = this->unk154.x -
                         12.0f * Math_SinS(this->actor.shape.rot.y -
                                           (s32)(1228.8f * this->actor.speedXZ * sin_rad(this->unk14E * 0.62831855f)));
        this->unk160.z = this->unk154.z -
                         12.0f * Math_CosS(this->actor.shape.rot.y -
                                           (s32)(1228.8f * this->actor.speedXZ * sin_rad(this->unk14E * 0.62831855f)));
    }
}

void func_80A4EBBC(EnMkk* this, PlayState* play) {
    if (this->collider.base.acFlags & 2) {
        this->collider.base.acFlags &= 0xFFFD;
        Actor_SetDropFlag(&this->actor, &this->collider.info);
        Enemy_StartFinishingBlow(play, &this->actor);
        func_80A4E67C(this);
    }
}

void EnMkk_Update(Actor* thisx, PlayState* play) {
    s32 pad;
    Player* player;
    EnMkk* this = THIS;

    if (this->unk148 > 0) {
        this->unk148--;
    } else if ((Rand_ZeroOne() < 0.075f) && (this->actor.params != 1)) {
        this->unk148 = 6;
    }
    func_80A4EBBC(&this->actor, play);
    this->actionFunc(this, play);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    Actor_MoveWithGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 20.0f, 20.0f, 0x1D);
    if (this->actor.params == 0) {
        func_80A4E84C(this);
    }
    if (Actor_IsFacingPlayer(&this->actor, 0x3000)) {
        player = GET_PLAYER(play);
        this->actor.shape.rot.x = Actor_PitchToPoint(&this->actor, &player->actor.focus.pos);
        this->actor.shape.rot.x = CLAMP(this->actor.shape.rot.x, -0x1800, 0x1800);
    }
    Actor_SetFocus(&this->actor, 10.0f);
    this->collider.dim.worldSphere.center.x = this->actor.focus.pos.x;
    this->collider.dim.worldSphere.center.y = this->actor.focus.pos.y;
    this->collider.dim.worldSphere.center.z = this->actor.focus.pos.z;
    if (this->collider.base.atFlags & 1) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    }
    if (this->collider.base.acFlags & 1) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
}

void func_80A4EDF0(EnMkk* this) {
    this->unk14A = 0;
    this->unk14B |= 3;
    this->actor.flags &= -2;
    this->actor.draw = NULL;
    this->actor.update = func_80A4F16C;
    this->actor.gravity = -0.5f;
    this->unk14E = this->unk152;
    this->actionFunc = func_80A4EE48;
}

void func_80A4EE48(EnMkk* this, PlayState* play) {
    if (this->unk14E > 0) {
        this->unk14E--;
        if (this->unk14E == 0) {
            this->actor.flags &= -0x11;
        }
    } else if ((this->actor.xzDistToPlayer < this->unk178) && (Player_GetMask(play) != 0x10)) {
        this->actor.update = EnMkk_Update;
        this->actor.draw = EnMkk_Draw;
        this->actor.flags &= -0x11;
        func_80A4E0CC(this);
    }
}

void func_80A4EEF4(EnMkk* this) {
    Math_Vec3f_Copy(&this->unk16C, &this->actor.world.pos);
    Math_Vec3f_Copy(&this->unk154, &this->actor.world.pos);
    Math_Vec3f_Copy(&this->unk160, &this->actor.world.pos);
    Actor_SetScale(&this->actor, 0.005f);
    this->actor.update = func_80A4F16C;
    this->actor.draw = func_80A4F4C8;
    this->actionFunc = func_80A4EF74;
}

void func_80A4EF74(EnMkk* this, PlayState* play) {
    s32 temp = this->unk14A - 20;

    if (temp <= 0) {
        if ((this->unk14B & 8) != 0) {
            Actor_SetScale(&this->actor, 0.01f);
            this->unk148 = 0;
            this->unk149 = 0;
            Math_Vec3f_Copy(&this->actor.world.pos, &this->actor.home.pos);
            Math_Vec3f_Copy(&this->unk154, &this->actor.world.pos);
            Math_Vec3f_Copy(&this->unk160, &this->actor.world.pos);
            this->actor.speedXZ = 0.0f;
            this->actor.velocity.y = 0.0f;
            func_80A4EDF0(this);
        } else {
            Actor_MarkForDeath(&this->actor);
        }
    } else {
        this->unk14A = temp;
        this->actor.world.pos.x += 0.3f + (3.0f * Rand_ZeroOne());
        this->actor.world.pos.y += 0.5f + (3.5f * Rand_ZeroOne());
        this->unk16C.x -= 0.3f + (3.0f * Rand_ZeroOne());
        this->unk16C.y += 0.5f + (3.5f * Rand_ZeroOne());
        this->unk154.z += 0.3f + (3.0f * Rand_ZeroOne());
        this->unk154.y += 0.5f + (3.5f * Rand_ZeroOne());
        this->unk160.z -= 0.3f + (3.0f * Rand_ZeroOne());
        this->unk160.y += 0.5f + (3.5f * Rand_ZeroOne());
    }
}

void func_80A4F16C(EnMkk* this, PlayState* play) {
    this->actionFunc(this, play);
}

void EnMkk_Draw(Actor* thisx, PlayState* play) {
    struct EnMkkDlists* sp2C;
    Gfx* gfx;
    Color_RGBA8* temp_v0;
    EnMkk* this = (EnMkk*)thisx;

    sp2C = &sDLists[this->actor.params];
    if (this->actor.projectedPos.z > 0.0f) {
        MtxF* temp_v0_2;
        OPEN_DISPS(play->state.gfxCtx);
        
        if (this->unk14A == 0xFF) {
            temp_v0 = &D_80A4F7C4[this->unk148];
            gfx = POLY_OPA_DISP;
            gSPDisplayList(&gfx[0], &sSetupDL[6 * 25]);
            gDPSetPrimColor(&gfx[1], 0, 0xFF, temp_v0->r, temp_v0->g, temp_v0->b, temp_v0->a);
            gSPSegment(&gfx[2], 0x08, D_801AEFA0);
            gSPMatrix(&gfx[3], Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(&gfx[4], sp2C->unkC);
            POLY_OPA_DISP = &gfx[5];
        }
        gfx = POLY_XLU_DISP;
        gSPDisplayList(&gfx[0], &sSetupDL[6 * 25]);
        gDPSetEnvColor(&gfx[1], 255, 255, 255, this->unk14A);
        gSPDisplayList(&gfx[2], sp2C->unk0);
        Matrix_ReplaceRotation(&play->billboardMtxF);
        gSPMatrix(&gfx[3], Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(&gfx[4], sp2C->unk4);
        if (this->actor.params == 0) {
            temp_v0_2 = Matrix_GetCurrent();
            temp_v0_2->mf[3][0] = this->unk154.x;
            temp_v0_2->mf[3][1] = this->unk154.y + 8.5f;
            temp_v0_2->mf[3][2] = this->unk154.z;

            Matrix_Scale(0.85f, 0.85f, 0.85f, MTXMODE_APPLY);
            gSPMatrix(&gfx[5], Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(&gfx[6], sp2C->unk4);

            temp_v0_2->mf[3][0] = this->unk160.x;
            temp_v0_2->mf[3][1] = this->unk160.y + 7.2250004f;
            temp_v0_2->mf[3][2] = this->unk160.z;

            Matrix_Scale(0.85f, 0.85f, 0.85f, MTXMODE_APPLY);
            gSPMatrix(&gfx[7], Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(&gfx[8], sp2C->unk4);
            gSPDisplayList(&gfx[9], sp2C->unk8);

            POLY_XLU_DISP = &gfx[10];
        } else {
            gSPDisplayList(&gfx[5], sp2C->unk8);
            POLY_XLU_DISP = &gfx[6];
        }
    }
    CLOSE_DISPS(play->state.gfxCtx);
}

void func_80A4F4C8(EnMkk* this, PlayState* play) {
    Gfx* gfx;
    s32 pad;
    MtxF* temp_v0_2;
    struct EnMkkDlists* temp_a3 = &sDLists[this->actor.params];
    s32 pad2;

    OPEN_DISPS(play->state.gfxCtx);

    gfx = POLY_XLU_DISP;
    gSPDisplayList(&gfx[0], &sSetupDL[6 * 25]);
    gDPSetEnvColor(&gfx[1], 255, 255, 255, this->unk14A);
    gSPDisplayList(&gfx[2], temp_a3->unk0);
    Matrix_ReplaceRotation(&play->billboardMtxF);
    gSPMatrix(&gfx[3], Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(&gfx[4], temp_a3->unk4);
    temp_v0_2 = Matrix_GetCurrent();
    temp_v0_2->mf[3][0] = this->unk154.x;
    temp_v0_2->mf[3][1] = this->unk154.y + 5.0f;
    temp_v0_2->mf[3][2] = this->unk154.z;
    gSPMatrix(&gfx[5], Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(&gfx[6], temp_a3->unk4);
    temp_v0_2->mf[3][0] = this->unk160.x;
    temp_v0_2->mf[3][1] = this->unk160.y + 5.0f;
    temp_v0_2->mf[3][2] = this->unk160.z;
    gSPMatrix(&gfx[7], Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(&gfx[8], temp_a3->unk4);
    temp_v0_2->mf[3][0] = this->unk16C.x;
    temp_v0_2->mf[3][1] = this->unk16C.y + 5.0f;
    temp_v0_2->mf[3][2] = this->unk16C.z;
    gSPMatrix(&gfx[9], Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(&gfx[10], temp_a3->unk4);
    gSPDisplayList(&gfx[11], temp_a3->unk8);
    POLY_XLU_DISP = &gfx[12];

    CLOSE_DISPS(play->state.gfxCtx);
}