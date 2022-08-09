/*
 * File: z_obj_swprize.c
 * Overlay: ovl_Obj_Swprize
 * Description: Spawns item drop from soft soil
 */

#include "z_obj_swprize.h"

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((ObjSwprize*)thisx)

void ObjSwprize_Init(Actor* thisx, PlayState* play);
void ObjSwprize_Destroy(Actor* thisx, PlayState* play);
void ObjSwprize_Update(Actor* thisx, PlayState* play);

void ObjSwprize_DoNothing(ObjSwprize* this, PlayState* play);
void func_80C25654(ObjSwprize* this, PlayState* play);
void func_80C25640(ObjSwprize* this);
void func_80C25698(ObjSwprize* this);
void func_80C256AC(ObjSwprize* this, PlayState* play);
void func_80C25710(ObjSwprize* this);
void func_80C2572C(ObjSwprize* this, PlayState* play);
void func_80C25780(ObjSwprize* this);

const ActorInit Obj_Swprize_InitVars = {
    ACTOR_OBJ_SWPRIZE,
    ACTORCAT_PROP,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(ObjSwprize),
    (ActorFunc)ObjSwprize_Init,
    (ActorFunc)ObjSwprize_Destroy,
    (ActorFunc)ObjSwprize_Update,
    (ActorFunc)NULL,
};

static s16 D_80C257F0[] = { 2, 0x14, 1, 8 };

static s16 D_80C257F8[] = { -0x888, 0, 0x888 };

void func_80C25360(ObjSwprize* this, Vec3f* vec) {
    Matrix_Push();
    Matrix_RotateYS(this->actor.shape.rot.y, MTXMODE_NEW);
    Matrix_RotateXS(this->actor.shape.rot.x, MTXMODE_APPLY);
    Matrix_RotateZS(this->actor.shape.rot.z, MTXMODE_APPLY);
    Matrix_MultVecY(1.0f, vec);
    Matrix_Pop();
}

void func_80C253D0(ObjSwprize* this, PlayState* play) {
    s32 i;
    ObjSwprize* new_var;
    Actor* collectible;
    Vec3f sp78;
    s32 type = OBJ_SWPRIZE_GET_TYPE(&this->actor);
    u32 params = D_80C257F0[type];

    func_80C25360(this, &sp78);
    if (type == 2) {
        new_var = this;
        for (i = 0; i < 3; i++) {
            collectible = Item_DropCollectible(play, &new_var->actor.world.pos, params);
            if (collectible != NULL) {
                if (sp78.y < 0.98f) {
                    collectible->velocity.y = (sp78.y + 1.0f) * 4.0f;
                    collectible->speedXZ = (2.0f * (1.0f - fabsf(sp78.y))) + 2.0f;
                    collectible->world.rot.y = Math_FAtan2F(sp78.z, sp78.x) + D_80C257F8[i];
                } else {
                    collectible->world.rot.y = i * 0x5555;
                }
            }
        }
    } else {
        collectible = Item_DropCollectible(play, &this->actor.world.pos, params);
        if ((collectible != NULL) && (sp78.y < 0.98f)) {
            collectible->velocity.y = (sp78.y + 1.0f) * 4.0f;
            collectible->speedXZ = (2.0f * (1.0f - fabsf(sp78.y))) + 2.0f;
            collectible->world.rot.y = Math_FAtan2F(sp78.z, sp78.x);
        }
    }
}

void ObjSwprize_Init(Actor* thisx, PlayState* play) {
    ObjSwprize* this = THIS;

    if (Flags_GetSwitch(play, OBJ_SWPRIZE_GET_SWITCH_FLAG(&this->actor))) {
        func_80C25780(this);
    } else {
        func_80C25640(this);
    }
}

void ObjSwprize_Destroy(Actor* thisx, PlayState* play) {
}

void func_80C25640(ObjSwprize* this) {
    this->actionFunc = func_80C25654;
}

void func_80C25654(ObjSwprize* this, PlayState* play) {
    if (Flags_GetSwitch(play, OBJ_SWPRIZE_GET_SWITCH_FLAG(&this->actor))) {
        func_80C25698(this);
    }
}

void func_80C25698(ObjSwprize* this) {
    this->actionFunc = func_80C256AC;
}

void func_80C256AC(ObjSwprize* this, PlayState* play) {
    if (ActorCutscene_GetCanPlayNext(this->actor.cutscene)) {
        ActorCutscene_StartAndSetUnkLinkFields(this->actor.cutscene, &this->actor);
        func_80C253D0(this, play);
        func_80C25710(this);
    } else {
        ActorCutscene_SetIntentToPlay(this->actor.cutscene);
    }
}

void func_80C25710(ObjSwprize* this) {
    this->timer = 40;
    this->actionFunc = func_80C2572C;
}

void func_80C2572C(ObjSwprize* this, PlayState* play) {
    if (this->timer > 0) {
        this->timer--;
        if (this->timer == 0) {
            ActorCutscene_Stop(this->actor.cutscene);
            func_80C25780(this);
        }
    }
}

void func_80C25780(ObjSwprize* this) {
    this->actionFunc = ObjSwprize_DoNothing;
}

void ObjSwprize_DoNothing(ObjSwprize* this, PlayState* play) {
}

void ObjSwprize_Update(Actor* thisx, PlayState* play) {
    ObjSwprize* this = THIS;

    this->actionFunc(this, play);
}
