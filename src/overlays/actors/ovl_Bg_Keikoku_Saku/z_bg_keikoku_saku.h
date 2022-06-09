#ifndef Z_BG_KEIKOKU_SAKU_H
#define Z_BG_KEIKOKU_SAKU_H

#include "global.h"

#define BGKEIKOKUSAKU_GET_SWITCHFLAG(thisx) ((thisx)->params & 0x7F)

struct BgKeikokuSaku;

typedef void (*BgKeikokuSakuActionFunc)(struct BgKeikokuSaku*, GlobalContext*);

typedef struct BgKeikokuSaku {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ BgKeikokuSakuActionFunc actionFunc;
    /* 0x160 */ s16 unk160;
    /* 0x162 */ s16 unk162;
} BgKeikokuSaku; // Size = 0x164
extern const ActorInit Bg_Keikoku_Saku_InitVars;

#endif // Z_BG_KEIKOKU_SAKU_H
