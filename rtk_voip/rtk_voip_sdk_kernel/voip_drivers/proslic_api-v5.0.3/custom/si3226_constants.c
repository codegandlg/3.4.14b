/*
Copyright (c) 2008 Silicon Laboratories, Inc.
2008-03-18 16:06:28 */

#include "proslic.h"
#include "si3226.h"
Si3226_General_Cfg Si3226_General_Configuration  = {
BO_DCDC_QCUK,	
0x8720000L,	/* VBATR_EXPECT */
0x3D70000L,	/* VBATH_EXPECT */
0x0L,	    /* DCDC_FSW_VTHLO */
0x0L,	    /* DCDC_FSW_VHYST */
0x1300000L,	/* DCDC_VREF_MIN */
0x5A00000L,	/* DCDC_VREF_MIN_RING */
0x200000L,	/* DCDC_FSW_NORM */
0x200000L,	/* DCDC_FSW_NORM_LO */
0x200000L,	/* DCDC_FSW_RING */
0x100000L,	/* DCDC_FSW_RING_LO */
0xD980000L,	/* DCDC_DIN_LIM */
0xC00000L,	/* DCDC_VOUT_LIM */
0x0L,	    /* DCDC_DCFF_ENABLE */
0x500000L,	/* DCDC_UVHYST */
0x600000L,	/* DCDC_UVTHRESH */
0xB00000L,	/* DCDC_OVTHRESH */
0x1F00000L,	/* DCDC_OITHRESH */
0x100000L,	/* DCDC_SWDRV_POL */
0x300000L,	/* DCDC_SWFET */
0x600000L,	/* DCDC_VREF_CTRL */
0x200000L,	/* DCDC_RNGTYPE */
0x0L,	    /* DCDC_ANA_GAIN */
0x0L,	    /* DCDC_ANA_TOFF */
0x0L,	    /* DCDC_ANA_TONMIN */
0x0L,	    /* DCDC_ANA_TONMAX */
0x0L,	    /* DCDC_ANA_DSHIFT */
0x0L,	    /* DCDC_ANA_LPOLY */
0x7FeF000L,	/* COEF_P_HVIC */
0x68D000L,	/* P_TH_HVIC */
0x0,	    /* CM_CLAMP */
0x3F,	    /* AUTO */
0x0,	    /* DAA_CNTL */
0x7F,	    /* IRQEN1 */
0xFF,	    /* IRQEN2 */
0xFF,	    /* IRQEN3 */
0xFF,	    /* IRQEN4 */
0x0,	    /* ENHANCE */
0,	        /* DAA_ENABLE */
0x8000000L,	/* SCALE_KAUDIO */
0x151EB80L,	/* AC_ADC_GAIN */
};

Si3226_DTMFDec_Cfg Si3226_DTMFDec_Presets[] = {
	{0x2d40000L,0x1a660000L,0x2d40000L,0x6ba0000L,0x1dcc0000L,0x33f0000L,0xbd30000L,0x19d20000L,0x4150000L,0x188F0000L,0x4150000L,0xd970000L,0x18620000L,0xf1c0000L}
};
Si3226_GPIO_Cfg Si3226_GPIO_Configuration = {
	0,0,0,0,0,0,0
};

Si3226_CI_Cfg Si3226_CI_Presets [] = {
	{0}
};
Si3226_audioGain_Cfg Si3226_audioGain_Presets [] = {
	{0x1377080L,0},
	{0x80C3180L,0}
};

Si3226_Ring_Cfg Si3226_Ring_Presets[] ={
{ /* RING_F20_45VRMS_0VDC_LPR */
0x00050000L,		/* RTPER */
0x07EFD9D5L,		/* RINGFR */
0x001BD493L,		/* RINGAMP */
0x00000000L,		/* RINGPHAS */
0x00000000L,		/* RINGOF */
0x15E5200EL,		/* SLOPE_RING */
0x00D16348L,		/* IRING_LIM */
0x0068C6BBL,		/* RTACTH */
0x0FFFFFFFL,		/* RTDCTH */
0x00006000L,		/* RTACDB */
0x00006000L,		/* RTDCDB */
0x0064874DL,		/* VOV_RING_BAT */
0x0064874DL,		/* VOV_RING_GND */
0x049CE106L,		/* VBATR_EXPECT */
0x80,		/* RINGTALO */
0x3E,		/* RINGTAHI */
0x00,		/* RINGTILO */
0x7D,		/* RINGTIHI */
0x01000547L,		/* ADAP_RING_MIN_I */
0x00003000L,		/* COUNTER_IRING_VAL */
0x00051EB8L,		/* COUNTER_VTR_VAL */
0x0163063FL,		/* CONST_028 */
0x019E31F4L,		/* CONST_032 */
0x01F108BEL,		/* CONST_036 */
0x026D4AEEL,		/* CONST_046 */
0x00370000L,		/* RRD_DELAY */
0x00190000L,		/* RRD_DELAY2 */
0x01893740L,		/* DCDC_VREF_MIN_RNG */
0x58,		/* RINGCON */
0x01,		/* USERSTAT */
0x024E7083L,		/* VCM_RING */
0x024E7083L,		/* VCM_RING_FIXED */
0x003126E8L,		/* DELTA_VCM */
0x200000L,		/* DCDC_RNGTYPE */
},
{ /* RING_F20_45VRMS_0VDC_BAL */
0x00050000L,		/* RTPER */
0x07EFD9D5L,		/* RINGFR */
0x001BD493L,		/* RINGAMP */
0x00000000L,		/* RINGPHAS */
0x00000000L,		/* RINGOF */
0x15E5200EL,		/* SLOPE_RING */
0x00D16348L,		/* IRING_LIM */
0x0068C6BBL,		/* RTACTH */
0x0FFFFFFFL,		/* RTDCTH */
0x00006000L,		/* RTACDB */
0x00006000L,		/* RTDCDB */
0x0064874DL,		/* VOV_RING_BAT */
0x0064874DL,		/* VOV_RING_GND */
0x0565EFA2L,		/* VBATR_EXPECT */
0x80,		/* RINGTALO */
0x3E,		/* RINGTAHI */
0x00,		/* RINGTILO */
0x7D,		/* RINGTIHI */
0x01000547L,		/* ADAP_RING_MIN_I */
0x00003000L,		/* COUNTER_IRING_VAL */
0x00051EB8L,		/* COUNTER_VTR_VAL */
0x0163063FL,		/* CONST_028 */
0x019E31F4L,		/* CONST_032 */
0x01F108BEL,		/* CONST_036 */
0x026D4AEEL,		/* CONST_046 */
0x00370000L,		/* RRD_DELAY */
0x00190000L,		/* RRD_DELAY2 */
0x01893740L,		/* DCDC_VREF_MIN_RNG */
0x58,		/* RINGCON */
0x00,		/* USERSTAT */
0x02B2F7D1L,		/* VCM_RING */
0x02B2F7D1L,		/* VCM_RING_FIXED */
0x003126E8L,		/* DELTA_VCM */
0x200000L,		/* DCDC_RNGTYPE */
},
{ /* RING_F20_55VRMS_48VDC_LPR */
0x00050000L,		/* RTPER */
0x07EFD9D5L,		/* RINGFR */
0x002203D0L,		/* RINGAMP */
0x00000000L,		/* RINGPHAS */
0x049A5F69L,		/* RINGOF */
0x15E5200EL,		/* SLOPE_RING */
0x00D16348L,		/* IRING_LIM */
0x032EDF91L,		/* RTACTH */
0x0038E38EL,		/* RTDCTH */
0x00006000L,		/* RTACDB */
0x00006000L,		/* RTDCDB */
0x007ADE42L,		/* VOV_RING_BAT */
0x007ADE42L,		/* VOV_RING_GND */
0x08B5BA6CL,		/* VBATR_EXPECT */
0x80,		/* RINGTALO */
0x3E,		/* RINGTAHI */
0x00,		/* RINGTILO */
0x7D,		/* RINGTIHI */
0x0138EA01L,		/* ADAP_RING_MIN_I */
0x00003000L,		/* COUNTER_IRING_VAL */
0x00051EB8L,		/* COUNTER_VTR_VAL */
0x01A40BA6L,		/* CONST_028 */
0x01EA0D97L,		/* CONST_032 */
0x024C104FL,		/* CONST_036 */
0x02DF1463L,		/* CONST_046 */
0x00370000L,		/* RRD_DELAY */
0x00190000L,		/* RRD_DELAY2 */
0x01893740L,		/* DCDC_VREF_MIN_RNG */
0x58,		/* RINGCON */
0x01,		/* USERSTAT */
0x045ADD36L,		/* VCM_RING */
0x045ADD36L,		/* VCM_FIXED_RING */
0x003126E8L,		/* DELTA_VCM */
0x200000L,		/* DCDC_RNGTYPE */
}
};

Si3226_DCfeed_Cfg Si3226_DCfeed_Presets[] = {

    /* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=10.000,v_ilim=33.200, i_ilim=20.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
{ /* DCFEED_48V_20MA */
0x1DDF41C9L,    /* SLOPE_VLIM */
0x1EF68D5EL,    /* SLOPE_RFEED */
0x40A0E0L,      /* SLOPE_ILIM */
0x18AAD168L,    /* SLOPE_DELTA1 */
0x1CA39FFAL,    /* SLOPE_DELTA2 */
0x5A38633L,     /* V_VLIM */
0x5072476L,     /* V_RFEED */
0x3E67006L,     /* V_ILIM */
0xFDFAB5L,      /* CONST_RFEED */
0x5D0FA6L,      /* CONST_ILIM */
0x2D8D96L,      /* I_VLIM */
// thlin
#if 0 // default
0x5B0AFBL,      /* LCRONHK */
0x6D4060L,      /* LCROFFHK */
#else // for 20pps pulse dial det
0x500000L,      /* LCRONHK */
0x600000L,      /* LCROFFHK */
#endif
0x8000L,        /* LCRDBI */
0x48D595L,      /* LONGHITH */
0x3FBAE2L,      /* LONGLOTH */
0x8000L,        /* LONGDBI */
0x120000L,       /* LCRMASK */
0x80000L,       /* LCRMASK_POLREV */
0x140000L,      /* LCRMASK_STATE */
0x140000L,      /* LCRMASK_LINECAP */
0x1BA5E35L,     /* VCM_OH */
0x418937L,      /* VOV_BAT */
0x418937L       /* VOV_GND */
},
{ /* DCFEED_48V_25MA */
0x1D46EA04L,    /* SLOPE_VLIM */
0x1EA4087EL,    /* SLOPE_RFEED */
0x40A0E0L,      /* SLOPE_ILIM */
0x1A224120L,    /* SLOPE_DELTA1 */
0x1D4FB32EL,    /* SLOPE_DELTA2 */
0x5A38633L,     /* V_VLIM */ 
0x5072476L,     /* V_RFEED */ 
0x3E67006L,     /* V_ILIM */ 
0x13D7962L,     /* CONST_RFEED */ 
0x74538FL,      /* CONST_ILIM */ 
0x2D8D96L,      /* I_VLIM */ 
// thlin
#if 0 // default
0x5B0AFBL,      /* LCRONHK */
0x6D4060L,      /* LCROFFHK */
#else // for 20pps pulse dial det
0x500000L,      /* LCRONHK */
0x600000L,      /* LCROFFHK */
#endif
0x8000L,        /* LCRDBI */
0x48D595L,      /* LONGHITH */
0x3FBAE2L,      /* LONGLOTH */
0x8000L,        /* LONGDBI */
0x120000L,       /* LCRMASK */
0x80000L,       /* LCRMASK_POLREV */
0x140000L,      /* LCRMASK_STATE */
0x140000L,      /* LCRMASK_LINECAP */
0x1BA5E35L,     /* VCM_OH */
0x418937L,      /* VOV_BAT */
0x418937L       /* VOV_GND */
}
};

#if 0 //thlin

Si3226_Impedance_Cfg Si3226_Impedance_Presets[] =
{ 
	// 0: USA, TW, HK, Japan (600 ohm)
	{{0x07F95E80, 0x0024CF80, 0x1FEF3300, 0x00159D00, 0x04D30480, 0x00097500, 0x1FEF2800, 0x00036800}, {0x000A0000, 0x00290000, 0x01380000, 0x01780000, 0x00E10000, 0x1FF50000, 0x00150000, 0x1FEB0000, 0x00080000, 0x1FF20000, 0x02860000, 0x056C0000}, {0x012D9280, 0x1E21B900, 0x00B12380, 0x01880F00, 0x0677A400, 0x5D}, {0x08000000, 0}, {0x02000000, 0}, 0x07B33300, 0x184CCD00, 0x07666680}, // Si3226 GUI compute
	//{{0x07F95B80, 0x0024CA80, 0x1FEF3D00, 0x0015A300, 0x0513F900, 0x00098D80, 0x1FEDEB80, 0x00035F00}, {0x000A0000, 0x00280000, 0x01310000, 0x016D0000, 0x00DD0000, 0x1FF20000, 0x00180000, 0x1FE90000, 0x000A0000, 0x1FF00000, 0x02480000, 0x05A80000}, {0x013F4480, 0x1E062A00, 0x00BB1480, 0x0026D500, 0x07D89900, 0x64}, {0x08000000, 0}, {0x02000000, 0}, 0x07B43980, 0x184BC680, 0x07687280}, //rev-0.24 table
	// 1: Japan (600 ohm + 1000nF)
	{{0x07D71D80, 0x00248400, 0x1FEF3A00, 0x00154280, 0x04E7B780, 0x0019E180, 0x1FF6C700, 0x000A6180}, {0x00060000, 0x00350000, 0x013D0000, 0x01C00000, 0x01340000, 0x00560000, 0x00870000, 0x003A0000, 0x00860000, 0x00390000, 0x000A0000, 0x07E60000}, {0x00316980, 0x1FA1F800, 0x002A6800, 0x0CF3D700, 0x1B027D00, 0x60}, {0x08000000, 0}, {0x02000000, 0}, 0x071CAC00, 0x18E35400, 0x06395800},
	// 2: China-1 (200 ohm + (560 ohm||100nF))
	{{0x0747A600, 0x001F2700, 0x1FF19E00, 0x00136480, 0x0513DF00, 0x1F058300, 0x001DEB80, 0x1FFFFD00}, {0x1FD60000, 0x00710000, 0x00750000, 0x01FC0000, 0x011C0000, 0x00A70000, 0x00280000, 0x000B0000, 0x00010000, 0x1FFC0000, 0x0AC50000, 0x1D2E0000}, {0x1D88CB00, 0x04A5C900, 0x1DD15C80, 0x0F165500, 0x18E96A00, 0xE5}, {0x08000000, 0}, {0x02000000, 0}, 0x07A2D100, 0x185D2F00, 0x0745A200},
	// 3: China-2 (200 ohm + (680 ohm||100nF))
	{{0x06DA1700, 0x001D8C00, 0x1FF24B00, 0x00121580, 0x04FB8E00, 0x1EC26200, 0x002A1780, 0x1FFF8E80}, {0x1FFD0000, 0x002E0000, 0x00D20000, 0x01D00000, 0x017F0000, 0x00C10000, 0x00480000, 0x001A0000, 0x00070000, 0x1FF60000, 0x0B310000, 0x1CC90000}, {0x1D5BD800, 0x04DE4E80, 0x1DC5D180, 0x0EC58A00, 0x193A4F00, 0xE4}, {0x08000000, 0}, {0x02000000, 0}, 0x079A9F80, 0x18656080, 0x07353F80},
	// 4: CTR21 (270 ohm + (750 ohm||150nF))
	{{0x06B6B000, 0x001E9280, 0x1FF1E900, 0x0011EE80, 0x054EFE00, 0x1E187D80, 0x002A8300, 0x00087E80}, {0x1FDF0000, 0x00610000, 0x009D0000, 0x01F60000, 0x016D0000, 0x00F20000, 0x007E0000, 0x00460000, 0x00260000, 0x1FD80000, 0x0C8C0000, 0x1B6C0000}, {0x1E8F7B00, 0x025CDE80, 0x1F139000, 0x0E4CB300, 0x19B2ED00, 0xC9}, {0x08000000, 0}, {0x02000000, 0}, 0x077DF380, 0x18820C80, 0x06FBE780},
	// 5: Australia, Spain (220 ohm + (820 ohm||120nF))
	{{0x068AFA00, 0x001D2A00, 0x1FF22280, 0x00113280, 0x05250080, 0x1E2E7680, 0x003B3000, 0x0001CE00}, {0x1FE00000, 0x00610000, 0x00940000, 0x020E0000, 0x01850000, 0x01070000, 0x007E0000, 0x00400000, 0x001F0000, 0x1FDE0000, 0x0C380000, 0x1BC00000}, {0x1D98FE00, 0x04201400, 0x1E46F080, 0x0E203400, 0x19DFC000, 0xE3}, {0x08000000, 0}, {0x02000000, 0}, 0x07862500, 0x1879DB00, 0x070C4980},
	// 6: Germany (220 ohm + (820 ohm||115nF))
	{{0x0681DA00, 0x001D2C00, 0x1FF23C80, 0x00114680, 0x0512E700, 0x1E436E80, 0x00396C00, 0x00014E80}, {0x00010000, 0x00270000, 0x00DF0000, 0x01D80000, 0x01AF0000, 0x00F80000, 0x007C0000, 0x003D0000, 0x001C0000, 0x1FE10000, 0x0C180000, 0x1BE20000}, {0x1D8FCA00, 0x04386C00, 0x1E37B380, 0x0E283100, 0x19D77B00, 0xE0}, {0x08000000, 0}, {0x02000000, 0}, 0x07893780, 0x1876C880, 0x07126E80},
	// 7: Italy (400 ohm + (700 ohm||200nF))
	{{0x06A35F80, 0x001E9B80, 0x1FF20600, 0x0011C700, 0x051A1480, 0x1E5F3C80, 0x1FF2C500, 0x0010EA80}, {0x1FFA0000, 0x00430000, 0x00F00000, 0x01FD0000, 0x01750000, 0x00DE0000, 0x00800000, 0x00500000, 0x00310000, 0x1FCD0000, 0x0D120000, 0x1AE80000}, {0x1F8D8C80, 0x00AE3780, 0x1FC43D80, 0x0EDF3D00, 0x1920B800, 0x8E}, {0x08000000, 0}, {0x02000000, 0}, 0x076F9D80, 0x18906280, 0x06DF3B80},
	// 8: Sweden (200 ohm + (1000 ohm||100nF))
	{{0x061D5300, 0x001ABD80, 0x1FF31F80, 0x000FAB00, 0x04E07480, 0x1E2CB000, 0x0043D900, 0x1FFEE100}, {0x1FF20000, 0x00460000, 0x00C20000, 0x02290000, 0x01D70000, 0x01380000, 0x009C0000, 0x00510000, 0x00280000, 0x1FD50000, 0x0C440000, 0x1BB40000}, {0x1D0F9F00, 0x055D6280, 0x1D92F800, 0x0EA8F400, 0x1956F000, 0xE3}, {0x08000000, 0}, {0x02000000, 0}, 0x07841880, 0x187BE780, 0x07083100},
	// 9: Belgium (150 ohm + (830 ohm||72nF))
	{{0x06721B00, 0x001BC500, 0x1FF44100, 0x00114080, 0x049A6C80, 0x1EF58100, 0x0026F780, 0x1FFDBA00}, {0x00030000, 0x00240000, 0x00F00000, 0x02000000, 0x01C70000, 0x00D70000, 0x004A0000, 0x00160000, 0x00040000, 0x1FF80000, 0x0ABF0000, 0x1D3B0000}, {0x17ED4880, 0x0AD7B980, 0x1D3AFF00, 0x0328CC00, 0x04D6C500, 0xFF}, {0x08000000, 0}, {0x02000000, 0}, 0x079FBE80, 0x18604180, 0x073F7D00},
	// 10: France (215 ohm + (1000 ohm||137nF))
	{{0x06676800, 0x001B8D00, 0x1FF37D00, 0x00112E80, 0x05666E80, 0x1D930400, 0x004D0680, 0x0004AD00}, {0x1FDB0000, 0x00640000, 0x008A0000, 0x02030000, 0x019D0000, 0x01360000, 0x00B80000, 0x00720000, 0x00460000, 0x1FB80000, 0x0D030000, 0x1AF60000}, {0x1DBCC080, 0x03B9A100, 0x1E898E00, 0x0E041E00, 0x19FBAC00, 0xEC}, {0x08000000, 0}, {0x02000000, 0}, 0x07687280, 0x18978D80, 0x06D0E580},
	// 11: Norway (120 ohm + (820 ohm||110nF))
	{{0x06CDA480, 0x000A5E80, 0x1FFDA400, 0x00192100, 0x0557D700, 0x1E07CF80, 0x005C2700, 0x1FFA8000}, {0x00CA0000, 0x1EB20000, 0x028B0000, 0x1FF60000, 0x02A70000, 0x00870000, 0x008F0000, 0x003D0000, 0x00190000, 0x1FE50000, 0x0B710000, 0x1C860000}, {0x0240BF00, 0x14FBC800, 0x08363980, 0x1F778900, 0x072B8800, 0xFE}, {0x08000000, 0}, {0x02000000, 0}, 0x078E5600, 0x1871AA00, 0x071CAC00},
	// 12: Denmark (300 ohm + (1000 ohm||220nF))
	{{0x06EA2000, 0x001ECC80, 0x1FF20780, 0x00127300, 0x05CC9400, 0x1D7A5200, 0x1FF63E00, 0x00186680}, {0x1FF70000, 0x003C0000, 0x00BD0000, 0x01BC0000, 0x01700000, 0x010B0000, 0x00BC0000, 0x008C0000, 0x00680000, 0x1F970000, 0x0DF70000, 0x1A050000}, {0x1F247500, 0x015AD380, 0x1F80B780, 0x0EDF8100, 0x19207600, 0xC1}, {0x08000000, 0}, {0x02000000, 0}, 0x072E1480, 0x18D1EB80, 0x065C2900},
	// 13: (900 ohm)
	{{0x0682CE80, 0x001E2100, 0x1FF24000, 0x0011B600, 0x03F1D380, 0x1FFF2B00, 0x1FF82D00, 0x1FFC5E80}, {0x000F0000, 0x003F0000, 0x01D10000, 0x02390000, 0x014B0000, 0x1FF60000, 0x00190000, 0x1FE70000, 0x00070000, 0x1FF00000, 0x02E30000, 0x050E0000}, {0x01B8A300, 0x1D49BC00, 0x00FDF580, 0x1FFFA000, 0x07FF8200, 0x2E}, {0x08000000, 0}, {0x02000000, 0}, 0x07B43980, 0x184BC680, 0x07687280},
	// 14: (900 ohm + 216nF)
	{{0x05A31380, 0x001BE200, 0x1FF2C600, 0x000F3380, 0x04695E00, 0x1FF92280, 0x1FF84D80, 0x1FF75100}, {0x1FA50000, 0x00FC0000, 0x014D0000, 0x03FE0000, 0x02610000, 0x01E80000, 0x01A00000, 0x01920000, 0x01840000, 0x1E7E0000, 0x0FAE0000, 0x18510000}, {0x001A8E00, 0x1FC21F00, 0x00222000, 0x0EC54600, 0x19312000, 0x33}, {0x08000000, 0}, {0x02000000, 0}, 0x05F5C280, 0x1A0A3D80, 0x03EB8500},

};

#else

#if 1
// config tool v5.2.1
Si3226_Impedance_Cfg Si3226_Impedance_Presets[] ={
{
{0x08066500L, 0x1FF6FC80L, 0x00050B00L, 0x1FFE2680L,    /* TXACEQ */
 0x0800D000L, 0x1FF6C700L, 0x00025700L, 0x1FFB2300L},   /* RXACEQ */
{0x00151F00L, 0x1FE4D400L, 0x01E78880L, 0x00573380L,    /* ECFIR/ECIIR */
 0x020AF880L, 0x1EF79A80L, 0x00B35800L, 0x1F955080L,
 0x003E6600L, 0x1FBC7880L, 0x01E9AA00L, 0x05F27E00L},
{0x025AFA00L, 0x1C1FAD80L, 0x0185EC00L, 0x0A195E00L,    /* ZSYNTH */
 0x1DE3B180L, 0x31}, 
 0x08775F80L,   /* TXACGAIN */
 0x01450700L,   /* RXACGAIN */
 0x07B41580L, 0x184BEB00L, 0x07682B80L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* 600 */
{
{0x06CF4900L, 0x1FF74400L, 0x00042200L, 0x1FFE8980L,    /* TXACEQ */
 0x0801D580L, 0x1FF58C80L, 0x00022600L, 0x1FFB4300L},   /* RXACEQ */
{0x001F5E80L, 0x1FE62800L, 0x02A1E180L, 0x0114DB80L,    /* ECFIR/ECIIR */
 0x023BE600L, 0x1F679B80L, 0x00330380L, 0x0006CD00L,
 0x1FE22F00L, 0x00142580L, 0x022A5800L, 0x05B27980L},
{0x02887F00L, 0x1BD16480L, 0x01A6AD00L, 0x0A12B400L,    /* ZSYNTH */
 0x1DEA5880L, 0x00}, 
 0x08000000L,   /* TXACGAIN */
 0x01047D80L,   /* RXACGAIN */
 0x07B3DF80L, 0x184C2100L, 0x0767BE80L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* 900 */
{
{0x08087B80L, 0x1FEDBD00L, 0x00009500L, 0x1FFA6E80L,    /* TXACEQ */
 0x084A9900L, 0x0000FF80L, 0x00034180L, 0x1FFEC480L},   /* RXACEQ */
{0x1FEAFE00L, 0x009EA280L, 0x004CEA80L, 0x0319D500L,    /* ECFIR/ECIIR */
 0x1FB07680L, 0x01712C80L, 0x1FE8DF00L, 0x007E6F80L,
 0x005D9B00L, 0x1FA63000L, 0x0FA56D80L, 0x1859DF80L},
{0x011BA700L, 0x1E007380L, 0x00E2B200L, 0x0D9CC700L,    /* ZSYNTH */
 0x1A5B5F80L, 0x3D}, 
 0x084E0800L,   /* TXACGAIN */
 0x013ED400L,   /* RXACGAIN */
 0x07215F80L, 0x18DEA100L, 0x0642BF80L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* 600+1000 */
{
{0x07B23300L, 0x1FEBD700L, 0x00085D00L, 0x1FFDD500L,    /* TXACEQ */
 0x093C4B80L, 0x1E079980L, 0x00506100L, 0x1FE74380L},   /* RXACEQ */
{0x00098100L, 0x00157E00L, 0x00CFAA80L, 0x01CBE280L,    /* ECFIR/ECIIR */
 0x01098A00L, 0x00A65880L, 0x00608300L, 0x1F9EEA80L,
 0x006D4600L, 0x1F8C7C80L, 0x027B5C80L, 0x05623F80L},
{0x1EB3C080L, 0x012AB500L, 0x00208E00L, 0x0A1ADE00L,    /* ZSYNTH */
 0x1DE08280L, 0xC9}, 
 0x08000000L,   /* TXACGAIN */
 0x01255580L,   /* RXACGAIN */
 0x07B97E80L, 0x18468200L, 0x0772FD00L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* 200+560||100 */
{
{0x0736FA80L, 0x1FEB3F00L, 0x00082D00L, 0x1FFDD780L,    /* TXACEQ */
 0x09A10800L, 0x1D5B8B80L, 0x006B8C00L, 0x1FE03200L},   /* RXACEQ */
{0x1FFF7780L, 0x0031A800L, 0x00C25180L, 0x01C70F00L,    /* ECFIR/ECIIR */
 0x01D10D80L, 0x1FD37F00L, 0x01A3FD00L, 0x1EB36A00L,
 0x0111A500L, 0x1EE9C780L, 0x03955E00L, 0x044B0A00L},
{0x1EE29480L, 0x00B56C00L, 0x00675000L, 0x0A129500L,    /* ZSYNTH */
 0x1DEA5280L, 0xC2}, 
 0x08000000L,   /* TXACGAIN */
 0x0112D480L,   /* RXACGAIN */
 0x07B9E780L, 0x18461900L, 0x0773CF00L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* 200+680||100 */
{
{0x07129500L, 0x1FDB0C00L, 0x0008DB00L, 0x1FFDE480L,    /* TXACEQ */
 0x0A7C0680L, 0x1BB3E880L, 0x0075B280L, 0x1FDE9500L},   /* RXACEQ */
{0x0016D680L, 0x1FDC9180L, 0x01629700L, 0x013C5180L,    /* ECFIR/ECIIR */
 0x01B33380L, 0x010EFC80L, 0x00379E80L, 0x00708E00L,
 0x001CFD00L, 0x1FE12180L, 0x0CF80700L, 0x1AFAE100L},
{0x002D5A00L, 0x1F24C280L, 0x00ADAE00L, 0x0D904C00L,    /* ZSYNTH */
 0x1A6E7E80L, 0x9E}, 
 0x08000000L,   /* TXACGAIN */
 0x010B9C00L,   /* RXACGAIN */
 0x07BB6900L, 0x18449780L, 0x0776D280L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* 270+750||150 */
{
{0x06DF0B80L, 0x1FE0F200L, 0x000A4D00L, 0x1FFDFF80L,    /* TXACEQ */
 0x0A6CC700L, 0x1BDF5100L, 0x00992B80L, 0x1FD6CB80L},   /* RXACEQ */
{0x001FF900L, 0x1FBA9300L, 0x019C5A80L, 0x01051100L,    /* ECFIR/ECIIR */
 0x02088C80L, 0x01093C00L, 0x003B2700L, 0x006DCB80L,
 0x0015E400L, 0x1FE83700L, 0x0CA11D00L, 0x1B4F3600L},
{0x00EAEB00L, 0x1CE34A80L, 0x02313B00L, 0x0A910700L,    /* ZSYNTH */
 0x1D6C4480L, 0xAA}, 
 0x08000000L,   /* TXACGAIN */
 0x01048A00L,   /* RXACGAIN */
 0x07BBEC00L, 0x18441480L, 0x0777D800L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* 220+820||120 */
{
{0x06D2EB00L, 0x1FE29680L, 0x0009E880L, 0x1FFDF400L,    /* TXACEQ */
 0x0A568280L, 0x1C0C4400L, 0x00955E80L, 0x1FD7C280L},   /* RXACEQ */
{0x006ACB80L, 0x1E9F9B80L, 0x03F04300L, 0x1E13F080L,    /* ECFIR/ECIIR */
 0x04731480L, 0x0033F400L, 0x1FC40780L, 0x01289200L,
 0x1FC2A500L, 0x001C8200L, 0x031E9C80L, 0x1DA04080L},
{0x00CAA900L, 0x1D056B80L, 0x022F4F00L, 0x0A12AF00L,    /* ZSYNTH */
 0x1DEA5D80L, 0xAA}, 
 0x08000000L,   /* TXACGAIN */
 0x0102E480L,   /* RXACGAIN */
 0x07BBB500L, 0x18444B80L, 0x07776A00L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* 220+820||115 */
{
{0x06FCED80L, 0x1FDFF280L, 0x00033780L, 0x1FFEBA80L,    /* TXACEQ */
 0x0A262D80L, 0x1C287980L, 0x1FF81680L, 0x1FED5000L},   /* RXACEQ */
{0x000DC180L, 0x00068C80L, 0x01541E80L, 0x01930F00L,    /* ECFIR/ECIIR */
 0x019F8400L, 0x00E25500L, 0x00615D80L, 0x0062C200L,
 0x002CB900L, 0x1FD15800L, 0x0D1E8C80L, 0x1AD9C200L},
{0x007C5000L, 0x1EE2BE80L, 0x00A0E500L, 0x0EB1FE00L,    /* ZSYNTH */
 0x194D5D80L, 0x6B}, 
 0x08000000L,   /* TXACGAIN */
 0x0108C380L,   /* RXACGAIN */
 0x07B87380L, 0x18478D00L, 0x0770E700L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* 400+700||200 */
{
{0x0661E180L, 0x1FE6DF00L, 0x00075380L, 0x1FFDD780L,    /* TXACEQ */
 0x0A8D6C00L, 0x1B9D0900L, 0x00AC3980L, 0x1FD1A900L},   /* RXACEQ */
{0x00165A00L, 0x1FDFC600L, 0x015C2A00L, 0x01AB1300L,    /* ECFIR/ECIIR */
 0x01D96480L, 0x018FF600L, 0x0032FD00L, 0x00855200L,
 0x001FA880L, 0x1FDD7A80L, 0x0C75E300L, 0x1B814200L},
{0x1F3EFB80L, 0x1FD64980L, 0x00E9FB00L, 0x0A12B400L,    /* ZSYNTH */
 0x1DEA5880L, 0xB6}, 
 0x08000000L,   /* TXACGAIN */
 0x00F27200L,   /* RXACGAIN */
 0x07BF7A80L, 0x18408600L, 0x077EF500L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* 200+1000||100 */
{
{0x06BCC800L, 0x1FEFA700L, 0x00075B80L, 0x1FFDEA80L,    /* TXACEQ */
 0x09724480L, 0x1DA9AE00L, 0x00667580L, 0x1FE08700L},   /* RXACEQ */
{0x001C0480L, 0x1FC37880L, 0x01AC4700L, 0x0115FC00L,    /* ECFIR/ECIIR */
 0x02847180L, 0x00435400L, 0x00CB2B80L, 0x1F81E480L,
 0x009C0780L, 0x1F5C6100L, 0x01E12480L, 0x05FAEA00L},
{0x1C7E8A80L, 0x0534D800L, 0x1E4BCE80L, 0x0A12B400L,    /* ZSYNTH */
 0x1DEA5880L, 0xCF}, 
 0x08000000L,   /* TXACGAIN */
 0x0100F700L,   /* RXACGAIN */
 0x07BBED80L, 0x18441300L, 0x0777DA80L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* 150+830||72 */
{
{0x06B7CD80L, 0x1FDACA00L, 0x0009EC00L, 0x1FFDF700L,    /* TXACEQ */
 0x0B222300L, 0x1A47A380L, 0x00C11900L, 0x1FCED880L},   /* RXACEQ */
{0x000E0E00L, 0x1FEAA800L, 0x01566A00L, 0x01041680L,    /* ECFIR/ECIIR */
 0x026E6100L, 0x00B4A580L, 0x00E22980L, 0x006E8900L,
 0x00432580L, 0x1FBB1100L, 0x0D0AAD00L, 0x1AECE300L},
{0x1FBB2880L, 0x1F7CF680L, 0x00C76600L, 0x0C975500L,    /* ZSYNTH */
 0x1B66F980L, 0xC2}, 
 0x08000000L,   /* TXACGAIN */
 0x00FE0600L,   /* RXACGAIN */
 0x07BF8900L, 0x18407780L, 0x077F1200L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* 215+1000||137 */
{
{0x0750BC80L, 0x1FCA6880L, 0x00043700L, 0x1FFDE600L,    /* TXACEQ */
 0x0AE75800L, 0x19FAA580L, 0x00016A80L, 0x1FE51780L},   /* RXACEQ */
{0x0033D580L, 0x1F55CD80L, 0x029C1A00L, 0x1F243900L,    /* ECFIR/ECIIR */
 0x03DBBC00L, 0x1F6C9F80L, 0x016D1A80L, 0x0065A900L,
 0x00685400L, 0x1F977300L, 0x0DE71B80L, 0x1A10E400L},
{0x0028E300L, 0x1F5AB080L, 0x007C4700L, 0x0EABA100L,    /* ZSYNTH */
 0x1953B780L, 0xA1}, 
 0x08000000L,   /* TXACGAIN */
 0x01133D00L,   /* RXACGAIN */
 0x07BC6500L, 0x18439B80L, 0x0778CA80L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* 300+1000||220 */
{
{0x06CC5A80L, 0x1FF70780L, 0x00037D00L, 0x1FFE1F00L,    /* TXACEQ */
 0x0808A880L, 0x1FF73C80L, 0x00025700L, 0x1FFBAF00L},   /* RXACEQ */
{0x1FECA380L, 0x00B58D00L, 0x00E99F00L, 0x03A3BF00L,    /* ECFIR/ECIIR */
 0x00072C00L, 0x0120BC80L, 0x1FC5E300L, 0x0040AF00L,
 0x00269300L, 0x1FDC2D80L, 0x0F6AED80L, 0x1893CA00L},
{0x027CA000L, 0x1BE4B980L, 0x019E8B00L, 0x0A0E8100L,    /* ZSYNTH */
 0x1DEB7180L, 0x00}, 
 0x08000000L,   /* TXACGAIN */
 0x01041B80L,   /* RXACGAIN */
 0x07995A00L, 0x1866A680L, 0x0732B380L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
},  /* 900+2160 */
{
{0x08277600L, 0x1FB1BE80L, 0x1FFC1100L, 0x1FEED700L,    /* TXACEQ */
 0x09137680L, 0x1FBB6580L, 0x1FFF3600L, 0x1FF38580L},   /* RXACEQ */
{0x000A5B00L, 0x1F8BF100L, 0x0165D780L, 0x1FED3900L,    /* ECFIR/ECIIR */
 0x00B4EC00L, 0x006E8280L, 0x1FFC8E80L, 0x005A6280L,
 0x1FD65780L, 0x007C5E80L, 0x1FE52A80L, 0x007BBE00L},

{0x08CC5600L, 0x10000300L, 0x07089480L, 0x0073CD80L,    /* ZSYNTH */
 0x0716D180L, 0xF1}, 
 0x0CD02680L,   /* TXACGAIN */
 0x01CFF400L,   /* RXACGAIN */
 0x069BB380L, 0x19644C80L, 0x05376700L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* 250+750||150 */
{
{0x0808B300L, 0x1FE95600L, 0x00000C80L, 0x1FF7CB00L,    /* TXACEQ */
 0x084B8400L, 0x00001D80L, 0x00004100L, 0x00009D80L},   /* RXACEQ */
{0x000EB000L, 0x1FE7B900L, 0x010D6400L, 0x00469C00L,    /* ECFIR/ECIIR */
 0x01269480L, 0x1FD51B80L, 0x003FE700L, 0x00393F00L,
 0x00077F00L, 0x003DF880L, 0x00C07000L, 0x06CDF780L},

{0x1FE0CF80L, 0x00323480L, 0x1FECF380L, 0x0FDC7680L,    /* ZSYNTH */
 0x18237400L, 0xED}, 
 0x0BD9FB80L,   /* TXACGAIN */
 0x01AEF800L,   /* RXACGAIN */
 0x0721BA00L, 0x18DE4600L, 0x06437400L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },    /* 320+1150||230 */
{
{0x07045780L, 0x1FD3E280L, 0x0002B580L, 0x1FFE6300L,    /* TXACEQ */
 0x0AAF2000L, 0x1A839800L, 0x1FE16980L, 0x1FE61380L},   /* RXACEQ */
{0x003FC900L, 0x1F308A00L, 0x030AE500L, 0x1EE6CB00L,    /* ECFIR/ECIIR */
 0x044B0680L, 0x1F45DC00L, 0x01794600L, 0x00654380L,
 0x0064DA80L, 0x1F9AA580L, 0x0DD06780L, 0x1A269780L},
{0x00521500L, 0x1F215380L, 0x008C7C00L, 0x0EC4DC00L,    /* ZSYNTH */
 0x193A7980L, 0x83}, 
 0x08000000L,   /* TXACGAIN */
 0x01089680L,   /* RXACGAIN */
 0x07BBC280L, 0x18443E00L, 0x07778500L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 }  /* 350+1000||210 */
};
#else
Si3226_Impedance_Cfg Si3226_Impedance_Presets[] ={
{  /* Si3226_600_0_0_30_0.txt - ZSYN_600_0_0 */
  {0x07F46C00L,0x000E4600L,0x00008580L,0x1FFD6100L, /* TXACEQ */
   0x07EF5000L,0x0013F580L,0x1FFDE000L,0x1FFCB280L}, /* RXACEQ */
  {0x0027CB00L,0x1F8A8880L,0x02801180L,0x1F625C80L,  /* ECFIR/ECIIR */
   0x0314FB00L,0x1E6B8E80L,0x00C5FF00L,0x1FC96F00L,
   0x1FFD1200L,0x00023C00L,0x0ED29D00L,0x192A9400L},  
  {0x00810E00L,0x1EFEBE80L,0x00803500L,0x0FF66D00L, /* ZSYNTH */
   0x18099080L,0x59},  
  0x088E0D80L, /* TXACGAIN */
  0x01456D80L, /* RXACGAIN */
   0x07ABE580L,0x18541B00L,0x0757CB00L,  /* RXHPF */
  0, 0 /* 0dB RX, 0dB TX */
},
{  /* Si3226_270_750_150_30_0.txt - ZSYN_270_750_150*/
  {0x071F7A80L,0x1FD01280L,0x00132700L,0x1FFEF980L, /* TXACEQ */
   0x0A8AA300L,0x1B9A5500L,0x008E7F00L,0x1FD7F300L}, /* RXACEQ */
  {0x0068CA00L,0x1EAE1E00L,0x0394FA00L,0x1E94AE80L,  /* ECFIR/ECIIR */
   0x0356D800L,0x0166CA80L,0x1EC16380L,0x01DE2780L,
   0x1F852300L,0x0046BE80L,0x02F17C80L,0x1EBCD280L},  
  {0x028A0C00L,0x19EE4580L,0x03876100L,0x0A762700L, /* ZSYNTH */
   0x1D87A380L,0x93},  
  0x08000000L, /* TXACGAIN */
  0x0109C280L, /* RXACGAIN */
   0x07BC6F00L,0x18439180L,0x0778DE00L,  /* RXHPF */
  0, 0 /* 0dB RX, 0dB TX */
},
{  /* Si3226_370_620_310_30_0.txt - ZSYN_370_620_310 */
  {0x07E59E80L,0x1FD33400L,0x1FFDF800L,0x1FFD8300L, /* TXACEQ */
   0x09F38000L,0x1C1C5A00L,0x1F94D700L,0x1FDE5800L}, /* RXACEQ */
  {0x00234480L,0x1F9CDD00L,0x01F5D580L,0x1FF39000L,  /* ECFIR/ECIIR */
   0x02C17180L,0x1FBE2500L,0x00DFFE80L,0x00441A80L,
   0x003BF800L,0x1FC42400L,0x0D9EB380L,0x1A514580L},  
  {0x003ED200L,0x1F5D6B80L,0x0063B100L,0x0F12E200L, /* ZSYNTH */
   0x18EC9380L,0x8B},  
  0x08000000L, /* TXACGAIN */
  0x0127C700L, /* RXACGAIN */
   0x07B51200L,0x184AEE80L,0x076A2480L,  /* RXHPF */
  0, 0 /* 0dB RX, 0dB TX */
},
{  /* Si3226_220_820_120_30_0.txt - ZSYN_220_820_120 */
  {0x06E38480L,0x1FD33B00L,0x00069780L,0x1FFCAB80L, /* TXACEQ */
   0x0A78F680L,0x1BC5C880L,0x009AEA00L,0x1FD66D80L}, /* RXACEQ */
  {0x00378B00L,0x1F3FCA00L,0x02B5ED00L,0x1F2B6200L,  /* ECFIR/ECIIR */
   0x04189080L,0x1F8A4480L,0x01113680L,0x00373100L,
   0x001DAE80L,0x1FE02F00L,0x0C89C780L,0x1B689680L},  
  {0x02391100L,0x1A886080L,0x033E3B00L,0x0A136200L, /* ZSYNTH */
   0x1DEA4180L,0x8C},  
  0x08000000L, /* TXACGAIN */
  0x01019200L, /* RXACGAIN */
   0x07BD1680L,0x1842EA00L,0x077A2D00L,  /* RXHPF */
  0, 0 /* 0dB RX, 0dB TX */
},
{  /* Si3226_600_0_1000_30_0.txt - ZSYN_600_0_1000 */
  {0x07F83980L,0x00056200L,0x1FFEE880L,0x1FFB1900L, /* TXACEQ */
   0x08405000L,0x001DD200L,0x0003DB80L,0x00008700L}, /* RXACEQ */
  {0x1FEE8700L,0x009B2B00L,0x000C9680L,0x03700100L,  /* ECFIR/ECIIR */
   0x1F62E400L,0x01C77400L,0x1FBBCF80L,0x0089D500L,
   0x005CFF80L,0x1FA96E80L,0x0F679480L,0x18962A80L},  
  {0x00657C00L,0x1F2FA580L,0x006ADE00L,0x0FE12100L, /* ZSYNTH */
   0x181ED080L,0x57},  
  0x08618D80L, /* TXACGAIN */
  0x013E3400L, /* RXACGAIN */
   0x0717CE80L,0x18E83200L,0x062F9C80L,  /* RXHPF */
  0, 0 /* 0dB RX, 0dB TX */
},
{  /* Si3226_200_680_100_30_0.txt - ZSYN_200_680_100 */
  {0x073A7B00L,0x1FCEB400L,0x0002C680L,0x1FFD0780L, /* TXACEQ */
   0x09BA8580L,0x1D2DF780L,0x006F5000L,0x1FDFE200L}, /* RXACEQ */
  {0x0004B700L,0x000F9800L,0x01201200L,0x00E1D880L,  /* ECFIR/ECIIR */
   0x03314A00L,0x1E84A580L,0x029D2380L,0x1E6F3400L,
   0x00E99200L,0x1F121100L,0x0588BC00L,0x025CAE00L},  
  {0x01415C00L,0x1C98C180L,0x0225A500L,0x0A138200L, /* ZSYNTH */
   0x1DEA2280L,0x8E},  
  0x08000000L, /* TXACGAIN */
  0x010DFD80L, /* RXACGAIN */
   0x07BA2180L,0x1845DF00L,0x07744380L,  /* RXHPF */
  0, 0 /* 0dB RX, 0dB TX */
},
{  /* Si3226_220_820_115_30_0.txt - ZSYN_200_820_115 */
  {0x06D56380L,0x1FDF1900L,0x00095A00L,0x1FFDAA80L, /* TXACEQ */
   0x0A596300L,0x1C067880L,0x0095EF00L,0x1FD7AF00L}, /* RXACEQ */
  {0x00687000L,0x1EAE1800L,0x03983D80L,0x1EB14B00L,  /* ECFIR/ECIIR */
   0x037B3E80L,0x016FC900L,0x1ED60100L,0x01B17D80L,
   0x1FA20D00L,0x001CE900L,0x027D3380L,0x1DBDBA80L},  
  {0x00246300L,0x1E5E0580L,0x017D2300L,0x0A138100L, /* ZSYNTH */
   0x1DEA2280L,0xA7},  
  0x08000000L, /* TXACGAIN */
  0x01009500L, /* RXACGAIN */
   0x07BBEE80L,0x18441200L,0x0777DD80L,  /* RXHPF */
  0, 0 /* 0dB RX, 0dB TX */
}
};
#endif
#endif

Si3226_FSK_Cfg Si3226_FSK_Presets[] ={

    /* inputs: mark freq=1200.000, space freq2200.000, amp=0.220, baud=1200.000, startStopDis=0, interrupt depth = 0 */
    { 0x2232000L, 0x77C2000L, 0x3C0000L, 0x200000L, 0x6B60000L, 0x79C0000L,0, 0 }
};

Si3226_Tone_Cfg Si3226_Tone_Presets[] ={

    /* inputs:  freq1 = 350.000, amp1 = -18.000, freq2 = 440.000, amp2 = -18.000, ta1 = 0.000, ti1 = 0.000, ta2 = 0.000, ti2 = 0.000*/
    { {0x7B30000L, 0x3A000L, 0x0L, 0x0, 0x0, 0x0, 0x0}, {0x7870000L, 0x4A000L, 0x0L, 0x0, 0x0, 0x0, 0x0}, 0x66 },
    /* inputs:  freq1 = 480.000, amp1 = -18.000, freq2 = 620.000, amp2 = -18.000, ta1 = 0.500, ti1 = 0.500, ta2 = 0.500, ti2 = 0.500*/
    { {0x7700000L, 0x52000L, 0x0L, 0xA0, 0xF, 0xA0, 0xF}, {0x7120000L, 0x6A000L, 0x0L, 0xA0, 0xF, 0xA0, 0xF}, 0x66 },
    /* inputs:  freq1 = 480.000, amp1 = -18.000, freq2 = 440.000, amp2 = -18.000, ta1 = 2.000, ti1 = 4.000, ta2 = 2.000, ti2 = 4.000*/
    { {0x7700000L, 0x52000L, 0x0L, 0x80, 0x3E, 0x0, 0x7D}, {0x7870000L, 0x4A000L, 0x0L, 0x80, 0x3E, 0x0, 0x7D}, 0x66 },
    /* inputs:  freq1 = 480.000, amp1 = -18.000, freq2 = 620.000, amp2 = -18.000, ta1 = 0.300, ti1 = 0.200, ta2 = 0.300, ti2 = 0.200*/
    { {0x7700000L, 0x52000L, 0x0L, 0x60, 0x9, 0x40, 0x6}, {0x7120000L, 0x6A000L, 0x0L, 0x60, 0x9, 0x40, 0x6}, 0x66 },
    /* inputs:  freq1 = 480.000, amp1 = -18.000, freq2 = 620.000, amp2 = -18.000, ta1 = 0.200, ti1 = 0.200, ta2 = 0.200, ti2 = 0.200*/
    { {0x7700000L, 0x52000L, 0x0L, 0x40, 0x6, 0x40, 0x6}, {0x7120000L, 0x6A000L, 0x0L, 0x40, 0x6, 0x40, 0x6}, 0x66 }
};

#if 1 //thlin add
/*
** PCM configuration
*/
//#define ALAW			0
#define ALAW			4	//invert alaw even bit
#define ULAW			1
#define LINEAR_8BIT		2
#define LINEAR_16BIT		3
#endif
Si3226_PCM_Cfg Si3226_PCM_Presets[] ={

    /* inputs:  u-law narrowband positive edge, dtx positive edge, both disabled, tx hwy = A, rx hwy = A */
#if 1 //thlin add
    // narrowband
    { LINEAR_16BIT, 0x0, 0x0, 0x0 },
    { ALAW, 0x0, 0x0, 0x0 },
    { ULAW, 0x0, 0x0, 0x0 },
    // wideband
    { LINEAR_16BIT, 0x1, 0x0, 0x0 },
    { ALAW, 0x1, 0x0, 0x0 },
    { ULAW, 0x1, 0x0, 0x0 }
#else
    { 0x1, 0x0, 0x0, 0x0 }
#endif
};

