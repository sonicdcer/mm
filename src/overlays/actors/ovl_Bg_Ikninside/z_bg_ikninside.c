/*
 * File: z_bg_ikninside.c
 * Overlay: ovl_Bg_Ikninside
 * Description: Ancient Castle of Ikana Objects
 */

#include "z_bg_ikninside.h"
#include "objects/object_ikninside_obj/object_ikninside_obj.h"

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((BgIkninside*)thisx)

void BgIkninside_Init(Actor* thisx, GlobalContext* globalCtx);
void BgIkninside_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgIkninside_Update(Actor* thisx, GlobalContext* globalCtx);
void BgIkninside_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80C07220(BgIkninside* this, GlobalContext* globalCtx);
void func_80C07230(BgIkninside* this, GlobalContext* globalCtx);
void func_80C072D0(BgIkninside* this, GlobalContext* globalCtx);

const ActorInit Bg_Ikninside_InitVars = {
    ACTOR_BG_IKNINSIDE,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_IKNINSIDE_OBJ,
    sizeof(BgIkninside),
    (ActorFunc)BgIkninside_Init,
    (ActorFunc)BgIkninside_Destroy,
    (ActorFunc)BgIkninside_Update,
    (ActorFunc)BgIkninside_Draw,
};

static Gfx* D_80C076A0[] = { object_ikninside_obj_DL_00A748, object_ikninside_obj_DL_00A5A8 };

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x80000008, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 32, 32, 0, { 0, 0, 0 } },
};

static Vec3f D_80C076D4 = { 0.0f, -1.0f, 0.0f };

void BgIkninside_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgIkninside* this = THIS;
    CollisionHeader* colHeader = NULL;
    s16 pad;

    Actor_SetScale(&this->dyna.actor, 0.1f);
    this->actionFunc = func_80C072D0;
    DynaPolyActor_Init(&this->dyna, 0);
    CollisionHeader_GetVirtual(&object_ikninside_obj_Colheader_00DE48, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, colHeader);
    Collider_InitAndSetCylinder(globalCtx, &this->collider, &this->dyna.actor, &sCylinderInit);
    Collider_UpdateCylinder(&this->dyna.actor, &this->collider);
    if (Flags_GetSwitch(globalCtx, DMIKNINSIDE_GET_SWITCH(thisx))) {
        Actor_MarkForDeath(&this->dyna.actor);
    }
}

void BgIkninside_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgIkninside* this = THIS;

    Collider_DestroyCylinder(globalCtx, &this->collider);
    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, this->dyna.bgId);
}

void func_80C07220(BgIkninside* this, GlobalContext* globalCtx) {
}

void func_80C07230(BgIkninside* this, GlobalContext* globalCtx) {
    if (this->dyna.actor.cutscene == -1) {
        this->actionFunc = func_80C07220;
    } else if (ActorCutscene_GetCurrentIndex() == 0x7c) {
        ActorCutscene_Stop(0x7c);
        ActorCutscene_SetIntentToPlay(this->dyna.actor.cutscene);
    } else if (ActorCutscene_GetCanPlayNext(this->dyna.actor.cutscene)) {
        ActorCutscene_StartAndSetUnkLinkFields(this->dyna.actor.cutscene, &this->dyna.actor);
        this->actionFunc = func_80C07220;
    } else {
        ActorCutscene_SetIntentToPlay(this->dyna.actor.cutscene);
    }
}

void func_80C072D0(BgIkninside* this, GlobalContext* globalCtx) {
    s16 altitude;
    s16 azimuth;
    Vec3f pos;
    Vec3f velocity;
    f32 speed;
    s32 i;

    if (this->collider.base.acFlags & AC_HIT) {
        if ((this->collider.info.acHitInfo != NULL) && (this->collider.info.acHitInfo->toucher.dmgFlags & 0x80000000)) {
            for (i = 0; i < 20; i++) {
                altitude = Rand_S16Offset(0x1800, 0x2800);
                azimuth = (u32)Rand_Next() >> 0x10;
                speed = Rand_ZeroFloat(3.0f) + 8.0f;
                velocity.x = speed * Math_CosS(altitude) * Math_SinS(azimuth);
                velocity.y = speed * Math_SinS(altitude) + Rand_ZeroFloat(5.0f);
                velocity.z = speed * Math_CosS(altitude) * Math_CosS(azimuth);
                pos.x = Rand_ZeroFloat(10.0f) * velocity.x + this->dyna.actor.world.pos.x;
                pos.y = Rand_ZeroFloat(1.0f) * velocity.y + this->dyna.actor.world.pos.y;
                pos.z = Rand_ZeroFloat(10.0f) * velocity.z + this->dyna.actor.world.pos.z;
                EffectSsHahen_Spawn(globalCtx, &pos, &velocity, &D_80C076D4, 0, 30, 566, 25, D_80C076A0[i & 1]);
            }
            Flags_SetSwitch(globalCtx, DMIKNINSIDE_GET_SWITCH(&this->dyna.actor));
            this->actionFunc = func_80C07230;
            this->dyna.actor.draw = NULL;
            func_800C62BC(globalCtx, &globalCtx->colCtx.dyna, this->dyna.bgId);
        } else {
            this->unk_1AA = 20;
        }
    }
    if (this->unk_1AA > 0) {
        if ((this->unk_1AA % 2) != 0) {
            this->dyna.actor.world.pos.y = this->dyna.actor.home.pos.y - 1.0f;
        } else {
            this->dyna.actor.world.pos.y = this->dyna.actor.home.pos.y;
        }
        this->unk_1AA--;
    } else {
        this->unk_1AA = 0;
        this->dyna.actor.world.pos.y = this->dyna.actor.home.pos.y;
    }
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &this->collider.base);
}

void BgIkninside_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgIkninside* this = THIS;

    this->actionFunc(this, globalCtx);
}

void BgIkninside_Draw(Actor* thisx, GlobalContext* globalCtx) {
    OPEN_DISPS(globalCtx->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    func_8012C28C(globalCtx->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, object_ikninside_obj_DL_00CC78);
    CLOSE_DISPS(globalCtx->state.gfxCtx);
}
