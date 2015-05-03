/*
 * package.h
 * 打包解包
 * 创建者：死猫
 * 创建时间：2015.01.24
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.03.23
 */
 
//每个周期Master发送的信息
//    MODE_CMD         | PACK_LEN | LEN_CHK | MODE_CMD | POWER | RUNNING_MODE | ECHO_MODE | ALGORI_MODE | PACK_CHK |
//    MODE_CTRL        | PACK_LEN | LEN_CHK | MODE_CTRL | .. | PACK_CHK |
//    MODE_RC          | PACK_LEN | LEN_CHK | MODE_RC  | RC0[B1..B0] | .. | RC7[B1..B0] | PACK_CHK |
//    MODE_PWM         | PACK_LEN | LEN_CHK | MODE_PWM | PWM[0][B1..B0] | ... | PWM[3][B1..B0] | PACK_CHK |

//CTRL命令
//    CTRL_SET_PID     | PACK_LEN | LEN_CHK | MODE_CTRL | CTRL_SET_PID | MASK[.., ATT_Z, ATT_Y, ATT_X, HIGH] | PID_K(float32) | PID_I | PID_D | PACK_CHK | (18Bytes)

//RC命令
//    | PITCH | ROLL | YAW | THROTTLE | 0 | 0 | 0 | 0 |

//每个周期Slave发送的信息
//    ECHO_NULL
//    ECHO_CMD         | NULL |
//    ECHO_RAW         | NULL |
//    ECHO_CALI        | NULL |
//    ECHO_MERG        | PACK_LEN | LEN_CHK | ECHO_MERG | MERG_AGM[0][B3..B0] | ... | MERG_AGM[8][B3..B0] | PACK_CHK |
//    ECHO_ATT_PWM     | PACK_LEN | LEN_CHK | ECHO_ATT_PWM | ATT[0][B3..B0] | ... | ATT[3][B3..B0] | PWM[0][B1..B0] | ... | PWM[3][B1..B0] | PACK_CHK |
//    ECHO_ATT_PID     | PACK_LEN | LEN_CHK | ECHO_ATT_PWM | ATT[0][B3..B0] | ... | ATT[3][B3..B0] | PID[0][B3..B0] | ... | PID[2][B3..B0] | PACK_CHK |

#ifndef PACKAGE_H_
#define PACKAGE_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "system_define.h"

#define CHECK_CODE (uint8_t)0x88

#define MODE_NULL 0
#define MODE_CMD  1
#define MODE_CTRL 2
#define MODE_RC   3
#define MODE_PWM  4

//Ctrl
#define CTRL_NULL           0
#define CTRL_SET_FIELD      1
#define CTRL_SET_PID        2
#define CTRL_CALI_GYRO      3
#define CTRL_CALI_ACC       4
#define CTRL_MAG_CALI_BEGIN 5
#define CTRL_MAG_CALI_END   6
#define CTRL_CALI_TRIM      7

#define SIZE_CMD          8
#define SIZE_RC           20
#define SIZE_PWM          12

#define SIZE_ECHO_NULL    0
#define SIZE_ECHO_CMD     0
#define SIZE_ECHO_RAW     0
#define SIZE_ECHO_CALI    0
#define SIZE_ECHO_MERG    40
#define SIZE_ECHO_ATT_PWM 28
#define SIZE_ECHO_ATT_PID 32

#define SIZE_CTRL_SET_PID   16

extern void package_listen_loop(void);

extern void package_craft_decode(const uint8_t *pack);
extern void package_craft_encode(uint8_t *pack);

extern void package_master_decode(const uint8_t *pack);
extern void package_master_encode(const uint8_t *pack);

extern void package_master_encode_cmd(uint8_t *pack);
extern void package_master_encode_rc(uint8_t *pack);
extern void package_master_encode_pid(uint8_t *pack);

extern void package_cc1101_recv(const uint8_t *pack);
extern void package_uart_recv(const uint8_t *pack);

extern uint8_t package_check(const uint8_t *src);
extern void package_en_check(uint8_t *dst);

#ifdef __cplusplus
};
#endif

#endif
