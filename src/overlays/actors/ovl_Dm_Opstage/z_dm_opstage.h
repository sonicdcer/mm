#ifndef Z_DM_OPSTAGE_H
#define Z_DM_OPSTAGE_H

#include "global.h"

#define DMOPSTAGE_GET_TYPE(this) (((this)->dyna.actor.params & 0xFF))

struct DmOpstage;

typedef void (*DmOpstageActionFunc)(struct DmOpstage*, GlobalContext*);
typedef struct DmOpstage { 
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ char pad_15C[0x44];
    /* 0x1A0 */ DmOpstageActionFunc actionFunc;
    /* 0x1A4 */ f32 posX;
    /* 0x1A8 */ f32 posY;
    /* 0x1AC */ f32 posZ;
} DmOpstage;

extern const ActorInit Dm_Opstage_InitVars;

#endif // Z_DM_OPSTAGE_H
