/*
 * Le880NB_HV.c --
 *
 * This file contains profile data in byte format
 *
 * Project Info --
 *   File:   V:\work\VoIP\branches\VoIP-jungle-2.4-1.5\rlxlinux-v2.2\linux-2.6.30\rtk_voip-1.5\voip_drivers\zarlink\api_lib-2.16.1\profiles\Le880NB_HV.vpw
 *   Type:   Le71HR0864G Line Module Project - VBL Reg ABS Power Supply and 8.192MHz PCLK
 *   Date:   Thursday, February 17, 2011 18:53:42
 *   Device: VoicePort Le88286
 *
 *   This file was generated with Profile Wizard Version: P2.0.0
 */

#include "Le880NB_HV.h"


/************** Device Parameters **************/

/* Device Configuration Data */
const VpProfileDataType LE880_ABS_VBL_FLYBACK[] =
{
  /* Device Profile for VoicePort Device Family */
 0x00, 0xFF, 0x00, 0x20, /* Profile Index and Length */
 0x05, 0x00, /* Version and MPI Command */
 0x08, 0x00, /* PCLK Frequency */
 0x82, 0x00, /* Device Config 1, Battery Control Mode */
 0x40, 0x02, /* Clock Slot and Max Events */
 0x0A, 0x00, /* Device Tickrate */
 0xE4, 0x00, 0x05, 0x00, /* Switching Regulator Parameters */
 0x1E, 0x05, /* Y-Voltage, Z-Voltage */
 0x00, 0x00, 0x00, 0x00, /* RSVD */
 0xB1, 0x00, 0xB1, 0x00, 0xB0, 0x40, /* Regulator Timing Parameters */
 0x2C, 0x40, 0x2C, 0x40, 0x2C, 0x40  /* FreeRun Timing Parameters */
};

/************** AC Filter Coefficients **************/

/* AC FXS RF14 600 Ohm Coefficients [Default} */
const VpProfileDataType LE880_AC_FXS_RF14_DEF[] =
{
  /* AC Profile */
 0xA4, 0x00, 0xF4, 0x4C, 0x01, 0x49, 0xCA, 0xF5, 0x98, 0xAA, 0x7B, 0xAB,
 0x2C, 0xA3, 0x25, 0xA5, 0x24, 0xB2, 0x3D, 0x9A, 0x2A, 0xAA, 0xA6, 0x9F,
 0x01, 0x8A, 0x1D, 0x01, 0xA3, 0xA0, 0x2E, 0xB2, 0xB2, 0xBA, 0xAC, 0xA2,
 0xA6, 0xCB, 0x3B, 0x45, 0x88, 0x2A, 0x20, 0x3C, 0xBC, 0x4E, 0xA6, 0x2B,
 0xA5, 0x2B, 0x3E, 0xBA, 0x8F, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x2A, 0x42, 0xA1, 0xCB, 0x1B, 0xA3, 0xA8, 0xFB, 0x87, 0xAA,
 0xFB, 0x9F, 0xA9, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/* AC FXS RF14 900 Ohm Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_900[] =
{
  /* AC Profile */
 0xE4, 0x00, 0xF7, 0x4C, 0x01, 0x49, 0xCA, 0x0B, 0x98, 0x34, 0x3B, 0xB2,
 0x2C, 0x9F, 0xA4, 0xD2, 0xA3, 0x22, 0xAC, 0x9A, 0xA4, 0xAA, 0x67, 0x9F,
 0x01, 0x8A, 0xB3, 0xD0, 0xA6, 0x21, 0xAA, 0x34, 0x2B, 0x2F, 0x4A, 0xA6,
 0xAB, 0x2F, 0x4A, 0x37, 0x88, 0x2A, 0xC0, 0x23, 0xAC, 0x2A, 0x26, 0x32,
 0xA5, 0x3C, 0xAD, 0xA4, 0xA7, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0xAB, 0x49, 0xF1, 0x2A, 0x0A, 0x43, 0xC9, 0xF3, 0x27, 0xBC,
 0xE2, 0xAF, 0xAD, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/* AC FXS RF14 Australia 220+820//220nF Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_AU[] =
{
  /* AC Profile */
 0xBF, 0x00, 0xAF, 0x4C, 0x01, 0x49, 0xCA, 0xE8, 0x98, 0xBA, 0x41, 0x54,
 0xA2, 0x2A, 0x5B, 0xA4, 0xAA, 0x3A, 0xB3, 0x9A, 0x87, 0xA2, 0x97, 0x9F,
 0x01, 0x8A, 0x2E, 0x01, 0x23, 0x20, 0x3E, 0xA8, 0x2A, 0x57, 0xCB, 0x34,
 0xBA, 0xAC, 0xBB, 0xA6, 0x88, 0xAA, 0xB0, 0x49, 0xF6, 0x3B, 0x2E, 0x49,
 0xFF, 0xBC, 0xA5, 0x3A, 0xBE, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x34, 0x5C, 0x32, 0x25, 0x1A, 0xA1, 0x3B, 0x23, 0xB3, 0x5B,
 0x4D, 0xB5, 0xDC, 0x60, 0x96, 0x3B, 0xA1, 0x00
};

/* AC FXS RF14 Austria 220+820//115nF Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_AT[] =
{
  /* AC Profile */
 0x6D, 0x00, 0xA7, 0x4C, 0x01, 0x49, 0xCA, 0xEA, 0x98, 0xFA, 0xE1, 0x23,
 0xA2, 0xEA, 0xCB, 0xB3, 0xAA, 0xA3, 0xB3, 0x9A, 0x98, 0x92, 0x97, 0x9F,
 0x01, 0x8A, 0x2E, 0x01, 0xB3, 0x20, 0xAB, 0xA8, 0xCC, 0x24, 0x22, 0x97,
 0xAF, 0x8F, 0xD3, 0xBF, 0x88, 0x3B, 0xB0, 0x33, 0x46, 0x2A, 0x2E, 0x33,
 0x4F, 0xAE, 0xA5, 0xB3, 0xBE, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x2A, 0x42, 0x22, 0x98, 0x02, 0x42, 0xA2, 0x43, 0xA6, 0xD8,
 0xFA, 0x8F, 0x29, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/* AC FXS RF14 Belgium 150+830//72 Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_BE[] =
{
  /* AC Profile */
 0x8F, 0x00, 0x2E, 0x4C, 0x01, 0x49, 0xCA, 0xF3, 0x98, 0x2C, 0xA1, 0xB4,
 0xE3, 0x34, 0xCB, 0x2C, 0x2B, 0xA3, 0xA3, 0x9A, 0x87, 0xA6, 0x97, 0x9F,
 0x01, 0x8A, 0x42, 0xD0, 0xBA, 0x30, 0xC2, 0x49, 0xA3, 0x4C, 0xDC, 0xC2,
 0x22, 0x2B, 0xBA, 0xA4, 0x88, 0xB3, 0xB0, 0xA3, 0xA4, 0x4A, 0xDD, 0xCA,
 0xAD, 0x43, 0xA4, 0x22, 0xAD, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x3A, 0x42, 0xA1, 0xA4, 0x19, 0x81, 0x5A, 0x4B, 0x2F, 0xA2,
 0xFB, 0xAF, 0xBA, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/* AC FXS RF14 Brazil 900 Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_BR[] =
{
  /* AC Profile */
 0xE4, 0x00, 0xF7, 0x4C, 0x01, 0x49, 0xCA, 0x0B, 0x98, 0x34, 0x3B, 0xB2,
 0x2C, 0x9F, 0xA4, 0xD2, 0xA3, 0x22, 0xAC, 0x9A, 0xA4, 0xAA, 0x67, 0x9F,
 0x01, 0x8A, 0xB3, 0xD0, 0xA6, 0x21, 0xAA, 0x34, 0x2B, 0x2F, 0x4A, 0xA6,
 0xAB, 0x2F, 0x4A, 0x37, 0x88, 0x2A, 0xC0, 0xA2, 0xAC, 0x2A, 0x26, 0x32,
 0xA5, 0x4C, 0xAD, 0xA4, 0xA7, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x3D, 0x53, 0x22, 0x52, 0x1A, 0x22, 0x2B, 0x6A, 0x87, 0x29,
 0xFB, 0x2F, 0xFC, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/* AC FXA RF14 China 200+680//100nF Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_CN[] =
{
  /* AC Profile */
 0xB9, 0x00, 0x1F, 0x4C, 0x01, 0x49, 0xCA, 0xE9, 0x98, 0x5C, 0xB1, 0xAA,
 0x23, 0x43, 0xEB, 0x32, 0xAA, 0x23, 0xB3, 0x9A, 0x98, 0x94, 0x97, 0x9F,
 0x01, 0x8A, 0x2D, 0x01, 0x22, 0x30, 0x6A, 0x59, 0x2A, 0xCB, 0xB4, 0xB2,
 0xBF, 0x3B, 0xA3, 0x25, 0x88, 0x43, 0xC0, 0x3A, 0x45, 0x2B, 0xBD, 0x4C,
 0x2E, 0x4C, 0x25, 0x42, 0xAD, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0xA4, 0x5A, 0x32, 0xB5, 0x12, 0xA1, 0xA2, 0x3A, 0xA4, 0xB5,
 0x64, 0x97, 0xAA, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/* AC FXS RF14 Czech Republic 600 Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_CZ[] =
{
  /* AC Profile */
 0x1A, 0x00, 0x03, 0x4C, 0x01, 0x49, 0xCA, 0xF5, 0x98, 0xAA, 0x7B, 0xAB,
 0x2C, 0xA3, 0x25, 0xA5, 0x24, 0xB2, 0x3D, 0x9A, 0x22, 0xBA, 0xA6, 0x9F,
 0x01, 0x8A, 0xAA, 0xC0, 0x97, 0xA0, 0xAA, 0x33, 0xD2, 0xDB, 0x7C, 0x23,
 0x2A, 0xEB, 0x32, 0xB4, 0x88, 0x2A, 0x20, 0x6C, 0xBC, 0xCE, 0xA6, 0x2B,
 0xA5, 0xAA, 0x3E, 0xBA, 0x8F, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0xC3, 0x5A, 0x22, 0xC7, 0x16, 0xE2, 0x2B, 0x42, 0xC6, 0xC8,
 0xFA, 0x9F, 0x2B, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/* AC FXS RF14 Denmark 300+1000//220nF Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_DK[] =
{
  /* AC Profile */
 0x7D, 0x00, 0x9A, 0x4C, 0x01, 0x49, 0xCA, 0xE6, 0x98, 0x22, 0xA1, 0x52,
 0xD2, 0xE7, 0x24, 0xF4, 0xCD, 0xAA, 0x97, 0x9A, 0xCA, 0xB3, 0xA5, 0x6E,
 0x01, 0x8A, 0x32, 0xD0, 0xAB, 0x20, 0xAA, 0x49, 0x29, 0x7A, 0x32, 0xD6,
 0xA2, 0xED, 0xC5, 0x97, 0x88, 0xBA, 0xC0, 0x22, 0xA7, 0x2C, 0xFE, 0x42,
 0xDE, 0x22, 0xB6, 0xB3, 0x9F, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x23, 0x53, 0x32, 0x2C, 0x1B, 0x22, 0xCA, 0x23, 0x63, 0x2A,
 0x33, 0xD4, 0x2A, 0x40, 0x96, 0xA2, 0xA0, 0x00
};

/* AC FXS RF14 ETSI 270+750//150nF Harmonized Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_EU[] =
{
  /* AC Profile */
 0xD5, 0x00, 0xEC, 0x4C, 0x01, 0x49, 0xCA, 0xE8, 0x98, 0x4A, 0xA1, 0x34,
 0xA2, 0xA2, 0xAC, 0x2C, 0xBB, 0x2B, 0xA4, 0x9A, 0x98, 0x92, 0x97, 0x9F,
 0x01, 0x8A, 0x9C, 0xD0, 0x2B, 0x20, 0x8F, 0x39, 0x2A, 0xBB, 0xA4, 0x24,
 0xB2, 0xBC, 0x4B, 0x26, 0x88, 0xA3, 0xC0, 0xC2, 0x27, 0xBA, 0x3E, 0xAC,
 0xAF, 0xB4, 0x26, 0x24, 0x3F, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0xC3, 0x5F, 0x32, 0x98, 0x03, 0xA1, 0xAA, 0x23, 0xB3, 0x2C,
 0x4B, 0x55, 0x25, 0x60, 0x96, 0xF2, 0xC1, 0x00
};

/* AC FXS RF14 Finland 270+910//120 Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_FI[] =
{
  /* AC Profile */
 0xFF, 0x00, 0x8D, 0x4C, 0x01, 0x49, 0xCA, 0xEF, 0x98, 0x2A, 0xB1, 0x23,
 0x33, 0xB2, 0xAB, 0x44, 0xEB, 0xA4, 0x44, 0x9A, 0x98, 0x92, 0x97, 0x9F,
 0x01, 0x8A, 0x2D, 0x01, 0x24, 0x20, 0x25, 0xB8, 0x2D, 0x42, 0xDB, 0xBA,
 0xE3, 0x43, 0xF2, 0xAC, 0x88, 0xC4, 0xB0, 0xA2, 0xA7, 0x42, 0x3E, 0xA2,
 0x9F, 0x32, 0x26, 0xBE, 0x2F, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0xA2, 0x5C, 0x22, 0xA2, 0x13, 0xC1, 0xB3, 0x22, 0x23, 0x7A,
 0x3D, 0xB4, 0xD4, 0x50, 0x96, 0x3C, 0x41, 0x00
};

/* AC FXS RF14 France 215+1000//137nF Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_FR[] =
{
  /* AC Profile */
 0xFC, 0x00, 0xA4, 0x4C, 0x01, 0x49, 0xCA, 0xE7, 0x98, 0xBA, 0x31, 0xA5,
 0x62, 0x35, 0xAB, 0xAA, 0xAA, 0xAC, 0xA3, 0x9A, 0xAA, 0xA2, 0xC7, 0x6F,
 0x01, 0x8A, 0x2D, 0x01, 0xBB, 0x10, 0xC2, 0x38, 0x2A, 0xA1, 0xC2, 0xA9,
 0x23, 0xA2, 0x22, 0x2D, 0x88, 0xA3, 0xB0, 0x53, 0x55, 0x4A, 0xAD, 0xB2,
 0xAD, 0x5A, 0x25, 0x72, 0x5E, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x2A, 0x43, 0x22, 0x9F, 0x1A, 0x22, 0x4B, 0x33, 0xD5, 0x2C,
 0x7A, 0x87, 0xA8, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/* AC FXS RF14 German 220+820//115nF Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_DE[] =
{
  /* AC Profile */
 0xEB, 0x00, 0xDC, 0x4C, 0x01, 0x49, 0xCA, 0xEA, 0x98, 0xA2, 0xE1, 0x24,
 0xA2, 0x2A, 0xCB, 0xB3, 0xAA, 0xD3, 0xB3, 0x9A, 0x98, 0x92, 0x97, 0x9F,
 0x01, 0x8A, 0x2D, 0x01, 0x24, 0x20, 0x2C, 0xA8, 0xAB, 0x24, 0x22, 0x97,
 0xAF, 0x8F, 0x23, 0xBF, 0x88, 0x3B, 0xB0, 0x33, 0x46, 0x2A, 0x2E, 0x33,
 0x4F, 0xAE, 0xA5, 0xB3, 0xBE, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x53, 0x54, 0x32, 0xC4, 0x1A, 0xA1, 0x2B, 0x2A, 0xA3, 0xAA,
 0x4A, 0xA5, 0xBA, 0x60, 0x96, 0xD2, 0xA1, 0x00
};

/* AC FXS RF14 Greece 400+500//50nF Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_GR[] =
{
  /* AC Profile */
 0x63, 0x00, 0x7D, 0x4C, 0x01, 0x49, 0xCA, 0x03, 0x98, 0x22, 0xA6, 0x2C,
 0x3E, 0x2B, 0x87, 0x44, 0x36, 0x2A, 0x9F, 0x9A, 0x22, 0xBA, 0xA7, 0x9F,
 0x01, 0x8A, 0x7D, 0xD0, 0xAA, 0xA0, 0x22, 0xAA, 0x98, 0xB4, 0x23, 0xBE,
 0xAB, 0xBF, 0xAA, 0x97, 0x88, 0x3C, 0xC0, 0x3A, 0xBD, 0xED, 0x8F, 0xEB,
 0xA6, 0x3A, 0x8F, 0x2B, 0x6F, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0xD2, 0x52, 0x22, 0xA2, 0x13, 0xA1, 0x97, 0x32, 0x25, 0xCA,
 0x6B, 0x87, 0x2B, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/* AC FXS RF14 Hungary 600 Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_HU[] =
{
  /* AC Profile */
 0xA4, 0x00, 0xF4, 0x4C, 0x01, 0x49, 0xCA, 0xF5, 0x98, 0xAA, 0x7B, 0xAB,
 0x2C, 0xA3, 0x25, 0xA5, 0x24, 0xB2, 0x3D, 0x9A, 0x2A, 0xAA, 0xA6, 0x9F,
 0x01, 0x8A, 0x1D, 0x01, 0xA3, 0xA0, 0x2E, 0xB2, 0xB2, 0xBA, 0xAC, 0xA2,
 0xA6, 0xCB, 0x3B, 0x45, 0x88, 0x2A, 0x20, 0x3C, 0xBC, 0x4E, 0xA6, 0x2B,
 0xA5, 0x2B, 0x3E, 0xBA, 0x8F, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x2A, 0x42, 0xA1, 0xCB, 0x1B, 0xA3, 0xA8, 0xFB, 0x87, 0xAA,
 0xFB, 0x9F, 0xA9, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/* AC FXS RF14 India 600 Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_IN[] =
{
  /* AC Profile */
 0xA4, 0x00, 0xF4, 0x4C, 0x01, 0x49, 0xCA, 0xF5, 0x98, 0xAA, 0x7B, 0xAB,
 0x2C, 0xA3, 0x25, 0xA5, 0x24, 0xB2, 0x3D, 0x9A, 0x2A, 0xAA, 0xA6, 0x9F,
 0x01, 0x8A, 0x1D, 0x01, 0xA3, 0xA0, 0x2E, 0xB2, 0xB2, 0xBA, 0xAC, 0xA2,
 0xA6, 0xCB, 0x3B, 0x45, 0x88, 0x2A, 0x20, 0x3C, 0xBC, 0x4E, 0xA6, 0x2B,
 0xA5, 0x2B, 0x3E, 0xBA, 0x8F, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x2A, 0x42, 0xA1, 0xCB, 0x1B, 0xA3, 0xA8, 0xFB, 0x87, 0xAA,
 0xFB, 0x9F, 0xA9, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/* AC FXS RF14 Italy 180+630//60nF Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_IT[] =
{
  /* AC Profile */
 0xEE, 0x00, 0xBD, 0x4C, 0x01, 0x49, 0xCA, 0xF3, 0x98, 0x22, 0xD2, 0xBA,
 0x34, 0x22, 0x3C, 0x53, 0xAB, 0xAA, 0xB4, 0x9A, 0x98, 0x1B, 0x97, 0x9F,
 0x01, 0x8A, 0x9C, 0xD0, 0xBC, 0xD0, 0x2A, 0x2A, 0x3C, 0x4C, 0x4E, 0x23,
 0xD2, 0xBB, 0xDB, 0x55, 0x88, 0xAA, 0xC0, 0x2C, 0xB5, 0x23, 0xAD, 0x4A,
 0x5E, 0xAD, 0xB5, 0x2B, 0x2E, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0xA2, 0x52, 0xA1, 0xA3, 0x1A, 0x32, 0xAA, 0x4A, 0xAF, 0x3B,
 0xF4, 0x9F, 0x3A, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/* AC FXS RF14 Japan 600+1uF Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_JP[] =
{
  /* AC Profile */
 0x0C, 0x00, 0x79, 0x4C, 0x01, 0x49, 0xCA, 0xF9, 0x98, 0xAC, 0xEE, 0x2A,
 0x87, 0x2A, 0x3F, 0xA3, 0x97, 0xE5, 0xC3, 0x9A, 0xAB, 0xA2, 0x24, 0xA6,
 0xD0, 0x8A, 0x42, 0xB0, 0x2A, 0xA0, 0x34, 0xAA, 0x3B, 0xA2, 0xCA, 0xAA,
 0xA4, 0x24, 0x3B, 0x4D, 0x88, 0xBA, 0x30, 0xAC, 0x2D, 0xAA, 0xCE, 0xAA,
 0x25, 0x22, 0x2C, 0xB2, 0x65, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x3B, 0x52, 0xA1, 0xA5, 0x14, 0xA2, 0x23, 0x44, 0x24, 0xA5,
 0x4B, 0xA4, 0xAC, 0x40, 0x96, 0x32, 0xD0, 0x00
};

/* AC FXS RF14 S. Korea 600 Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_KR[] =
{
  /* AC Profile */
 0xA4, 0x00, 0xF4, 0x4C, 0x01, 0x49, 0xCA, 0xF5, 0x98, 0xAA, 0x7B, 0xAB,
 0x2C, 0xA3, 0x25, 0xA5, 0x24, 0xB2, 0x3D, 0x9A, 0x2A, 0xAA, 0xA6, 0x9F,
 0x01, 0x8A, 0x1D, 0x01, 0xA3, 0xA0, 0x2E, 0xB2, 0xB2, 0xBA, 0xAC, 0xA2,
 0xA6, 0xCB, 0x3B, 0x45, 0x88, 0x2A, 0x20, 0x3C, 0xBC, 0x4E, 0xA6, 0x2B,
 0xA5, 0x2B, 0x3E, 0xBA, 0x8F, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x2A, 0x42, 0xA1, 0xCB, 0x1B, 0xA3, 0xA8, 0xFB, 0x87, 0xAA,
 0xFB, 0x9F, 0xA9, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/* AC FXS RF14 Mexico 600 Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_MX[] =
{
  /* AC Profile */
 0xA4, 0x00, 0xF4, 0x4C, 0x01, 0x49, 0xCA, 0xF5, 0x98, 0xAA, 0x7B, 0xAB,
 0x2C, 0xA3, 0x25, 0xA5, 0x24, 0xB2, 0x3D, 0x9A, 0x2A, 0xAA, 0xA6, 0x9F,
 0x01, 0x8A, 0x1D, 0x01, 0xA3, 0xA0, 0x2E, 0xB2, 0xB2, 0xBA, 0xAC, 0xA2,
 0xA6, 0xCB, 0x3B, 0x45, 0x88, 0x2A, 0x20, 0x3C, 0xBC, 0x4E, 0xA6, 0x2B,
 0xA5, 0x2B, 0x3E, 0xBA, 0x8F, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x2A, 0x42, 0xA1, 0xCB, 0x1B, 0xA3, 0xA8, 0xFB, 0x87, 0xAA,
 0xFB, 0x9F, 0xA9, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/* AC FXS RF14 Netherlands 600/340+422//100 Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_NL[] =
{
  /* AC Profile */
 0x7D, 0x00, 0x38, 0x4C, 0x01, 0x49, 0xCA, 0xF5, 0x98, 0xAA, 0x7B, 0xAB,
 0x2C, 0xA3, 0x25, 0xA5, 0x24, 0xB2, 0x3D, 0x9A, 0x22, 0xBA, 0xA6, 0x9F,
 0x01, 0x8A, 0x34, 0xD0, 0xCD, 0xA0, 0xAB, 0xD3, 0x22, 0xAB, 0x4A, 0xA3,
 0xA2, 0x2C, 0xAB, 0x45, 0x88, 0x2A, 0x20, 0x6C, 0xBC, 0xCE, 0xA6, 0x2B,
 0xA5, 0xAA, 0x3E, 0xBA, 0x8F, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x62, 0x52, 0x22, 0xCD, 0x12, 0x23, 0x44, 0x7A, 0x87, 0x3B,
 0xFC, 0xBF, 0xBA, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/* AC FXS RF14 New Zealand 370+620//310nF Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_NZ[] =
{
  /* AC Profile */
 0x14, 0x00, 0x00, 0x4C, 0x01, 0x49, 0xCA, 0xE9, 0x98, 0xAA, 0xB3, 0xB2,
 0xA2, 0xDA, 0xA3, 0x2B, 0x36, 0xAA, 0x25, 0x9A, 0x2B, 0xA2, 0x27, 0x6F,
 0x01, 0x8A, 0x1D, 0x01, 0x52, 0x50, 0x2A, 0x3B, 0x45, 0xA9, 0xCA, 0xA3,
 0x2C, 0xAB, 0xAA, 0x27, 0x88, 0xC3, 0x50, 0xBA, 0x8F, 0x37, 0x2F, 0xC2,
 0xBE, 0xEA, 0x87, 0xAA, 0x87, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x53, 0x53, 0x32, 0xCB, 0x1C, 0x42, 0x32, 0x3B, 0xB3, 0xA2,
 0x42, 0xB4, 0x23, 0x50, 0x96, 0x42, 0xA0, 0x00
};

/* AC FXS RF14 Norway 120+820//110nF Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_NO[] =
{
  /* AC Profile */
 0x04, 0x00, 0xA8, 0x4C, 0x01, 0x49, 0xCA, 0xE4, 0x98, 0xAA, 0x41, 0x22,
 0xA1, 0xAA, 0x3B, 0x23, 0x2A, 0xD4, 0x23, 0x9A, 0x98, 0x92, 0x97, 0x9F,
 0x01, 0x8A, 0x2D, 0x01, 0xEA, 0x10, 0x22, 0xB8, 0x22, 0x3B, 0x23, 0xA1,
 0xAA, 0xBA, 0x32, 0xC4, 0x88, 0x25, 0xB0, 0x3B, 0x24, 0x2C, 0x2D, 0xA3,
 0xAC, 0xF4, 0xC4, 0xAB, 0xAD, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0xBB, 0x52, 0xE2, 0xBD, 0x1B, 0xA1, 0x3B, 0x2A, 0xA3, 0x3A,
 0x4A, 0x26, 0x29, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/* AC FXS RF14 Portugal 600 Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_PT[] =
{
  /* AC Profile */
 0xA4, 0x00, 0xF4, 0x4C, 0x01, 0x49, 0xCA, 0xF5, 0x98, 0xAA, 0x7B, 0xAB,
 0x2C, 0xA3, 0x25, 0xA5, 0x24, 0xB2, 0x3D, 0x9A, 0x2A, 0xAA, 0xA6, 0x9F,
 0x01, 0x8A, 0x1D, 0x01, 0xA3, 0xA0, 0x2E, 0xB2, 0xB2, 0xBA, 0xAC, 0xA2,
 0xA6, 0xCB, 0x3B, 0x45, 0x88, 0x2A, 0x20, 0x3C, 0xBC, 0x4E, 0xA6, 0x2B,
 0xA5, 0x2B, 0x3E, 0xBA, 0x8F, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x2A, 0x42, 0xA1, 0xCB, 0x1B, 0xA3, 0xA8, 0xFB, 0x87, 0xAA,
 0xFB, 0x9F, 0xA9, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/* AC FXS RF14 Slovenia 600/220+820//115 Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_SI[] =
{
  /* AC Profile */
 0x00, 0x00, 0x00, 0x4C, 0x00, 0x49, 0xCA, 0xF5, 0x98, 0x2A, 0x7B, 0xAB,
 0x2C, 0xA3, 0x25, 0xA5, 0x24, 0xB2, 0x3D, 0x9A, 0x2B, 0xAA, 0xA6, 0x9F,
 0x01, 0x8A, 0x1D, 0x01, 0x55, 0xA0, 0xA2, 0xA2, 0x57, 0x2B, 0xC6, 0x23,
 0x34, 0xBB, 0x6A, 0xC5, 0x88, 0x2A, 0x20, 0x6C, 0xBC, 0xCE, 0xA6, 0x2B,
 0xA5, 0xAA, 0x3E, 0xBA, 0x8F, 0x82, 0xA8, 0x71, 0x80, 0xC8, 0xF0, 0x50,
 0x00, 0x86, 0xC3, 0x5A, 0x22, 0xC7, 0x16, 0xE2, 0x2B, 0x42, 0xC6, 0xC8,
 0xFA, 0x9F, 0x2B, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/* AC FXS RF14 Spain 220+820//120nF Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_ES[] =
{
  /* AC Profile */
 0xBF, 0x00, 0xAF, 0x4C, 0x01, 0x49, 0xCA, 0xE8, 0x98, 0xBA, 0x41, 0x54,
 0xA2, 0x2A, 0x5B, 0xA4, 0xAA, 0x3A, 0xB3, 0x9A, 0x87, 0xA2, 0x97, 0x9F,
 0x01, 0x8A, 0x2E, 0x01, 0x23, 0x20, 0x3E, 0xA8, 0x2A, 0x57, 0xCB, 0x34,
 0xBA, 0xAC, 0xBB, 0xA6, 0x88, 0xAA, 0xB0, 0x49, 0xF6, 0x3B, 0x2E, 0x49,
 0xFF, 0xBC, 0xA5, 0x3A, 0xBE, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x34, 0x5C, 0x32, 0x25, 0x1A, 0xA1, 0x3B, 0x23, 0xB3, 0x5B,
 0x4D, 0xB5, 0xDC, 0x60, 0x96, 0x3B, 0xA1, 0x00
};

/* AC FXS RF14 Sweden 200+1000//100nF_900//30nF Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_SE[] =
{
  /* AC Profile */
 0x0E, 0x00, 0x42, 0x4C, 0x01, 0x49, 0xCA, 0xF0, 0x98, 0x24, 0xF1, 0xB4,
 0xA2, 0xAB, 0x4B, 0x4B, 0xAA, 0xA7, 0xC3, 0x9A, 0x98, 0x92, 0x97, 0x9F,
 0x01, 0x8A, 0x2D, 0x01, 0xB3, 0x20, 0x5B, 0xB8, 0x24, 0xA2, 0x3C, 0xAC,
 0xAA, 0x27, 0x32, 0x9F, 0x88, 0x3A, 0xA0, 0x3A, 0x25, 0x3A, 0xAD, 0x22,
 0xAD, 0xBE, 0x25, 0xA5, 0x2E, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x73, 0x52, 0x22, 0xD2, 0x12, 0xB1, 0x4A, 0x23, 0xC4, 0xC3,
 0x6A, 0x97, 0xA8, 0xF0, 0x96, 0x2E, 0x01, 0x00
};

/* AC FXS RF14 U.K. 300+1000//220_370+620//310nF Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_GB[] =
{
  /* AC Profile */
 0x24, 0x00, 0x0A, 0x4C, 0x01, 0x49, 0xCA, 0xE6, 0x98, 0x22, 0xA1, 0x52,
 0xD2, 0xE7, 0x24, 0xF4, 0xCD, 0xAA, 0x97, 0x9A, 0x2B, 0xA2, 0x27, 0x6E,
 0x01, 0x8A, 0x32, 0xD0, 0xCB, 0x20, 0xBB, 0x59, 0x5A, 0x4A, 0xAA, 0xB5,
 0x2C, 0x2D, 0x42, 0xC7, 0x88, 0xAA, 0xC0, 0xA3, 0xA7, 0xBA, 0xDE, 0x5C,
 0xCE, 0xAA, 0xA6, 0xA2, 0x9F, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x43, 0x5B, 0x32, 0x2C, 0x1D, 0x22, 0xAA, 0x2B, 0x33, 0xBA,
 0x3C, 0x34, 0xBA, 0x40, 0x96, 0xA2, 0xA0, 0x00
};

/* AC FXS RF14 US 900//2.16uF_1650//(100+5nF) Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_US_loaded[] =
{
  /* AC Profile */
 0x01, 0x00, 0x90, 0x4C, 0x01, 0x49, 0xCA, 0x0E, 0x98, 0x32, 0x2B, 0x24,
 0x7C, 0x4A, 0x24, 0x55, 0xA3, 0x3A, 0xBC, 0x9A, 0xBA, 0xB3, 0xF4, 0x2D,
 0x01, 0x8A, 0xEB, 0xC0, 0x26, 0x21, 0xB2, 0x35, 0xB3, 0xA6, 0x2A, 0xAD,
 0xD4, 0xA6, 0x52, 0x4F, 0x88, 0xA3, 0xD0, 0x23, 0xAA, 0xAF, 0x62, 0xEA,
 0xBA, 0x2A, 0xB3, 0x4A, 0x3D, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0x2B, 0x4C, 0x21, 0xAC, 0x03, 0xB2, 0xA5, 0x5A, 0xE4, 0x74,
 0xF3, 0xA5, 0xBE, 0x60, 0x96, 0xAA, 0xC0, 0x00
};

/* AC FXS RF14 US 900//2,16uF_800//(100+50nF) Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_US_Nonloaded[] =
{
  /* AC Profile */
 0x01, 0x00, 0x90, 0x4C, 0x01, 0x49, 0xCA, 0x0E, 0x98, 0x32, 0x2B, 0x24,
 0x7C, 0x4A, 0x24, 0x55, 0xA3, 0x3A, 0xBC, 0x9A, 0xBA, 0xB3, 0xF4, 0x2D,
 0x01, 0x8A, 0xEB, 0xC0, 0x26, 0x21, 0xB2, 0x35, 0xB3, 0xA6, 0x2A, 0xAD,
 0xD4, 0xA6, 0x52, 0x4F, 0x88, 0xA3, 0xD0, 0x23, 0xAA, 0xAF, 0x62, 0xEA,
 0xBA, 0x2A, 0xB3, 0x4A, 0x3D, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0xBD, 0x52, 0xA1, 0x62, 0x1B, 0x22, 0x3A, 0x4B, 0xB5, 0x2A,
 0x65, 0xA7, 0xB6, 0x70, 0x96, 0x23, 0xC0, 0x00
};

/* AC FXS RF14 US 900//2.16uF Coefficients */
const VpProfileDataType LE880_AC_FXS_RF14_US_SS[] =
{
  /* AC Profile */
 0x01, 0x00, 0x90, 0x4C, 0x01, 0x49, 0xCA, 0x0E, 0x98, 0x32, 0x2B, 0x24,
 0x7C, 0x4A, 0x24, 0x55, 0xA3, 0x3A, 0xBC, 0x9A, 0xBA, 0xB3, 0xF4, 0x2D,
 0x01, 0x8A, 0xEB, 0xC0, 0x26, 0x21, 0xB2, 0x35, 0xB3, 0xA6, 0x2A, 0xAD,
 0xD4, 0xA6, 0x52, 0x4F, 0x88, 0xA3, 0xD0, 0x23, 0xAA, 0xAF, 0x62, 0xEA,
 0xBA, 0x2A, 0xB3, 0x4A, 0x3D, 0x82, 0xA8, 0x71, 0x80, 0xA9, 0xF0, 0x50,
 0x00, 0x86, 0xDA, 0x4A, 0x41, 0x3A, 0x02, 0x23, 0x3A, 0x42, 0x25, 0xAA,
 0x72, 0xA5, 0x2B, 0x50, 0x96, 0x1D, 0x01, 0x00
};

/************** DC Feed Parameters **************/

/* DC FXS Default 22 mA current feed */
const VpProfileDataType LE880_DC_FXS_DEF[] =
{
 /* DC Profile */
 0x00, 0x01, 0x00, 0x0B, 0x01, 0x08, 0xC2, 0x19, 0x84, 0xB0, 0x05, 0xC6,
 0x11, 0x04, 0x01
};

/************** Ring Signal Parameters **************/

/* Default Rnging, 25Hz, Sinewave, 80Vpk */
const VpProfileDataType LE880_RING_DEF[] =
{
  /* Sine, 25 Hz, 1.41 CF, 80.00 Vpk, 0.00 Bias */
  /* Ringing Profile */
 0x00, 0x04, 0x00, 0x12, 0x00, 0x0E, 0xD2, 0x00, 0x00, 0x00, 0x00, 0x44,
 0x42, 0x55, 0x00, 0x00, 0x00, 0x00, 0x68, 0x01, 0x00, 0x00
};

/* US Ringing, 20Hz, Sinewave, 80Vpk */
const VpProfileDataType LE880_RING_US[] =
{
  /* Sine, 20 Hz, 1.41 CF, 80.00 Vpk, 0.00 Bias */
  /* Ringing Profile */
 0x00, 0x04, 0x00, 0x12, 0x00, 0x0E, 0xD2, 0x00, 0x00, 0x00, 0x00, 0x37,
 0x42, 0x55, 0x00, 0x00, 0x00, 0x00, 0x68, 0x01, 0x00, 0x00
};

/* Canada Ringing 20Hz, Sinewave, 80Vpk */
const VpProfileDataType LE880_RING_CA[] =
{
  /* Sine, 20 Hz, 1.41 CF, 80.00 Vpk, 0.00 Bias */
  /* Ringing Profile */
 0x00, 0x04, 0x00, 0x12, 0x00, 0x0E, 0xD2, 0x00, 0x00, 0x00, 0x00, 0x37,
 0x42, 0x55, 0x00, 0x00, 0x00, 0x00, 0x68, 0x01, 0x00, 0x00
};

/* France Ringing 50Hz, Sinewave, 80Vpk */
const VpProfileDataType LE880_RING_FR[] =
{
  /* Sine, 50 Hz, 1.41 CF, 80.00 Vpk, 0.00 Bias */
  /* Ringing Profile */
 0x00, 0x04, 0x00, 0x12, 0x00, 0x0E, 0xD2, 0x00, 0x00, 0x00, 0x00, 0x89,
 0x42, 0x55, 0x00, 0x00, 0x00, 0x00, 0x68, 0x01, 0x00, 0x00
};

/* Japan Ringing 16Hz, Sinewave, 80Vpk */
const VpProfileDataType LE880_RING_JP[] =
{
  /* Sine, 16 Hz, 1.41 CF, 80.00 Vpk, 0.00 Bias */
  /* Ringing Profile */
 0x00, 0x04, 0x00, 0x12, 0x00, 0x0E, 0xD2, 0x00, 0x00, 0x00, 0x00, 0x2C,
 0x42, 0x55, 0x00, 0x00, 0x00, 0x00, 0x68, 0x01, 0x00, 0x00
};

/* S. Korea Ringing 20Hz, Sinewave, 80Vpk */
const VpProfileDataType LE880_RING_KR[] =
{
  /* Sine, 20 Hz, 1.41 CF, 80.00 Vpk, 0.00 Bias */
  /* Ringing Profile */
 0x00, 0x04, 0x00, 0x12, 0x00, 0x0E, 0xD2, 0x00, 0x00, 0x00, 0x00, 0x37,
 0x42, 0x55, 0x00, 0x00, 0x00, 0x00, 0x68, 0x01, 0x00, 0x00
};

/* Taiwan Ringing 20Hz, Sinewave, 80Vpk */
const VpProfileDataType LE880_RING_TW[] =
{
  /* Sine, 20 Hz, 1.41 CF, 80.00 Vpk, 0.00 Bias */
  /* Ringing Profile */
 0x00, 0x04, 0x00, 0x12, 0x00, 0x0E, 0xD2, 0x00, 0x00, 0x00, 0x00, 0x37,
 0x42, 0x55, 0x00, 0x00, 0x00, 0x00, 0x68, 0x01, 0x00, 0x00
};

/* Hong Kong Ringing 20Hz, Sinewave, 80Vpk */
const VpProfileDataType LE880_RING_HK[] =
{
  /* Sine, 20 Hz, 1.41 CF, 80.00 Vpk, 0.00 Bias */
  /* Ringing Profile */
 0x00, 0x04, 0x00, 0x12, 0x00, 0x0E, 0xD2, 0x00, 0x00, 0x00, 0x00, 0x37,
 0x42, 0x55, 0x00, 0x00, 0x00, 0x00, 0x68, 0x01, 0x00, 0x00
};

/* Singapore Ringing, 24Hz, Sinewave, 80Vpk */
const VpProfileDataType LE880_RING_SG[] =
{
  /* Sine, 24 Hz, 1.41 CF, 80.00 Vpk, 0.00 Bias */
  /* Ringing Profile */
 0x00, 0x04, 0x00, 0x12, 0x00, 0x0E, 0xD2, 0x00, 0x00, 0x00, 0x00, 0x42,
 0x42, 0x55, 0x00, 0x00, 0x00, 0x00, 0x68, 0x01, 0x00, 0x00
};

/* Austria Ringing 50Hz, Sinewave, 80Vpk */
const VpProfileDataType LE880_RING_AT[] =
{
  /* Sine, 50 Hz, 1.41 CF, 80.00 Vpk, 0.00 Bias */
  /* Ringing Profile */
 0x00, 0x04, 0x00, 0x12, 0x00, 0x0E, 0xD2, 0x00, 0x00, 0x00, 0x00, 0x89,
 0x42, 0x55, 0x00, 0x00, 0x00, 0x00, 0x68, 0x01, 0x00, 0x00
};

/* Australia Ringing 20Hz, Sinewave, 80Vpk */
const VpProfileDataType LE880_RING_AU[] =
{
  /* Sine, 20 Hz, 1.41 CF, 80.00 Vpk, 0.00 Bias */
  /* Ringing Profile */
 0x00, 0x04, 0x00, 0x12, 0x00, 0x0E, 0xD2, 0x00, 0x00, 0x00, 0x00, 0x37,
 0x42, 0x55, 0x00, 0x00, 0x00, 0x00, 0x68, 0x01, 0x00, 0x00
};

/************** Call Progress Tones **************/

/************** Cadence Definitions **************/

/* Short Ringing Cadence (2 sec on 4 sec off) */
const VpProfileDataType LE880_RING_CAD_STD[] =
{
  /* Cadence Profile */
 0x00, 0x08, 0x01, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x01, 0x07, 0x21, 0x90,
 0x01, 0x05, 0x23, 0x20, 0x40, 0x00
};

/* Short Ringing Cadence (0.5 sec on 0.5 sec off) */
const VpProfileDataType LE880_RING_CAD_SHORT[] =
{
  /* Cadence Profile */
 0x00, 0x08, 0x01, 0x0E, 0x00, 0x00, 0x00, 0x0A, 0x01, 0x07, 0x20, 0x64,
 0x01, 0x05, 0x20, 0x64, 0x40, 0x00
};

/************** Caller ID **************/

/************** Metering Profile **************/

/* end of file Le880NB_HV.c */
