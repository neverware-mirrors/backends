/* sane - Scanner Access Now Easy.

   Copyright (C) 2019 Povilas Kanapickas <povilas@radix.lt>

   This file is part of the SANE package.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston,
   MA 02111-1307, USA.

   As a special exception, the authors of SANE give permission for
   additional uses of the libraries contained in this release of SANE.

   The exception is that, if you link a SANE library with other files
   to produce an executable, this does not by itself cause the
   resulting executable to be covered by the GNU General Public
   License.  Your use of that executable is in no way restricted on
   account of linking the SANE library code into it.

   This exception does not, however, invalidate any other reasons why
   the executable file might be covered by the GNU General Public
   License.

   If you submit changes to SANE to the maintainers to be included in
   a subsequent release, you agree by submitting the changes that
   those changes may be distributed with this exception intact.

   If you write modifications of your own for SANE, it is your choice
   whether to permit this exception to apply to your modifications.
   If you do not wish that, delete this exception notice.
*/

#ifndef BACKEND_GENESYS_GL843_REGISTERS_H
#define BACKEND_GENESYS_GL843_REGISTERS_H

#define REG01           0x01
#define REG01_CISSET	0x80
#define REG01_DOGENB	0x40
#define REG01_DVDSET	0x20
#define REG01_STAGGER   0x10
#define REG01_COMPENB	0x08
#define REG01_TRUEGRAY  0x04
#define REG01_SHDAREA	0x02
#define REG01_SCAN	0x01

#define REG02        	0x02
#define REG02_NOTHOME	0x80
#define REG02_ACDCDIS	0x40
#define REG02_AGOHOME	0x20
#define REG02_MTRPWR	0x10
#define REG02_FASTFED	0x08
#define REG02_MTRREV	0x04
#define REG02_HOMENEG	0x02
#define REG02_LONGCURV	0x01

#define REG03           0x03
#define REG03_LAMPDOG	0x80
#define REG03_AVEENB	0x40
#define REG03_XPASEL	0x20
#define REG03_LAMPPWR	0x10
#define REG03_LAMPTIM	0x0f

#define REG04        	0x04
#define REG04_LINEART	0x80
#define REG04_BITSET	0x40
#define REG04_AFEMOD	0x30
#define REG04_FILTER	0x0c
#define REG04_FESET	0x03

#define REG04S_AFEMOD   4

#define REG05 		0x05
#define REG05_DPIHW	0xc0
#define REG05_DPIHW_600	0x00
#define REG05_DPIHW_1200	0x40
#define REG05_DPIHW_2400	0x80
#define REG05_DPIHW_4800	0xc0
#define REG05_MTLLAMP	0x30
#define REG05_GMMENB	0x08
#define REG05_MTLBASE	0x03

#define REG06 		0x06
#define REG06_SCANMOD	0xe0
#define REG06S_SCANMOD	5
#define REG06_PWRBIT	0x10
#define REG06_GAIN4	0x08
#define REG06_OPTEST	0x07

#define	REG07_LAMPSIM	0x80

#define REG08_DECFLAG 	0x40
#define REG08_GMMFFR    0x20
#define REG08_GMMFFG    0x10
#define REG08_GMMFFB	0x08
#define REG08_GMMZR     0x04
#define REG08_GMMZG     0x02
#define REG08_GMMZB     0x01

#define REG09_MCNTSET	0xc0
#define REG09_EVEN1ST   0x20
#define REG09_BLINE1ST  0x10
#define REG09_BACKSCAN	0x08
#define REG09_ENHANCE	0x04
#define REG09_SHORTTG	0x02
#define REG09_NWAIT	0x01

#define REG09S_MCNTSET  6
#define REG09S_CLKSET   4

#define REG0B           0x0b
#define REG0B_DRAMSEL   0x07
#define REG0B_ENBDRAM   0x08
#define REG0B_ENBDRAM   0x08
#define REG0B_RFHDIS    0x10
#define REG0B_CLKSET    0xe0
#define REG0B_24MHZ     0x00
#define REG0B_30MHZ     0x20
#define REG0B_40MHZ     0x40
#define REG0B_48MHZ     0x60
#define REG0B_60MHZ     0x80

#define REG0D 		0x0d
#define REG0D_JAMPCMD   0x80
#define REG0D_DOCCMD    0x40
#define REG0D_CCDCMD    0x20
#define REG0D_FULLSTP   0x10
#define REG0D_SEND      0x08
#define REG0D_CLRMCNT   0x04
#define REG0D_CLRDOCJM  0x02
#define REG0D_CLRLNCNT	0x01

#define REG0F 		0x0f

#define REG_EXPR        0x10
#define REG_EXPG        0x12
#define REG_EXPB        0x14

#define REG16_CTRLHI	0x80
#define REG16_TOSHIBA	0x40
#define REG16_TGINV	0x20
#define REG16_CK1INV	0x10
#define REG16_CK2INV	0x08
#define REG16_CTRLINV	0x04
#define REG16_CKDIS	0x02
#define REG16_CTRLDIS	0x01

#define REG17_TGMODE	0xc0
#define REG17_TGMODE_NO_DUMMY	0x00
#define REG17_TGMODE_REF	0x40
#define REG17_TGMODE_XPA	0x80
#define REG17_TGW	0x3f
#define REG17S_TGW      0

#define REG18 		0x18
#define REG18_CNSET	0x80
#define REG18_DCKSEL	0x60
#define REG18_CKTOGGLE	0x10
#define REG18_CKDELAY	0x0c
#define REG18_CKSEL	0x03

#define REG_EXPDMY      0x19

#define REG1A_TGLSW2 	0x80
#define REG1A_TGLSW1 	0x40
#define REG1A_MANUAL3	0x02
#define REG1A_MANUAL1	0x01
#define REG1A_CK4INV	0x08
#define REG1A_CK3INV	0x04
#define REG1A_LINECLP	0x02

#define REG1C           0x1c
#define REG1C_TGTIME    0x07

#define REG1D_CK4LOW	0x80
#define REG1D_CK3LOW	0x40
#define REG1D_CK1LOW	0x20
#define REG1D_TGSHLD	0x1f
#define REG1DS_TGSHLD   0


#define REG1E           0x1e
#define REG1E_WDTIME	0xf0
#define REG1ES_WDTIME   4
#define REG1E_LINESEL	0x0f
#define REG1ES_LINESEL  0

#define REG21           0x21
#define REG_STEPNO      0x21
#define REG_FWDSTEP     0x22
#define REG_BWDSTEP     0x23
#define REG_FASTNO      0x24
#define REG_LINCNT      0x25

#define REG29           0x29
#define REG2A           0x2a
#define REG2B           0x2b
#define REG_DPISET      0x2c
#define REG2E           0x2e
#define REG2F           0x2f

#define REG_STRPIXEL    0x30
#define REG_ENDPIXEL    0x32
#define REG_DUMMY       0x34
#define REG_MAXWD       0x35
#define REG_LPERIOD     0x38
#define REG_FEEDL       0x3d

#define REG40           0x40
#define REG40_DOCSNR    0x80
#define REG40_ADFSNR    0x40
#define REG40_COVERSNR  0x20
#define REG40_CHKVER    0x10
#define REG40_DOCJAM    0x08
#define REG40_HISPDFLG  0x04
#define REG40_MOTMFLG   0x02
#define REG40_DATAENB   0x01

#define REG41_PWRBIT	0x80
#define REG41_BUFEMPTY	0x40
#define REG41_FEEDFSH	0x20
#define REG41_SCANFSH	0x10
#define REG41_HOMESNR	0x08
#define REG41_LAMPSTS	0x04
#define REG41_FEBUSY	0x02
#define REG41_MOTORENB	0x01

#define REG58_VSMP      0xf8
#define REG58S_VSMP     3
#define REG58_VSMPW     0x07
#define REG58S_VSMPW    0

#define REG59_BSMP      0xf8
#define REG59S_BSMP     3
#define REG59_BSMPW     0x07
#define REG59S_BSMPW    0

#define REG5A_ADCLKINV  0x80
#define REG5A_RLCSEL    0x40
#define REG5A_CDSREF    0x30
#define REG5AS_CDSREF   4
#define REG5A_RLC       0x0f
#define REG5AS_RLC      0

#define REG5E 		0x5e
#define REG5E_DECSEL    0xe0
#define REG5ES_DECSEL   5
#define REG5E_STOPTIM   0x1f
#define REG5ES_STOPTIM  0

#define REG_FMOVDEC     0x5f

#define REG60           0x60
#define REG60_Z1MOD	0x1f
#define REG61           0x61
#define REG61_Z1MOD	0xff
#define REG62           0x62
#define REG62_Z1MOD	0xff

#define REG63           0x63
#define REG63_Z2MOD	0x1f
#define REG64           0x64
#define REG64_Z2MOD	0xff
#define REG65           0x65
#define REG65_Z2MOD	0xff

#define REG67 		0x67

#define REG68 		0x68

#define REG67S_STEPSEL      6
#define REG67_STEPSEL	 0xc0
#define REG67_FULLSTEP	 0x00
#define REG67_HALFSTEP	 0x20
#define REG67_EIGHTHSTEP 0x60
#define REG67_16THSTEP   0x80

#define REG68S_FSTPSEL      6
#define REG68_FSTPSEL	 0xc0
#define REG68_FULLSTEP	 0x00
#define REG68_HALFSTEP	 0x20
#define REG68_EIGHTHSTEP 0x60
#define REG68_16THSTEP   0x80

#define REG_FSHDEC     	0x69
#define REG_FMOVNO     	0x6a

#define REG6B          	0x6b
#define REG6B_MULTFILM	0x80
#define REG6B_GPOM13	0x40
#define REG6B_GPOM12	0x20
#define REG6B_GPOM11	0x10
#define REG6B_GPOCK4    0x08
#define REG6B_GPOCP     0x04
#define REG6B_GPOLEDB   0x02
#define REG6B_GPOADF	0x01

#define REG6C           0x6c
#define REG6C_GPIO16    0x80
#define REG6C_GPIO15    0x40
#define REG6C_GPIO14    0x20
#define REG6C_GPIO13    0x10
#define REG6C_GPIO12    0x08
#define REG6C_GPIO11    0x04
#define REG6C_GPIO10    0x02
#define REG6C_GPIO9     0x01
#define REG6C_GPIOH	0xff
#define REG6C_GPIOL	0xff

#define REG_Z1MOD       0x60
#define REG_Z2MOD       0x63

#define REG6D           0x6d
#define REG6E           0x6e
#define REG6F           0x6f

#define REG_CK1MAP      0x74
#define REG_CK3MAP      0x77
#define REG_CK4MAP      0x7a

#define REG7E           0x7e

#define REG9D           0x9d
#define REG9DS_STEPTIM  2

#define REG87_LEDADD    0x04

#define REGA6   	0xa6
#define REGA6_GPIO24    0x80
#define REGA6_GPIO23    0x40
#define REGA6_GPIO22    0x20
#define REGA6_GPIO21    0x10
#define REGA6_GPIO20    0x08
#define REGA6_GPIO19    0x04
#define REGA6_GPIO18    0x02
#define REGA6_GPIO17    0x01
#define REGA7 		0xa7
#define REGA7_GPOE24    0x80
#define REGA7_GPOE23    0x40
#define REGA7_GPOE22    0x20
#define REGA7_GPOE21    0x10
#define REGA7_GPOE20    0x08
#define REGA7_GPOE19    0x04
#define REGA7_GPOE18    0x02
#define REGA7_GPOE17    0x01
#define REGA8 		0xa8
#define REGA8_GPOE27    0x20
#define REGA8_GPOE26    0x10
#define REGA8_GPOE25    0x08
#define REGA8_GPO27     0x04
#define REGA8_GPO26     0x02
#define REGA8_GPO25     0x01
#define REGA9 		0xa9
#define REGA9_GPO33     0x20
#define REGA9_GPO32     0x10
#define REGA9_GPO31     0x08
#define REGA9_GPO30     0x04
#define REGA9_GPO29     0x02
#define REGA9_GPO28     0x01

#endif // BACKEND_GENESYS_GL843_REGISTERS_H