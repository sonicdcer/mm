/*
 * File: z_elf_msg4.c
 * Overlay: ovl_Elf_Msg4
 * Description: Tatl Hint (another proximity-activated C-up hint?)
 */

#include "z_elf_msg4.h"
#include "overlays/actors/ovl_En_Elf/z_en_elf.h"

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((ElfMsg4*)thisx)

void ElfMsg4_Init(Actor* thisx, PlayState* play);
void ElfMsg4_Destroy(Actor* thisx, PlayState* play);
void ElfMsg4_Update(Actor* thisx, PlayState* play);

void func_80AFD668(ElfMsg4* this, PlayState* play);
void func_80AFD770(ElfMsg4* this, PlayState* play);
s32 func_80AFD380(ElfMsg4* this, PlayState* play);
s32 func_80AFD5B4(ElfMsg4* this);
s32 func_80AFD5E0(ElfMsg4* this);

const ActorInit Elf_Msg4_InitVars = {
    ACTOR_ELF_MSG4,
    ACTORCAT_ITEMACTION,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(ElfMsg4),
    (ActorFunc)ElfMsg4_Init,
    (ActorFunc)ElfMsg4_Destroy,
    (ActorFunc)ElfMsg4_Update,
    (ActorFunc)NULL,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_STOP),
};

s32 func_80AFD380(ElfMsg4* this, PlayState* play) {
    if ((this->actor.home.rot.y > 0) && (this->actor.home.rot.y < 0x81) &&
        (Flags_GetSwitch(play, this->actor.home.rot.y - 1))) {
        (void)"共倒れ"; // "Collapse together"
        if (ELFMSG4_GET_7F00(&this->actor) != 0x7F) {
            Flags_SetSwitch(play, ELFMSG4_GET_7F00(&this->actor));
        }
        Actor_MarkForDeath(&this->actor);
        return true;
    }
    if (this->actor.home.rot.y == 0x81) {
        if (Flags_GetClear(play, this->actor.room)) {
            if (ELFMSG4_GET_7F00(&this->actor) != 0x7F) {
                Flags_SetSwitch(play, ELFMSG4_GET_7F00(&this->actor));
            }
            Actor_MarkForDeath(&this->actor);
            return true;
        }
    }
    if (ELFMSG4_GET_7F00(&this->actor) == 0x7F) {
        return false;
    }
    if (Flags_GetSwitch(play, ELFMSG4_GET_7F00(&this->actor)) != 0) {
        (void)"共倒れ"; // "Collapse together"
        Actor_MarkForDeath(&this->actor);
        return true;
    }
    return false;
}

void ElfMsg4_Init(Actor* thisx, PlayState* play) {
    ElfMsg4* this = THIS;

    if (!func_80AFD380(this, play)) {
        Actor_ProcessInitChain(&this->actor, sInitChain);

        if (ABS_ALT(this->actor.home.rot.x) == 0) {
            this->actor.scale.z = 0.4f;
            this->actor.scale.x = 0.4f;
        } else {
            this->actor.scale.x = this->actor.scale.z = ABS_ALT(this->actor.home.rot.x) * 0.04f;
        }
        if (this->actor.home.rot.z == 0) {
            this->actor.scale.y = 0.4f;
        } else {
            this->actor.scale.y = this->actor.home.rot.z * 0.04f;
        }
        this->actor.shape.rot.z = 0;
        this->actionFunc = func_80AFD770;
        this->unk_144 = NULL;
        this->actor.shape.rot.x = this->actor.shape.rot.y = this->actor.shape.rot.z;
    }
}

void ElfMsg4_Destroy(Actor* thisx, PlayState* play) {
}

s32 func_80AFD5B4(ElfMsg4* this) {
    if (ELFMSG4_GET_8000(&this->actor) != 0) {
        return ELFMSG4_GET_FF(&this->actor) + 0x200;
    } else {
        return -0x200 - ELFMSG4_GET_FF(&this->actor);
    }
}

s32 func_80AFD5E0(ElfMsg4* this) {
    return (this->actor.xzDistToPlayer < (100.0f * this->actor.scale.x)) && (this->actor.playerHeightRel >= 0.0f) &&
           (this->actor.playerHeightRel < (100.0f * this->actor.scale.y));
}

void func_80AFD668(ElfMsg4* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    EnElf* tatl = (EnElf*)player->tatlActor;

    if ((player->tatlActor != NULL) && func_80AFD5E0(this)) {
        player->tatlTextId = func_80AFD5B4(this);
        ActorCutscene_SetIntentToPlay(0x7C);
        tatl->elfMsg = this->unk_144;
        if (this->actor.cutscene == -1) {
            this->actor.cutscene = 0x7C;
        }
        if ((player->tatlTextId < 0) && (this->actor.home.rot.x < 0)) {
            if (ActorCutscene_GetCurrentIndex() == 0x7D) {
                ActorCutscene_Stop(0x7D);
                ActorCutscene_SetIntentToPlay((s16)this->actor.cutscene);
            } else if (ActorCutscene_GetCanPlayNext((s16)this->actor.cutscene)) {
                ActorCutscene_Start(this->actor.cutscene, &this->actor);
                func_800E0348(play->cameraPtrs[0]);
            } else {
                ActorCutscene_SetIntentToPlay((s16)this->actor.cutscene);
            }
        }
    }
}

void func_80AFD770(ElfMsg4* this, PlayState* play) {
    ElfMsg4* actorPtr = (ElfMsg4*)play->actorCtx.actorLists[ACTORCAT_BG].first;

    while (actorPtr != NULL) {
        if ((actorPtr->actor.id != ACTOR_ELF_MSG5) ||
            (ELFMSG4_GET_FF(&this->actor) != (actorPtr->actor.params & 0xFF)) ||
            (this->actor.cutscene != actorPtr->actor.cutscene)) {
            actorPtr = (ElfMsg4*)actorPtr->actor.next;
        } else {
            this->unk_144 = (Actor*)actorPtr;
            this->actionFunc = func_80AFD668;
            actorPtr = (ElfMsg4*)actorPtr->actor.next;
        }
    }
}

void ElfMsg4_Update(Actor* thisx, PlayState* play) {
    Actor* actorPtr;
    ElfMsg4* this = THIS;

    if (!func_80AFD380(this, play)) {
        actorPtr = this->unk_144;
        if ((actorPtr != NULL) && (actorPtr->update == NULL)) {
            Actor_MarkForDeath(&this->actor);
        } else if ((actorPtr != NULL) && (Actor_ProcessTalkRequest(actorPtr, &play->state))) {
            if (ELFMSG4_GET_7F00(thisx) != 0x7F) {
                Flags_SetSwitch(play, ELFMSG4_GET_7F00(thisx));
            }
            Actor_MarkForDeath(&this->actor);
        } else if ((this->actor.home.rot.y >= 0) || (this->actor.home.rot.y < -0x80) ||
                   (Flags_GetSwitch(play, -1 - this->actor.home.rot.y))) {
            this->actionFunc(this, play);
        }
    }
}
