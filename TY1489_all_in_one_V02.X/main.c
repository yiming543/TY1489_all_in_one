/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules
  selected in the GUI. Generation Information : Product Revision  :  PIC10 /
  PIC12 / PIC16 / PIC18 MCUs - 1.81.8 Device            :  PIC16F1936 Driver
  Version    :  2.00
 */

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip software
   and any derivatives exclusively with Microchip products. It is your
   responsibility to comply with third party license terms applicable to your
   use of third party software (including open source software) that may
   accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
    FOR A PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
    SOFTWARE.
 */
//**********************************************************************************
//                        PIC16F1936 Pinout for this example
//                                  ------------
//                             MCLR |1       28| RB7 -> P2_EN
//                    HB_EN ->  RA0 |2       27| RB6 ->
//                          ->  RA1 |3       26| RB5 ->// 加RB5電阻對地 是低配
//                          // 沒加RB5電阻對地 是高配
//                          ->  RA2 |4       25| RB4 ->
//                          ->  RA3 |5       24| RB3 -> // 加RB3電阻對地 是R邊
//                          // 沒加RB3電阻對地 是L邊
//                          ->  RA4 |6       23| RB2 ->
//                          ->  RA5 |7       22| RB1 ->
//                              GND |8       21| RB0 ->
//                          ->  RA7 |9       20| VDD
//                          ->  RA6 |10      19| VSS
//                          ->  RC0 |11      18| RC7 ->
//            (CCP2)  DT_EN ->  RC1 |12      17| RC6 -> RX //IN
//            (CCP1) LHB_EN ->  RC2 |13      16| RC5 ->
//                          ->  RC3 |14      15| RC4 ->
//                                  ------------
//
//***********************************************************************************
// 202500806 TY1489_HIGH V01 CS:6502
// 參考 TY1489_HIGH件動作表.xlsx
// 晝行+方向燈 晝行OFF，方向燈結束時，晝行呼吸燈亮起
// 解鎖行車，行車會有呼吸亮
// HIBEAM:Action-low

// 20251218 TY1489_all_on_one V01 CS:4959
// 更新解碼,檢誤碼(TY1475)
// 消除未呼叫警告
// 新增RB3判斷是左邊的燈還是右邊的燈
// 加RB3電阻對地 是R邊
// 沒加RB3電阻對地 是L邊
// 新增RB5判斷是高配/低配
// 加RB5電阻對地 是低配
// 沒加RB5電阻對地 是高配
// 高配    DRL_ON()
// 低配 Lo_DRL_ON()
// 高配 POS
// 低配 POS(不打件)

// 高配
// 晝行 亮晝行亮度100%
// 行車 亮晝行亮度  7%
// 畫行與方向燈共用燈條，有方向燈時的那一邊關閉晝行，POS則無需變動.

// 低配
// 晝行 亮近燈亮度50%
// 無行車

// 20251218 TY1489_all_on_one V02 CS:0384
//程式碼優化

#include "main.h"
#include <pic16f1936.h>
#include <stdbool.h>

static const uint8_t CS[64] = {
    // 行車,晝行,近燈,遠燈,左方,右方
    0x4C, // 000000 關
    0x9F, // 000001 右方
    0xED, // 000010 左方
    0x3E, // 000011 右方+左方(警示)

    // 0x9A, // 000100 遠燈 (KOSO)
    0x66, // 000100 遠燈
    0xB5, // 000101 遠燈+右方
    0xC7, // 000110 遠燈+左方
    0x14, // 000111 遠燈+右方+左方(警示)

    //     0x27, // 001000 近燈 (KOSO)
    0x59, // 001000 近燈
    0x8A, // 001001 近燈+右方
    0xF8, // 001010 近燈+左方
    0x2B, // 001011 近燈+右方+左方(警示)

    //    0xF1, // 001100 遠燈+近燈(超車) (KOSO)
    0x73, // 001100 遠燈+近燈(超車)
    0xA0, // 001101 遠燈+近燈(超車)+右方
    0xD2, // 001110 遠燈+近燈(超車)+左方
    0x01, // 001111 遠燈+近燈(超車)+右方+左方(警示)

    // 0xB1, // 010000 晝行 (KOSO)
    0x8E, // 010000 晝行
    0x5D, // 010001 晝行+右方
    0x2F, // 010010 晝行+左方
    0xFC, // 010011 晝行+右方+左方(警示)

    //    0x67, // 010100 晝行+遠燈 (KOSO)
    0xA4, // 010100 晝行+遠燈
    0x77, // 010101 晝行+遠燈+右方
    0x05, // 010110 晝行+遠燈+左方
    0xD6, // 010111 晝行+遠燈+右方+左方(警示)

    //    0xDA, // 011000 晝行+近燈 (KOSO)
    0x9B, // 011000 晝行+近燈
    0x48, // 011001 晝行+近燈+右方
    0x3A, // 011010 晝行+近燈+左方
    0xE9, // 011011 晝行+近燈+右方+左方(警示)

    //    0x0C, // 011100 畫行+遠燈+近燈(超車) (KOSO)
    0xB1, // 011100 畫行+遠燈+近燈(超車)
    0x62, // 011101 畫行+遠燈+近燈(超車)+右方
    0x10, // 011110 畫行+遠燈+近燈(超車)+左方
    0xC3, // 011111 晝行+遠燈+近燈(超車)+右方+左方(警示)

    // 0xFA, // 100000 行車 (KOSO)
    0x2D, // 100000 行車
    0xFE, // 100001 行車+右方
    0x8C, // 100010 行車+左方
    0x5F, // 100011 行車+右方+左方(警示)

    //    0x2C, // 100100 行車+遠燈 (KOSO)
    0x07, // 100100 行車+遠燈
    0xD4, // 100101 行車+遠燈+右方
    0xA6, // 100110 行車+遠燈+左方
    0x75, // 100111 行車+遠燈+右方+左方(警示)

    // 0x91, // 101000 行車+近燈 (KOSO)
    0x38, // 101000 行車+近燈
    0xEB, // 101001 行車+近燈+右方
    0x99, // 101010 行車+近燈+左方
    0x4A, // 101011 行車+近燈+右方+左方(警示)

    //    0x47, // 101100 行車+遠燈+近燈(超車) (KOSO)
    0x12, // 101100 行車+遠燈+近燈(超車)
    0xC1, // 101101 行車+遠燈+近燈(超車)+右方
    0xB3, // 101110 行車+遠燈+近燈(超車)+左方
    0x60, // 101111 行車+遠燈+近燈(超車)+右方+左方(警示)

    // 0x07, // 110000 行車+晝行 (KOSO)
    0xEF, // 110000 行車+晝行
    0x3C, // 110001 行車+晝行+右方
    0x4E, // 110010 行車+晝行+左方
    0x9D, // 110011 行車+晝行+右方+左方(警示)

    //    0xD1, // 110100 行車+晝行+遠燈 (KOSO)
    0xC5, // 110100 行車+晝行+遠燈
    0x16, // 110101 行車+晝行+遠燈+右方
    0x64, // 110110 行車+晝行+遠燈+左方
    0xB7, // 110111 行車+晝行+遠燈+右方+左方(警示)

    //    0x6C, // 111000 行車+晝行+近燈 (KOSO)
    0xFA, // 111000 行車+晝行+近燈
    0x29, // 111001 行車+晝行+近燈+右方
    0x5B, // 111010 行車+晝行+近燈+左方
    0x88, // 111011 行車+晝行+近燈+右方+左方(警示)

    //    0xBA, // 111100 行車+晝行+遠燈+近燈 (KOSO)
    0xD0, // 111100 行車+晝行+遠燈+近燈
    0x03, // 111101 行車+晝行+遠燈+近燈+右方
    0x71, // 111110 行車+晝行+遠燈+近燈+左方
    0xA2, // 111111 行車+晝行+遠燈+近燈(超車)+右方+左方(警示)
};

uint8_t rx_data = 0;
volatile bool data_1byte_OK = 0;
bool fFrame_OK = 0;
uint8_t data_buf[11];
uint8_t step = 0;

flag_D2 D2;
flag_D3 D3;
flag_CS_index csFlag;

volatile uint8_t T10MS_CNT = 0;  // 單位:10ms
volatile uint8_t T10MS_CNT1 = 0; // 單位:10ms
bool fUnlock = 0;
bool fReceive_failed = 0; // 0:接收正常 1:2秒沒收到正確的資料
// DRL Gradually brighten
bool fDRL_Gradually = 0;
// POS Gradually brighten
bool fPOS_Gradually = 0;
bool fLamp_RL_Side = 0;    // 0:R side 1:L side
bool fLamp_Conf = 0;       // 1:HiPe 0:LoPe
bool fLoPe_DRL_In_use = 0; // 1:in use(don't change CCPR1L PWM) 0:Unuse

// 特殊信號(unlock) 亮P2
void Exception_handling(void) {
  if ((data_buf[2] == 6) && (data_buf[10] == 0xD5)) {
    fUnlock = 1;
    P2_ON();      // P2亮
    LoBeam_OFF(); // 遠燈 OFF
    HiBeam_OFF(); // 遠燈 OFF
    if (fPOS_Gradually == 0) {
      INTERRUPT_GlobalInterruptDisable();
      POS_Gradually();
      INTERRUPT_GlobalInterruptEnable();
    }
    T10MS_CNT = 0;  // 重置2000ms計數器
    T10MS_CNT1 = 0; // 重置2000ms計數器
  } else {
    fUnlock = 0;
    if (T10MS_CNT1 > 50) {
      T10MS_CNT1 = 50;
      fPOS_Gradually = 0;
    }
  }
}

void HiBeam_ON(void) { HB_EN_SetLow(); }

void HiBeam_OFF(void) { HB_EN_SetHigh(); }

// duty 100%
void LoBeam_ON() {
  fLoPe_DRL_In_use = false;
  CCPR1L = 201;
}

// duty 0%
void LoBeam_OFF(void) {
  fLoPe_DRL_In_use = false;
  CCPR1L = 0;
}

void DRL_ON(void) {
  if (Is_R_Side()) {
    // 右方向 關畫行
    if (Is_trun_right()) {
      DRL_PWM_OFF();
      fDRL_Gradually = 1;
    } else {
      if (Is_DRL_enable()) {
        DRL_PWM_SET();
      } else {
        INTERRUPT_GlobalInterruptDisable();
        // 晝行+方向燈 晝行OFF，方向燈結束時，晝行呼吸燈亮起
        DRL_Gradually();
        INTERRUPT_GlobalInterruptEnable();
      }
    }
  } else {
    // 左方向 關畫行
    if (Is_trun_left()) {
      DRL_PWM_OFF();
      fDRL_Gradually = 1;
    } else {
      if (Is_DRL_enable()) {
        DRL_PWM_SET();
      } else {
        INTERRUPT_GlobalInterruptDisable();
        // 晝行+方向燈 晝行OFF，方向燈結束時，晝行呼吸燈亮起
        DRL_Gradually();
        INTERRUPT_GlobalInterruptEnable();
      }
    }
  }
}

void DRL_OFF(void) { DRL_PWM_OFF(); }
void POS_ON(void) { POS_PWM_SET(); }
void P2_ON(void) { P2_EN_SetHigh(); }
void P2_OFF(void) { P2_EN_SetLow(); }

// 低配晝 亮近燈50%
void LoPe_DRL_ON() {
  if (fLoPe_DRL_In_use == true) {
    fLoPe_DRL_In_use = true;
    // duty 50%
    CCPR1L = 100;
  }
}

// 晝行+方向燈 晝行OFF，方向燈結束時，晝行呼吸燈亮起
void DRL_Gradually(void) {
  for (uint8_t i = 0; i < 201; i++) {
    while (!TMR2IF)
      ;
    TMR2IF = 0;
    CCPR2L++;
  }
  fDRL_Gradually = 0;
}

// duty 6~30%

void POS_Gradually(void) {
  for (uint8_t i = 0; i < 49; i++) {
    CCPR2L = i + 12;
    __delay_ms(41);
  }
  CCPR2L = 60;
  fPOS_Gradually = 1;
}

void LED_output(void) {
  uint8_t crc8 = CS[csFlag.Byte];
  Lamp_Signal fLampSignal;
  fLampSignal.Byte = 0;

  // CRC檢查
  if (data_buf[10] != crc8)
    return;
  if (data_buf[0] != 0xC8)
    return; // CRC檢查
  if (data_buf[1] != 0x8B)
    return; // CRC檢查

  T10MS_CNT = 0; // 重置2000ms計數器

  // 有行車 P2亮
  if (Is_POS()) {
    P2_ON(); // P2亮
  } else {
    P2_OFF(); // P2暗
  }

  fLampSignal.POS = csFlag.POS;
  fLampSignal.DRL = csFlag.DRL;
  fLampSignal.LoBeam = csFlag.LoBeam;
  fLampSignal.HiBeam = csFlag.HiBeam;

  switch (fLampSignal.Byte) {
  case Lamp_siganl_All_Off:
    LoBeam_OFF(); // 近燈 OFF
    HiBeam_OFF(); // 遠燈 OFF
    DRL_OFF();    // 晝行 OFF
    break;
  case Lamp_Signal_POS:
    LoBeam_OFF(); // 近燈 OFF
    HiBeam_OFF(); // 遠燈 OFF
    POS_ON();     // POS ON
    break;
  case Lamp_Signal_DRL:
    if (fLoPe_DRL_In_use == 0)
      LoBeam_OFF(); // 近燈 OFF
    HiBeam_OFF();   // 遠燈 OFF
    if (Is_HiPe())
      DRL_ON(); // DRL ON
    else
      LoPe_DRL_ON(); // 低配DRL亮近燈50%
    break;
  case Lamp_Signal_DRL_POS:
    if (fLoPe_DRL_In_use == 0)
      LoBeam_OFF(); // 近燈 OFF
    HiBeam_OFF();   // 遠燈 OFF
    if (Is_HiPe())
      DRL_ON(); // DRL ON
    else
      LoPe_DRL_ON(); // 低配DRL亮近燈50%
  case Lamp_Signal_LoBeam:
    LoBeam_ON();  // 近燈 ON
    HiBeam_OFF(); // 遠燈 OFF
    DRL_OFF();    // 晝行 OFF
    break;
  case Lamp_Signal_LoBeam_POS:
    LoBeam_ON();  // 近燈 ON
    HiBeam_OFF(); // 遠燈 OFF
    POS_ON();     // POS ON
    break;
  case Lamp_Signal_LoBeam_DRL:
    LoBeam_ON();  // 近燈 ON
    HiBeam_OFF(); // 遠燈 OFF
    DRL_OFF();    // 晝行 OFF
    break;
  case Lamp_Signal_LoBeam_DRL_POS:
    LoBeam_ON();  // 近燈 ON
    HiBeam_OFF(); // 遠燈 OFF
    POS_ON();     // POS ON
    break;
  case Lamp_Signal_HiBeam:
    LoBeam_OFF(); // 近燈 OFF
    HiBeam_OFF(); // 遠燈 OFF
    DRL_OFF();    // 晝行 OFF
    break;
  case Lamp_Signal_HiBeam_POS:
    LoBeam_OFF(); // 近燈 OFF
    HiBeam_OFF(); // 遠燈 OFF
    POS_ON();     // POS ON
    break;
  case Lamp_Signal_HiBeam_DRL:
    if (fLoPe_DRL_In_use == 0)
      LoBeam_OFF(); // 近燈 OFF
    HiBeam_OFF();   // 遠燈 OFF
    if (Is_HiPe())
      DRL_ON(); // DRL ON
    else
      LoPe_DRL_ON(); // 低配DRL亮近燈50%
    break;
  case Lamp_Signal_HiBeam_DRL_POS:
    if (fLoPe_DRL_In_use == 0)
      LoBeam_OFF(); // 近燈 OFF
    HiBeam_OFF();   // 遠燈 OFF
    if (Is_HiPe())
      DRL_ON(); // DRL ON
    else
      LoPe_DRL_ON(); // 低配DRL亮近燈50%
    break;
  case Lamp_Signal_HiBeam_LoBeam:
    LoBeam_ON(); // 近燈 ON
    HiBeam_ON(); // 遠燈 ON
    DRL_OFF();
    break;
  case Lamp_Signal_HiBeam_LoBeam_POS:
    LoBeam_ON(); // 近燈 ON
    HiBeam_ON(); // 遠燈 ON
    POS_ON();    // POS ON
    break;
  case Lamp_Signal_HiBeam_LoBeam_DRL:
    LoBeam_ON(); // 近燈 ON
    HiBeam_ON(); // 遠燈 ON
    DRL_OFF();   // 晝行 OFF
    break;
  case Lamp_Signal_HiBeam_LoBeam_DRL_POS:
    LoBeam_ON(); // 近燈 ON
    HiBeam_ON(); // 遠燈 ON
    POS_ON();
    break;
  default:
    break;
  } // switch

  // if (csFlag.HiBeam == 0 && csFlag.LoBeam == 0 && csFlag.DRL == 0 &&
  //     csFlag.POS == 0) { // 0
  //   // ALL OFF
  //   LoBeam_OFF();  // 近燈 OFF
  //   HiBeam_OFF();  // 遠燈 OFF
  //   DRL_PWM_OFF(); // 晝行 OFF
  // } else if (csFlag.HiBeam == 0 && csFlag.LoBeam == 0 && csFlag.DRL == 0 &&
  //            csFlag.POS == 1) { // 1
  //   // POS ON
  //   LoBeam_OFF(); // 近燈 OFF
  //   HiBeam_OFF(); // 遠燈 OFF
  //   POS_ON();     // POS ON
  // } else if (csFlag.HiBeam == 0 && csFlag.LoBeam == 0 && csFlag.DRL == 1 &&
  //            csFlag.POS == 0) { // 2
  //   // DRL ON
  //   if (fLoDRL == 0)
  //     LoBeam_OFF(); // 近燈 OFF
  //   HiBeam_OFF();   // 遠燈 OFF
  //   if (fLampLo_Hi == LAMP_HI)
  //     DRL_ON(); // DRL ON
  //   else
  //     Lo_DRL_ON(); // 低配DRL亮近燈50%

  // } else if (csFlag.HiBeam == 0 && csFlag.LoBeam == 0 && csFlag.DRL == 1 &&
  //            csFlag.POS == 1) { // 3
  //   // POS + DLR = DRL
  //   if (fLoDRL == 0)
  //     LoBeam_OFF(); // 近燈 OFF
  //   HiBeam_OFF();   // 遠燈 OFF
  //   if (fLampLo_Hi == LAMP_HI)
  //     DRL_ON(); // DRL ON
  //   else
  //     Lo_DRL_ON(); // 低配DRL亮近燈50%
  // } else if (csFlag.HiBeam == 0 && csFlag.LoBeam == 1 && csFlag.DRL == 0 &&
  //            csFlag.POS == 0) { // 4
  //   // LOBEAM = LOBEAM
  //   LoBeam_ON();   // 近燈 ON
  //   HiBeam_OFF();  // 遠燈 OFF
  //   DRL_PWM_OFF(); // 晝行 OFF
  // } else if (csFlag.HiBeam == 0 && csFlag.LoBeam == 1 && csFlag.DRL == 0 &&
  //            csFlag.POS == 1) { // 5
  //   // LOBEAM + POS = LOBEAM + POS
  //   LoBeam_ON();  // 近燈 ON
  //   HiBeam_OFF(); // 遠燈 OFF
  //   POS_ON();     // POS ON
  // } else if (csFlag.HiBeam == 0 && csFlag.LoBeam == 1 && csFlag.DRL == 1 &&
  //            csFlag.POS == 0) { // 6
  //   // LOBEAM + DRL = LOBEAM
  //   LoBeam_ON();   // 近燈 ON
  //   HiBeam_OFF();  // 遠燈 OFF
  //   DRL_PWM_OFF(); // 晝行 OFF
  // } else if (csFlag.HiBeam == 0 && csFlag.LoBeam == 1 && csFlag.DRL == 1 &&
  //            csFlag.POS == 1) { // 7
  //   // LOBEAM + DRL + POS = LOBEAM + POS
  //   LoBeam_ON();  // 近燈 ON
  //   HiBeam_OFF(); // 遠燈 OFF
  //   POS_ON();     // POS ON
  // } else if (csFlag.HiBeam == 1 && csFlag.LoBeam == 0 && csFlag.DRL == 0 &&
  //            csFlag.POS == 0) { // 8
  //   // HIBEAM = ALL OFF
  //   LoBeam_OFF();  // 近燈 OFF
  //   HiBeam_OFF();  // 遠燈 OFF
  //   DRL_PWM_OFF(); // 晝行 OFF
  // } else if (csFlag.HiBeam == 1 && csFlag.LoBeam == 0 && csFlag.DRL == 0 &&
  //            csFlag.POS == 1) { // 9
  //   // HIBEAM + POS = POS
  //   LoBeam_OFF(); // 近燈 OFF
  //   HiBeam_OFF(); // 遠燈 OFF
  //   POS_ON();     // POS ON
  // } else if (csFlag.HiBeam == 1 && csFlag.LoBeam == 0 && csFlag.DRL == 1 &&
  //            csFlag.POS == 0) { // 10
  //   // HIBEAM + DRL = DRL
  //   if (fLoDRL == 0)
  //     LoBeam_OFF(); // 近燈 OFF
  //   HiBeam_OFF();   // 遠燈 OFF
  //   if (fLampLo_Hi == LAMP_HI)
  //     DRL_ON(); // DRL ON
  //   else
  //     Lo_DRL_ON(); // 低配DRL亮近燈50%
  // } else if (csFlag.HiBeam == 1 && csFlag.LoBeam == 0 && csFlag.DRL == 1 &&
  //            csFlag.POS == 1) { // 11
  //   // HIBEAM + DRL + POS = DRL
  //   if (fLoDRL == 0)
  //     LoBeam_OFF(); // 近燈 OFF
  //   HiBeam_OFF();   // 遠燈 OFF
  //   if (fLampLo_Hi == LAMP_HI)
  //     DRL_ON(); // DRL ON
  //   else
  //     Lo_DRL_ON(); // 低配DRL亮近燈50%
  // } else if (csFlag.HiBeam == 1 && csFlag.LoBeam == 1 && csFlag.DRL == 0 &&
  //            csFlag.POS == 0) { // 12
  //   // HIBEAM + LOBEAM = HIBEAM + LOBEAM
  //   LoBeam_ON(); // 近燈 ON
  //   HiBeam_ON(); // 遠燈 ON
  //   DRL_PWM_OFF();
  // } else if (csFlag.HiBeam == 1 && csFlag.LoBeam == 1 && csFlag.DRL == 0 &&
  //            csFlag.POS == 1) { // 13
  //   // HIBEAM + LOBEAM + POS = HIBEAM + LOBEAM + POS
  //   LoBeam_ON(); // 近燈 ON
  //   HiBeam_ON(); // 遠燈 ON
  //   POS_ON();    // POS ON
  // } else if (csFlag.HiBeam == 1 && csFlag.LoBeam == 1 && csFlag.DRL == 1 &&
  //            csFlag.POS == 0) { // 14
  //   // HIBEAM + LOBEAM + DRL = HIBEAM + LOBEAM
  //   LoBeam_ON();   // 近燈 ON
  //   HiBeam_ON();   // 遠燈 ON
  //   DRL_PWM_OFF(); // 晝行 OFF
  // } else if (csFlag.HiBeam == 1 && csFlag.LoBeam == 1 && csFlag.DRL == 1 &&
  //            csFlag.POS == 1) { // 15
  //   // HIBEAM + LOBEAM + DRL + POS = LOBEAM + POS
  //   LoBeam_ON(); // 近燈 ON
  //   HiBeam_ON(); // 遠燈 ON
  //   POS_ON();
  // }

  for (uint8_t i = 0; i < 11; i++) {
    data_buf[i] = 0; // 清除資料緩衝區
  }
}

void check_input(void) {
  D2.Byte = data_buf[2];
  D3.Byte = data_buf[3];
  csFlag.Byte = 0;

  // 行車
  csFlag.POS = D2.POS;
  csFlag.DRL = D2.DRL;
  csFlag.LoBeam = D2.LoBeam;
  csFlag.HiBeam = D2.HiBeam;
  csFlag.turnLight_L = D3.turnLight_L;
  csFlag.turnLight_R = D3.turnLight_R;
  // if (D2.POS == 1)
  //   csFlag.POS = 1;
  // else
  //   csFlag.POS = 0;

  // 晝行
  // if (D2.DRL == 1)
  //   csFlag.DRL = 1;
  // else
  //   csFlag.DRL = 0;

  // 近燈
  // if (D2.LoBeam == 1)
  //   csFlag.LoBeam = 1;
  // else
  //   csFlag.LoBeam = 0;

  // 遠燈
  // if (D2.HiBeam == 1)
  //   csFlag.HiBeam = 1;
  // else
  //   csFlag.HiBeam = 0;

  // 左方向
  // if (D3.turnLight_L == 1)
  //   csFlag.turnLight_L = 1;
  // else
  //   csFlag.turnLight_L = 0;

  // 右方向
  // if (D3.turnLight_R == 1)
  //   csFlag.turnLight_R = 1;
  // else
  //   csFlag.turnLight_R = 0;
}

void getFrameData(void) {
  if (data_1byte_OK == 1) {
    data_1byte_OK = 0;
    // printf("%0
    // printf("\n
    switch (step) {
    case 0:
      if (rx_data == 0xC8) {
        step = 1;
        data_buf[0] = rx_data;
      } else {
        step = 0;
      }
      break;
    case 1:
      if (rx_data == 0x8B) {
        data_buf[1] = rx_data;
        step = 2;
      } else {
        step = 0;
      }
      break;
    case 2:
      data_buf[2] = rx_data;
      step = 3;
      break;
    case 3:
      data_buf[3] = rx_data;
      step = 4;
      break;
    case 4:
      data_buf[4] = rx_data;
      step = 5;
      break;
    case 5:
      data_buf[5] = rx_data;
      step = 6;
      break;
    case 6:
      data_buf[6] = rx_data;
      step = 7;
      break;
    case 7:
      data_buf[7] = rx_data;
      step = 8;
      break;
    case 8:
      data_buf[8] = rx_data;
      step = 9;
      break;
    case 9:
      data_buf[9] = rx_data;
      step = 10;
      break;
    case 10:
      data_buf[10] = rx_data;
      fFrame_OK = 1;
      step = 0;
      break;
    default:
      step = 0;
      break;
    }
  }
}

void ECCP3_CallBack(uint16_t capturedValue) {
  static uint16_t pluse_width_LO = 0;
  static uint16_t pluse_width_HI = 0;
  static uint8_t HI_us = 0;
  static uint8_t LO_us = 0;
  static uint8_t diff_us = 0;
  static uint16_t falling_edge_time = 0;
  static uint16_t rising_edge_time = 0;
  static bool rise_edge_flag = 0;
  static bool fHead = 0;
  static uint8_t data_cnt = 0;

  // falling edge tringgered
  if (Is_falling_edge()) {
    falling_edge_time = capturedValue;
    CCP3CON = 0x05; // set to rising edge trigger
    if (rise_edge_flag == 1) {
      rise_edge_flag = 0;

      if (falling_edge_time >= rising_edge_time) {
        pluse_width_HI = falling_edge_time - rising_edge_time;
      } else {
        pluse_width_HI = (0xffff - rising_edge_time) + falling_edge_time;
      }
      LO_us = (pluse_width_LO >> 3) & 0xff;
      HI_us = (pluse_width_HI >> 3) & 0xff;
      if (HI_us > LO_us)
        diff_us = HI_us - LO_us;
      else
        diff_us = LO_us - HI_us;

      if (fHead == 0) {
        UART_TX_SetHigh();
        // init
        if (diff_us < LOGIC_LEVEL) {
          // logical '0'
          fHead = 1;
          data_cnt = 0;
          UART_TX_SetLow();
        }
      } else {
        // data
        if (diff_us < LOGIC_LEVEL) {
          // logical '0'
          UART_TX_SetLow();
          rx_data &= ~(1 << data_cnt);
        } else {
          // logical '1'
          UART_TX_SetHigh();
          rx_data |= (1 << data_cnt);
        }

        data_cnt++;
        if (data_cnt > 7) {
          fHead = 0;
          data_1byte_OK = 1;
        }
      }
    }
  } // rising edge tringgered
  else if (Is_rising_edge()) {
    rising_edge_time = capturedValue;
    CCP3CON = 0x04; // set to falling edge trigger
    // 計算低電位脈衝寬度
    if (rising_edge_time >= falling_edge_time) {
      pluse_width_LO = rising_edge_time - falling_edge_time;
    } else {
      pluse_width_LO = (0xffff - falling_edge_time) + rising_edge_time;
    }
    rise_edge_flag = 1;
  }
}

// 如果2秒沒收到正確的信號 設定旗標
void TMR0_10MS_EvenHandler(void) {
  T10MS_CNT++;
  T10MS_CNT1++;
  if (T10MS_CNT >= 200) { // 2000ms
    fReceive_failed = 1;  // 2秒沒收到正確的資料
    T10MS_CNT = 0;
  }
}

// 消除未呼叫警告 假裝有使用這些函數
void Clear_No_Call_Warning(void) {
  uint8_t i = 0;
  if (i == 1) {
    EPWM2_LoadDutyValue(0);
    EPWM1_LoadDutyValue(0);
  } else if (i == 2) {
    TMR2_HasOverflowOccured();
  } else if (i == 3) {
    TMR2_LoadPeriodRegister(0);
    TMR2_WriteTimer(0);
    TMR2_ReadTimer();
    TMR2_StopTimer();
    TMR2_StartTimer();
  } else if (i == 4) {
    TMR0_Reload();
    TMR0_WriteTimer(0);
    TMR0_ReadTimer();
  } else if (i == 5) {
    TMR1_HasOverflowOccured();
    TMR1_CheckGateValueStatus();
    TMR1_StartSinglePulseAcquisition();
    TMR1_Reload();
    TMR1_WriteTimer(0);
    TMR1_ReadTimer();
    TMR1_StartTimer();
    TMR1_StopTimer();
  } else if (i == 5) {
    PIN_MANAGER_IOC();
  }
}

/*
                         Main application
 */
int main(void) {
  // initialize the device
  SYSTEM_Initialize();
  //timer0中斷副函式設定
  TMR0_SetInterruptHandler(TMR0_10MS_EvenHandler);

  // When using interrupts, you need to set the Global and Peripheral
  // Interrupt Enable bits Use the following macros to:

  // Enable the Global Interrupts
  INTERRUPT_GlobalInterruptEnable();

  // Enable the Peripheral Interrupts
  INTERRUPT_PeripheralInterruptEnable();

  // Disable the Global Interrupts
  // INTERRUPT_GlobalInterruptDisable();

  // Disable the Peripheral Interrupts
  // INTERRUPT_PeripheralInterruptDisable();

  // 燈具低配選擇 RB5
  fLamp_Conf = HiLoPe_state;
  // if (Is_LoPa())
  //     fLamp_Conf = LAMP_LoPe;
  // else
  //     fLamp_Conf = LAMP_HiPe;

  // 左邊燈具選擇 RB3
  fLamp_RL_Side = RL_Side_state;
  // if (Is_R_Side())
  //     fLamp_RL_Side = LAMP_R_SIDE;
  // else
  //     fLamp_RL_Side = LAMP_L_SIDE;

  Clear_No_Call_Warning();

  while (1) {
    getFrameData();
    check_input();
    // LED_output();
    Exception_handling();
    if (Is_Receiver_ok()) {
      if (Is_unlock_signal()) {
        // 異常處理
        fUnlock = 0; // 重置異常狀態
      } else
        LED_output();
    } else {
      // 2秒沒收到正確的資料開啟近燈+P2
      fReceive_failed = 0;
      P2_ON();      // P2亮
      HiBeam_OFF(); // 遠燈 OFF
      LoBeam_ON();  // 近燈 ON
      POS_ON();     // POS ON
    }
  }
  return 0;
}

/**
 End of File
 */