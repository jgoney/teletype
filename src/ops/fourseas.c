#include "ops/fourseas.h"

#include "helpers.h"
#include "i2c.h"
#include "ii.h"
#include "teletype_io.h"

// Generate set+get functions for a global 16-bit param
#define FS_GLOBAL_OP(name, cmd)                  \
    I2C_WRITE_16(name##_set, FS_ADDR, cmd)       \
    I2C_RECV_16(name##_get, FS_ADDR, cmd)

// Generate set+get functions for a per-osc 16-bit param
#define FS_OSC_OP(name, cmd)                     \
    I2C_WRITE_16_16(name##_set, FS_ADDR, cmd)    \
    static void name##_get(                      \
        const void *NOTUSED(data),               \
        scene_state_t *NOTUSED(ss),              \
        exec_state_t *NOTUSED(es),               \
        command_state_t *cs) {                   \
        int16_t osc = cs_pop(cs);                \
        uint8_t d[] = { cmd | II_GET,            \
                        osc >> 8, osc & 0xff };  \
        tele_ii_tx(FS_ADDR, d, 3);               \
        uint8_t b[2] = { 0 };                    \
        tele_ii_rx(FS_ADDR, b, 2);               \
        cs_push(cs, (b[0] << 8) + b[1]);         \
    }

FS_GLOBAL_OP(op_FS_X, FS_X)
FS_GLOBAL_OP(op_FS_Y, FS_Y)
FS_GLOBAL_OP(op_FS_Z, FS_Z)
FS_GLOBAL_OP(op_FS_X_SPR, FS_X_SPREAD)
FS_GLOBAL_OP(op_FS_Y_SPR, FS_Y_SPREAD)
FS_GLOBAL_OP(op_FS_Z_SPR, FS_Z_SPREAD)
FS_GLOBAL_OP(op_FS_TUNE, FS_TUNE)
FS_GLOBAL_OP(op_FS_MD1, FS_MOD_DEPTH_1)
FS_GLOBAL_OP(op_FS_MD2, FS_MOD_DEPTH_2)
FS_GLOBAL_OP(op_FS_BANK, FS_BANK)
FS_GLOBAL_OP(op_FS_F_SPR, FS_FREQ_SPREAD)

FS_OSC_OP(op_FS_O_NT, FS_OSC_NOTE)
FS_OSC_OP(op_FS_O_X, FS_OSC_X)
FS_OSC_OP(op_FS_O_Y, FS_OSC_Y)
FS_OSC_OP(op_FS_O_Z, FS_OSC_Z)

static void op_FS_REL_get(const void *NOTUSED(data),
                           scene_state_t *NOTUSED(ss),
                           exec_state_t *NOTUSED(es),
                           command_state_t *NOTUSED(cs)) {
    uint8_t d[] = { FS_RELEASE, 0x00, 0x00 };
    tele_ii_tx(FS_ADDR, d, 3);
}

// clang-format off
const tele_op_t op_FS_X      = MAKE_GET_SET_OP(FS.X    , op_FS_X_get    , op_FS_X_set    , 0, true);
const tele_op_t op_FS_Y      = MAKE_GET_SET_OP(FS.Y    , op_FS_Y_get    , op_FS_Y_set    , 0, true);
const tele_op_t op_FS_Z      = MAKE_GET_SET_OP(FS.Z    , op_FS_Z_get    , op_FS_Z_set    , 0, true);
const tele_op_t op_FS_X_SPR  = MAKE_GET_SET_OP(FS.X.SPR, op_FS_X_SPR_get, op_FS_X_SPR_set, 0, true);
const tele_op_t op_FS_Y_SPR  = MAKE_GET_SET_OP(FS.Y.SPR, op_FS_Y_SPR_get, op_FS_Y_SPR_set, 0, true);
const tele_op_t op_FS_Z_SPR  = MAKE_GET_SET_OP(FS.Z.SPR, op_FS_Z_SPR_get, op_FS_Z_SPR_set, 0, true);
const tele_op_t op_FS_TUNE   = MAKE_GET_SET_OP(FS.TUNE , op_FS_TUNE_get , op_FS_TUNE_set , 0, true);
const tele_op_t op_FS_MD1    = MAKE_GET_SET_OP(FS.MD1  , op_FS_MD1_get  , op_FS_MD1_set  , 0, true);
const tele_op_t op_FS_MD2    = MAKE_GET_SET_OP(FS.MD2  , op_FS_MD2_get  , op_FS_MD2_set  , 0, true);
const tele_op_t op_FS_BANK   = MAKE_GET_SET_OP(FS.BANK , op_FS_BANK_get , op_FS_BANK_set , 0, true);
const tele_op_t op_FS_F_SPR  = MAKE_GET_SET_OP(FS.F.SPR, op_FS_F_SPR_get, op_FS_F_SPR_set, 0, true);
const tele_op_t op_FS_O_NT   = MAKE_GET_SET_OP(FS.O.NT , op_FS_O_NT_get , op_FS_O_NT_set , 1, true);
const tele_op_t op_FS_O_X    = MAKE_GET_SET_OP(FS.O.X  , op_FS_O_X_get  , op_FS_O_X_set  , 1, true);
const tele_op_t op_FS_O_Y    = MAKE_GET_SET_OP(FS.O.Y  , op_FS_O_Y_get  , op_FS_O_Y_set  , 1, true);
const tele_op_t op_FS_O_Z    = MAKE_GET_SET_OP(FS.O.Z  , op_FS_O_Z_get  , op_FS_O_Z_set  , 1, true);
const tele_op_t op_FS_REL    = MAKE_GET_OP(FS.REL      , op_FS_REL_get  , 0, false);
// clang-format on
