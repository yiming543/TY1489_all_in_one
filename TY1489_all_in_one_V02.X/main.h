/*
 * File:   main.h
 * Author: T00904
 *
 * Created on 2025年12月19日, 上午 8:49
 */

#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/pin_manager.h"
#include <pic.h>
#include <stdint.h> //uint8_t
#include <stdio.h>  //printf

void HiBeam_ON(void);
void HiBeam_OFF(void);
void LoBeam_ON();
void LoBeam_OFF(void);
void DRL_ON(void);
void DRL_OFF(void);
void LoPe_DRL_ON();
void POS_ON(void);

void DRL_Gradually(void);
void POS_Gradually(void);
void P2_ON(void);
void P2_OFF(void);

#define LOGIC_LEVEL (16)
// #define LOGIC_LEVEL (20)

// duty 100%
#define DRL_PWM_SET() CCPR2L = 201
// duyt 7%
#define POS_PWM_SET() CCPR2L = 14
// duty 0%
#define DRL_PWM_OFF() CCPR2L = 0

// 高低配共用程式 用電阻判別高低配
#define HiLoPe_state Lamp_LOW_EN_PORT
// #define Is_LoPa() (Lamp_LOW_EN_PORT==LAMP_LoPe)
#define Is_HiPe() (fLamp_Conf == LAMP_HiPe)
#define LAMP_LoPe 0 // 低配
#define LAMP_HiPe 1 // 高配

// 左右邊共用程式 用電阻判別左右邊
#define RL_Side_state Lamp_R_EN_PORT
// #define Is_R_Side() (Lamp_R_EN_PORT==LAMP_R_SIDE)
#define Is_R_Side() (fLamp_RL_Side==LAMP_R_SIDE)
#define LAMP_R_SIDE 0 // 右邊
#define LAMP_L_SIDE 1 // 左邊

#define Is_trun_left() (csFlag.turnLight_L == 1)
#define Is_trun_right() (csFlag.turnLight_R == 1)
#define Is_DRL_enable() (fDRL_Gradually == 0)
#define Is_POS() (csFlag.POS == 1)
#define Is_Receiver_ok() (fReceive_failed == 0)
#define Is_unlock_signal() (fUnlock == 1)
#define Is_falling_edge() (CCP3CON == 0x04)
#define Is_rising_edge() (CCP3CON == 0x05)

typedef union {
  struct {
    _Bool b0 : 1;
    _Bool b1 : 1;
    _Bool POS : 1;
    _Bool DRL : 1;
    _Bool LoBeam : 1;
    _Bool HiBeam : 1;
    _Bool b6 : 1;
    _Bool b7 : 1;
  };
  uint8_t Byte;
} flag_D2;
typedef union {
  struct {
    _Bool b0 : 1;
    _Bool b1 : 1;
    _Bool b2 : 1;
    _Bool b3 : 1;
    _Bool b4 : 1;
    _Bool turnLight_L : 1;
    _Bool turnLight_R : 1;
    _Bool b7 : 1;
  };
  uint8_t Byte;
} flag_D3;
typedef union {
  struct {
    _Bool turnLight_R : 1;
    _Bool turnLight_L : 1;
    _Bool HiBeam : 1;
    _Bool LoBeam : 1;
    _Bool DRL : 1;
    _Bool POS : 1;
    _Bool b6 : 1;
    _Bool b7 : 1;
  };
  uint8_t Byte;
} flag_CS_index;

typedef union {
  struct {
    _Bool POS : 1;
    _Bool DRL : 1;
    _Bool LoBeam : 1;
    _Bool HiBeam : 1;
    _Bool b4 : 1;
    _Bool b5 : 1;
    _Bool b6 : 1;
    _Bool b7 : 1;
  };
  uint8_t Byte;
} Lamp_Signal;

enum Lamp_Signal_State {
  Lamp_siganl_All_Off = 0,
  Lamp_Signal_POS = 1,
  Lamp_Signal_DRL = 2,
  Lamp_Signal_DRL_POS = 3,
  Lamp_Signal_LoBeam = 4,
  Lamp_Signal_LoBeam_POS = 5,
  Lamp_Signal_LoBeam_DRL = 6,
  Lamp_Signal_LoBeam_DRL_POS = 7,
  Lamp_Signal_HiBeam = 8,
  Lamp_Signal_HiBeam_POS = 9,
  Lamp_Signal_HiBeam_DRL = 10,
  Lamp_Signal_HiBeam_DRL_POS = 11,
  Lamp_Signal_HiBeam_LoBeam = 12,
  Lamp_Signal_HiBeam_LoBeam_POS = 13,
  Lamp_Signal_HiBeam_LoBeam_DRL = 14,
  Lamp_Signal_HiBeam_LoBeam_DRL_POS = 15,
};

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */
