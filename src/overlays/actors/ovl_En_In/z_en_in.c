/*
 * File: z_en_in.c
 * Overlay: ovl_En_In
 * Description: Gorman Bros
 */

#include "z_en_in.h"
#include "objects/object_in/object_in.h"
#include "overlays/actors/ovl_En_Horse_Game_Check/z_en_horse_game_check.h"

#define FLAGS (ACTOR_FLAG_1 | ACTOR_FLAG_8 | ACTOR_FLAG_10)

#define THIS ((EnIn*)thisx)

void EnIn_Init(Actor* thisx, PlayState* play);
void EnIn_Destroy(Actor* thisx, PlayState* play);
void EnIn_Update(Actor* thisx, PlayState* play);
void EnIn_Draw(Actor* thisx, PlayState* play);

void func_808F5A94(EnIn* this, PlayState* play);
void func_808F3690(EnIn* this, PlayState* play);
void func_808F5A34(EnIn* this, PlayState* play);
s32 func_808F5994(EnIn* this, PlayState* play, Vec3f* arg2, s16 arg3);

const ActorInit En_In_InitVars = {
    ACTOR_EN_IN,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_IN,
    sizeof(EnIn),
    (ActorFunc)EnIn_Init,
    (ActorFunc)EnIn_Destroy,
    (ActorFunc)EnIn_Update,
    (ActorFunc)EnIn_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT0,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK1,
        { 0x00000000, 0x00, 0x00 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 18, 64, 0, { 0, 0, 0 } },
};

static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0xF7CFFFFF, 0x00, 0x00 },
            { 0x00000000, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_NONE,
            OCELEM_NONE,
        },
        { 12, { { 1600, 0, 0 }, 5 }, 200 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_2,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit),
    sJntSphElementsInit,
};

static CollisionCheckInfoInit2 sColChkInfoInit2 = { 0, 0, 0, 0, MASS_HEAVY };

static DamageTable sDamageTable = {
    /* Deku Nut       */ DMG_ENTRY(0, 0x0),
    /* Deku Stick     */ DMG_ENTRY(0, 0x0),
    /* Horse trample  */ DMG_ENTRY(0, 0x0),
    /* Explosives     */ DMG_ENTRY(0, 0x0),
    /* Zora boomerang */ DMG_ENTRY(0, 0x0),
    /* Normal arrow   */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x06   */ DMG_ENTRY(0, 0x0),
    /* Hookshot       */ DMG_ENTRY(0, 0x0),
    /* Goron punch    */ DMG_ENTRY(0, 0x0),
    /* Sword          */ DMG_ENTRY(0, 0x0),
    /* Goron pound    */ DMG_ENTRY(0, 0x0),
    /* Fire arrow     */ DMG_ENTRY(0, 0x0),
    /* Ice arrow      */ DMG_ENTRY(0, 0x0),
    /* Light arrow    */ DMG_ENTRY(0, 0x0),
    /* Goron spikes   */ DMG_ENTRY(0, 0x0),
    /* Deku spin      */ DMG_ENTRY(0, 0x0),
    /* Deku bubble    */ DMG_ENTRY(0, 0x0),
    /* Deku launch    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x12   */ DMG_ENTRY(0, 0x0),
    /* Zora barrier   */ DMG_ENTRY(0, 0x0),
    /* Normal shield  */ DMG_ENTRY(0, 0x0),
    /* Light ray      */ DMG_ENTRY(0, 0x0),
    /* Thrown object  */ DMG_ENTRY(0, 0x0),
    /* Zora punch     */ DMG_ENTRY(0, 0x0),
    /* Spin attack    */ DMG_ENTRY(0, 0x0),
    /* Sword beam     */ DMG_ENTRY(0, 0x0),
    /* Normal Roll    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1B   */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1C   */ DMG_ENTRY(0, 0x0),
    /* Unblockable    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1E   */ DMG_ENTRY(0, 0x0),
    /* Powder Keg     */ DMG_ENTRY(0, 0x0),
};

static AnimationInfoS sAnimationInfo[] = {
    { &object_in_Anim_001D10, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &object_in_Anim_001D10, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &object_in_Anim_014F8C, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &object_in_Anim_014F8C, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &object_in_Anim_000CB0, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &object_in_Anim_0003B4, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &object_in_Anim_001BE0, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &object_in_Anim_015918, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &object_in_Anim_01C0B0, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &object_in_Anim_01C0B0, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &object_in_Anim_01A140, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &object_in_Anim_01A140, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &object_in_Anim_01B904, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &object_in_Anim_01B904, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &object_in_Anim_01B3C4, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &object_in_Anim_01B3C4, 0.0f, 0, -1, ANIMMODE_ONCE, 0 },
    { &object_in_Anim_01B3C4, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &object_in_Anim_019EB4, 1.0f, 0, -1, ANIMMODE_ONCE, -4 },
};

static TrackOptionsSet sTrackOptions = {
    { 0xFA0, 4, 1, 3 },
    { 0x1770, 4, 1, 6 },
    { 0xFA0, 4, 1, 3 },
    { 0x1770, 4, 1, 6 },
};

s32 EnIn_ChangeAnim(SkelAnime* skelAnime, s16 animIndex) {
    s16 frameCount;
    s32 ret = false;

    if (animIndex >= 0 && animIndex < 18) {
        ret = true;
        frameCount = sAnimationInfo[animIndex].frameCount;
        if (frameCount < 0) {
            frameCount = Animation_GetLastFrame(sAnimationInfo[animIndex].animation);
        }
        Animation_Change(skelAnime, sAnimationInfo[animIndex].animation, sAnimationInfo[animIndex].playSpeed,
                         sAnimationInfo[animIndex].startFrame, frameCount, sAnimationInfo[animIndex].mode,
                         sAnimationInfo[animIndex].morphFrames);
    }
    return ret;
}

s32 func_808F3178(EnIn* this, PlayState* play) {
    u8 prevUnk260 = this->unk260;
    u8 prevUnk261 = this->unk261;
    u8 tmp;

    this->unk260 = tmp = SubS_IsFloorAbove(play, &this->unk248, -6.0f);
    if (this->unk260 != 0 && prevUnk260 == 0 && tmp) {
        Actor_PlaySfxAtPos(&this->actor, NA_SE_PL_WALK_CONCRETE);
    }
    this->unk261 = tmp = SubS_IsFloorAbove(play, &this->unk254, -6.0f);
    if (this->unk261 != 0 && prevUnk261 == 0 && tmp) {
        Actor_PlaySfxAtPos(&this->actor, NA_SE_PL_WALK_CONCRETE);
    }

    return 0;
}

void func_808F322C(EnIn* this, s32 arg1) {
    if (DECR(this->unk484) == 0) {
        this->unk482++;
        if (this->unk482 >= arg1) {
            this->unk482 = 0;
            this->unk484 = Rand_S16Offset(30, 30);
        }
    }
}

void func_808F32A0(EnIn* this, PlayState* play) {
    this->colliderCylinder.dim.pos.x = this->actor.world.pos.x;
    this->colliderCylinder.dim.pos.y = this->actor.world.pos.y;
    this->colliderCylinder.dim.pos.z = this->actor.world.pos.z;
    if (this->unk23D == 0) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->colliderCylinder.base);
    }
}

s32 func_808F3310(EnIn* this, PlayState* play) {
    if (this->colliderCylinder.base.acFlags & AC_HIT) {
        this->colliderCylinder.base.acFlags &= ~AC_HIT;
    }
    return 0;
}

s32 func_808F3334(EnIn* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (this->colliderJntSph.base.atFlags & AT_HIT) {
        this->colliderJntSph.base.atFlags &= ~AT_HIT;
        if (this->colliderJntSph.base.atFlags & AT_BOUNCED) {
            return 0;
        }
        Actor_PlaySfxAtPos(&player->actor, NA_SE_PL_BODY_HIT);
        func_800B8D98(play, &this->actor, 3.0f, this->actor.yawTowardsPlayer, 6.0f);
    }
    return 1;
}

s32 func_808F33B8(void) {
    s32 ret = (((gSaveContext.save.day == 1) &&
                ((gSaveContext.save.time >= CLOCK_TIME(5, 30)) && (gSaveContext.save.time <= CLOCK_TIME(6, 0)))) ||
               (gSaveContext.save.day >= 2)) &&
              !(gSaveContext.save.weekEventReg[22] & 1);

    return ret;
}

void func_808F3414(EnIn* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Vec3f point;

    if (this->unk23D == 0) {
        this->unk494 = SkelAnime_Update(&this->skelAnime);
    }
    if (SubS_AngleDiffLessEqual(this->actor.shape.rot.y, 0x2710, this->actor.yawTowardsPlayer)) {
        point.x = player->actor.world.pos.x;
        point.y = player->bodyPartsPos[7].y + 3.0f;
        point.z = player->actor.world.pos.z;
        SubS_TrackPoint(&point, &this->actor.focus.pos, &this->actor.shape.rot, &this->trackTarget, &this->headRot,
                        &this->torsoRot, &sTrackOptions);
    } else {
        Math_SmoothStepToS(&this->trackTarget.x, 0, 4, 0x3E8, 1);
        Math_SmoothStepToS(&this->trackTarget.y, 0, 4, 0x3E8, 1);
        Math_SmoothStepToS(&this->headRot.x, 0, 4, 0x3E8, 1);
        Math_SmoothStepToS(&this->headRot.y, 0, 4, 0x3E8, 1);
        Math_SmoothStepToS(&this->torsoRot.x, 0, 4, 0x3E8, 1);
        Math_SmoothStepToS(&this->torsoRot.y, 0, 4, 0x3E8, 1);
    }
    func_808F322C(this, 3);
    func_808F3178(this, play);
    SubS_FillLimbRotTables(play, this->unk376, this->unk39E, ARRAY_COUNT(this->unk376));
}

void func_808F35AC(EnIn* this, PlayState* play) {
    this->unk4AC |= 0x10;
    if (this->unk4A4 != NULL) {
        this->unk4A4->unk4AC |= 0x10;
    }
}

void func_808F35D8(EnIn* this, PlayState* play) {
    this->unk4AC &= ~0x10;
    if (this->unk4A4 != NULL) {
        this->unk4A4->unk4AC &= ~0x10;
    }
}

void EnIn_DoNothing(EnIn* this, PlayState* play) {
}

void func_808F3618(EnIn* this, PlayState* play) {
    if (ENIN_GET_PATH(&this->actor) != 0x3F) {
        EnIn_ChangeAnim(&this->skelAnime, ENIN_ANIM_9);
    }
    if (ENIN_GET_PATH(&this->actor) != 0x3F) {
        this->actionFunc = func_808F3690;
    } else {
        this->actionFunc = EnIn_DoNothing;
    }
}

void func_808F3690(EnIn* this, PlayState* play) {
    s16 sp36;
    Vec3f sp28;

    Math_SmoothStepToF(&this->actor.speedXZ, 1.0f, 0.4f, 1000.0f, 0.0f);
    sp36 = this->actor.speedXZ * 400.0f;
    if (SubS_CopyPointFromPath(this->path, this->unk244, &sp28) && SubS_MoveActorToPoint(&this->actor, &sp28, sp36)) {
        this->unk244++;
        if (this->unk244 >= this->path->count) {
            this->unk244 = 0;
        }
    }
}

void func_808F374C(EnIn* this, PlayState* play) {
    AnimationHeader* sAnimations[] = {
        &object_in_Anim_015E38, &object_in_Anim_016A60, &object_in_Anim_0177AC, &object_in_Anim_016484,
        &object_in_Anim_0170DC, &object_in_Anim_018240, &object_in_Anim_0187C8, &object_in_Anim_0198A8,
    };

    if (this->skelAnime.animation == &object_in_Anim_016484 || this->skelAnime.animation == &object_in_Anim_0170DC) {
        if (Animation_OnFrame(&this->skelAnime, 8.0f)) {
            func_8019F88C(&this->actor.projectedPos, NA_SE_VO_IN_LASH_0, 2);
            if (Rand_ZeroOne() < 0.3f) {
                Audio_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_IT_INGO_HORSE_NEIGH);
            }
            Audio_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_IT_LASH);
        }
    }
    if (this->skelAnime.animation == &object_in_Anim_0198A8 && Animation_OnFrame(&this->skelAnime, 20.0f)) {
        Actor_PlaySfxAtPos(&this->actor, NA_SE_VO_IN_CRY_0);
    }
    if (SkelAnime_Update(&this->skelAnime)) {
        this->unk488 %= ARRAY_COUNT(sAnimations);
        this->unk486 = this->unk488;
        Animation_Change(&this->skelAnime, sAnimations[this->unk488], 1.0f, 0.0f,
                         Animation_GetLastFrame(sAnimations[this->unk488]), ANIMMODE_ONCE, -10.0f);
    }
}

void func_808F38F8(EnIn* this, PlayState* play) {
    this->unk4A4 = NULL;
    while (true) {
        //! @bug: Infinite loop if there is only one ACTOR_EN_IN
        this->unk4A4 = (EnIn*)SubS_FindActor(play, &this->unk4A4->actor, ACTORCAT_NPC, ACTOR_EN_IN);
        if (this->unk4A4 != NULL && this->unk4A4 != this) {
            break;
        }
        this->unk4A4 = (EnIn*)this->unk4A4->actor.next;
    }
}

void func_808F395C(EnIn* this, PlayState* play) {
    if (this->unk4B0 == RACE_FLAG_END) {
        this->actionFunc = func_808F5A94;
    }
    if (Actor_ProcessTalkRequest(&this->actor, &play->state)) {
        this->actor.flags &= ~ACTOR_FLAG_10000;
        this->actionFunc = func_808F5A34;
        this->unk48C = 1;
    } else {
        func_800B8614(&this->actor, play, 200.0f);
    }
}

void func_808F39DC(EnIn* this, PlayState* play) {
    u16 textId = 0;

    if (gSaveContext.save.day != 3) {
        switch (GET_RACE_FLAGS) {
            case RACE_FLAG_2:
                textId = 0x347A;
                break;
            case RACE_FLAG_3:
                textId = 0x3476;
                break;
        }
        SET_RACE_FLAGS(RACE_FLAG_END);
    } else {
        switch (GET_RACE_FLAGS) {
            case RACE_FLAG_2:
                textId = 0x349D;
                break;
            case RACE_FLAG_3:
                textId = 0x3499;
                break;
        }
        SET_RACE_FLAGS(RACE_FLAG_END);
    }
    this->actor.flags |= ACTOR_FLAG_10000;
    this->actor.textId = textId;
    this->actionFunc = func_808F395C;
    if (this->unk4B0 == RACE_FLAG_2) {
        Actor_PlaySfxAtPos(&this->actor, NA_SE_VO_IN_LOST);
    } else {
        Actor_PlaySfxAtPos(&this->actor, NA_SE_VO_IN_JOY0);
    }
}

void func_808F3AD4(EnIn* this, PlayState* play) {
    if (Actor_ProcessTalkRequest(&this->actor, &play->state)) {
        this->actor.flags &= ~ACTOR_FLAG_10000;
        this->unk48C = 1;
        this->actionFunc = func_808F5A94;
    } else {
        func_800B85E0(&this->actor, play, 200.0f, PLAYER_AP_MINUS1);
    }
}

void func_808F3B40(EnIn* this, PlayState* play) {
    u16 textId;

    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actor.flags |= ACTOR_FLAG_10000;
        this->actionFunc = func_808F3AD4;
        textId = gSaveContext.save.day != 3 ? 0x3481 : 0x34A4;
        this->actor.textId = textId;
    } else {
        Actor_PickUp(&this->actor, play, GI_MILK, 500.0f, 100.0f);
    }
}

void func_808F3BD4(EnIn* this, PlayState* play) {
    if (Actor_ProcessTalkRequest(&this->actor, &play->state)) {
        this->actor.flags &= ~ACTOR_FLAG_10000;
        this->unk48C = 1;
        this->actionFunc = func_808F5A94;
    } else {
        func_800B85E0(&this->actor, play, 200.0f, PLAYER_AP_MINUS1);
    }
}

void func_808F3C40(EnIn* this, PlayState* play) {
    u16 textId;

    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actor.flags |= ACTOR_FLAG_10000;
        this->actionFunc = func_808F3BD4;
        textId = gSaveContext.save.day != 3 ? 0x346A : 0x3492;
        this->actor.textId = textId;
    } else {
        Actor_PickUp(&this->actor, play, GI_MILK, 500.0f, 100.0f);
    }
}

void func_808F3CD4(EnIn* this, PlayState* play) {
    if (Actor_ProcessTalkRequest(&this->actor, &play->state)) {
        this->actor.flags &= ~ACTOR_FLAG_10000;
        this->unk48C = 1;
        this->actionFunc = func_808F5A94;
    } else {
        func_800B85E0(&this->actor, play, 200.0f, PLAYER_AP_MINUS1);
    }
}

void func_808F3D40(EnIn* this, PlayState* play) {
    u16 textId;

    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = func_808F3CD4;
        textId = gSaveContext.save.day != 3 ? 0x347D : 0x34A0;
        this->actor.textId = textId;
        this->actor.flags |= ACTOR_FLAG_10000;
    } else {
        Actor_PickUp(&this->actor, play, GI_MASK_GARO, 500.0f, 100.0f);
    }
}

u16 func_808F3DD4(PlayState* play, EnIn* this, u32 arg2) {
    u16 textId = 0;

    if (Player_GetMask(play) == PLAYER_MASK_CIRCUS_LEADER) {
        if (!(gSaveContext.save.weekEventReg[63] & 0x40)) {
            textId = 0x34A9;
        } else if (this->unk4AC & 8) {
            textId = 0x34B1;
        } else {
            textId = 0x34AF;
        }
        return textId;
    }

    switch (arg2) {
        case 0:
            if ((gSaveContext.save.playerForm == PLAYER_FORM_ZORA) ||
                (gSaveContext.save.playerForm == PLAYER_FORM_GORON)) {
                textId = 0x345C;
            } else if (gSaveContext.save.playerForm == PLAYER_FORM_DEKU) {
                textId = 0x3460;
            } else if (!(gSaveContext.save.weekEventReg[15] & 8)) {
                textId = 0x3458;
            } else {
                textId = 0x345B;
            }
            break;
        case 1:
            if (!(gSaveContext.save.weekEventReg[15] & 0x10)) {
                textId = 0x3463;
            } else {
                textId = 0x346B;
            }
            break;
        case 3:
            if (gSaveContext.save.playerForm == PLAYER_FORM_DEKU) {
                textId = 0x3485;
            } else if (gSaveContext.save.playerForm == PLAYER_FORM_ZORA ||
                       gSaveContext.save.playerForm == PLAYER_FORM_GORON) {
                textId = 0x3484;
            } else if (!(gSaveContext.save.weekEventReg[56] & 4)) {
                textId = 0x346D;
            } else {
                textId = 0x3482;
            }
            break;
        case 4:
            if (gSaveContext.save.playerForm == PLAYER_FORM_ZORA || gSaveContext.save.playerForm == PLAYER_FORM_GORON) {
                textId = 0x348A;
            } else if (gSaveContext.save.playerForm == PLAYER_FORM_DEKU) {
                textId = 0x348B;
            } else if (!(gSaveContext.save.weekEventReg[16] & 1)) {
                textId = 0x3486;
            } else {
                textId = 0x3489;
            }
            break;
        case 5:
            if (func_808F33B8()) {
                textId = 0x34B3;
            } else if (!(gSaveContext.save.weekEventReg[16] & 2)) {
                textId = 0x348E;
            } else {
                textId = 0x3493;
            }
            break;
        case 7:
            if (gSaveContext.save.playerForm == PLAYER_FORM_DEKU) {
                textId = 0x34A8;
            } else if (gSaveContext.save.playerForm == PLAYER_FORM_ZORA ||
                       gSaveContext.save.playerForm == PLAYER_FORM_GORON) {
                textId = 0x34A7;
            } else if (!(gSaveContext.save.weekEventReg[16] & 4)) {
                textId = 0x3495;
            } else {
                textId = 0x34A5;
            }
    }
    if (textId == 0) {
        textId = 1;
    }
    return textId;
}

void func_808F4054(PlayState* play, EnIn* this, s32 arg2, u16 textId) {
    s32 unused;

    if (textId == 0x34AE || textId == 0x34B0 || textId == 0x34B2) {
        unused = textId;
        func_80151BB4(play, 0x11);
    }
    switch (arg2) {
        case 1:
        case 5:
        case 7:
            if (textId == 0x3473) {
                func_808F35D8(this, play);
            }
            break;
        case 2:
            if (textId == 0x3473) {
                func_808F35D8(this, play);
            }
            break;
        case 0:
            break;
    }
}

void func_808F4108(EnIn* this, PlayState* play, u16 textId) {
    Actor_ChangeFocus(&this->actor, play, &this->unk4A4->actor);
    this->actor.textId = 0;
    this->unk4A4->actor.textId = textId;
    this->unk4A4->unk48C = 2;
    this->unk48C = 0;
}

s32 func_808F4150(PlayState* play, EnIn* this, s32 arg2, MessageContext* msgCtx) {
    EnIn* this2 = this;

    if (msgCtx->choiceIndex == 0) {
        func_8019F208();
        if (gSaveContext.save.playerData.rupees >= play->msgCtx.unk1206C) {
            Rupees_ChangeBy(-play->msgCtx.unk1206C);
            if (!(gSaveContext.save.weekEventReg[57] & 1)) {
                func_808F4108(this, play, 0x3474);
            } else if (this->unk4AC & 8) {
                func_808F4108(this, play, 0x3475);
            } else {
                Actor_ContinueText(play, &this->actor, 0x3475);
            }
        } else {
            play_sound(NA_SE_SY_ERROR);
            Actor_ContinueText(play, &this->actor, 0x3473);
        }
    } else {
        func_8019F230();
        Actor_ContinueText(play, &this->actor, 0x3472);
    }
    return 0;
}

s32 func_808F4270(PlayState* play, EnIn* this, s32 arg2, MessageContext* msgCtx, s32 arg4) {
    s32 pad;
    s32 fee = play->msgCtx.unk1206C != 0xFFFF ? play->msgCtx.unk1206C : 10;

    if (msgCtx->choiceIndex == 0) {
        func_8019F208();
        if (gSaveContext.save.playerData.rupees >= fee) {
            Rupees_ChangeBy(-fee);
            if (!(gSaveContext.save.weekEventReg[57] & 1)) {
                if (arg4 != 0) {
                    Actor_ContinueText(play, &this->actor, 0x3474);
                } else {
                    func_808F4108(this, play, 0x3474);
                }
            } else {
                if (arg4 != 0) {
                    Actor_ContinueText(play, &this->actor, 0x3475);
                } else {
                    func_808F4108(this, play, 0x3475);
                }
            }
        } else {
            play_sound(NA_SE_SY_ERROR);
            if (arg4 != 0) {
                Actor_ContinueText(play, &this->actor, 0x3473);
            } else {
                func_808F4108(this, play, 0x3473);
            }
        }
    } else {
        func_8019F230();
        Actor_ContinueText(play, &this->actor, 0x3472);
    }
    return 0;
}

s32 func_808F43E0(EnIn* this) {
    this->unk48C = 0;
    this->actor.textId = 0;
    SET_RACE_FLAGS(RACE_FLAG_END);
    return 0;
}

s32 func_808F4414(PlayState* play, EnIn* this, s32 arg2) {
    u16 textId = this->actor.textId;
    s32 ret = true;
    MessageContext* msgCtx = &play->msgCtx;
    s32 pad[2];

    switch (textId) {
        case 0x34AF:
            Actor_ContinueText(play, &this->actor, 0x34B0);
            ret = false;
            break;
        case 0x34A9:
            func_808F4108(this, play, 0x34AA);
            gSaveContext.save.weekEventReg[63] |= 0x40;
            ret = false;
            break;
        case 0x34AA:
            func_808F4108(this, play, 0x34AB);
            ret = false;
            break;
        case 0x34AB:
            func_808F4108(this, play, 0x34AC);
            ret = false;
            break;
        case 0x34AC:
            func_808F4108(this, play, 0x34AD);
            ret = false;
            break;
        case 0x34AD:
            Actor_ContinueText(play, &this->actor, 0x34AE);
            ret = false;
            break;
        case 0x34B1:
            Actor_ContinueText(play, &this->actor, 0x34B2);
            ret = false;
            break;
        default:
            break;
    }

    switch (arg2) {
        case 0:
            switch (textId) {
                case 0x3458:
                    gSaveContext.save.weekEventReg[15] |= 8;
                    Actor_ContinueText(play, &this->actor, 0x3459);
                    ret = false;
                    break;
                case 0x3459:
                    Actor_ContinueText(play, &this->actor, 0x345A);
                    ret = false;
                    break;
                case 0x345A:
                    func_80151BB4(play, 0x11);
                    ret = true;
                    break;
                case 0x345B:
                    func_80151BB4(play, 0x11);
                    ret = true;
                    break;
                case 0x345C:
                    Actor_ContinueText(play, &this->actor, 0x345D);
                    ret = false;
                    break;
                case 0x345D:
                    Actor_ContinueText(play, &this->actor, 0x345E);
                    ret = false;
                    break;
                case 0x345E:
                    Actor_ContinueText(play, &this->actor, 0x345F);
                    ret = false;
                    break;
                case 0x345F:
                    func_80151BB4(play, 0x11);
                    ret = true;
                    break;
                case 0x3460:
                    Actor_ContinueText(play, &this->actor, 0x3461);
                    ret = false;
                    break;
                case 0x3461:
                    Actor_ContinueText(play, &this->actor, 0x3462);
                    ret = false;
                    break;
                case 0x3462:
                    func_80151BB4(play, 0x11);
                    ret = true;
                    break;
            }
            break;
        case 1:
            switch (textId) {
                case 0x3463:
                    gSaveContext.save.weekEventReg[15] |= 0x10;
                    Actor_ContinueText(play, &this->actor, 0x3464);
                    ret = false;
                    break;
                case 0x3464:
                    Actor_ContinueText(play, &this->actor, 0x3465);
                    ret = false;
                    break;
                case 0x3465:
                    Actor_ContinueText(play, &this->actor, 0x3466);
                    ret = false;
                    break;
                case 0x3466:
                    if (msgCtx->choiceIndex == 0) {
                        func_8019F208();
                        if (gSaveContext.save.playerData.rupees >= play->msgCtx.unk1206C) {
                            if (Inventory_HasEmptyBottle()) {
                                this->actionFunc = func_808F3C40;
                                Actor_PickUp(&this->actor, play, GI_MILK, 500.0f, 100.0f);
                                Rupees_ChangeBy(-play->msgCtx.unk1206C);
                                ret = true;
                            } else {
                                Actor_ContinueText(play, &this->actor, 0x3469);
                                ret = false;
                            }
                        } else {
                            play_sound(NA_SE_SY_ERROR);
                            Actor_ContinueText(play, &this->actor, 0x3468);
                            ret = false;
                        }
                    } else {
                        func_8019F230();
                        Actor_ContinueText(play, &this->actor, 0x3467);
                        ret = false;
                    }
                    break;
                case 0x3467:
                case 0x3468:
                case 0x3469:
                    func_80151BB4(play, 0x11);
                    ret = true;
                    break;
                case 0x346B:
                    Actor_ContinueText(play, &this->actor, 0x346C);
                    ret = false;
                    break;
                case 0x346C:
                    Actor_ContinueText(play, &this->actor, 0x3466);
                    ret = false;
                    break;
                case 0x346A:
                    this->actionFunc = func_808F5A94;
                    func_80151BB4(play, 0x11);
                    break;
            }
            break;
        case 2:
            switch (textId) {
                case 0x346E:
                    func_808F4108(this, play, 0x346F);
                    ret = false;
                    break;
                case 0x3470:
                    Actor_ContinueText(play, &this->actor, 0x3471);
                    ret = false;
                    break;
                case 0x3483:
                    Actor_ContinueText(play, &this->actor, 0x3471);
                    ret = false;
                    break;
                case 0x3471:
                    func_808F4150(play, this, arg2, msgCtx);
                    ret = false;
                    break;
                case 0x3472:
                    func_808F43E0(this);
                    gSaveContext.save.weekEventReg[56] &= (u8)~8;
                    func_80151BB4(play, 0x11);
                    ret = true;
                    break;
                case 0x3473:
                    gSaveContext.save.weekEventReg[56] &= (u8)~8;
                    func_80151BB4(play, 0x11);
                    break;
                case 0x3475:
                    SET_RACE_FLAGS(RACE_FLAG_START);
                    func_800FD750(NA_BGM_HORSE);
                    play->nextEntrance = ENTRANCE(GORMAN_TRACK, 5);
                    play->transitionType = TRANS_TYPE_05;
                    play->transitionTrigger = TRANS_TRIGGER_START;
                    gSaveContext.save.weekEventReg[57] |= 1;
                    break;
                case 0x3478:
                    if (msgCtx->choiceIndex == 0) {
                        func_808F4150(play, this, arg2, msgCtx);
                        ret = false;
                    } else {
                        func_8019F230();
                        gSaveContext.save.weekEventReg[56] &= (u8)~8;
                        func_808F4108(this, play, 0x3479);
                        ret = false;
                    }
                    break;
                case 0x347B:
                    func_808F4108(this, play, 0x347C);
                    gSaveContext.save.weekEventReg[56] &= (u8)~8;
                    ret = false;
                    break;
            }
            break;
        case 3:
            switch (textId) {
                case 0x346D:
                    func_808F4108(this, play, 0x346E);
                    gSaveContext.save.weekEventReg[56] |= 4;
                    gSaveContext.save.weekEventReg[56] |= 8;
                    ret = false;
                    break;
                case 0x346F:
                    func_808F4108(this, play, 0x3470);
                    ret = false;
                    break;
                case 0x3482:
                    func_808F4108(this, play, 0x3483);
                    gSaveContext.save.weekEventReg[56] |= 8;
                    ret = false;
                    break;
                case 0x3484:
                    func_80151BB4(play, 0x11);
                    break;
                case 0x3485:
                    func_80151BB4(play, 0x11);
                    break;
                case 0x3474:
                    func_808F4108(this, play, 0x3475);
                    ret = false;
                    break;
                case 0x3476:
                    Actor_ContinueText(play, &this->actor, 0x3477);
                    EnIn_ChangeAnim(&this->skelAnime, ENIN_ANIM_1);
                    EnIn_ChangeAnim(&this->unk4A4->skelAnime, ENIN_ANIM_7);
                    ret = false;
                    break;
                case 0x3477:
                    gSaveContext.save.weekEventReg[56] |= 8;
                    func_808F4108(this, play, 0x3478);
                    ret = false;
                    break;
                case 0x347A:
                    EnIn_ChangeAnim(&this->skelAnime, ENIN_ANIM_1);
                    EnIn_ChangeAnim(&this->unk4A4->skelAnime, ENIN_ANIM_7);
                    if (INV_CONTENT(ITEM_MASK_GARO) == ITEM_MASK_GARO) {
                        Actor_ContinueText(play, &this->actor, 0x347E);
                        ret = false;
                    } else {
                        gSaveContext.save.weekEventReg[56] |= 8;
                        func_808F4108(this, play, 0x347B);
                        ret = false;
                    }
                    break;
                case 0x347E:
                    func_808F35D8(this, play);
                    if (Inventory_HasEmptyBottle()) {
                        this->actionFunc = func_808F3B40;
                        Actor_PickUp(&this->actor, play, GI_MILK, 500.0f, 100.0f);
                        ret = true;
                    } else {
                        Actor_ContinueText(play, &this->actor, 0x347F);
                        ret = false;
                    }
                    break;
                case 0x347F:
                    Actor_ContinueText(play, &this->actor, 0x3480);
                    ret = false;
                    break;
                case 0x3480:
                    func_808F43E0(this);
                    func_80151BB4(play, 0x11);
                    ret = true;
                    break;
                case 0x3479:
                    func_808F43E0(this);
                    func_808F35D8(this, play);
                    func_80151BB4(play, 0x11);
                    ret = true;
                    break;
                case 0x347C:
                    this->actionFunc = func_808F3D40;
                    Actor_PickUp(&this->actor, play, GI_MASK_GARO, 500.0f, 100.0f);
                    func_808F35D8(this, play);
                    ret = true;
                    break;
                case 0x3481:
                    this->actionFunc = func_808F5A34;
                    func_808F43E0(this);
                    func_80151BB4(play, 0x11);
                    ret = true;
                    break;
                case 0x347D:
                    this->actionFunc = func_808F5A34;
                    func_808F43E0(this);
                    func_80151BB4(play, 0x11);
                    func_80151BB4(play, 0x2F);
                    ret = true;
                    break;
            }
            break;
        case 4:
            switch (textId) {
                case 0x3486:
                    Actor_ContinueText(play, &this->actor, 0x3487);
                    gSaveContext.save.weekEventReg[16] |= 1;
                    ret = false;
                    break;
                case 0x3487:
                    Actor_ContinueText(play, &this->actor, 0x3488);
                    ret = false;
                    break;
                case 0x3488:
                    func_80151BB4(play, 0x11);
                    ret = true;
                    break;
                case 0x3489:
                    func_80151BB4(play, 0x11);
                    break;
                case 0x348A:
                    func_80151BB4(play, 0x11);
                    break;
                case 0x348B:
                    Actor_ContinueText(play, &this->actor, 0x348C);
                    ret = false;
                    break;
                case 0x348C:
                    Actor_ContinueText(play, &this->actor, 0x348D);
                    ret = false;
                    break;
                case 0x348D:
                    func_80151BB4(play, 0x11);
                    ret = true;
                    break;
            }
            break;
        case 5:
            switch (textId) {
                case 0x3468:
                case 0x3469:
                case 0x3491:
                    func_80151BB4(play, 0x11);
                    break;
                case 0x348E:
                case 0x34B3:
                    Actor_ContinueText(play, &this->actor, 0x348F);
                    gSaveContext.save.weekEventReg[16] |= 2;
                    ret = false;
                    break;
                case 0x3493:
                    Actor_ContinueText(play, &this->actor, 0x3494);
                    ret = false;
                    break;
                case 0x348F:
                case 0x3494:
                    Actor_ContinueText(play, &this->actor, 0x3490);
                    ret = false;
                    break;
                case 0x3490:
                    if (msgCtx->choiceIndex == 0) {
                        func_8019F208();
                        if (gSaveContext.save.playerData.rupees >= play->msgCtx.unk1206C) {
                            if (Inventory_HasEmptyBottle()) {
                                this->actionFunc = func_808F3C40;
                                Actor_PickUp(&this->actor, play, GI_MILK, 500.0f, 100.0f);
                                Rupees_ChangeBy(-play->msgCtx.unk1206C);
                                ret = true;
                            } else {
                                Actor_ContinueText(play, &this->actor, 0x3469);
                                ret = false;
                            }
                        } else {
                            play_sound(NA_SE_SY_ERROR);
                            Actor_ContinueText(play, &this->actor, 0x3468);
                            ret = false;
                        }
                    } else {
                        func_8019F230();
                        Actor_ContinueText(play, &this->actor, 0x3491);
                        ret = false;
                    }
                    break;
                case 0x3492:
                    this->actionFunc = func_808F5A94;
                    func_80151BB4(play, 0x11);
                    break;
            }
            break;
        case 7:
            switch (textId) {
                case 0x34A8:
                    func_80151BB4(play, 0x11);
                    break;
                case 0x34A7:
                    func_80151BB4(play, 0x11);
                    break;
                case 0x3495:
                    func_808F4108(this, play, 0x3496);
                    gSaveContext.save.weekEventReg[16] |= 4;
                    gSaveContext.save.weekEventReg[56] |= 8;
                    ret = false;
                    break;
                case 0x3497:
                    func_808F4108(this, play, 0x3498);
                    ret = false;
                    break;
                case 0x34A4:
                    this->actionFunc = func_808F5A34;
                    func_808F43E0(this);
                    func_80151BB4(play, 0x11);
                    ret = true;
                    break;
                case 0x34A5:
                    func_808F4108(this, play, 0x34A6);
                    gSaveContext.save.weekEventReg[56] |= 8;
                    ret = false;
                    break;
                case 0x3473:
                    gSaveContext.save.weekEventReg[56] &= (u8)~8;
                    func_80151BB4(play, 0x11);
                    break;
                case 0x3474:
                    Actor_ContinueText(play, &this->actor, 0x3475);
                    ret = false;
                    break;
                case 0x3475:
                    SET_RACE_FLAGS(RACE_FLAG_START);
                    func_800FD750(NA_BGM_HORSE);
                    play->nextEntrance = ENTRANCE(GORMAN_TRACK, 5);
                    play->transitionType = TRANS_TYPE_05;
                    play->transitionTrigger = TRANS_TRIGGER_START;
                    gSaveContext.save.weekEventReg[57] |= 1;
                    break;
                case 0x349D:
                    EnIn_ChangeAnim(&this->skelAnime, ENIN_ANIM_1);
                    EnIn_ChangeAnim(&this->unk4A4->skelAnime, ENIN_ANIM_7);
                    if (INV_CONTENT(ITEM_MASK_GARO) == ITEM_MASK_GARO) {
                        Actor_ContinueText(play, &this->actor, 0x34A1);
                        ret = false;
                    } else {
                        gSaveContext.save.weekEventReg[56] |= 8;
                        func_808F4108(this, play, 0x349E);
                        ret = false;
                    }
                    break;
                case 0x349F:
                    this->actionFunc = func_808F3D40;
                    Actor_PickUp(&this->actor, play, GI_MASK_GARO, 500.0f, 100.0f);
                    func_808F35D8(this, play);
                    ret = true;
                    break;
                case 0x34A0:
                    this->actionFunc = func_808F5A34;
                    func_80151BB4(play, 0x11);
                    func_80151BB4(play, 0x2F);
                    func_808F43E0(this);
                    ret = true;
                    break;
                case 0x34A1:
                    func_808F35D8(this, play);
                    if (Inventory_HasEmptyBottle()) {
                        this->actionFunc = func_808F3B40;
                        Actor_PickUp(&this->actor, play, GI_MILK, 500.0f, 100.0f);
                        ret = true;
                    } else {
                        Actor_ContinueText(play, &this->actor, 0x34A2);
                        ret = false;
                    }
                    break;
                case 0x34A2:
                    Actor_ContinueText(play, &this->actor, 0x34A3);
                    ret = false;
                    break;
                case 0x34A3:
                    func_808F43E0(this);
                    func_80151BB4(play, 0x11);
                    ret = true;
                    break;
                case 0x3499:
                    Actor_ContinueText(play, &this->actor, 0x349A);
                    EnIn_ChangeAnim(&this->skelAnime, ENIN_ANIM_1);
                    EnIn_ChangeAnim(&this->unk4A4->skelAnime, ENIN_ANIM_7);
                    ret = false;
                    break;
                case 0x349A:
                    Actor_ContinueText(play, &this->actor, 0x349B);
                    ret = false;
                    break;
                case 0x349B:
                    if (msgCtx->choiceIndex == 0) {
                        func_808F4270(play, this, arg2, msgCtx, 1);
                        ret = false;
                    } else {
                        func_8019F230();
                        Actor_ContinueText(play, &this->actor, 0x349C);
                        ret = false;
                    }
                    break;
                case 0x349C:
                    func_808F43E0(this);
                    func_808F35D8(this, play);
                    func_80151BB4(play, 0x11);
                    ret = true;
                    break;
            }
            break;
        case 6:
            switch (textId) {
                case 0x3496:
                    func_808F4108(this, play, 0x3497);
                    ret = false;
                    break;
                case 0x3498:
                case 0x34A6:
                    Actor_ContinueText(play, &this->actor, 0x3471);
                    ret = false;
                    break;
                case 0x3471:
                    func_808F4270(play, this, arg2, msgCtx, 0);
                    ret = false;
                    break;
                case 0x3472:
                    func_808F43E0(this);
                    gSaveContext.save.weekEventReg[56] &= (u8)~8;
                    func_80151BB4(play, 0x11);
                    ret = true;
                    break;
                case 0x349E:
                    func_808F4108(this, play, 0x349F);
                    gSaveContext.save.weekEventReg[56] &= (u8)~8;
                    ret = false;
                    break;
            }
            break;
    }
    return ret;
}

s32 func_808F5674(PlayState* play, EnIn* this, s32 arg2) {
    s32 pad;
    s32 ret = false;

    switch (Message_GetState(&play->msgCtx)) {
        case TEXT_STATE_CLOSING:
            func_808F4054(play, this, arg2, this->actor.textId);
            ret = true;
            break;
        case TEXT_STATE_CHOICE:
        case TEXT_STATE_5:
            if (Message_ShouldAdvance(play) && func_808F4414(play, this, arg2)) {
                func_801477B4(play);
                ret = true;
            }
            break;
    }
    return ret;
}

s32 func_808F5728(PlayState* play, EnIn* this, s32 arg2, s32* arg3) {
    s16 rotDiff;
    s16 yawDiff;
    Player* player;

    if (*arg3 == 4) {
        return 0;
    }
    if (*arg3 == 2) {
        Message_StartTextbox(play, this->actor.textId, &this->actor);
        *arg3 = 1;
        return 0;
    }
    if (*arg3 == 3) {
        func_80151938(play, this->actor.textId);
        *arg3 = 1;
        return 0;
    }
    if (Actor_ProcessTalkRequest(&this->actor, &play->state)) {
        *arg3 = 1;
        return 1;
    }
    if (*arg3 == 1) {
        s32 requiredScopeTemp;

        player = GET_PLAYER(play);
        func_808F5994(this, play, &player->actor.world.pos, 0xC80);
    } else {
        rotDiff = this->actor.home.rot.y - this->actor.world.rot.y;
        if (rotDiff > 0x320) {
            this->actor.world.rot.y += 0x320;
        } else if (rotDiff < -0x320) {
            this->actor.world.rot.y -= 0x320;
        } else {
            this->actor.world.rot.y += rotDiff;
        }
        this->actor.shape.rot.y = this->actor.world.rot.y;
    }
    if (*arg3 == 1) {
        if (func_808F5674(play, this, arg2)) {
            *arg3 = 0;
        }
        return 0;
    }
    if (!Actor_OnScreen(play, &this->actor)) {
        return 0;
    }
    yawDiff = ABS_ALT(BINANG_SUB(this->actor.yawTowardsPlayer, this->actor.shape.rot.y));
    if (yawDiff >= 0x4300) {
        return 0;
    }
    if (this->actor.xyzDistToPlayerSq > SQ(160.0f) && !this->actor.isTargeted) {
        return 0;
    }
    if (this->actor.xyzDistToPlayerSq <= SQ(80.0f)) {
        if (func_800B8614(&this->actor, play, 80.0f)) {
            this->actor.textId = func_808F3DD4(play, this, arg2);
        }
    } else if (func_800B863C(&this->actor, play)) {
        this->actor.textId = func_808F3DD4(play, this, arg2);
    }
    return 0;
}

s32 func_808F5994(EnIn* this, PlayState* play, Vec3f* arg2, s16 arg3) {
    s32 ret = 0;
    s16 yaw = Math_Vec3f_Yaw(&this->actor.world.pos, arg2) - this->actor.world.rot.y;

    if (yaw > arg3) {
        this->actor.world.rot.y += arg3;
    } else if (yaw < -arg3) {
        this->actor.world.rot.y -= arg3;
    } else {
        this->actor.world.rot.y += yaw;
        ret = 1;
    }
    this->actor.shape.rot.y = this->actor.world.rot.y;

    return ret;
}

void func_808F5A34(EnIn* this, PlayState* play) {
    if (gSaveContext.save.day != 3) {
        func_808F5728(play, this, 3, &this->unk48C);
    } else {
        func_808F5728(play, this, 7, &this->unk48C);
    }
}

void func_808F5A94(EnIn* this, PlayState* play) {
    if (func_800F41E4(play, &play->actorCtx)) {
        if (gSaveContext.save.day == 3) {
            func_808F5728(play, this, 7, &this->unk48C);
        } else {
            func_808F5728(play, this, 3, &this->unk48C);
        }
    } else {
        if (gSaveContext.save.day == 3) {
            func_808F5728(play, this, 5, &this->unk48C);
        } else {
            func_808F5728(play, this, 1, &this->unk48C);
        }
    }
}

void func_808F5B58(EnIn* this, PlayState* play) {
    if (func_800F41E4(play, &play->actorCtx)) {
        if ((Player_GetMask(play) == PLAYER_MASK_CIRCUS_LEADER && gSaveContext.save.weekEventReg[63] & 0x40) ||
            gSaveContext.save.weekEventReg[56] & 8) {
            if (gSaveContext.save.day == 3) {
                func_808F5728(play, this, 6, &this->unk48C);
            } else {
                func_808F5728(play, this, 2, &this->unk48C);
            }
        }
    } else if (Player_GetMask(play) != PLAYER_MASK_CIRCUS_LEADER ||
               (Player_GetMask(play) == PLAYER_MASK_CIRCUS_LEADER && gSaveContext.save.weekEventReg[63] & 0x40)) {
        if (gSaveContext.save.day == 3) {
            func_808F5728(play, this, 4, &this->unk48C);
        } else {
            func_808F5728(play, this, 0, &this->unk48C);
        }
    }
}

void func_808F5C98(EnIn* this, PlayState* play) {
    if (this->unk4B0 == RACE_FLAG_END) {
        this->actionFunc = func_808F5B58;
    }
    if ((Player_GetMask(play) == PLAYER_MASK_CIRCUS_LEADER && gSaveContext.save.weekEventReg[63] & 0x40) ||
        gSaveContext.save.weekEventReg[56] & 8) {
        if (gSaveContext.save.day != 3) {
            func_808F5728(play, this, 2, &this->unk48C);
        } else {
            func_808F5728(play, this, 6, &this->unk48C);
        }
    }
    if (this->unk4A8 == 2) {
        if (this->unk4B0 == RACE_FLAG_2) {
            Actor_PlaySfxAtPos(&this->actor, NA_SE_VO_IN_LOST);
        } else {
            Actor_PlaySfxAtPos(&this->actor, NA_SE_VO_IN_JOY0);
        }
        this->unk4A8 = 3;
    } else if (this->unk4A8 < 3) {
        this->unk4A8++;
    }
}

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneScale, 1200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 300, ICHAIN_STOP),
};

void EnIn_Init(Actor* thisx, PlayState* play) {
    EnIn* this = THIS;
    s32 pad[2];
    s16 type;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    SkelAnime_InitFlex(play, &this->skelAnime, &object_in_Skel_014EA8, NULL, this->jointTable, this->morphTable, 20);
    EnIn_ChangeAnim(&this->skelAnime, ENIN_ANIM_0);
    Collider_InitCylinder(play, &this->colliderCylinder);
    Collider_SetCylinder(play, &this->colliderCylinder, &this->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&this->actor.colChkInfo, &sDamageTable, &sColChkInfoInit2);
    this->unk48A = 0;
    this->unk48C = 0;
    this->unk4AC = 0;
    type = ENIN_GET_TYPE(thisx);
    this->unk4B0 = GET_RACE_FLAGS;
    if (type == ENIN_HORSE_RIDER_BLUE_SHIRT || type == ENIN_BLUE_SHIRT) {
        this->unk4AC |= 8;
    }
    if (type == ENIN_HORSE_RIDER_YELLOW_SHIRT || type == ENIN_HORSE_RIDER_BLUE_SHIRT) {
        ActorShape_Init(&this->actor.shape, 0.0f, NULL, 0.0f);
        this->unk488 = 1;
        Animation_Change(&this->skelAnime, &object_in_Anim_016A60, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_in_Anim_016A60), ANIMMODE_ONCE, 0.0f);
        Actor_SetScale(&this->actor, 0.01f);
        this->unk23C = 0;
        this->unk23D = 1;
        this->actionFunc = func_808F374C;
    } else {
        Collider_InitJntSph(play, &this->colliderJntSph);
        Collider_SetJntSph(play, &this->colliderJntSph, &this->actor, &sJntSphInit, &this->colliderJntSphElement);
        Actor_SetScale(&this->actor, 0.01f);
        this->actor.gravity = -4.0f;
        this->path = SubS_GetPathByIndex(play, ENIN_GET_PATH(&this->actor), 0x3F);
        this->unk23D = 0;
        if (type == ENIN_YELLOW_SHIRT || type == ENIN_BLUE_SHIRT) {
            if (GET_RACE_FLAGS == RACE_FLAG_2 || GET_RACE_FLAGS == RACE_FLAG_3) {
                gSaveContext.save.weekEventReg[56] &= (u8)~8;
                this->unk4A8 = 0;
                this->unk4AC |= 2;
                func_808F35AC(this, play);
                this->unk23C = 0;
                D_801BDAA0 = false;
                if (GET_RACE_FLAGS == RACE_FLAG_2) {
                    EnIn_ChangeAnim(&this->skelAnime, ENIN_ANIM_6);
                } else {
                    EnIn_ChangeAnim(&this->skelAnime, ENIN_ANIM_4);
                }
                if (GET_RACE_FLAGS == RACE_FLAG_2) {
                    this->skelAnime.curFrame = ((Rand_ZeroOne() * 0.6f) + 0.2f) * this->skelAnime.endFrame;
                }
                if (this->unk4AC & 8) {
                    this->actionFunc = func_808F39DC;
                } else {
                    this->actionFunc = func_808F5C98;
                }
            } else {
                if (GET_RACE_FLAGS != RACE_FLAG_START) {
                    gSaveContext.save.weekEventReg[56] &= (u8)~8;
                    this->unk23C = 0;
                    this->unk4AC |= 2;
                    if (type == ENIN_BLUE_SHIRT) {
                        if (func_808F33B8()) {
                            EnIn_ChangeAnim(&this->skelAnime, ENIN_ANIM_0);
                            this->actionFunc = func_808F5A94;
                        } else {
                            if (gSaveContext.save.weekEventReg[52] & 1) {
                                Actor_Spawn(&play->actorCtx, play, ACTOR_EN_KANBAN, this->actor.world.pos.x,
                                            this->actor.world.pos.y, this->actor.world.pos.z, this->actor.shape.rot.x,
                                            this->actor.shape.rot.y, this->actor.shape.rot.z, 0xF);
                                Actor_MarkForDeath(&this->actor);
                            } else {
                                EnIn_ChangeAnim(&this->skelAnime, ENIN_ANIM_0);
                                this->actionFunc = func_808F5A94;
                            }
                        }
                    } else {
                        if (gSaveContext.save.weekEventReg[52] & 1) {
                            Actor_MarkForDeath(&this->actor);
                        } else {
                            EnIn_ChangeAnim(&this->skelAnime, ENIN_ANIM_7);
                            this->actionFunc = func_808F5B58;
                        }
                    }
                } else {
                    Actor_MarkForDeath(&this->actor);
                }
            }
        } else {
            this->actionFunc = func_808F3618;
        }
    }
}

void EnIn_Destroy(Actor* thisx, PlayState* play) {
    EnIn* this = THIS;

    Collider_DestroyCylinder(play, &this->colliderCylinder);
}

void EnIn_Update(Actor* thisx, PlayState* play) {
    EnIn* this = THIS;

    func_808F3310(this, play);
    func_808F3334(this, play);
    if (this->unk4AC & 2) {
        this->unk4AC &= ~2;
        func_808F38F8(this, play);
    }
    if (Player_GetMask(play) == PLAYER_MASK_CIRCUS_LEADER) {
        this->unk4AC |= 0x40;
    } else {
        this->unk4AC &= ~0x40;
    }
    this->actionFunc(this, play);
    Actor_UpdateBgCheckInfo(play, &this->actor, 0.0f, 0.0f, 0.0f, 0x4);
    func_808F3414(this, play);
    func_808F32A0(this, play);
}

void func_808F6334(EnIn* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 talkState = Message_GetState(&play->msgCtx);

    this->unk4C4 += this->unk4C0 != 0.0f ? 40.0f : -40.0f;
    this->unk4C4 = CLAMP(this->unk4C4, 0.0f, 80.0f);

    Matrix_Translate(this->unk4C4, 0.0f, 0.0f, MTXMODE_APPLY);
    if ((&this->actor == player->targetActor) &&
        !((play->msgCtx.currentTextId >= 0xFF) && (play->msgCtx.currentTextId <= 0x200)) &&
        (talkState == TEXT_STATE_3) && (this->prevTalkState == TEXT_STATE_3)) {
        if (!(play->state.frames & 1)) {
            this->unk4C0 = this->unk4C0 != 0.0f ? 0.0f : 1.0f;
        }
    } else {
        this->unk4C0 = 0.0f;
    }
    this->prevTalkState = talkState;
}

s32 EnIn_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnIn* this = THIS;
    s32 pad;
    Gfx* sp50[] = {
        NULL,
        NULL,
        object_in_DL_0149A8,
        object_in_DL_014AE0,
        object_in_DL_014C30,
        object_in_DL_0145D8,
        object_in_DL_014710,
        object_in_DL_014860,
        object_in_DL_014420,
        object_in_DL_012A78,
        object_in_DL_013DE0,
        object_in_DL_013F10,
        object_in_DL_014040,
        object_in_DL_0137A0,
        object_in_DL_0138D0,
        object_in_DL_013A00,
        object_in_DL_012DF8,
        object_in_DL_013670,
        object_in_DL_013540,
        object_in_DL_013440,
    };

    if (this->unk23C != 0 && limbIndex != 16) {
        if (sp50[limbIndex] != NULL) {
            *dList = sp50[limbIndex];
        }
    }
    if (this->unk4AC & 4 && limbIndex == 16) {
        *dList = object_in_DL_01C528;
    }

    OPEN_DISPS(play->state.gfxCtx);
    if (limbIndex == 16) {
        TexturePtr sp38[] = { object_in_Tex_0035E0, object_in_Tex_004820, object_in_Tex_004C20, object_in_Tex_0043E0 };

        gSPSegment(POLY_OPA_DISP++, 0x08, Lib_SegmentedToVirtual(sp38[this->unk482]));
        gSPSegment(POLY_OPA_DISP++, 0x09, Lib_SegmentedToVirtual(object_in_Tex_003520));
    } else {
        AnimatedMat_DrawStep(play, Lib_SegmentedToVirtual(object_in_Matanimheader_001C30), this->unk4AC & 8 ? 1 : 0);
    }
    CLOSE_DISPS(play->state.gfxCtx);

    if (limbIndex == 16) {
        Matrix_Translate(1500.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        Matrix_RotateXS(this->headRot.y, MTXMODE_APPLY);
        Matrix_RotateZS(-this->headRot.x, MTXMODE_APPLY);
        Matrix_Translate(-1500.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        func_808F6334(this, play);
    }
    if (limbIndex == 9) {
        Matrix_RotateYS(this->torsoRot.y, MTXMODE_APPLY);
        Matrix_RotateXS(this->torsoRot.x, MTXMODE_APPLY);
    }
    if (limbIndex == 9 || limbIndex == 10 || limbIndex == 13) {
        rot->y += (s16)(Math_SinS(this->unk376[limbIndex]) * 200.0f);
        rot->z += (s16)(Math_CosS(this->unk39E[limbIndex]) * 200.0f);
    }
    if (this->unk4AC & 0x40) {
        if (limbIndex == 18) {
            rot->x = 0x1F40;
            rot->y = -0x3E8;
            rot->z = 0x1D4C;
        } else if (limbIndex == 17) {
            rot->x = -0x1F40;
            rot->y = 0x3E8;
            rot->z = 0x1D4C;
        } else if (limbIndex == 19) {
            rot->x = 0;
            rot->y = 0x7FFF;
            rot->z = -0x2328;
        }
    }
    return 0;
}

void EnIn_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    EnIn* this = THIS;
    Vec3f sp50 = { 1600.0f, 0.0f, 0.0f };
    Vec3f sp44 = { 0.0f, 0.0f, 0.0f };

    if (limbIndex == 16) {
        Matrix_MultVec3f(&sp50, &this->unk4B4);
        Math_Vec3f_Copy(&this->actor.focus.pos, &this->unk4B4);
    }
    if (this->unk23D == 0) {
        Collider_UpdateSpheres(limbIndex, &this->colliderJntSph);
        if (limbIndex == 4) {
            Matrix_MultVec3f(&sp44, &this->unk248);
        }
        if (limbIndex == 7) {
            Matrix_MultVec3f(&sp44, &this->unk254);
        }
        if (this->unk23C == 0) {
            if (!(this->unk4AC & 8)) {
                OPEN_DISPS(play->state.gfxCtx);
                if (limbIndex == 12) {
                    gSPDisplayList(POLY_OPA_DISP++, object_in_DL_007A70);
                }
                if (limbIndex == 15) {
                    gSPDisplayList(POLY_OPA_DISP++, object_in_DL_007C48);
                }
                CLOSE_DISPS(play->state.gfxCtx);
            }
        }
    }
    if (this->unk4AC & 0x20) {
        OPEN_DISPS(play->state.gfxCtx);
        if (limbIndex == 12) {
            gSPDisplayList(POLY_OPA_DISP++, object_in_DL_007C48);
        }
        CLOSE_DISPS(play->state.gfxCtx);
    }
}

void EnIn_Draw(Actor* thisx, PlayState* play) {
    EnIn* this = THIS;

    OPEN_DISPS(play->state.gfxCtx);
    func_8012C28C(play->state.gfxCtx);
    gDPPipeSync(POLY_OPA_DISP++);
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                          EnIn_OverrideLimbDraw, EnIn_PostLimbDraw, &this->actor);
    CLOSE_DISPS(play->state.gfxCtx);
}
