/* sane - Scanner Access Now Easy.

   Copyright (C) 2003 Oliver Rauch
   Copyright (C) 2003, 2004 Henning Meier-Geinitz <henning@meier-geinitz.de>
   Copyright (C) 2004 Gerhard Jaeger <gerhard@gjaeger.de>
   Copyright (C) 2004-2013 Stéphane Voltz <stef.dev@free.fr>
   Copyright (C) 2005 Philipp Schmid <philipp8288@web.de>
   Copyright (C) 2005-2009 Pierre Willenbrock <pierre@pirsoft.dnsalias.org>
   Copyright (C) 2006 Laurent Charpentier <laurent_pubs@yahoo.com>
   Copyright (C) 2010 Chris Berry <s0457957@sms.ed.ac.uk> and Michael Rickmann <mrickma@gwdg.de>
                 for Plustek Opticbook 3600 support


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

#define DEBUG_DECLARE_ONLY

#include "gl841.h"
#include "gl841_registers.h"

#include <vector>

namespace genesys {

// Set address for writing data
static void gl841_set_buffer_address_gamma(Genesys_Device* dev, uint32_t addr)
{
    DBG_HELPER_ARGS(dbg, "setting address to 0x%05x", addr & 0xfffffff0);

  addr = addr >> 4;

    dev->write_register(0x5c, (addr & 0xff));

  addr = addr >> 8;
    dev->write_register(0x5b, (addr & 0xff));
}

bool CommandSetGl841::get_gain4_bit(Genesys_Register_Set* regs) const
{
    GenesysRegister *r = sanei_genesys_get_address(regs, 0x06);
    return (r && (r->value & REG06_GAIN4));
}

bool CommandSetGl841::test_buffer_empty_bit(SANE_Byte val) const
{
    return (val & REG41_BUFEMPTY);
}

/** copy sensor specific settings */
/* *dev  : device infos
   *regs : registers to be set
   extended : do extended set up
   ccd_size_divisor: set up for half ccd resolution
   all registers 08-0B, 10-1D, 52-59 are set up. They shouldn't
   appear anywhere else but in register_ini

Responsible for signals to CCD/CIS:
  CCD_CK1X (CK1INV(0x16),CKDIS(0x16),CKTOGGLE(0x18),CKDELAY(0x18),MANUAL1(0x1A),CK1MTGL(0x1C),CK1LOW(0x1D),CK1MAP(0x74,0x75,0x76),CK1NEG(0x7D))
  CCD_CK2X (CK2INV(0x16),CKDIS(0x16),CKTOGGLE(0x18),CKDELAY(0x18),MANUAL1(0x1A),CK1LOW(0x1D),CK1NEG(0x7D))
  CCD_CK3X (MANUAL3(0x1A),CK3INV(0x1A),CK3MTGL(0x1C),CK3LOW(0x1D),CK3MAP(0x77,0x78,0x79),CK3NEG(0x7D))
  CCD_CK4X (MANUAL3(0x1A),CK4INV(0x1A),CK4MTGL(0x1C),CK4LOW(0x1D),CK4MAP(0x7A,0x7B,0x7C),CK4NEG(0x7D))
  CCD_CPX  (CTRLHI(0x16),CTRLINV(0x16),CTRLDIS(0x16),CPH(0x72),CPL(0x73),CPNEG(0x7D))
  CCD_RSX  (CTRLHI(0x16),CTRLINV(0x16),CTRLDIS(0x16),RSH(0x70),RSL(0x71),RSNEG(0x7D))
  CCD_TGX  (TGINV(0x16),TGMODE(0x17),TGW(0x17),EXPR(0x10,0x11),TGSHLD(0x1D))
  CCD_TGG  (TGINV(0x16),TGMODE(0x17),TGW(0x17),EXPG(0x12,0x13),TGSHLD(0x1D))
  CCD_TGB  (TGINV(0x16),TGMODE(0x17),TGW(0x17),EXPB(0x14,0x15),TGSHLD(0x1D))
  LAMP_SW  (EXPR(0x10,0x11),XPA_SEL(0x03),LAMP_PWR(0x03),LAMPTIM(0x03),MTLLAMP(0x04),LAMPPWM(0x29))
  XPA_SW   (EXPG(0x12,0x13),XPA_SEL(0x03),LAMP_PWR(0x03),LAMPTIM(0x03),MTLLAMP(0x04),LAMPPWM(0x29))
  LAMP_B   (EXPB(0x14,0x15),LAMP_PWR(0x03))

other registers:
  CISSET(0x01),CNSET(0x18),DCKSEL(0x18),SCANMOD(0x18),EXPDMY(0x19),LINECLP(0x1A),CKAREA(0x1C),TGTIME(0x1C),LINESEL(0x1E),DUMMY(0x34)

Responsible for signals to AFE:
  VSMP  (VSMP(0x58),VSMPW(0x58))
  BSMP  (BSMP(0x59),BSMPW(0x59))

other register settings depending on this:
  RHI(0x52),RLOW(0x53),GHI(0x54),GLOW(0x55),BHI(0x56),BLOW(0x57),

*/
static void sanei_gl841_setup_sensor(Genesys_Device * dev, const Genesys_Sensor& sensor,
                                     Genesys_Register_Set * regs,
                                     bool extended, unsigned ccd_size_divisor)
{
    DBG(DBG_proc, "%s\n", __func__);

    // that one is tricky at least
    for (uint16_t addr = 0x08; addr <= 0x0b; ++addr) {
        regs->set8(0x70 + addr - 0x08, sensor.custom_regs.get_value(addr));
    }

    // ignore registers in range [0x10..0x16)
    for (uint16_t addr = 0x16; addr < 0x1e; ++addr) {
        regs->set8(addr, sensor.custom_regs.get_value(addr));
    }

    // ignore registers in range [0x5b..0x5e]
    for (uint16_t addr = 0x52; addr < 0x52 + 9; ++addr) {
        regs->set8(addr, sensor.custom_regs.get_value(addr));
    }

  /* don't go any further if no extended setup */
  if (!extended)
    return;

  /* todo : add more CCD types if needed */
  /* we might want to expand the Sensor struct to have these
     2 kind of settings */
    if (dev->model->sensor_id == SensorId::CCD_5345) {
        if (ccd_size_divisor > 1) {
          GenesysRegister* r;
	  /* settings for CCD used at half is max resolution */
	  r = sanei_genesys_get_address (regs, 0x70);
	  r->value = 0x00;
	  r = sanei_genesys_get_address (regs, 0x71);
	  r->value = 0x05;
	  r = sanei_genesys_get_address (regs, 0x72);
	  r->value = 0x06;
	  r = sanei_genesys_get_address (regs, 0x73);
	  r->value = 0x08;
	  r = sanei_genesys_get_address (regs, 0x18);
	  r->value = 0x28;
	  r = sanei_genesys_get_address (regs, 0x58);
	  r->value = 0x80 | (r->value & 0x03);	/* VSMP=16 */
	}
      else
	{
          GenesysRegister* r;
	  /* swap latch times */
	  r = sanei_genesys_get_address (regs, 0x18);
	  r->value = 0x30;
          regs->set8(0x52, sensor.custom_regs.get_value(0x55));
          regs->set8(0x53, sensor.custom_regs.get_value(0x56));
          regs->set8(0x54, sensor.custom_regs.get_value(0x57));
          regs->set8(0x55, sensor.custom_regs.get_value(0x52));
          regs->set8(0x56, sensor.custom_regs.get_value(0x53));
          regs->set8(0x57, sensor.custom_regs.get_value(0x54));
	  r = sanei_genesys_get_address (regs, 0x58);
	  r->value = 0x20 | (r->value & 0x03);	/* VSMP=4 */
	}
      return;
    }

    if (dev->model->sensor_id == SensorId::CCD_HP2300) {
      /* settings for CCD used at half is max resolution */
      GenesysRegister* r;
        if (ccd_size_divisor > 1) {
	  r = sanei_genesys_get_address (regs, 0x70);
	  r->value = 0x16;
	  r = sanei_genesys_get_address (regs, 0x71);
	  r->value = 0x00;
	  r = sanei_genesys_get_address (regs, 0x72);
	  r->value = 0x01;
	  r = sanei_genesys_get_address (regs, 0x73);
	  r->value = 0x03;
	  /* manual clock programming */
	  r = sanei_genesys_get_address (regs, 0x1d);
	  r->value |= 0x80;
	}
      else
	{
	  r = sanei_genesys_get_address (regs, 0x70);
	  r->value = 1;
	  r = sanei_genesys_get_address (regs, 0x71);
	  r->value = 3;
	  r = sanei_genesys_get_address (regs, 0x72);
	  r->value = 4;
	  r = sanei_genesys_get_address (regs, 0x73);
	  r->value = 6;
	}
      r = sanei_genesys_get_address (regs, 0x58);
      r->value = 0x80 | (r->value & 0x03);	/* VSMP=16 */
      return;
    }
}

/** Test if the ASIC works
 */
// TODO: make this functional
static void sanei_gl841_asic_test(Genesys_Device* dev)
{
    DBG_HELPER(dbg);

  size_t size, verify_size;
  unsigned int i;

    throw SaneException("not implemented");

    // set and read exposure time, compare if it's the same
    dev->write_register(0x38, 0xde);
    dev->write_register(0x39, 0xad);

    uint8_t val = dev->read_register(0x38);

  if (val != 0xde)		/* value of register 0x38 */
    {
        throw SaneException("register contains invalid value");
    }

    val = dev->read_register(0x39);

  if (val != 0xad)		/* value of register 0x39 */
    {
        throw SaneException("register contains invalid value");
    }

  /* ram test: */
  size = 0x40000;
  verify_size = size + 0x80;
  /* todo: looks like the read size must be a multiple of 128?
     otherwise the read doesn't succeed the second time after the scanner has
     been plugged in. Very strange. */

  std::vector<uint8_t> data(size);
  std::vector<uint8_t> verify_data(verify_size);

  for (i = 0; i < (size - 1); i += 2)
    {
      data[i] = i / 512;
      data[i + 1] = (i / 2) % 256;
    }

    sanei_genesys_set_buffer_address(dev, 0x0000);

    // sanei_genesys_bulk_write_data(dev, 0x3c, data, size);

    sanei_genesys_set_buffer_address(dev, 0x0000);

    sanei_genesys_bulk_read_data(dev, 0x45, verify_data.data(), verify_size);

  /* todo: why i + 2 ? */
  for (i = 0; i < size; i++)
    {
      if (verify_data[i] != data[i])
	{
	  DBG(DBG_info, "0x%.8x: got %.2x %.2x %.2x %.2x, expected %.2x %.2x %.2x %.2x\n",
	      i,
	      verify_data[i],
	      verify_data[i+1],
	      verify_data[i+2],
	      verify_data[i+3],
	      data[i],
	      data[i+1],
	      data[i+2],
	      data[i+3]);
            throw SaneException("data verification error");
	}
    }
}

/*
 * Set all registers LiDE 80 to default values
 * (function called only once at the beginning)
 * we are doing a special case to ease development
 */
static void
gl841_init_lide80 (Genesys_Device * dev)
{
  uint8_t val;

  INITREG (0x01, 0x82); /* 0x02 = SHDAREA  and no CISSET ! */
  INITREG (0x02, 0x10);
  INITREG (0x03, 0x50);
  INITREG (0x04, 0x02);
  INITREG (0x05, 0x4c); /* 1200 DPI */
  INITREG (0x06, 0x38); /* 0x38 scanmod=1, pwrbit, GAIN4 */
  INITREG (0x07, 0x00);
  INITREG (0x08, 0x00);
  INITREG (0x09, 0x11);
  INITREG (0x0a, 0x00);

  INITREG (0x10, 0x40);
  INITREG (0x11, 0x00);
  INITREG (0x12, 0x40);
  INITREG (0x13, 0x00);
  INITREG (0x14, 0x40);
  INITREG (0x15, 0x00);
  INITREG (0x16, 0x00);
  INITREG (0x17, 0x01);
  INITREG (0x18, 0x00);
  INITREG (0x19, 0x06);
  INITREG (0x1a, 0x00);
  INITREG (0x1b, 0x00);
  INITREG (0x1c, 0x00);
  INITREG (0x1d, 0x04);
  INITREG (0x1e, 0x10);
  INITREG (0x1f, 0x04);
  INITREG (0x20, 0x02);
  INITREG (0x21, 0x10);
  INITREG (0x22, 0x20);
  INITREG (0x23, 0x20);
  INITREG (0x24, 0x10);
  INITREG (0x25, 0x00);
  INITREG (0x26, 0x00);
  INITREG (0x27, 0x00);

  INITREG (0x29, 0xff);

  const auto& sensor = sanei_genesys_find_sensor_any(dev);
  INITREG (0x2c, sensor.optical_res>>8);
  INITREG (0x2d, sensor.optical_res & 0xff);
  INITREG (0x2e, 0x80);
  INITREG (0x2f, 0x80);
  INITREG (0x30, 0x00);
  INITREG (0x31, 0x10);
  INITREG (0x32, 0x15);
  INITREG (0x33, 0x0e);
  INITREG (0x34, 0x40);
  INITREG (0x35, 0x00);
  INITREG (0x36, 0x2a);
  INITREG (0x37, 0x30);
  INITREG (0x38, 0x2a);
  INITREG (0x39, 0xf8);

  INITREG (0x3d, 0x00);
  INITREG (0x3e, 0x00);
  INITREG (0x3f, 0x00);

  INITREG (0x52, 0x03);
  INITREG (0x53, 0x07);
  INITREG (0x54, 0x00);
  INITREG (0x55, 0x00);
  INITREG (0x56, 0x00);
  INITREG (0x57, 0x00);
  INITREG (0x58, 0x29);
  INITREG (0x59, 0x69);
  INITREG (0x5a, 0x55);

  INITREG (0x5d, 0x20);
  INITREG (0x5e, 0x41);
  INITREG (0x5f, 0x40);
  INITREG (0x60, 0x00);
  INITREG (0x61, 0x00);
  INITREG (0x62, 0x00);
  INITREG (0x63, 0x00);
  INITREG (0x64, 0x00);
  INITREG (0x65, 0x00);
  INITREG (0x66, 0x00);
  INITREG (0x67, 0x40);
  INITREG (0x68, 0x40);
  INITREG (0x69, 0x20);
  INITREG (0x6a, 0x20);
    INITREG(0x6c, 0x00);
    INITREG(0x6d, 0x00);
    INITREG(0x6e, 0x00);
    INITREG(0x6f, 0x00);
  INITREG (0x70, 0x00);
  INITREG (0x71, 0x05);
  INITREG (0x72, 0x07);
  INITREG (0x73, 0x09);
  INITREG (0x74, 0x00);
  INITREG (0x75, 0x01);
  INITREG (0x76, 0xff);
  INITREG (0x77, 0x00);
  INITREG (0x78, 0x0f);
  INITREG (0x79, 0xf0);
  INITREG (0x7a, 0xf0);
  INITREG (0x7b, 0x00);
  INITREG (0x7c, 0x1e);
  INITREG (0x7d, 0x11);
  INITREG (0x7e, 0x00);
  INITREG (0x7f, 0x50);
  INITREG (0x80, 0x00);
  INITREG (0x81, 0x00);
  INITREG (0x82, 0x0f);
  INITREG (0x83, 0x00);
  INITREG (0x84, 0x0e);
  INITREG (0x85, 0x00);
  INITREG (0x86, 0x0d);
  INITREG (0x87, 0x02);
  INITREG (0x88, 0x00);
  INITREG (0x89, 0x00);

    for (const auto& reg : dev->gpo.regs) {
        dev->reg.set8(reg.address, reg.value);
    }

    // specific scanner settings, clock and gpio first
    val = dev->read_register(REG6B);
    dev->write_register(REG6B, 0x0c);
    dev->write_register(0x06, 0x10);
    dev->write_register(REG6E, 0x6d);
    dev->write_register(REG6F, 0x80);
    dev->write_register(REG6B, 0x0e);
    val = dev->read_register(REG6C);
    dev->write_register(REG6C, 0x00);
    val = dev->read_register(REG6D);
    dev->write_register(REG6D, 0x8f);
    val = dev->read_register(REG6B);
    dev->write_register(REG6B, 0x0e);
    val = dev->read_register(REG6B);
    dev->write_register(REG6B, 0x0e);
    val = dev->read_register(REG6B);
    dev->write_register(REG6B, 0x0a);
    val = dev->read_register(REG6B);
    dev->write_register(REG6B, 0x02);
    val = dev->read_register(REG6B);
    dev->write_register(REG6B, 0x06);

    (void) val; // FIXME: we don't use the information read from registers

    sanei_genesys_write_0x8c(dev, 0x10, 0x94);
    dev->write_register(0x09, 0x10);

  // FIXME: the following code originally changed 0x6b, but due to bug the 0x6c register was
  // effectively changed. The current behavior matches the old code, but should probably be fixed.
  dev->reg.find_reg(0x6c).value |= REG6B_GPO18;
  dev->reg.find_reg(0x6c).value &= ~REG6B_GPO17;

    sanei_gl841_setup_sensor(dev, sensor, &dev->reg, 0, 1);
}

/*
 * Set all registers to default values
 * (function called only once at the beginning)
 */
static void
gl841_init_registers (Genesys_Device * dev)
{
  int addr;

  DBG(DBG_proc, "%s\n", __func__);

  dev->reg.clear();
    if (dev->model->model_id == ModelId::CANON_LIDE_80) {
      gl841_init_lide80(dev);
      return ;
    }

    for (addr = 1; addr <= 0x0a; addr++) {
        dev->reg.init_reg(addr, 0);
    }
    for (addr = 0x10; addr <= 0x27; addr++) {
        dev->reg.init_reg(addr, 0);
    }
    dev->reg.init_reg(0x29, 0);
    for (addr = 0x2c; addr <= 0x39; addr++)
        dev->reg.init_reg(addr, 0);
    for (addr = 0x3d; addr <= 0x3f; addr++)
        dev->reg.init_reg(addr, 0);
    for (addr = 0x52; addr <= 0x5a; addr++)
        dev->reg.init_reg(addr, 0);
    for (addr = 0x5d; addr <= 0x87; addr++)
        dev->reg.init_reg(addr, 0);


    dev->reg.find_reg(0x01).value = 0x20;	/* (enable shading), CCD, color, 1M */
    if (dev->model->is_cis) {
        dev->reg.find_reg(0x01).value |= REG01_CISSET;
    } else {
        dev->reg.find_reg(0x01).value &= ~REG01_CISSET;
    }

    dev->reg.find_reg(0x02).value = 0x30 /*0x38 */ ;	/* auto home, one-table-move, full step */
    dev->reg.find_reg(0x02).value |= REG02_AGOHOME;
    sanei_genesys_set_motor_power(dev->reg, true);
    dev->reg.find_reg(0x02).value |= REG02_FASTFED;

    dev->reg.find_reg(0x03).value = 0x1f /*0x17 */ ;	/* lamp on */
    dev->reg.find_reg(0x03).value |= REG03_AVEENB;

    if (dev->model->sensor_id == SensorId::CCD_PLUSTEK_OPTICPRO_3600) {
        // AD front end
      dev->reg.find_reg(0x04).value  = (2 << REG04S_AFEMOD) | 0x02;
    }
  else /* Wolfson front end */
    {
      dev->reg.find_reg(0x04).value |= 1 << REG04S_AFEMOD;
    }

  const auto& sensor = sanei_genesys_find_sensor_any(dev);

  dev->reg.find_reg(0x05).value = 0x00;	/* disable gamma, 24 clocks/pixel */

    unsigned dpihw = 0;
    if (sensor.sensor_pixels < 0x1500) {
        dpihw = 600;
    } else if (sensor.sensor_pixels < 0x2a80) {
        dpihw = 1200;
    } else if (sensor.sensor_pixels < 0x5400) {
        dpihw = 2400;
    } else {
        throw SaneException("Cannot handle sensor pixel count %d", sensor.sensor_pixels);
    }
    sanei_genesys_set_dpihw(dev->reg, sensor, dpihw);

  dev->reg.find_reg(0x06).value |= REG06_PWRBIT;
  dev->reg.find_reg(0x06).value |= REG06_GAIN4;

  /* XP300 CCD needs different clock and clock/pixels values */
    if (dev->model->sensor_id != SensorId::CCD_XP300 &&
        dev->model->sensor_id != SensorId::CCD_DP685 &&
        dev->model->sensor_id != SensorId::CCD_PLUSTEK_OPTICPRO_3600)
    {
      dev->reg.find_reg(0x06).value |= 0 << REG06S_SCANMOD;
      dev->reg.find_reg(0x09).value |= 1 << REG09S_CLKSET;
    }
  else
    {
      dev->reg.find_reg(0x06).value |= 0x05 << REG06S_SCANMOD; /* 15 clocks/pixel */
      dev->reg.find_reg(0x09).value = 0; /* 24 MHz CLKSET */
    }

  dev->reg.find_reg(0x1e).value = 0xf0;	/* watch-dog time */

  dev->reg.find_reg(0x17).value |= 1 << REG17S_TGW;

  dev->reg.find_reg(0x19).value = 0x50;

  dev->reg.find_reg(0x1d).value |= 1 << REG1DS_TGSHLD;

  dev->reg.find_reg(0x1e).value |= 1 << REG1ES_WDTIME;

/*SCANFED*/
  dev->reg.find_reg(0x1f).value = 0x01;

/*BUFSEL*/
  dev->reg.find_reg(0x20).value = 0x20;

/*LAMPPWM*/
  dev->reg.find_reg(0x29).value = 0xff;

/*BWHI*/
  dev->reg.find_reg(0x2e).value = 0x80;

/*BWLOW*/
  dev->reg.find_reg(0x2f).value = 0x80;

/*LPERIOD*/
  dev->reg.find_reg(0x38).value = 0x4f;
  dev->reg.find_reg(0x39).value = 0xc1;

/*VSMPW*/
  dev->reg.find_reg(0x58).value |= 3 << REG58S_VSMPW;

/*BSMPW*/
  dev->reg.find_reg(0x59).value |= 3 << REG59S_BSMPW;

/*RLCSEL*/
  dev->reg.find_reg(0x5a).value |= REG5A_RLCSEL;

/*STOPTIM*/
  dev->reg.find_reg(0x5e).value |= 0x2 << REG5ES_STOPTIM;

    sanei_gl841_setup_sensor(dev, sensor, &dev->reg, 0, 1);

    // set up GPIO
    for (const auto& reg : dev->gpo.regs) {
        dev->reg.set8(reg.address, reg.value);
    }

  /* TODO there is a switch calling to be written here */
    if (dev->model->gpio_id == GpioId::CANON_LIDE_35) {
      dev->reg.find_reg(0x6b).value |= REG6B_GPO18;
      dev->reg.find_reg(0x6b).value &= ~REG6B_GPO17;
    }

    if (dev->model->gpio_id == GpioId::XP300) {
      dev->reg.find_reg(0x6b).value |= REG6B_GPO17;
    }

    if (dev->model->gpio_id == GpioId::DP685) {
      /* REG6B_GPO18 lights on green led */
      dev->reg.find_reg(0x6b).value |= REG6B_GPO17|REG6B_GPO18;
    }

  DBG(DBG_proc, "%s complete\n", __func__);
}

// Send slope table for motor movement slope_table in machine byte order
static void gl841_send_slope_table(Genesys_Device* dev, int table_nr,
                                   const std::vector<uint16_t>& slope_table,
                                   int steps)
{
    DBG_HELPER_ARGS(dbg, "table_nr = %d, steps = %d", table_nr, steps);
  int dpihw;
  int start_address;
  char msg[4000];
/*#ifdef WORDS_BIGENDIAN*/
  int i;
/*#endif*/

  dpihw = dev->reg.find_reg(0x05).value >> 6;

  if (dpihw == 0)		/* 600 dpi */
    start_address = 0x08000;
  else if (dpihw == 1)		/* 1200 dpi */
    start_address = 0x10000;
  else if (dpihw == 2)		/* 2400 dpi */
    start_address = 0x20000;
    else {
        throw SaneException("Unexpected dpihw");
    }

  std::vector<uint8_t> table(steps * 2);
  for(i = 0; i < steps; i++) {
      table[i * 2] = slope_table[i] & 0xff;
      table[i * 2 + 1] = slope_table[i] >> 8;
  }

  if (DBG_LEVEL >= DBG_io)
    {
      sprintf (msg, "write slope %d (%d)=", table_nr, steps);
      for (i = 0; i < steps; i++)
	{
	  sprintf (msg+strlen(msg), ",%d", slope_table[i]);
	}
      DBG(DBG_io, "%s: %s\n", __func__, msg);
    }

    sanei_genesys_set_buffer_address(dev, start_address + table_nr * 0x200);

    sanei_genesys_bulk_write_data(dev, 0x3c, table.data(), steps * 2);
}

static void gl841_set_lide80_fe(Genesys_Device* dev, uint8_t set)
{
    DBG_HELPER(dbg);

  if (set == AFE_INIT)
    {
        DBG(DBG_proc, "%s(): setting DAC %u\n", __func__,
            static_cast<unsigned>(dev->model->adc_id));

      dev->frontend = dev->frontend_initial;

        // write them to analog frontend
        sanei_genesys_fe_write_data(dev, 0x00, dev->frontend.regs.get_value(0x00));
        sanei_genesys_fe_write_data(dev, 0x03, dev->frontend.regs.get_value(0x01));
        sanei_genesys_fe_write_data(dev, 0x06, dev->frontend.regs.get_value(0x02));
    }

  if (set == AFE_SET)
    {
        sanei_genesys_fe_write_data(dev, 0x00, dev->frontend.regs.get_value(0x00));
        sanei_genesys_fe_write_data(dev, 0x06, dev->frontend.regs.get_value(0x20));
        sanei_genesys_fe_write_data(dev, 0x03, dev->frontend.regs.get_value(0x28));
    }
}

// Set values of Analog Device type frontend
static void gl841_set_ad_fe(Genesys_Device* dev, uint8_t set)
{
    DBG_HELPER(dbg);
  int i;

    if (dev->model->adc_id==AdcId::CANON_LIDE_80) {
        gl841_set_lide80_fe(dev, set);
        return;
    }

  if (set == AFE_INIT)
    {
        DBG(DBG_proc, "%s(): setting DAC %u\n", __func__,
            static_cast<unsigned>(dev->model->adc_id));

      dev->frontend = dev->frontend_initial;

        // write them to analog frontend
        sanei_genesys_fe_write_data(dev, 0x00, dev->frontend.regs.get_value(0x00));

        sanei_genesys_fe_write_data(dev, 0x01, dev->frontend.regs.get_value(0x01));

        for (i = 0; i < 6; i++) {
            sanei_genesys_fe_write_data(dev, 0x02 + i, 0x00);
        }
    }
  if (set == AFE_SET)
    {
        // write them to analog frontend
        sanei_genesys_fe_write_data(dev, 0x00, dev->frontend.regs.get_value(0x00));

        sanei_genesys_fe_write_data(dev, 0x01, dev->frontend.regs.get_value(0x01));

        // Write fe 0x02 (red gain)
        sanei_genesys_fe_write_data(dev, 0x02, dev->frontend.get_gain(0));

        // Write fe 0x03 (green gain)
        sanei_genesys_fe_write_data(dev, 0x03, dev->frontend.get_gain(1));

        // Write fe 0x04 (blue gain)
        sanei_genesys_fe_write_data(dev, 0x04, dev->frontend.get_gain(2));

        // Write fe 0x05 (red offset)
        sanei_genesys_fe_write_data(dev, 0x05, dev->frontend.get_offset(0));

        // Write fe 0x06 (green offset)
        sanei_genesys_fe_write_data(dev, 0x06, dev->frontend.get_offset(1));

        // Write fe 0x07 (blue offset)
        sanei_genesys_fe_write_data(dev, 0x07, dev->frontend.get_offset(2));
          }
}

// Set values of analog frontend
void CommandSetGl841::set_fe(Genesys_Device* dev, const Genesys_Sensor& sensor, uint8_t set) const
{
    DBG_HELPER_ARGS(dbg, "%s", set == AFE_INIT ? "init" :
                               set == AFE_SET ? "set" :
                               set == AFE_POWER_SAVE ? "powersave" : "huh?");
    (void) sensor;
  int i;

  /* Analog Device type frontend */
    uint8_t frontend_type = dev->reg.find_reg(0x04).value & REG04_FESET;

    if (frontend_type == 0x02) {
        gl841_set_ad_fe(dev, set);
        return;
    }

    if (frontend_type != 0x00) {
        throw SaneException("unsupported frontend type %d", frontend_type);
    }

  if (set == AFE_INIT)
    {
        DBG(DBG_proc, "%s(): setting DAC %u\n", __func__,
            static_cast<unsigned>(dev->model->adc_id));
      dev->frontend = dev->frontend_initial;

        // reset only done on init
        sanei_genesys_fe_write_data (dev, 0x04, 0x80);
      DBG(DBG_proc, "%s(): frontend reset complete\n", __func__);
    }


  if (set == AFE_POWER_SAVE)
    {
        sanei_genesys_fe_write_data (dev, 0x01, 0x02);
        return;
    }

  /* todo :  base this test on cfg reg3 or a CCD family flag to be created */
  /*if (dev->model->ccd_type!=SensorId::CCD_HP2300 && dev->model->ccd_type!=SensorId::CCD_HP2400) */
  {
    sanei_genesys_fe_write_data(dev, 0x00, dev->frontend.regs.get_value(0x00));
    sanei_genesys_fe_write_data(dev, 0x02, dev->frontend.regs.get_value(0x02));
  }

    sanei_genesys_fe_write_data(dev, 0x01, dev->frontend.regs.get_value(0x01));
    sanei_genesys_fe_write_data(dev, 0x03, dev->frontend.regs.get_value(0x03));
    sanei_genesys_fe_write_data(dev, 0x06, dev->frontend.reg2[0]);
    sanei_genesys_fe_write_data(dev, 0x08, dev->frontend.reg2[1]);
    sanei_genesys_fe_write_data(dev, 0x09, dev->frontend.reg2[2]);

  for (i = 0; i < 3; i++)
      {
        sanei_genesys_fe_write_data(dev, 0x24 + i, dev->frontend.regs.get_value(0x24 + i));
        sanei_genesys_fe_write_data(dev, 0x28 + i, dev->frontend.get_gain(i));
        sanei_genesys_fe_write_data(dev, 0x20 + i, dev->frontend.get_offset(i));
      }
}

#define MOTOR_ACTION_FEED       1
#define MOTOR_ACTION_GO_HOME    2
#define MOTOR_ACTION_HOME_FREE  3

// @brief turn off motor
static void gl841_init_motor_regs_off(Genesys_Register_Set* reg, unsigned int scan_lines)
{
    DBG_HELPER_ARGS(dbg, "scan_lines=%d", scan_lines);
    unsigned int feedl;
    GenesysRegister* r;

    feedl = 2;

    r = sanei_genesys_get_address (reg, 0x3d);
    r->value = (feedl >> 16) & 0xf;
    r = sanei_genesys_get_address (reg, 0x3e);
    r->value = (feedl >> 8) & 0xff;
    r = sanei_genesys_get_address (reg, 0x3f);
    r->value = feedl & 0xff;
    r = sanei_genesys_get_address (reg, 0x5e);
    r->value &= ~0xe0;

    r = sanei_genesys_get_address (reg, 0x25);
    r->value = (scan_lines >> 16) & 0xf;
    r = sanei_genesys_get_address (reg, 0x26);
    r->value = (scan_lines >> 8) & 0xff;
    r = sanei_genesys_get_address (reg, 0x27);
    r->value = scan_lines & 0xff;

    r = sanei_genesys_get_address (reg, 0x02);
    r->value &= ~0x01; /*LONGCURV OFF*/
    r->value &= ~0x80; /*NOT_HOME OFF*/

    r->value &= ~0x10;

    r->value &= ~0x06;

    r->value &= ~0x08;

    r->value &= ~0x20;

    r->value &= ~0x40;

    r = sanei_genesys_get_address (reg, 0x67);
    r->value = 0x3f;

    r = sanei_genesys_get_address (reg, 0x68);
    r->value = 0x3f;

    r = sanei_genesys_get_address (reg, REG_STEPNO);
    r->value = 0;

    r = sanei_genesys_get_address (reg, REG_FASTNO);
    r->value = 0;

    r = sanei_genesys_get_address (reg, 0x69);
    r->value = 0;

    r = sanei_genesys_get_address (reg, 0x6a);
    r->value = 0;

    r = sanei_genesys_get_address (reg, 0x5f);
    r->value = 0;
}

/** @brief write motor table frequency
 * Write motor frequency data table.
 * @param dev device to set up motor
 * @param ydpi motor target resolution
 */
static void gl841_write_freq(Genesys_Device* dev, unsigned int ydpi)
{
    DBG_HELPER(dbg);
/**< fast table */
uint8_t tdefault[] = {0x18,0x36,0x18,0x36,0x18,0x36,0x18,0x36,0x18,0x36,0x18,0x36,0x18,0x36,0x18,0x36,0x18,0x36,0x18,0x36,0x18,0x36,0x18,0x36,0x18,0x36,0x18,0x36,0x18,0x36,0x18,0x36,0x18,0xb6,0x18,0xb6,0x18,0xb6,0x18,0xb6,0x18,0xb6,0x18,0xb6,0x18,0xb6,0x18,0xb6,0x18,0xb6,0x18,0xb6,0x18,0xb6,0x18,0xb6,0x18,0xb6,0x18,0xb6,0x18,0xb6,0x18,0xb6,0x18,0xf6,0x18,0xf6,0x18,0xf6,0x18,0xf6,0x18,0xf6,0x18,0xf6,0x18,0xf6,0x18,0xf6,0x18,0xf6,0x18,0xf6,0x18,0xf6,0x18,0xf6,0x18,0xf6,0x18,0xf6,0x18,0xf6,0x18,0xf6,0x18,0x76,0x18,0x76,0x18,0x76,0x18,0x76,0x18,0x76,0x18,0x76,0x18,0x76,0x18,0x76,0x18,0x76,0x18,0x76,0x18,0x76,0x18,0x76,0x18,0x76,0x18,0x76,0x18,0x76,0x18,0x76};
uint8_t t1200[]    = {0xc7,0x31,0xc7,0x31,0xc7,0x31,0xc7,0x31,0xc7,0x31,0xc7,0x31,0xc7,0x31,0xc7,0x31,0xc0,0x11,0xc0,0x11,0xc0,0x11,0xc0,0x11,0xc0,0x11,0xc0,0x11,0xc0,0x11,0xc0,0x11,0xc7,0xb1,0xc7,0xb1,0xc7,0xb1,0xc7,0xb1,0xc7,0xb1,0xc7,0xb1,0xc7,0xb1,0xc7,0xb1,0x07,0xe0,0x07,0xe0,0x07,0xe0,0x07,0xe0,0x07,0xe0,0x07,0xe0,0x07,0xe0,0x07,0xe0,0xc7,0xf1,0xc7,0xf1,0xc7,0xf1,0xc7,0xf1,0xc7,0xf1,0xc7,0xf1,0xc7,0xf1,0xc7,0xf1,0xc0,0x51,0xc0,0x51,0xc0,0x51,0xc0,0x51,0xc0,0x51,0xc0,0x51,0xc0,0x51,0xc0,0x51,0xc7,0x71,0xc7,0x71,0xc7,0x71,0xc7,0x71,0xc7,0x71,0xc7,0x71,0xc7,0x71,0xc7,0x71,0x07,0x20,0x07,0x20,0x07,0x20,0x07,0x20,0x07,0x20,0x07,0x20,0x07,0x20,0x07,0x20};
uint8_t t300[]     = {0x08,0x32,0x08,0x32,0x08,0x32,0x08,0x32,0x08,0x32,0x08,0x32,0x08,0x32,0x08,0x32,0x00,0x13,0x00,0x13,0x00,0x13,0x00,0x13,0x00,0x13,0x00,0x13,0x00,0x13,0x00,0x13,0x08,0xb2,0x08,0xb2,0x08,0xb2,0x08,0xb2,0x08,0xb2,0x08,0xb2,0x08,0xb2,0x08,0xb2,0x0c,0xa0,0x0c,0xa0,0x0c,0xa0,0x0c,0xa0,0x0c,0xa0,0x0c,0xa0,0x0c,0xa0,0x0c,0xa0,0x08,0xf2,0x08,0xf2,0x08,0xf2,0x08,0xf2,0x08,0xf2,0x08,0xf2,0x08,0xf2,0x08,0xf2,0x00,0xd3,0x00,0xd3,0x00,0xd3,0x00,0xd3,0x00,0xd3,0x00,0xd3,0x00,0xd3,0x00,0xd3,0x08,0x72,0x08,0x72,0x08,0x72,0x08,0x72,0x08,0x72,0x08,0x72,0x08,0x72,0x08,0x72,0x0c,0x60,0x0c,0x60,0x0c,0x60,0x0c,0x60,0x0c,0x60,0x0c,0x60,0x0c,0x60,0x0c,0x60};
uint8_t t150[]     = {0x0c,0x33,0xcf,0x33,0xcf,0x33,0xcf,0x33,0xcf,0x33,0xcf,0x33,0xcf,0x33,0xcf,0x33,0x40,0x14,0x80,0x15,0x80,0x15,0x80,0x15,0x80,0x15,0x80,0x15,0x80,0x15,0x80,0x15,0x0c,0xb3,0xcf,0xb3,0xcf,0xb3,0xcf,0xb3,0xcf,0xb3,0xcf,0xb3,0xcf,0xb3,0xcf,0xb3,0x11,0xa0,0x16,0xa0,0x16,0xa0,0x16,0xa0,0x16,0xa0,0x16,0xa0,0x16,0xa0,0x16,0xa0,0x0c,0xf3,0xcf,0xf3,0xcf,0xf3,0xcf,0xf3,0xcf,0xf3,0xcf,0xf3,0xcf,0xf3,0xcf,0xf3,0x40,0xd4,0x80,0xd5,0x80,0xd5,0x80,0xd5,0x80,0xd5,0x80,0xd5,0x80,0xd5,0x80,0xd5,0x0c,0x73,0xcf,0x73,0xcf,0x73,0xcf,0x73,0xcf,0x73,0xcf,0x73,0xcf,0x73,0xcf,0x73,0x11,0x60,0x16,0x60,0x16,0x60,0x16,0x60,0x16,0x60,0x16,0x60,0x16,0x60,0x16,0x60};

uint8_t *table;

    if(dev->model->motor_id == MotorId::CANON_LIDE_80) {
      switch(ydpi)
        {
          case 3600:
          case 1200:
            table=t1200;
            break;
          case 900:
          case 300:
            table=t300;
            break;
          case 450:
          case 150:
            table=t150;
            break;
          default:
            table=tdefault;
        }
        dev->write_register(0x66, 0x00);
        dev->write_register(0x5b, 0x0c);
        dev->write_register(0x5c, 0x00);
        sanei_genesys_bulk_write_data(dev, 0x28, table, 128);
        dev->write_register(0x5b, 0x00);
        dev->write_register(0x5c, 0x00);
    }
}


static void gl841_init_motor_regs(Genesys_Device* dev, const Genesys_Sensor& sensor,
                                  Genesys_Register_Set* reg, unsigned int feed_steps,/*1/base_ydpi*/
                                  /*maybe float for half/quarter step resolution?*/
                                  unsigned int action, unsigned int flags)
{
    DBG_HELPER_ARGS(dbg, "feed_steps=%d, action=%d, flags=%x", feed_steps, action, flags);
    unsigned int fast_exposure;
    int use_fast_fed = 0;
    std::vector<uint16_t> fast_slope_table;
    unsigned int fast_slope_steps = 0;
    unsigned int feedl;
    GenesysRegister* r;
/*number of scan lines to add in a scan_lines line*/

    fast_slope_table.resize(256, 0xffff);

    gl841_send_slope_table(dev, 0, fast_slope_table, 256);
    gl841_send_slope_table(dev, 1, fast_slope_table, 256);
    gl841_send_slope_table(dev, 2, fast_slope_table, 256);
    gl841_send_slope_table(dev, 3, fast_slope_table, 256);
    gl841_send_slope_table(dev, 4, fast_slope_table, 256);

    gl841_write_freq(dev, dev->motor.base_ydpi / 4);

    fast_slope_steps = 256;
    if (action == MOTOR_ACTION_FEED || action == MOTOR_ACTION_GO_HOME)
      {
        /* FEED and GO_HOME can use fastest slopes available */
        fast_exposure = gl841_exposure_time(dev, sensor,
                                            dev->motor.base_ydpi / 4,
                                            0,
                                            0,
                                            0);
        DBG(DBG_info, "%s : fast_exposure=%d pixels\n", __func__, fast_exposure);
      }

    if (action == MOTOR_ACTION_HOME_FREE) {
/* HOME_FREE must be able to stop in one step, so do not try to get faster */
        fast_exposure = dev->motor.slopes[0].maximum_start_speed;
    }

    sanei_genesys_create_slope_table3(
	dev,
	fast_slope_table,
        256,
	fast_slope_steps,
	0,
	fast_exposure,
	dev->motor.base_ydpi / 4,
    &fast_slope_steps,
                                       &fast_exposure);

    feedl = feed_steps - fast_slope_steps*2;
    use_fast_fed = 1;

/* all needed slopes available. we did even decide which mode to use.
   what next?
   - transfer slopes
SCAN:
flags \ use_fast_fed    ! 0         1
------------------------\--------------------
                      0 ! 0,1,2     0,1,2,3
MOTOR_FLAG_AUTO_GO_HOME ! 0,1,2,4   0,1,2,3,4
OFF:       none
FEED:      3
GO_HOME:   3
HOME_FREE: 3
   - setup registers
     * slope specific registers (already done)
     * DECSEL for HOME_FREE/GO_HOME/SCAN
     * FEEDL
     * MTRREV
     * MTRPWR
     * FASTFED
     * STEPSEL
     * MTRPWM
     * FSTPSEL
     * FASTPWM
     * HOMENEG
     * BWDSTEP
     * FWDSTEP
     * Z1
     * Z2
 */

    r = sanei_genesys_get_address(reg, 0x3d);
    r->value = (feedl >> 16) & 0xf;
    r = sanei_genesys_get_address(reg, 0x3e);
    r->value = (feedl >> 8) & 0xff;
    r = sanei_genesys_get_address(reg, 0x3f);
    r->value = feedl & 0xff;
    r = sanei_genesys_get_address(reg, 0x5e);
    r->value &= ~0xe0;

    r = sanei_genesys_get_address(reg, 0x25);
    r->value = 0;
    r = sanei_genesys_get_address(reg, 0x26);
    r->value = 0;
    r = sanei_genesys_get_address(reg, 0x27);
    r->value = 0;

    r = sanei_genesys_get_address(reg, 0x02);
    r->value &= ~0x01; /*LONGCURV OFF*/
    r->value &= ~0x80; /*NOT_HOME OFF*/

    r->value |= 0x10;

    if (action == MOTOR_ACTION_GO_HOME)
	r->value |= 0x06;
    else
	r->value &= ~0x06;

    if (use_fast_fed)
	r->value |= 0x08;
    else
	r->value &= ~0x08;

    if (flags & MOTOR_FLAG_AUTO_GO_HOME)
	r->value |= 0x20;
    else
	r->value &= ~0x20;

    r->value &= ~0x40;

    gl841_send_slope_table(dev, 3, fast_slope_table, 256);

    r = sanei_genesys_get_address(reg, 0x67);
    r->value = 0x3f;

    r = sanei_genesys_get_address(reg, 0x68);
    r->value = 0x3f;

    r = sanei_genesys_get_address(reg, REG_STEPNO);
    r->value = 0;

    r = sanei_genesys_get_address(reg, REG_FASTNO);
    r->value = 0;

    r = sanei_genesys_get_address(reg, 0x69);
    r->value = 0;

    r = sanei_genesys_get_address(reg, 0x6a);
    r->value = (fast_slope_steps >> 1) + (fast_slope_steps & 1);

    r = sanei_genesys_get_address(reg, 0x5f);
    r->value = (fast_slope_steps >> 1) + (fast_slope_steps & 1);
}

static void gl841_init_motor_regs_scan(Genesys_Device* dev, const Genesys_Sensor& sensor,
                                       Genesys_Register_Set* reg,
                                       unsigned int scan_exposure_time,/*pixel*/
                                       unsigned scan_yres, // dpi, motor resolution
                                       int scan_step_type,/*0: full, 1: half, 2: quarter*/
                                       unsigned int scan_lines,/*lines, scan resolution*/
                                       unsigned int scan_dummy,
                                       // number of scan lines to add in a scan_lines line
                                       unsigned int feed_steps,/*1/base_ydpi*/
                                       // maybe float for half/quarter step resolution?
                                       unsigned int flags)
{
    DBG_HELPER_ARGS(dbg, "scan_exposure_time=%d, scan_yres=%d, scan_step_type=%d, scan_lines=%d,"
                         " scan_dummy=%d, feed_steps=%d, flags=%x",
                    scan_exposure_time, scan_yres, scan_step_type, scan_lines, scan_dummy,
                    feed_steps, flags);
    unsigned int fast_exposure;
    int use_fast_fed = 0;
    unsigned int fast_time;
    unsigned int slow_time;
    std::vector<uint16_t> slow_slope_table;
    std::vector<uint16_t> fast_slope_table;
    std::vector<uint16_t> back_slope_table;
    unsigned int slow_slope_time;
    unsigned int fast_slope_time;
    unsigned int slow_slope_steps = 0;
    unsigned int fast_slope_steps = 0;
    unsigned int back_slope_steps = 0;
    unsigned int feedl;
    GenesysRegister* r;
    unsigned int min_restep = 0x20;
    uint32_t z1, z2;

    fast_exposure = gl841_exposure_time(dev, sensor,
                                        dev->motor.base_ydpi / 4,
                                        0,
                                        0,
                                        0);

    DBG(DBG_info, "%s : fast_exposure=%d pixels\n", __func__, fast_exposure);

    slow_slope_table.resize(256, 0xffff);

    gl841_send_slope_table(dev, 0, slow_slope_table, 256);
    gl841_send_slope_table(dev, 1, slow_slope_table, 256);
    gl841_send_slope_table(dev, 2, slow_slope_table, 256);
    gl841_send_slope_table(dev, 3, slow_slope_table, 256);
    gl841_send_slope_table(dev, 4, slow_slope_table, 256);

    /* motor frequency table */
    gl841_write_freq(dev, scan_yres);

/*
  we calculate both tables for SCAN. the fast slope step count depends on
  how many steps we need for slow acceleration and how much steps we are
  allowed to use.
 */
    slow_slope_time = sanei_genesys_create_slope_table3 (
	dev,
	slow_slope_table, 256,
	256,
	scan_step_type,
	scan_exposure_time,
	scan_yres,
	&slow_slope_steps,
    nullptr);

     sanei_genesys_create_slope_table3 (
	dev,
	back_slope_table, 256,
	256,
	scan_step_type,
	0,
	scan_yres,
	&back_slope_steps,
    nullptr);

    if (feed_steps < (slow_slope_steps >> scan_step_type)) {
	/*TODO: what should we do here?? go back to exposure calculation?*/
	feed_steps = slow_slope_steps >> scan_step_type;
    }

    if (feed_steps > fast_slope_steps*2 -
	    (slow_slope_steps >> scan_step_type))
	fast_slope_steps = 256;
    else
/* we need to shorten fast_slope_steps here. */
	fast_slope_steps = (feed_steps -
			    (slow_slope_steps >> scan_step_type))/2;

    DBG(DBG_info, "%s: Maximum allowed slope steps for fast slope: %d\n", __func__,
        fast_slope_steps);

    fast_slope_time = sanei_genesys_create_slope_table3 (
	dev,
	fast_slope_table, 256,
	fast_slope_steps,
	0,
	fast_exposure,
	dev->motor.base_ydpi / 4,
	&fast_slope_steps,
    &fast_exposure);

    /* fast fed special cases handling */
    if (dev->model->gpio_id == GpioId::XP300
     || dev->model->gpio_id == GpioId::DP685)
      {
	/* quirk: looks like at least this scanner is unable to use
	   2-feed mode */
	use_fast_fed = 0;
      }
    else if (feed_steps < fast_slope_steps*2 + (slow_slope_steps >> scan_step_type)) {
	use_fast_fed = 0;
	DBG(DBG_info, "%s: feed too short, slow move forced.\n", __func__);
    } else {
/* for deciding whether we should use fast mode we need to check how long we
   need for (fast)accelerating, moving, decelerating, (TODO: stopping?)
   (slow)accelerating again versus (slow)accelerating and moving. we need
   fast and slow tables here.
*/
/*NOTE: scan_exposure_time is per scan_yres*/
/*NOTE: fast_exposure is per base_ydpi/4*/
/*we use full steps as base unit here*/
	fast_time =
	    fast_exposure / 4 *
	    (feed_steps - fast_slope_steps*2 -
	     (slow_slope_steps >> scan_step_type))
	    + fast_slope_time*2 + slow_slope_time;
	slow_time =
	    (scan_exposure_time * scan_yres) / dev->motor.base_ydpi *
	    (feed_steps - (slow_slope_steps >> scan_step_type))
	    + slow_slope_time;

	DBG(DBG_info, "%s: Time for slow move: %d\n", __func__, slow_time);
	DBG(DBG_info, "%s: Time for fast move: %d\n", __func__, fast_time);

	use_fast_fed = fast_time < slow_time;
    }

    if (use_fast_fed)
	feedl = feed_steps - fast_slope_steps*2 -
	    (slow_slope_steps >> scan_step_type);
    else
	if ((feed_steps << scan_step_type) < slow_slope_steps)
	    feedl = 0;
	else
	    feedl = (feed_steps << scan_step_type) - slow_slope_steps;
    DBG(DBG_info, "%s: Decided to use %s mode\n", __func__, use_fast_fed?"fast feed":"slow feed");

/* all needed slopes available. we did even decide which mode to use.
   what next?
   - transfer slopes
SCAN:
flags \ use_fast_fed    ! 0         1
------------------------\--------------------
                      0 ! 0,1,2     0,1,2,3
MOTOR_FLAG_AUTO_GO_HOME ! 0,1,2,4   0,1,2,3,4
OFF:       none
FEED:      3
GO_HOME:   3
HOME_FREE: 3
   - setup registers
     * slope specific registers (already done)
     * DECSEL for HOME_FREE/GO_HOME/SCAN
     * FEEDL
     * MTRREV
     * MTRPWR
     * FASTFED
     * STEPSEL
     * MTRPWM
     * FSTPSEL
     * FASTPWM
     * HOMENEG
     * BWDSTEP
     * FWDSTEP
     * Z1
     * Z2
 */

    r = sanei_genesys_get_address (reg, 0x3d);
    r->value = (feedl >> 16) & 0xf;
    r = sanei_genesys_get_address (reg, 0x3e);
    r->value = (feedl >> 8) & 0xff;
    r = sanei_genesys_get_address (reg, 0x3f);
    r->value = feedl & 0xff;
    r = sanei_genesys_get_address (reg, 0x5e);
    r->value &= ~0xe0;

    r = sanei_genesys_get_address (reg, 0x25);
    r->value = (scan_lines >> 16) & 0xf;
    r = sanei_genesys_get_address (reg, 0x26);
    r->value = (scan_lines >> 8) & 0xff;
    r = sanei_genesys_get_address (reg, 0x27);
    r->value = scan_lines & 0xff;

    r = sanei_genesys_get_address (reg, 0x02);
    r->value &= ~0x01; /*LONGCURV OFF*/
    r->value &= ~0x80; /*NOT_HOME OFF*/
    r->value |= 0x10;

    r->value &= ~0x06;

    if (use_fast_fed)
	r->value |= 0x08;
    else
	r->value &= ~0x08;

    if (flags & MOTOR_FLAG_AUTO_GO_HOME)
	r->value |= 0x20;
    else
	r->value &= ~0x20;

    if (flags & MOTOR_FLAG_DISABLE_BUFFER_FULL_MOVE)
	r->value |= 0x40;
    else
	r->value &= ~0x40;

    gl841_send_slope_table(dev, 0, slow_slope_table, 256);

    gl841_send_slope_table(dev, 1, back_slope_table, 256);

    gl841_send_slope_table(dev, 2, slow_slope_table, 256);

    if (use_fast_fed) {
        gl841_send_slope_table(dev, 3, fast_slope_table, 256);
    }

    if (flags & MOTOR_FLAG_AUTO_GO_HOME) {
        gl841_send_slope_table(dev, 4, fast_slope_table, 256);
    }

/* now reg 0x21 and 0x24 are available, we can calculate reg 0x22 and 0x23,
   reg 0x60-0x62 and reg 0x63-0x65
   rule:
   2*STEPNO+FWDSTEP=2*FASTNO+BWDSTEP
*/
/* steps of table 0*/
    if (min_restep < slow_slope_steps*2+2)
	min_restep = slow_slope_steps*2+2;
/* steps of table 1*/
    if (min_restep < back_slope_steps*2+2)
	min_restep = back_slope_steps*2+2;
/* steps of table 0*/
    r = sanei_genesys_get_address (reg, REG_FWDSTEP);
    r->value = min_restep - slow_slope_steps*2;
/* steps of table 1*/
    r = sanei_genesys_get_address (reg, REG_BWDSTEP);
    r->value = min_restep - back_slope_steps*2;

/*
  for z1/z2:
  in dokumentation mentioned variables a-d:
  a = time needed for acceleration, table 1
  b = time needed for reg 0x1f... wouldn't that be reg0x1f*exposure_time?
  c = time needed for acceleration, table 1
  d = time needed for reg 0x22... wouldn't that be reg0x22*exposure_time?
  z1 = (c+d-1) % exposure_time
  z2 = (a+b-1) % exposure_time
*/
/* i don't see any effect of this. i can only guess that this will enhance
   sub-pixel accuracy
   z1 = (slope_0_time-1) % exposure_time;
   z2 = (slope_0_time-1) % exposure_time;
*/
    z1 = z2 = 0;

    DBG(DBG_info, "%s: z1 = %d\n", __func__, z1);
    DBG(DBG_info, "%s: z2 = %d\n", __func__, z2);
    r = sanei_genesys_get_address (reg, 0x60);
    r->value = ((z1 >> 16) & 0xff);
    r = sanei_genesys_get_address (reg, 0x61);
    r->value = ((z1 >> 8) & 0xff);
    r = sanei_genesys_get_address (reg, 0x62);
    r->value = (z1 & 0xff);
    r = sanei_genesys_get_address (reg, 0x63);
    r->value = ((z2 >> 16) & 0xff);
    r = sanei_genesys_get_address (reg, 0x64);
    r->value = ((z2 >> 8) & 0xff);
    r = sanei_genesys_get_address (reg, 0x65);
    r->value = (z2 & 0xff);

    r = sanei_genesys_get_address (reg, REG1E);
    r->value &= REG1E_WDTIME;
    r->value |= scan_dummy;

    r = sanei_genesys_get_address (reg, 0x67);
    r->value = 0x3f | (scan_step_type << 6);

    r = sanei_genesys_get_address (reg, 0x68);
    r->value = 0x3f;

    r = sanei_genesys_get_address (reg, REG_STEPNO);
    r->value = (slow_slope_steps >> 1) + (slow_slope_steps & 1);

    r = sanei_genesys_get_address (reg, REG_FASTNO);
    r->value = (back_slope_steps >> 1) + (back_slope_steps & 1);

    r = sanei_genesys_get_address (reg, 0x69);
    r->value = (slow_slope_steps >> 1) + (slow_slope_steps & 1);

    r = sanei_genesys_get_address (reg, 0x6a);
    r->value = (fast_slope_steps >> 1) + (fast_slope_steps & 1);

    r = sanei_genesys_get_address (reg, 0x5f);
    r->value = (fast_slope_steps >> 1) + (fast_slope_steps & 1);
}

static int
gl841_get_dpihw(Genesys_Device * dev)
{
  GenesysRegister* r;
  r = sanei_genesys_get_address(&dev->reg, 0x05);
  if ((r->value & REG05_DPIHW) == REG05_DPIHW_600)
    return 600;
  if ((r->value & REG05_DPIHW) == REG05_DPIHW_1200)
    return 1200;
  if ((r->value & REG05_DPIHW) == REG05_DPIHW_2400)
    return 2400;
  return 0;
}

static void gl841_init_optical_regs_off(Genesys_Register_Set* reg)
{
    DBG_HELPER(dbg);
    GenesysRegister* r;

    r = sanei_genesys_get_address(reg, 0x01);
    r->value &= ~REG01_SCAN;
}

static void gl841_init_optical_regs_scan(Genesys_Device* dev, const Genesys_Sensor& sensor,
                                         Genesys_Register_Set* reg, unsigned int exposure_time,
                                         const ScanSession& session)
{
    DBG_HELPER_ARGS(dbg, "exposure_time=%d", exposure_time);
    GenesysRegister* r;
    uint16_t expavg, expr, expb, expg;

    dev->cmd_set->set_fe(dev, sensor, AFE_SET);

    /* gpio part.*/
    if (dev->model->gpio_id == GpioId::CANON_LIDE_35)
      {
	r = sanei_genesys_get_address (reg, REG6C);
        if (session.ccd_size_divisor > 1) {
            r->value &= ~0x80;
        } else {
            r->value |= 0x80;
        }
      }
    if (dev->model->gpio_id == GpioId::CANON_LIDE_80)
      {
	r = sanei_genesys_get_address (reg, REG6C);
        if (session.ccd_size_divisor > 1) {
	    r->value &= ~0x40;
	    r->value |= 0x20;
        } else {
	    r->value &= ~0x20;
	    r->value |= 0x40;
        }
      }

    /* enable shading */
    r = sanei_genesys_get_address (reg, 0x01);
    r->value |= REG01_SCAN;
    if ((session.params.flags & SCAN_FLAG_DISABLE_SHADING) ||
        (dev->model->flags & GENESYS_FLAG_NO_CALIBRATION))
    {
        r->value &= ~REG01_DVDSET;
    } else {
        r->value |= REG01_DVDSET;
    }

    /* average looks better than deletion, and we are already set up to
       use  one of the average enabled resolutions
    */
    r = sanei_genesys_get_address (reg, 0x03);
    r->value |= REG03_AVEENB;
    sanei_genesys_set_lamp_power(dev, sensor, *reg,
                                 !(session.params.flags & SCAN_FLAG_DISABLE_LAMP));

    /* BW threshold */
    r = sanei_genesys_get_address (reg, 0x2e);
    r->value = dev->settings.threshold;
    r = sanei_genesys_get_address (reg, 0x2f);
    r->value = dev->settings.threshold;


    /* monochrome / color scan */
    r = sanei_genesys_get_address (reg, 0x04);
    switch (session.params.depth) {
	case 8:
	    r->value &= ~(REG04_LINEART | REG04_BITSET);
	    break;
	case 16:
	    r->value &= ~REG04_LINEART;
	    r->value |= REG04_BITSET;
	    break;
    }

    /* AFEMOD should depend on FESET, and we should set these
     * bits separately */
    r->value &= ~(REG04_FILTER | REG04_AFEMOD);
    if (session.params.flags & SCAN_FLAG_ENABLE_LEDADD) {
        r->value |= 0x10;	/* no filter */
    }
    else if (session.params.channels == 1)
      {
    switch (session.params.color_filter)
	  {
            case ColorFilter::RED:
                r->value |= 0x14;
                break;
            case ColorFilter::GREEN:
                r->value |= 0x18;
                break;
            case ColorFilter::BLUE:
                r->value |= 0x1c;
                break;
            default:
                r->value |= 0x10;
                break;
	  }
      }
    else
      {
        if (dev->model->sensor_id == SensorId::CCD_PLUSTEK_OPTICPRO_3600) {
            r->value |= 0x22;	/* slow color pixel by pixel */
          }
	else
          {
	    r->value |= 0x10;	/* color pixel by pixel */
          }
      }

    /* CIS scanners can do true gray by setting LEDADD */
    r = sanei_genesys_get_address (reg, 0x87);
    r->value &= ~REG87_LEDADD;
    if (session.params.flags & SCAN_FLAG_ENABLE_LEDADD) {
        r->value |= REG87_LEDADD;
        expr = reg->get16(REG_EXPR);
        expg = reg->get16(REG_EXPG);
        expb = reg->get16(REG_EXPB);

	/* use minimal exposure for best image quality */
	expavg = expg;
	if (expr < expg)
	  expavg = expr;
	if (expb < expavg)
	  expavg = expb;

        dev->reg.set16(REG_EXPR, expavg);
        dev->reg.set16(REG_EXPG, expavg);
        dev->reg.set16(REG_EXPB, expavg);
      }

    // enable gamma tables
    r = sanei_genesys_get_address (reg, 0x05);
    if (session.params.flags & SCAN_FLAG_DISABLE_GAMMA) {
        r->value &= ~REG05_GMMENB;
    } else {
        r->value |= REG05_GMMENB;
    }

    /* sensor parameters */
    sanei_gl841_setup_sensor(dev, sensor, &dev->reg, 1, session.ccd_size_divisor);

    r = sanei_genesys_get_address (reg, 0x29);
    r->value = 255; /*<<<"magic" number, only suitable for cis*/

    reg->set16(REG_DPISET, gl841_get_dpihw(dev) * session.output_resolution / session.optical_resolution);
    reg->set16(REG_STRPIXEL, session.pixel_startx);
    reg->set16(REG_ENDPIXEL, session.pixel_endx);

    reg->set24(REG_MAXWD, session.output_line_bytes);

    reg->set16(REG_LPERIOD, exposure_time);

    r = sanei_genesys_get_address (reg, 0x34);
    r->value = sensor.dummy_pixel;
}

static int
gl841_get_led_exposure(Genesys_Device * dev, const Genesys_Sensor& sensor)
{
    int d,r,g,b,m;
    if (!dev->model->is_cis)
	return 0;
    d = dev->reg.find_reg(0x19).value;

    r = sensor.exposure.red;
    g = sensor.exposure.green;
    b = sensor.exposure.blue;

    m = r;
    if (m < g)
	m = g;
    if (m < b)
	m = b;

    return m + d;
}

/** @brief compute exposure time
 * Compute exposure time for the device and the given scan resolution
 */
static int
gl841_exposure_time(Genesys_Device *dev, const Genesys_Sensor& sensor,
                    float slope_dpi,
                    int scan_step_type,
                    int start,
                    int used_pixels)
{
int exposure_time = 0;
int led_exposure;

  led_exposure=gl841_get_led_exposure(dev, sensor);
  exposure_time = sanei_genesys_exposure_time2(
      dev,
      slope_dpi,
      scan_step_type,
      start+used_pixels,/*+tgtime? currently done in sanei_genesys_exposure_time2 with tgtime = 32 pixel*/
      led_exposure);

  return exposure_time;
}

/**@brief compute scan_step_type
 * Try to do at least 4 steps per line. if that is impossible we will have to
 * live with that.
 * @param dev device
 * @param yres motor resolution
 */
static int
gl841_scan_step_type(Genesys_Device *dev, int yres)
{
int scan_step_type=0;

  /* TODO : check if there is a bug around the use of max_step_type   */
  /* should be <=1, need to chek all devices entry in genesys_devices */
  if (yres*4 < dev->motor.base_ydpi || dev->motor.max_step_type <= 0)
    {
      scan_step_type = 0;
    }
  else if (yres*4 < dev->motor.base_ydpi*2 || dev->motor.max_step_type <= 1)
    {
      scan_step_type = 1;
    }
  else
    {
      scan_step_type = 2;
    }

  /* this motor behaves differently */
    if (dev->model->motor_id==MotorId::CANON_LIDE_80) {
      /* driven by 'frequency' tables ? */
      scan_step_type = 0;
    }

  return scan_step_type;
}

static void gl841_compute_session(Genesys_Device* dev, ScanSession& s,
                                  const Genesys_Sensor& sensor)
{
    DBG_HELPER(dbg);

    // no 16 bit gamma for this ASIC
    if (s.params.depth == 16) {
        s.params.flags |= SCAN_FLAG_DISABLE_GAMMA;
    }

    compute_session(dev, s, sensor);

    s.computed = true;

    DBG(DBG_info, "%s ", __func__);
    debug_dump(DBG_info, s);
}

static void gl841_assert_supported_resolution(const ScanSession& session)
{
    for (unsigned factor : {1, 2, 3, 4, 5, 6, 8, 10, 12, 15}) {
        if (session.output_resolution == session.optical_resolution / factor) {
            return;
        }
    }
    throw SaneException("Unsupported resolution %d for optical resolution %d",
                        session.output_resolution, session.optical_resolution);
}

// set up registers for an actual scan this function sets up the scanner to scan in normal or single
// line mode
static void gl841_init_scan_regs(Genesys_Device* dev, const Genesys_Sensor& sensor,
                                 Genesys_Register_Set* reg, const ScanSession& session)
{
    DBG_HELPER(dbg);
    session.assert_computed();

  int move;
  int exposure_time;

  int slope_dpi = 0;
  int dummy = 0;
  int scan_step_type = 1;

/*
results:

for scanner:
start
end
dpiset
exposure_time
dummy
z1
z2

for ordered_read:
  dev->words_per_line
  dev->read_factor
  dev->requested_buffer_size
  dev->read_buffer_size
  dev->read_pos
  dev->read_bytes_in_buffer
  dev->read_bytes_left
  dev->max_shift
  dev->stagger

independent of our calculated values:
  dev->total_bytes_read
  dev->bytes_to_read
 */

    gl841_assert_supported_resolution(session);

/* dummy */
  /* dummy lines: may not be usefull, for instance 250 dpi works with 0 or 1
     dummy line. Maybe the dummy line adds correctness since the motor runs
     slower (higher dpi)
  */
/* for cis this creates better aligned color lines:
dummy \ scanned lines
   0: R           G           B           R ...
   1: R        G        B        -        R ...
   2: R      G      B       -      -      R ...
   3: R     G     B     -     -     -     R ...
   4: R    G    B     -   -     -    -    R ...
   5: R    G   B    -   -   -    -   -    R ...
   6: R   G   B   -   -   -   -   -   -   R ...
   7: R   G  B   -  -   -   -  -   -  -   R ...
   8: R  G  B   -  -  -   -  -  -   -  -  R ...
   9: R  G  B  -  -  -  -  -  -  -  -  -  R ...
  10: R  G B  -  -  -  - -  -  -  -  - -  R ...
  11: R  G B  - -  - -  -  - -  - -  - -  R ...
  12: R G  B - -  - -  - -  - -  - - -  - R ...
  13: R G B  - - - -  - - -  - - - -  - - R ...
  14: R G B - - -  - - - - - -  - - - - - R ...
  15: R G B - - - - - - - - - - - - - - - R ...
 -- pierre
 */
  dummy = 0;

/* slope_dpi */
/* cis color scan is effectively a gray scan with 3 gray lines per color
   line and a FILTER of 0 */
    if (dev->model->is_cis) {
        slope_dpi = session.params.yres* session.params.channels;
    } else {
        slope_dpi = session.params.yres;
    }

  slope_dpi = slope_dpi * (1 + dummy);

    scan_step_type = gl841_scan_step_type(dev, session.params.yres);
    exposure_time = gl841_exposure_time(dev, sensor,
                    slope_dpi,
                    scan_step_type,
                                        session.pixel_startx,
                                        session.optical_pixels);
  DBG(DBG_info, "%s : exposure_time=%d pixels\n", __func__, exposure_time);

    gl841_init_optical_regs_scan(dev, sensor, reg, exposure_time, session);

    move = session.params.starty;
  DBG(DBG_info, "%s: move=%d steps\n", __func__, move);

  /* subtract current head position */
  move -= dev->scanhead_position_in_steps;
  DBG(DBG_info, "%s: move=%d steps\n", __func__, move);

  if (move < 0)
      move = 0;

  /* round it */
/* the move is not affected by dummy -- pierre */
/*  move = ((move + dummy) / (dummy + 1)) * (dummy + 1);
    DBG(DBG_info, "%s: move=%d steps\n", __func__, move);*/

    if (session.params.flags & SCAN_FLAG_SINGLE_LINE) {
        gl841_init_motor_regs_off(reg, dev->model->is_cis ? session.output_line_count * session.params.channels
                                                          : session.output_line_count);
    } else {
        gl841_init_motor_regs_scan(dev, sensor, reg, exposure_time, slope_dpi, scan_step_type,
                                   dev->model->is_cis ? session.output_line_count * session.params.channels
                                                      : session.output_line_count,
                                   dummy, move,
                                   (session.params.flags & SCAN_FLAG_DISABLE_BUFFER_FULL_MOVE) ?
                                   MOTOR_FLAG_DISABLE_BUFFER_FULL_MOVE : 0);
  }

    dev->read_buffer.clear();
    dev->read_buffer.alloc(session.buffer_size_read);

    build_image_pipeline(dev, session);

    dev->read_active = true;

    dev->session = session;
    dev->current_setup.pixels = session.output_pixels;
    dev->current_setup.lines = session.output_line_count;
  dev->current_setup.exposure_time = exposure_time;
    dev->current_setup.xres = session.params.xres;
    dev->current_setup.ccd_size_divisor = session.ccd_size_divisor;
    dev->current_setup.stagger = session.num_staggered_lines;
    dev->current_setup.max_shift = session.max_color_shift_lines + session.num_staggered_lines;

    dev->total_bytes_read = 0;
    dev->total_bytes_to_read = session.output_line_bytes_requested * session.params.lines;

    DBG(DBG_info, "%s: total bytes to send = %zu\n", __func__, dev->total_bytes_to_read);
}

void CommandSetGl841::calculate_current_setup(Genesys_Device * dev,
                                              const Genesys_Sensor& sensor) const
{
  int start;

  int exposure_time;

  int slope_dpi = 0;
  int dummy = 0;
  int scan_step_type = 1;

    DBG(DBG_info, "%s ", __func__);
    debug_dump(DBG_info, dev->settings);

/* start */
    start = static_cast<int>(dev->model->x_offset);
    start += static_cast<int>(dev->settings.tl_x);

    start = static_cast<int>((start * sensor.optical_res) / MM_PER_INCH);

    ScanSession session;
    session.params.xres = dev->settings.xres;
    session.params.yres = dev->settings.yres;
    session.params.startx = start;
    session.params.starty = 0; // not used
    session.params.pixels = dev->settings.pixels;
    session.params.requested_pixels = dev->settings.requested_pixels;
    session.params.lines = dev->settings.lines;
    session.params.depth = dev->settings.depth;
    session.params.channels = dev->settings.get_channels();
    session.params.scan_method = dev->settings.scan_method;
    session.params.scan_mode = dev->settings.scan_mode;
    session.params.color_filter = dev->settings.color_filter;
    session.params.flags = 0;

    gl841_compute_session(dev, session, sensor);

    gl841_assert_supported_resolution(session);

  /* dummy lines: may not be usefull, for instance 250 dpi works with 0 or 1
     dummy line. Maybe the dummy line adds correctness since the motor runs
     slower (higher dpi)
  */
/* for cis this creates better aligned color lines:
dummy \ scanned lines
   0: R           G           B           R ...
   1: R        G        B        -        R ...
   2: R      G      B       -      -      R ...
   3: R     G     B     -     -     -     R ...
   4: R    G    B     -   -     -    -    R ...
   5: R    G   B    -   -   -    -   -    R ...
   6: R   G   B   -   -   -   -   -   -   R ...
   7: R   G  B   -  -   -   -  -   -  -   R ...
   8: R  G  B   -  -  -   -  -  -   -  -  R ...
   9: R  G  B  -  -  -  -  -  -  -  -  -  R ...
  10: R  G B  -  -  -  - -  -  -  -  - -  R ...
  11: R  G B  - -  - -  -  - -  - -  - -  R ...
  12: R G  B - -  - -  - -  - -  - - -  - R ...
  13: R G B  - - - -  - - -  - - - -  - - R ...
  14: R G B - - -  - - - - - -  - - - - - R ...
  15: R G B - - - - - - - - - - - - - - - R ...
 -- pierre
 */
  dummy = 0;

/* cis color scan is effectively a gray scan with 3 gray lines per color
   line and a FILTER of 0 */
    if (dev->model->is_cis) {
        slope_dpi = session.params.yres * session.params.channels;
    } else {
        slope_dpi = session.params.yres;
    }

  slope_dpi = slope_dpi * (1 + dummy);

    scan_step_type = gl841_scan_step_type(dev, session.params.yres);
  exposure_time = gl841_exposure_time(dev, sensor,
                    slope_dpi,
                    scan_step_type,
                                        session.pixel_startx,
                                        session.optical_pixels);
  DBG(DBG_info, "%s : exposure_time=%d pixels\n", __func__, exposure_time);

    dev->session = session;
    dev->current_setup.pixels = session.output_pixels;
    dev->current_setup.lines = session.output_line_count;
  dev->current_setup.exposure_time = exposure_time;
    dev->current_setup.xres = session.params.xres;
    dev->current_setup.ccd_size_divisor = session.ccd_size_divisor;
    dev->current_setup.stagger = session.num_staggered_lines;
    dev->current_setup.max_shift = session.max_color_shift_lines + session.num_staggered_lines;
}

// for fast power saving methods only, like disabling certain amplifiers
void CommandSetGl841::save_power(Genesys_Device* dev, bool enable) const
{
    DBG_HELPER_ARGS(dbg, "enable = %d", enable);

    const auto& sensor = sanei_genesys_find_sensor_any(dev);

    if (enable)
    {
    if (dev->model->gpio_id == GpioId::CANON_LIDE_35)
	{
/* expect GPIO17 to be enabled, and GPIO9 to be disabled,
   while GPIO8 is disabled*/
/* final state: GPIO8 disabled, GPIO9 enabled, GPIO17 disabled,
   GPIO18 disabled*/

        uint8_t val = dev->read_register(REG6D);
        dev->write_register(REG6D, val | 0x80);

            sanei_genesys_sleep_ms(1);

	    /*enable GPIO9*/
        val = dev->read_register(REG6C);
        dev->write_register(REG6C, val | 0x01);

	    /*disable GPO17*/
        val = dev->read_register(REG6B);
        dev->write_register(REG6B, val & ~REG6B_GPO17);

	    /*disable GPO18*/
        val = dev->read_register(REG6B);
        dev->write_register(REG6B, val & ~REG6B_GPO18);

            sanei_genesys_sleep_ms(1);

        val = dev->read_register(REG6D);
        dev->write_register(REG6D, val & ~0x80);

	}
    if (dev->model->gpio_id == GpioId::DP685)
	  {
        uint8_t val = dev->read_register(REG6B);
        dev->write_register(REG6B, val & ~REG6B_GPO17);
            dev->reg.find_reg(0x6b).value &= ~REG6B_GPO17;
            dev->calib_reg.find_reg(0x6b).value &= ~REG6B_GPO17;
	  }

        set_fe(dev, sensor, AFE_POWER_SAVE);

    }
    else
    {
    if (dev->model->gpio_id == GpioId::CANON_LIDE_35)
	{
/* expect GPIO17 to be enabled, and GPIO9 to be disabled,
   while GPIO8 is disabled*/
/* final state: GPIO8 enabled, GPIO9 disabled, GPIO17 enabled,
   GPIO18 enabled*/

        uint8_t val = dev->read_register(REG6D);
        dev->write_register(REG6D, val | 0x80);

            sanei_genesys_sleep_ms(10);

	    /*disable GPIO9*/
        val = dev->read_register(REG6C);
        dev->write_register(REG6C, val & ~0x01);

	    /*enable GPIO10*/
        val = dev->read_register(REG6C);
        dev->write_register(REG6C, val | 0x02);

	    /*enable GPO17*/
        val = dev->read_register(REG6B);
        dev->write_register(REG6B, val | REG6B_GPO17);
            dev->reg.find_reg(0x6b).value |= REG6B_GPO17;
            dev->calib_reg.find_reg(0x6b).value |= REG6B_GPO17;

	    /*enable GPO18*/
        val = dev->read_register(REG6B);
        dev->write_register(REG6B, val | REG6B_GPO18);
            dev->reg.find_reg(0x6b).value |= REG6B_GPO18;
            dev->calib_reg.find_reg(0x6b).value |= REG6B_GPO18;

	}
    if (dev->model->gpio_id == GpioId::DP665
            || dev->model->gpio_id == GpioId::DP685)
	  {
        uint8_t val = dev->read_register(REG6B);
        dev->write_register(REG6B, val | REG6B_GPO17);
            dev->reg.find_reg(0x6b).value |= REG6B_GPO17;
            dev->calib_reg.find_reg(0x6b).value |= REG6B_GPO17;
	  }

    }
}

void CommandSetGl841::set_powersaving(Genesys_Device* dev, int delay /* in minutes */) const
{
    DBG_HELPER_ARGS(dbg, "delay = %d", delay);
  // FIXME: SEQUENTIAL not really needed in this case
  Genesys_Register_Set local_reg(Genesys_Register_Set::SEQUENTIAL);
  int rate, exposure_time, tgtime, time;

    local_reg.init_reg(0x01, dev->reg.get8(0x01));	/* disable fastmode */
    local_reg.init_reg(0x03, dev->reg.get8(0x03));	/* Lamp power control */
    local_reg.init_reg(0x05, dev->reg.get8(0x05)); /*& ~REG05_BASESEL*/;	/* 24 clocks/pixel */
    local_reg.init_reg(0x18, 0x00); // Set CCD type
    local_reg.init_reg(0x38, 0x00);
    local_reg.init_reg(0x39, 0x00);

    // period times for LPeriod, expR,expG,expB, Z1MODE, Z2MODE
    local_reg.init_reg(0x1c, dev->reg.get8(0x05) & ~REG1C_TGTIME);

    if (!delay) {
        local_reg.find_reg(0x03).value = local_reg.find_reg(0x03).value & 0xf0;	/* disable lampdog and set lamptime = 0 */
    } else if (delay < 20) {
        local_reg.find_reg(0x03).value = (local_reg.find_reg(0x03).value & 0xf0) | 0x09;	/* enable lampdog and set lamptime = 1 */
    } else {
        local_reg.find_reg(0x03).value = (local_reg.find_reg(0x03).value & 0xf0) | 0x0f;	/* enable lampdog and set lamptime = 7 */
    }

  time = delay * 1000 * 60;	/* -> msec */
  exposure_time = static_cast<std::uint32_t>(time * 32000.0 /
                 (24.0 * 64.0 * (local_reg.find_reg(0x03).value & REG03_LAMPTIM) *
		  1024.0) + 0.5);
  /* 32000 = system clock, 24 = clocks per pixel */
  rate = (exposure_time + 65536) / 65536;
  if (rate > 4)
    {
      rate = 8;
      tgtime = 3;
    }
  else if (rate > 2)
    {
      rate = 4;
      tgtime = 2;
    }
  else if (rate > 1)
    {
      rate = 2;
      tgtime = 1;
    }
  else
    {
      rate = 1;
      tgtime = 0;
    }

  local_reg.find_reg(0x1c).value |= tgtime;
  exposure_time /= rate;

  if (exposure_time > 65535)
    exposure_time = 65535;

  local_reg.set8(0x38, exposure_time >> 8);
  local_reg.set8(0x39, exposure_time & 255);	/* lowbyte */

    dev->write_registers(local_reg);
}

static void gl841_start_action(Genesys_Device* dev)
{
    DBG_HELPER(dbg);
    dev->write_register(0x0f, 0x01);
}

static void gl841_stop_action(Genesys_Device* dev)
{
    DBG_HELPER(dbg);
  Genesys_Register_Set local_reg;
    uint8_t val;
  unsigned int loop;

    val = sanei_genesys_get_status(dev);
  if (DBG_LEVEL >= DBG_io)
    {
      sanei_genesys_print_status (val);
    }

    uint8_t val40 = dev->read_register(0x40);

  /* only stop action if needed */
  if (!(val40 & REG40_DATAENB) && !(val40 & REG40_MOTMFLG))
    {
      DBG(DBG_info, "%s: already stopped\n", __func__);
      return;
    }

  local_reg = dev->reg;

  gl841_init_optical_regs_off(&local_reg);

    gl841_init_motor_regs_off(&local_reg,0);
    dev->write_registers(local_reg);

  /* looks like writing the right registers to zero is enough to get the chip
     out of scan mode into command mode, actually triggering(writing to
     register 0x0f) seems to be unnecessary */

  loop = 10;
    while (loop > 0) {
        val40 = dev->read_register(0x40);
        if (DBG_LEVEL >= DBG_io) {
            sanei_genesys_print_status(val);
        }

      /* if scanner is in command mode, we are done */
      if (!(val40 & REG40_DATAENB) && !(val40 & REG40_MOTMFLG))
	{
      return;
	}

      sanei_genesys_sleep_ms(100);
      loop--;
    }

    throw SaneException(SANE_STATUS_IO_ERROR, "could not stop motor");
}

static bool gl841_get_paper_sensor(Genesys_Device* dev)
{
    DBG_HELPER(dbg);

    uint8_t val = dev->read_register(REG6D);

    return (val & 0x1) == 0;
}

void CommandSetGl841::eject_document(Genesys_Device* dev) const
{
    DBG_HELPER(dbg);
  Genesys_Register_Set local_reg;
  unsigned int init_steps;
  float feed_mm;
  int loop;

    if (!dev->model->is_sheetfed) {
      DBG(DBG_proc, "%s: there is no \"eject sheet\"-concept for non sheet fed\n", __func__);
      DBG(DBG_proc, "%s: finished\n", __func__);
      return;
    }


  local_reg.clear();

    sanei_genesys_get_status(dev);

    gl841_stop_action(dev);

  local_reg = dev->reg;

  gl841_init_optical_regs_off(&local_reg);

  const auto& sensor = sanei_genesys_find_sensor_any(dev);
  gl841_init_motor_regs(dev, sensor, &local_reg,
			65536,MOTOR_ACTION_FEED,0);

    dev->write_registers(local_reg);

    try {
        gl841_start_action(dev);
    } catch (...) {
        catch_all_exceptions(__func__, [&]() { gl841_stop_action(dev); });
        // restore original registers
        catch_all_exceptions(__func__, [&]()
        {
            dev->write_registers(dev->reg);
        });
        throw;
    }

    if (gl841_get_paper_sensor(dev)) {
      DBG(DBG_info, "%s: paper still loaded\n", __func__);
      /* force document TRUE, because it is definitely present */
        dev->document = true;
      dev->scanhead_position_in_steps = 0;

      loop = 300;
      while (loop > 0)		/* do not wait longer then 30 seconds */
	{

            if (!gl841_get_paper_sensor(dev)) {
	      DBG(DBG_info, "%s: reached home position\n", __func__);
	      DBG(DBG_proc, "%s: finished\n", __func__);
	      break;
	    }
          sanei_genesys_sleep_ms(100);
	  --loop;
	}

      if (loop == 0)
	{
          // when we come here then the scanner needed too much time for this, so we better stop
          // the motor
          catch_all_exceptions(__func__, [&](){ gl841_stop_action(dev); });
          throw SaneException(SANE_STATUS_IO_ERROR,
                              "timeout while waiting for scanhead to go home");
	}
    }

    feed_mm = static_cast<float>(dev->model->eject_feed);
  if (dev->document)
    {
        feed_mm += static_cast<float>(dev->model->post_scan);
    }

        sanei_genesys_read_feed_steps(dev, &init_steps);

  /* now feed for extra <number> steps */
  loop = 0;
  while (loop < 300)		/* do not wait longer then 30 seconds */
    {
      unsigned int steps;

        sanei_genesys_read_feed_steps(dev, &steps);

      DBG(DBG_info, "%s: init_steps: %d, steps: %d\n", __func__, init_steps, steps);

      if (steps > init_steps + (feed_mm * dev->motor.base_ydpi) / MM_PER_INCH)
	{
	  break;
	}

      sanei_genesys_sleep_ms(100);
      ++loop;
    }

    gl841_stop_action(dev);

    dev->document = false;
}


void CommandSetGl841::load_document(Genesys_Device* dev) const
{
    DBG_HELPER(dbg);
  int loop = 300;
  while (loop > 0)		/* do not wait longer then 30 seconds */
    {
        if (gl841_get_paper_sensor(dev)) {
	  DBG(DBG_info, "%s: document inserted\n", __func__);

	  /* when loading OK, document is here */
        dev->document = true;

          // give user some time to place document correctly
          sanei_genesys_sleep_ms(1000);
	  break;
	}
      sanei_genesys_sleep_ms(100);
      --loop;
    }

  if (loop == 0)
    {
        // when we come here then the user needed to much time for this
        throw SaneException(SANE_STATUS_IO_ERROR, "timeout while waiting for document");
    }
}

/**
 * detects end of document and adjust current scan
 * to take it into account
 * used by sheetfed scanners
 */
void CommandSetGl841::detect_document_end(Genesys_Device* dev) const
{
    DBG_HELPER(dbg);
    bool paper_loaded = gl841_get_paper_sensor(dev);

  /* sheetfed scanner uses home sensor as paper present */
    if (dev->document && !paper_loaded) {
      DBG(DBG_info, "%s: no more document\n", __func__);
        dev->document = false;

      /* we can't rely on total_bytes_to_read since the frontend
       * might have been slow to read data, so we re-evaluate the
       * amount of data to scan form the hardware settings
       */
        unsigned scanned_lines = 0;
        try {
            sanei_genesys_read_scancnt(dev, &scanned_lines);
        } catch (...) {
            dev->total_bytes_to_read = dev->total_bytes_read;
            throw;
        }

        if (dev->settings.scan_mode == ScanColorMode::COLOR_SINGLE_PASS && dev->model->is_cis) {
            scanned_lines /= 3;
        }

        std::size_t output_lines = dev->session.output_line_count;

        std::size_t offset_lines = static_cast<std::size_t>(
                (dev->model->post_scan / MM_PER_INCH) * dev->settings.yres);

        std::size_t scan_end_lines = scanned_lines + offset_lines;

        std::size_t remaining_lines = dev->get_pipeline_source().remaining_bytes() /
                dev->session.output_line_bytes_raw;

        DBG(DBG_io, "%s: scanned_lines=%u\n", __func__, scanned_lines);
        DBG(DBG_io, "%s: scan_end_lines=%zu\n", __func__, scan_end_lines);
        DBG(DBG_io, "%s: output_lines=%zu\n", __func__, output_lines);
        DBG(DBG_io, "%s: remaining_lines=%zu\n", __func__, remaining_lines);

        if (scan_end_lines > output_lines) {
            auto skip_lines = scan_end_lines - output_lines;

            if (remaining_lines > skip_lines) {
                DBG(DBG_io, "%s: skip_lines=%zu\n", __func__, skip_lines);

                remaining_lines -= skip_lines;
                dev->get_pipeline_source().set_remaining_bytes(remaining_lines *
                                                               dev->session.output_line_bytes_raw);
                dev->total_bytes_to_read -= skip_lines * dev->session.output_line_bytes_requested;
            }
        }
    }
}

// Send the low-level scan command
// todo : is this that useful ?
void CommandSetGl841::begin_scan(Genesys_Device* dev, const Genesys_Sensor& sensor,
                                 Genesys_Register_Set* reg, bool start_motor) const
{
    DBG_HELPER(dbg);
    (void) sensor;
  // FIXME: SEQUENTIAL not really needed in this case
  Genesys_Register_Set local_reg(Genesys_Register_Set::SEQUENTIAL);
  uint8_t val;

    if (dev->model->gpio_id == GpioId::CANON_LIDE_80) {
        val = dev->read_register(REG6B);
        val = REG6B_GPO18;
        dev->write_register(REG6B, val);
    }

    if (dev->model->sensor_id != SensorId::CCD_PLUSTEK_OPTICPRO_3600) {
        local_reg.init_reg(0x03, reg->get8(0x03) | REG03_LAMPPWR);
    } else {
        // TODO PLUSTEK_3600: why ??
        local_reg.init_reg(0x03, reg->get8(0x03));
    }

    local_reg.init_reg(0x01, reg->get8(0x01) | REG01_SCAN);
    local_reg.init_reg(0x0d, 0x01);

    if (start_motor) {
        local_reg.init_reg(0x0f, 0x01);
    } else {
        // do not start motor yet
        local_reg.init_reg(0x0f, 0x00);
    }

    dev->write_registers(local_reg);
}


// Send the stop scan command
void CommandSetGl841::end_scan(Genesys_Device* dev, Genesys_Register_Set __sane_unused__* reg,
                               bool check_stop) const
{
    DBG_HELPER_ARGS(dbg, "check_stop = %d", check_stop);

    if (!dev->model->is_sheetfed) {
        gl841_stop_action(dev);
    }
}

// Moves the slider to steps
static void gl841_feed(Genesys_Device* dev, int steps)
{
    DBG_HELPER_ARGS(dbg, "steps = %d", steps);
  Genesys_Register_Set local_reg;
  uint8_t val;
  int loop;

    gl841_stop_action(dev);

  // FIXME: we should pick sensor according to the resolution scanner is currently operating on
  const auto& sensor = sanei_genesys_find_sensor_any(dev);

  local_reg = dev->reg;

  gl841_init_optical_regs_off(&local_reg);

  gl841_init_motor_regs(dev, sensor, &local_reg, steps,MOTOR_ACTION_FEED,0);

    dev->write_registers(local_reg);

    try {
        gl841_start_action(dev);
    } catch (...) {
        catch_all_exceptions(__func__, [&]() { gl841_stop_action (dev); });
        // restore original registers
        catch_all_exceptions(__func__, [&]()
        {
            dev->write_registers(dev->reg);
        });
        throw;
    }

  loop = 0;
  while (loop < 300)		/* do not wait longer then 30 seconds */
  {
        val = sanei_genesys_get_status(dev);

      if (!(val & REG41_MOTORENB))	/* motor enabled */
      {
          DBG(DBG_proc, "%s: finished\n", __func__);
	  dev->scanhead_position_in_steps += steps;
            return;
      }
      sanei_genesys_sleep_ms(100);
      ++loop;
  }

  /* when we come here then the scanner needed too much time for this, so we better stop the motor */
  gl841_stop_action (dev);

    throw SaneException(SANE_STATUS_IO_ERROR, "timeout while waiting for scanhead to go home");
}

// Moves the slider to the home (top) position slowly
void CommandSetGl841::slow_back_home(Genesys_Device* dev, bool wait_until_home) const
{
    DBG_HELPER_ARGS(dbg, "wait_until_home = %d", wait_until_home);
  Genesys_Register_Set local_reg;
  GenesysRegister *r;
  int loop = 0;

    if (dev->model->is_sheetfed) {
      DBG(DBG_proc, "%s: there is no \"home\"-concept for sheet fed\n", __func__);
      DBG(DBG_proc, "%s: finished\n", __func__);
      return;
    }

    // reset gpio pin
    uint8_t val;
    if (dev->model->gpio_id == GpioId::CANON_LIDE_35) {
        val = dev->read_register(REG6C);
        val = dev->gpo.regs.get_value(0x6c);
        dev->write_register(REG6C, val);
    }
    if (dev->model->gpio_id == GpioId::CANON_LIDE_80) {
        val = dev->read_register(REG6B);
        val = REG6B_GPO18 | REG6B_GPO17;
        dev->write_register(REG6B, val);
    }
    dev->cmd_set->save_power(dev, false);

    // first read gives HOME_SENSOR true
    val = sanei_genesys_get_status(dev);

  if (DBG_LEVEL >= DBG_io)
    {
      sanei_genesys_print_status (val);
    }
  sanei_genesys_sleep_ms(100);

    // second is reliable
    val = sanei_genesys_get_status(dev);

  if (DBG_LEVEL >= DBG_io)
    {
      sanei_genesys_print_status (val);
    }

  dev->scanhead_position_in_steps = 0;

  if (val & REG41_HOMESNR)	/* is sensor at home? */
    {
      DBG(DBG_info, "%s: already at home, completed\n", __func__);
      dev->scanhead_position_in_steps = 0;
      return;
    }

  /* end previous scan if any */
  r = sanei_genesys_get_address(&dev->reg, REG01);
  r->value &= ~REG01_SCAN;
    dev->write_register(REG01, r->value);

  /* if motor is on, stop current action */
  if (val & REG41_MOTORENB)
    {
        gl841_stop_action(dev);
    }

  local_reg = dev->reg;

  const auto& sensor = sanei_genesys_find_sensor_any(dev);

  gl841_init_motor_regs(dev, sensor, &local_reg, 65536,MOTOR_ACTION_GO_HOME,0);

  /* set up for reverse and no scan */
  r = sanei_genesys_get_address(&local_reg, REG02);
  r->value |= REG02_MTRREV;
  r = sanei_genesys_get_address(&local_reg, REG01);
  r->value &= ~REG01_SCAN;

    dev->write_registers(local_reg);

    try {
        gl841_start_action(dev);
    } catch (...) {
        catch_all_exceptions(__func__, [&]() { gl841_stop_action(dev); });
        // restore original registers
        catch_all_exceptions(__func__, [&]()
        {
            dev->write_registers(dev->reg);
        });
        throw;
    }

  if (wait_until_home)
    {
      while (loop < 300)		/* do not wait longer then 30 seconds */
	{
        val = sanei_genesys_get_status(dev);

	  if (val & REG41_HOMESNR)	/* home sensor */
	    {
	      DBG(DBG_info, "%s: reached home position\n", __func__);
	      DBG(DBG_proc, "%s: finished\n", __func__);
          return;
	    }
          sanei_genesys_sleep_ms(100);
	  ++loop;
	}

        // when we come here then the scanner needed too much time for this, so we better stop
        // the motor
        catch_all_exceptions(__func__, [&](){ gl841_stop_action(dev); });
        throw SaneException(SANE_STATUS_IO_ERROR, "timeout while waiting for scanhead to go home");
    }

  DBG(DBG_info, "%s: scanhead is still moving\n", __func__);
}

// Automatically set top-left edge of the scan area by scanning a 200x200 pixels area at 600 dpi
// from very top of scanner
void CommandSetGl841::search_start_position(Genesys_Device* dev) const
{
    DBG_HELPER(dbg);
  int size;
  Genesys_Register_Set local_reg;

  int pixels = 600;
  int dpi = 300;

  local_reg = dev->reg;

  /* sets for a 200 lines * 600 pixels */
  /* normal scan with no shading */

    // FIXME: the current approach of doing search only for one resolution does not work on scanners
    // whith employ different sensors with potentially different settings.
    const auto& sensor = sanei_genesys_find_sensor(dev, dpi, 1, dev->model->default_method);

    ScanSession session;
    session.params.xres = dpi;
    session.params.yres = dpi;
    session.params.startx = 0;
    session.params.starty = 0; /*we should give a small offset here~60 steps*/
    session.params.pixels = 600;
    session.params.lines = dev->model->search_lines;
    session.params.depth = 8;
    session.params.channels = 1;
    session.params.scan_method = dev->settings.scan_method;
    session.params.scan_mode = ScanColorMode::GRAY;
    session.params.color_filter = ColorFilter::GREEN;
    session.params.flags = SCAN_FLAG_DISABLE_SHADING |
                           SCAN_FLAG_DISABLE_GAMMA |
                           SCAN_FLAG_IGNORE_LINE_DISTANCE |
                           SCAN_FLAG_DISABLE_BUFFER_FULL_MOVE;
    gl841_compute_session(dev, session, sensor);

    gl841_init_scan_regs(dev, sensor, &local_reg, session);

    // send to scanner
    dev->write_registers(local_reg);

  size = pixels * dev->model->search_lines;

  std::vector<uint8_t> data(size);

    dev->cmd_set->begin_scan(dev, sensor, &local_reg, true);

    wait_until_buffer_non_empty(dev);

    // now we're on target, we can read data
    sanei_genesys_read_data_from_scanner(dev, data.data(), size);

    if (DBG_LEVEL >= DBG_data) {
        sanei_genesys_write_pnm_file("gl841_search_position.pnm", data.data(), 8, 1, pixels,
                                     dev->model->search_lines);
    }

    dev->cmd_set->end_scan(dev, &local_reg, true);

  /* update regs to copy ASIC internal state */
  dev->reg = local_reg;

    for (auto& sensor_update :
            sanei_genesys_find_sensors_all_for_write(dev, dev->model->default_method))
    {
        sanei_genesys_search_reference_point(dev, sensor_update, data.data(), 0, dpi, pixels,
                                             dev->model->search_lines);
    }
}

// sets up register for coarse gain calibration
// todo: check it for scanners using it
void CommandSetGl841::init_regs_for_coarse_calibration(Genesys_Device* dev,
                                                       const Genesys_Sensor& sensor,
                                                       Genesys_Register_Set& regs) const
{
    DBG_HELPER(dbg);

    ScanSession session;
    session.params.xres = dev->settings.xres;
    session.params.yres = dev->settings.yres;
    session.params.startx = 0;
    session.params.starty = 0;
    session.params.pixels = sensor.optical_res / sensor.ccd_pixels_per_system_pixel();
    session.params.lines = 20;
    session.params.depth = 16;
    session.params.channels = dev->settings.get_channels();
    session.params.scan_method = dev->settings.scan_method;
    session.params.scan_mode = dev->settings.scan_mode;
    session.params.color_filter = dev->settings.color_filter;
    session.params.flags = SCAN_FLAG_DISABLE_SHADING |
                           SCAN_FLAG_DISABLE_GAMMA |
                           SCAN_FLAG_SINGLE_LINE |
                           SCAN_FLAG_IGNORE_LINE_DISTANCE;
    gl841_compute_session(dev, session, sensor);

    gl841_init_scan_regs(dev, sensor, &regs, session);

  DBG(DBG_info, "%s: optical sensor res: %d dpi, actual res: %d\n", __func__,
      sensor.optical_res / sensor.ccd_pixels_per_system_pixel(), dev->settings.xres);

    dev->write_registers(regs);

/*  if (DBG_LEVEL >= DBG_info)
    sanei_gl841_print_registers (regs);*/
}


// init registers for shading calibration
void CommandSetGl841::init_regs_for_shading(Genesys_Device* dev, const Genesys_Sensor& sensor,
                                            Genesys_Register_Set& regs) const
{
    DBG_HELPER_ARGS(dbg, "lines = %zu", dev->calib_lines);
  SANE_Int ydpi;
    unsigned starty = 0;

  /* initial calibration reg values */
  regs = dev->reg;

  ydpi = dev->motor.base_ydpi;
  if (dev->model->motor_id == MotorId::PLUSTEK_OPTICPRO_3600)  /* TODO PLUSTEK_3600: 1200dpi not yet working, produces dark bar */
    {
      ydpi = 600;
    }
    if (dev->model->motor_id == MotorId::CANON_LIDE_80) {
      ydpi = gl841_get_dpihw(dev);
      /* get over extra dark area for this model.
	 It looks like different devices have dark areas of different width
	 due to manufacturing variability. The initial value of starty was 140,
	 but it moves the sensor almost past the dark area completely in places
	 on certain devices.

	 On a particular device the black area starts at roughly position
	 160 to 230 depending on location (the dark area is not completely
	 parallel to the frame).
      */
      starty = 70;
    }

  dev->calib_channels = 3;
  dev->calib_lines = dev->model->shading_lines;

    unsigned resolution = sensor.get_logical_hwdpi(dev->settings.xres);
    unsigned factor = sensor.optical_res / resolution;

    const auto& calib_sensor = sanei_genesys_find_sensor(dev, resolution, dev->calib_channels,
                                                         dev->settings.scan_method);

    dev->calib_pixels = calib_sensor.sensor_pixels / factor;

    ScanSession session;
    session.params.xres = resolution;
    session.params.yres = ydpi;
    session.params.startx = 0;
    session.params.starty = starty;
    session.params.pixels = dev->calib_pixels;
    session.params.lines = dev->calib_lines;
    session.params.depth = 16;
    session.params.channels = dev->calib_channels;
    session.params.scan_method = dev->settings.scan_method;
    session.params.scan_mode = ScanColorMode::COLOR_SINGLE_PASS;
    session.params.color_filter = dev->settings.color_filter;
    session.params.flags = SCAN_FLAG_DISABLE_SHADING |
                           SCAN_FLAG_DISABLE_GAMMA |
                           /*SCAN_FLAG_DISABLE_BUFFER_FULL_MOVE |*/
                           SCAN_FLAG_IGNORE_LINE_DISTANCE;
    gl841_compute_session(dev, session, calib_sensor);

    gl841_init_scan_regs(dev, calib_sensor, &regs, session);

  dev->scanhead_position_in_steps += dev->calib_lines + starty;

    dev->write_registers(regs);
}

// set up registers for the actual scan
void CommandSetGl841::init_regs_for_scan(Genesys_Device* dev, const Genesys_Sensor& sensor) const
{
    DBG_HELPER(dbg);
  int flags;
  float move;
  int move_dpi;
  float start;

    debug_dump(DBG_info, dev->settings);

  /* steps to move to reach scanning area:
     - first we move to physical start of scanning
     either by a fixed steps amount from the black strip
     or by a fixed amount from parking position,
     minus the steps done during shading calibration
     - then we move by the needed offset whitin physical
     scanning area

     assumption: steps are expressed at maximum motor resolution

     we need:
     float y_offset;
     float y_size;
     float y_offset_calib;
     mm_to_steps()=motor dpi / 2.54 / 10=motor dpi / MM_PER_INCH */

  /* if scanner uses GENESYS_FLAG_SEARCH_START y_offset is
     relative from origin, else, it is from parking position */

  move_dpi = dev->motor.base_ydpi;

  move = 0;
    if (dev->model->flags & GENESYS_FLAG_SEARCH_START) {
        move += static_cast<float>(dev->model->y_offset_calib_white);
    }

  DBG(DBG_info, "%s move=%f steps\n", __func__, move);

    move += static_cast<float>(dev->model->y_offset);
  DBG(DBG_info, "%s: move=%f steps\n", __func__, move);

    move += static_cast<float>(dev->settings.tl_y);
  DBG(DBG_info, "%s: move=%f steps\n", __func__, move);

    move = static_cast<float>((move * move_dpi) / MM_PER_INCH);

/* start */
    start = static_cast<float>(dev->model->x_offset);

    start += static_cast<float>(dev->settings.tl_x);

    start = static_cast<float>((start * sensor.optical_res) / MM_PER_INCH);

  /* we enable true gray for cis scanners only, and just when doing
   * scan since color calibration is OK for this mode
   */
  flags = 0;

  /* true gray (led add for cis scanners) */
  if(dev->model->is_cis && dev->settings.true_gray
    && dev->settings.scan_mode != ScanColorMode::COLOR_SINGLE_PASS
    && dev->model->sensor_id != SensorId::CIS_CANON_LIDE_80)
    {
      // on Lide 80 the LEDADD bit results in only red LED array being lit
      DBG(DBG_io, "%s: activating LEDADD\n", __func__);
      flags |= SCAN_FLAG_ENABLE_LEDADD;
    }

    ScanSession session;
    session.params.xres = dev->settings.xres;
    session.params.yres = dev->settings.yres;
    session.params.startx = static_cast<unsigned>(start);
    session.params.starty = static_cast<unsigned>(move);
    session.params.pixels = dev->settings.pixels;
    session.params.requested_pixels = dev->settings.requested_pixels;
    session.params.lines = dev->settings.lines;
    session.params.depth = dev->settings.depth;
    session.params.channels = dev->settings.get_channels();
    session.params.scan_method = dev->settings.scan_method;
    session.params.scan_mode = dev->settings.scan_mode;
    session.params.color_filter = dev->settings.color_filter;
    session.params.flags = flags;
    gl841_compute_session(dev, session, sensor);

    gl841_init_scan_regs(dev, sensor, &dev->reg, session);
}


// this function sends generic gamma table (ie linear ones) or the Sensor specific one if provided
void CommandSetGl841::send_gamma_table(Genesys_Device* dev, const Genesys_Sensor& sensor) const
{
    DBG_HELPER(dbg);
  int size;

  size = 256;

  /* allocate temporary gamma tables: 16 bits words, 3 channels */
  std::vector<uint8_t> gamma(size * 2 * 3);

    sanei_genesys_generate_gamma_buffer(dev, sensor, 16, 65535, size, gamma.data());

    // send address
    gl841_set_buffer_address_gamma (dev, 0x00000);

    // send data
    sanei_genesys_bulk_write_data(dev, 0x28, gamma.data(), size * 2 * 3);
}


/* this function does the led calibration by scanning one line of the calibration
   area below scanner's top on white strip.

-needs working coarse/gain
*/
SensorExposure CommandSetGl841::led_calibration(Genesys_Device* dev, const Genesys_Sensor& sensor,
                                                Genesys_Register_Set& regs) const
{
    DBG_HELPER(dbg);
  int num_pixels;
  int total_size;
  int i, j;
  int val;
  int channels;
  int avg[3], avga, avge;
  int turn;
  uint16_t exp[3], target;
  int move;

  /* these 2 boundaries should be per sensor */
  uint16_t min_exposure=500;
  uint16_t max_exposure;

  /* feed to white strip if needed */
    if (dev->model->y_offset_calib_white > 0) {
        move = static_cast<int>(dev->model->y_offset_calib_white);
        move = static_cast<int>((move * (dev->motor.base_ydpi)) / MM_PER_INCH);
      DBG(DBG_io, "%s: move=%d lines\n", __func__, move);
        gl841_feed(dev, move);
    }

  /* offset calibration is always done in color mode */
  channels = 3;

    unsigned resolution = sensor.get_logical_hwdpi(dev->settings.xres);
    unsigned factor = sensor.optical_res / resolution;

    const auto& calib_sensor_base = sanei_genesys_find_sensor(dev, resolution, channels,
                                                              dev->settings.scan_method);

    num_pixels = calib_sensor_base.sensor_pixels / factor;

    ScanSession session;
    session.params.xres = resolution;
    session.params.yres = dev->settings.yres;
    session.params.startx = 0;
    session.params.starty = 0;
    session.params.pixels = num_pixels;
    session.params.lines = 1;
    session.params.depth = 16;
    session.params.channels = channels;
    session.params.scan_method = dev->settings.scan_method;
    session.params.scan_mode = ScanColorMode::COLOR_SINGLE_PASS;
    session.params.color_filter = dev->settings.color_filter;
    session.params.flags = SCAN_FLAG_DISABLE_SHADING |
                           SCAN_FLAG_DISABLE_GAMMA |
                           SCAN_FLAG_SINGLE_LINE |
                           SCAN_FLAG_IGNORE_LINE_DISTANCE;
    gl841_compute_session(dev, session, calib_sensor_base);

    gl841_init_scan_regs(dev, calib_sensor_base, &regs, session);

    dev->write_registers(regs);


  total_size = num_pixels * channels * 2 * 1;	/* colors * bytes_per_color * scan lines */

  std::vector<uint8_t> line(total_size);

/*
   we try to get equal bright leds here:

   loop:
     average per color
     adjust exposure times
 */

  exp[0] = sensor.exposure.red;
  exp[1] = sensor.exposure.green;
  exp[2] = sensor.exposure.blue;

  turn = 0;
  /* max exposure is set to ~2 time initial average
   * exposure, or 2 time last calibration exposure */
  max_exposure=((exp[0]+exp[1]+exp[2])/3)*2;
  target=sensor.gain_white_ref*256;

    auto calib_sensor = calib_sensor_base;

    bool acceptable = false;
    do {
        calib_sensor.exposure.red = exp[0];
        calib_sensor.exposure.green = exp[1];
        calib_sensor.exposure.blue = exp[2];

        sanei_genesys_set_exposure(regs, calib_sensor.exposure);
        dev->write_register(0x10, (calib_sensor.exposure.red >> 8) & 0xff);
        dev->write_register(0x11, calib_sensor.exposure.red & 0xff);
        dev->write_register(0x12, (calib_sensor.exposure.green >> 8) & 0xff);
        dev->write_register(0x13, calib_sensor.exposure.green & 0xff);
        dev->write_register(0x14, (calib_sensor.exposure.blue >> 8) & 0xff);
        dev->write_register(0x15, calib_sensor.exposure.blue & 0xff);

        dev->write_registers(regs);

      DBG(DBG_info, "%s: starting line reading\n", __func__);
        dev->cmd_set->begin_scan(dev, calib_sensor, &regs, true);
        sanei_genesys_read_data_from_scanner(dev, line.data(), total_size);

      if (DBG_LEVEL >= DBG_data) {
          char fn[30];
          snprintf(fn, 30, "gl841_led_%d.pnm", turn);
          sanei_genesys_write_pnm_file(fn, line.data(), 16, channels, num_pixels, 1);
      }

     /* compute average */
      for (j = 0; j < channels; j++)
      {
	  avg[j] = 0;
	  for (i = 0; i < num_pixels; i++)
	  {
	      if (dev->model->is_cis)
		  val =
		      line[i * 2 + j * 2 * num_pixels + 1] * 256 +
		      line[i * 2 + j * 2 * num_pixels];
	      else
		  val =
		      line[i * 2 * channels + 2 * j + 1] * 256 +
		      line[i * 2 * channels + 2 * j];
	      avg[j] += val;
	  }

	  avg[j] /= num_pixels;
      }

      DBG(DBG_info,"%s: average: %d,%d,%d\n", __func__, avg[0], avg[1], avg[2]);

        acceptable = true;

     /* exposure is acceptable if each color is in the %5 range
      * of other color channels */
      if (avg[0] < avg[1] * 0.95 || avg[1] < avg[0] * 0.95 ||
	  avg[0] < avg[2] * 0.95 || avg[2] < avg[0] * 0.95 ||
	  avg[1] < avg[2] * 0.95 || avg[2] < avg[1] * 0.95)
        {
            acceptable = false;
        }

      /* led exposure is not acceptable if white level is too low
       * ~80 hardcoded value for white level */
      if(avg[0]<20000 || avg[1]<20000 || avg[2]<20000)
        {
            acceptable = false;
        }

      /* for scanners using target value */
      if(target>0)
        {
            acceptable = true;
          for(i=0;i<3;i++)
            {
              /* we accept +- 2% delta from target */
              if(abs(avg[i]-target)>target/50)
                {
                  exp[i]=(exp[i]*target)/avg[i];
                    acceptable = false;
                }
            }
        }
      else
        {
          if (!acceptable)
            {
              avga = (avg[0]+avg[1]+avg[2])/3;
              exp[0] = (exp[0] * avga) / avg[0];
              exp[1] = (exp[1] * avga) / avg[1];
              exp[2] = (exp[2] * avga) / avg[2];
              /*
                keep the resulting exposures below this value.
                too long exposure drives the ccd into saturation.
                we may fix this by relying on the fact that
                we get a striped scan without shading, by means of
                statistical calculation
              */
              avge = (exp[0] + exp[1] + exp[2]) / 3;

              if (avge > max_exposure) {
                  exp[0] = (exp[0] * max_exposure) / avge;
                  exp[1] = (exp[1] * max_exposure) / avge;
                  exp[2] = (exp[2] * max_exposure) / avge;
              }
              if (avge < min_exposure) {
                  exp[0] = (exp[0] * min_exposure) / avge;
                  exp[1] = (exp[1] * min_exposure) / avge;
                  exp[2] = (exp[2] * min_exposure) / avge;
              }

            }
        }

        gl841_stop_action(dev);

      turn++;

  } while (!acceptable && turn < 100);

  DBG(DBG_info,"%s: acceptable exposure: %d,%d,%d\n", __func__, exp[0], exp[1], exp[2]);

    dev->cmd_set->slow_back_home(dev, true);

    return calib_sensor.exposure;
}

/** @brief calibration for AD frontend devices
 * offset calibration assumes that the scanning head is on a black area
 * For LiDE80 analog frontend
 * 0x0003 : is gain and belongs to [0..63]
 * 0x0006 : is offset
 * We scan a line with no gain until average offset reaches the target
 */
static void ad_fe_offset_calibration(Genesys_Device* dev, const Genesys_Sensor& sensor,
                                     Genesys_Register_Set& regs)
{
    DBG_HELPER(dbg);
  int num_pixels;
  int total_size;
  int i;
  int average;
  int turn;
  int top;
  int bottom;
  int target;

  /* don't impact 3600 behavior since we can't test it */
    if (dev->model->sensor_id == SensorId::CCD_PLUSTEK_OPTICPRO_3600) {
      return;
    }

    unsigned resolution = sensor.get_logical_hwdpi(dev->settings.xres);
    unsigned factor = sensor.optical_res / resolution;

    const auto& calib_sensor = sanei_genesys_find_sensor(dev, resolution, 3,
                                                              dev->settings.scan_method);

    num_pixels = calib_sensor.sensor_pixels / factor;

    ScanSession session;
    session.params.xres = resolution;
    session.params.yres = dev->settings.yres;
    session.params.startx = 0;
    session.params.starty = 0;
    session.params.pixels = num_pixels;
    session.params.lines = 1;
    session.params.depth = 8;
    session.params.channels = 3;
    session.params.scan_method = dev->settings.scan_method;
    session.params.scan_mode = ScanColorMode::COLOR_SINGLE_PASS;
    session.params.color_filter = dev->settings.color_filter;
    session.params.flags = SCAN_FLAG_DISABLE_SHADING |
                           SCAN_FLAG_DISABLE_GAMMA |
                           SCAN_FLAG_SINGLE_LINE |
                           SCAN_FLAG_IGNORE_LINE_DISTANCE;
    gl841_compute_session(dev, session, calib_sensor);

    gl841_init_scan_regs(dev, calib_sensor, &regs, session);

  total_size = num_pixels * 3 * 2 * 1;

  std::vector<uint8_t> line(total_size);

  dev->frontend.set_gain(0, 0);
  dev->frontend.set_gain(1, 0);
  dev->frontend.set_gain(2, 0);

  /* loop on scan until target offset is reached */
  turn=0;
  target=24;
  bottom=0;
  top=255;
  do {
      /* set up offset mid range */
      dev->frontend.set_offset(0, (top + bottom) / 2);
      dev->frontend.set_offset(1, (top + bottom) / 2);
      dev->frontend.set_offset(2, (top + bottom) / 2);

      /* scan line */
      DBG(DBG_info, "%s: starting line reading\n", __func__);
        dev->write_registers(regs);
      dev->cmd_set->set_fe(dev, calib_sensor, AFE_SET);
        dev->cmd_set->begin_scan(dev, calib_sensor, &regs, true);
      sanei_genesys_read_data_from_scanner(dev, line.data(), total_size);
      gl841_stop_action (dev);
      if (DBG_LEVEL >= DBG_data) {
          char fn[30];
          snprintf(fn, 30, "gl841_offset_%02d.pnm", turn);
          sanei_genesys_write_pnm_file(fn, line.data(), 8, 3, num_pixels, 1);
      }

      /* search for minimal value */
      average=0;
      for(i=0;i<total_size;i++)
        {
          average+=line[i];
        }
      average/=total_size;
      DBG(DBG_data, "%s: average=%d\n", __func__, average);

      /* if min value is above target, the current value becomes the new top
       * else it is the new bottom */
      if(average>target)
        {
          top=(top+bottom)/2;
        }
      else
        {
          bottom=(top+bottom)/2;
        }
      turn++;
  } while ((top-bottom)>1 && turn < 100);

  // FIXME: don't overwrite the calibrated values
  dev->frontend.set_offset(0, 0);
  dev->frontend.set_offset(1, 0);
  dev->frontend.set_offset(2, 0);
  DBG(DBG_info, "%s: offset=(%d,%d,%d)\n", __func__,
      dev->frontend.get_offset(0),
      dev->frontend.get_offset(1),
      dev->frontend.get_offset(2));
}

/* this function does the offset calibration by scanning one line of the calibration
   area below scanner's top. There is a black margin and the remaining is white.
   sanei_genesys_search_start() must have been called so that the offsets and margins
   are allready known.

this function expects the slider to be where?
*/
void CommandSetGl841::offset_calibration(Genesys_Device* dev, const Genesys_Sensor& sensor,
                                         Genesys_Register_Set& regs) const
{
    DBG_HELPER(dbg);
  int num_pixels;
  int total_size;
  int i, j;
  int val;
  int channels;
  int off[3],offh[3],offl[3],off1[3],off2[3];
  int min1[3],min2[3];
  int cmin[3],cmax[3];
  int turn;
  int mintgt = 0x400;

  /* Analog Device fronted have a different calibration */
  if ((dev->reg.find_reg(0x04).value & REG04_FESET) == 0x02)
    {
      return ad_fe_offset_calibration(dev, sensor, regs);
    }

  /* offset calibration is always done in color mode */
  channels = 3;

    unsigned resolution = sensor.get_logical_hwdpi(dev->settings.xres);
    unsigned factor = sensor.optical_res / resolution;

    const auto& calib_sensor = sanei_genesys_find_sensor(dev, resolution, channels,
                                                         dev->settings.scan_method);

    num_pixels = calib_sensor.sensor_pixels / factor;

    ScanSession session;
    session.params.xres = resolution;
    session.params.yres = dev->settings.yres;
    session.params.startx = 0;
    session.params.starty = 0;
    session.params.pixels = num_pixels;
    session.params.lines = 1;
    session.params.depth = 16;
    session.params.channels = channels;
    session.params.scan_method = dev->settings.scan_method;
    session.params.scan_mode = ScanColorMode::COLOR_SINGLE_PASS;
    session.params.color_filter = dev->settings.color_filter;
    session.params.flags = SCAN_FLAG_DISABLE_SHADING |
                           SCAN_FLAG_DISABLE_GAMMA |
                           SCAN_FLAG_SINGLE_LINE |
                           SCAN_FLAG_IGNORE_LINE_DISTANCE |
                           SCAN_FLAG_DISABLE_LAMP;
    gl841_compute_session(dev, session, calib_sensor);

    gl841_init_scan_regs(dev, calib_sensor, &regs, session);

  total_size = num_pixels * channels * 2 * 1;	/* colors * bytes_per_color * scan lines */

  std::vector<uint8_t> first_line(total_size);
  std::vector<uint8_t> second_line(total_size);

  /* scan first line of data with no offset nor gain */
/*WM8199: gain=0.73; offset=-260mV*/
/*okay. the sensor black level is now at -260mV. we only get 0 from AFE...*/
/* we should probably do real calibration here:
 * -detect acceptable offset with binary search
 * -calculate offset from this last version
 *
 * acceptable offset means
 *   - few completely black pixels(<10%?)
 *   - few completely white pixels(<10%?)
 *
 * final offset should map the minimum not completely black
 * pixel to 0(16 bits)
 *
 * this does account for dummy pixels at the end of ccd
 * this assumes slider is at black strip(which is not quite as black as "no
 * signal").
 *
 */
  dev->frontend.set_gain(0, 0);
  dev->frontend.set_gain(1, 0);
  dev->frontend.set_gain(2, 0);
  offh[0] = 0xff;
  offh[1] = 0xff;
  offh[2] = 0xff;
  offl[0] = 0x00;
  offl[1] = 0x00;
  offl[2] = 0x00;
  turn = 0;

    bool acceptable = false;
  do {

        dev->write_registers(regs);

      for (j=0; j < channels; j++) {
	  off[j] = (offh[j]+offl[j])/2;
          dev->frontend.set_offset(j, off[j]);
      }

        dev->cmd_set->set_fe(dev, calib_sensor, AFE_SET);

      DBG(DBG_info, "%s: starting first line reading\n", __func__);
        dev->cmd_set->begin_scan(dev, calib_sensor, &regs, true);

        sanei_genesys_read_data_from_scanner(dev, first_line.data(), total_size);

      if (DBG_LEVEL >= DBG_data) {
          char fn[30];
          snprintf(fn, 30, "gl841_offset1_%02d.pnm", turn);
          sanei_genesys_write_pnm_file(fn, first_line.data(), 16, channels, num_pixels, 1);
      }

        acceptable = true;

      for (j = 0; j < channels; j++)
      {
	  cmin[j] = 0;
	  cmax[j] = 0;

	  for (i = 0; i < num_pixels; i++)
	  {
	      if (dev->model->is_cis)
		  val =
		      first_line[i * 2 + j * 2 * num_pixels + 1] * 256 +
		      first_line[i * 2 + j * 2 * num_pixels];
	      else
		  val =
		      first_line[i * 2 * channels + 2 * j + 1] * 256 +
		      first_line[i * 2 * channels + 2 * j];
	      if (val < 10)
		  cmin[j]++;
	      if (val > 65525)
		  cmax[j]++;
	  }

          /* TODO the DP685 has a black strip in the middle of the sensor
           * should be handled in a more elegant way , could be a bug */
          if (dev->model->sensor_id == SensorId::CCD_DP685)
              cmin[j] -= 20;

	  if (cmin[j] > num_pixels/100) {
          acceptable = false;
	      if (dev->model->is_cis)
		  offl[0] = off[0];
	      else
		  offl[j] = off[j];
	  }
	  if (cmax[j] > num_pixels/100) {
          acceptable = false;
	      if (dev->model->is_cis)
		  offh[0] = off[0];
	      else
		  offh[j] = off[j];
	  }
      }

      DBG(DBG_info,"%s: black/white pixels: %d/%d,%d/%d,%d/%d\n", __func__, cmin[0], cmax[0],
          cmin[1], cmax[1], cmin[2], cmax[2]);

      if (dev->model->is_cis) {
	  offh[2] = offh[1] = offh[0];
	  offl[2] = offl[1] = offl[0];
      }

        gl841_stop_action(dev);

      turn++;
  } while (!acceptable && turn < 100);

  DBG(DBG_info,"%s: acceptable offsets: %d,%d,%d\n", __func__, off[0], off[1], off[2]);


  for (j = 0; j < channels; j++)
  {
      off1[j] = off[j];

      min1[j] = 65536;

      for (i = 0; i < num_pixels; i++)
      {
	  if (dev->model->is_cis)
	      val =
		  first_line[i * 2 + j * 2 * num_pixels + 1] * 256 +
		  first_line[i * 2 + j * 2 * num_pixels];
	  else
	      val =
		  first_line[i * 2 * channels + 2 * j + 1] * 256 +
		  first_line[i * 2 * channels + 2 * j];
	  if (min1[j] > val && val >= 10)
	      min1[j] = val;
      }
  }


  offl[0] = off[0];
  offl[1] = off[0];
  offl[2] = off[0];
  turn = 0;

  do {

      for (j=0; j < channels; j++) {
	  off[j] = (offh[j]+offl[j])/2;
          dev->frontend.set_offset(j, off[j]);
      }

        dev->cmd_set->set_fe(dev, calib_sensor, AFE_SET);

      DBG(DBG_info, "%s: starting second line reading\n", __func__);
        dev->write_registers(regs);
        dev->cmd_set->begin_scan(dev, calib_sensor, &regs, true);
        sanei_genesys_read_data_from_scanner(dev, second_line.data(), total_size);

      if (DBG_LEVEL >= DBG_data) {
          char fn[30];
          snprintf(fn, 30, "gl841_offset2_%02d.pnm", turn);
          sanei_genesys_write_pnm_file(fn, second_line.data(), 16, channels, num_pixels, 1);
      }

        acceptable = true;

      for (j = 0; j < channels; j++)
      {
	  cmin[j] = 0;
	  cmax[j] = 0;

	  for (i = 0; i < num_pixels; i++)
	  {
	      if (dev->model->is_cis)
		  val =
		      second_line[i * 2 + j * 2 * num_pixels + 1] * 256 +
		      second_line[i * 2 + j * 2 * num_pixels];
	      else
		  val =
		      second_line[i * 2 * channels + 2 * j + 1] * 256 +
		      second_line[i * 2 * channels + 2 * j];
	      if (val < 10)
		  cmin[j]++;
	      if (val > 65525)
		  cmax[j]++;
	  }

	  if (cmin[j] > num_pixels/100) {
            acceptable = false;
	      if (dev->model->is_cis)
		  offl[0] = off[0];
	      else
		  offl[j] = off[j];
	  }
	  if (cmax[j] > num_pixels/100) {
            acceptable = false;
	      if (dev->model->is_cis)
		  offh[0] = off[0];
	      else
		  offh[j] = off[j];
	  }
      }

      DBG(DBG_info, "%s: black/white pixels: %d/%d,%d/%d,%d/%d\n", __func__, cmin[0], cmax[0],
          cmin[1], cmax[1], cmin[2], cmax[2]);

      if (dev->model->is_cis) {
	  offh[2] = offh[1] = offh[0];
	  offl[2] = offl[1] = offl[0];
      }

        gl841_stop_action(dev);

      turn++;

  } while (!acceptable && turn < 100);

  DBG(DBG_info, "%s: acceptable offsets: %d,%d,%d\n", __func__, off[0], off[1], off[2]);


  for (j = 0; j < channels; j++)
  {
      off2[j] = off[j];

      min2[j] = 65536;

      for (i = 0; i < num_pixels; i++)
      {
	  if (dev->model->is_cis)
	      val =
		  second_line[i * 2 + j * 2 * num_pixels + 1] * 256 +
		  second_line[i * 2 + j * 2 * num_pixels];
	  else
	      val =
		  second_line[i * 2 * channels + 2 * j + 1] * 256 +
		  second_line[i * 2 * channels + 2 * j];
	  if (min2[j] > val && val != 0)
	      min2[j] = val;
      }
  }

  DBG(DBG_info, "%s: first set: %d/%d,%d/%d,%d/%d\n", __func__, off1[0], min1[0], off1[1], min1[1],
      off1[2], min1[2]);

  DBG(DBG_info, "%s: second set: %d/%d,%d/%d,%d/%d\n", __func__, off2[0], min2[0], off2[1], min2[1],
      off2[2], min2[2]);

/*
  calculate offset for each channel
  based on minimal pixel value min1 at offset off1 and minimal pixel value min2
  at offset off2

  to get min at off, values are linearly interpolated:
  min=real+off*fact
  min1=real+off1*fact
  min2=real+off2*fact

  fact=(min1-min2)/(off1-off2)
  real=min1-off1*(min1-min2)/(off1-off2)

  off=(min-min1+off1*(min1-min2)/(off1-off2))/((min1-min2)/(off1-off2))

  off=(min*(off1-off2)+min1*off2-off1*min2)/(min1-min2)

 */
  for (j = 0; j < channels; j++)
  {
      if (min2[j]-min1[j] == 0) {
/*TODO: try to avoid this*/
	  DBG(DBG_warn, "%s: difference too small\n", __func__);
	  if (mintgt * (off1[j] - off2[j]) + min1[j] * off2[j] - min2[j] * off1[j] >= 0)
	      off[j] = 0x0000;
	  else
	      off[j] = 0xffff;
      } else
	  off[j] = (mintgt * (off1[j] - off2[j]) + min1[j] * off2[j] - min2[j] * off1[j])/(min1[j]-min2[j]);
      if (off[j] > 255)
	  off[j] = 255;
      if (off[j] < 0)
	  off[j] = 0;
      dev->frontend.set_offset(j, off[j]);
  }

  DBG(DBG_info, "%s: final offsets: %d,%d,%d\n", __func__, off[0], off[1], off[2]);

  if (dev->model->is_cis) {
      if (off[0] < off[1])
	  off[0] = off[1];
      if (off[0] < off[2])
	  off[0] = off[2];
      dev->frontend.set_offset(0, off[0]);
      dev->frontend.set_offset(1, off[0]);
      dev->frontend.set_offset(2, off[0]);
  }

  if (channels == 1)
    {
      dev->frontend.set_offset(1, dev->frontend.get_offset(0));
      dev->frontend.set_offset(2, dev->frontend.get_offset(0));
    }
}


/* alternative coarse gain calibration
   this on uses the settings from offset_calibration and
   uses only one scanline
 */
/*
  with offset and coarse calibration we only want to get our input range into
  a reasonable shape. the fine calibration of the upper and lower bounds will
  be done with shading.
 */
void CommandSetGl841::coarse_gain_calibration(Genesys_Device* dev, const Genesys_Sensor& sensor,
                                              Genesys_Register_Set& regs, int dpi) const
{
    DBG_HELPER_ARGS(dbg, "dpi=%d", dpi);
  int num_pixels;
  int total_size;
  int i, j, channels;
  int max[3];
  float gain[3];
  int val;
  int lines=1;
  int move;

    // feed to white strip if needed
    if (dev->model->y_offset_calib_white > 0) {
        move = static_cast<int>(dev->model->y_offset_calib_white);
        move = static_cast<int>((move * (dev->motor.base_ydpi)) / MM_PER_INCH);
      DBG(DBG_io, "%s: move=%d lines\n", __func__, move);
        gl841_feed(dev, move);
    }

  /* coarse gain calibration is allways done in color mode */
  channels = 3;

    unsigned resolution = sensor.get_logical_hwdpi(dev->settings.xres);
    unsigned factor = sensor.optical_res / resolution;

    const auto& calib_sensor = sanei_genesys_find_sensor(dev, resolution, channels,
                                                         dev->settings.scan_method);

    num_pixels = calib_sensor.sensor_pixels / factor;

    ScanSession session;
    session.params.xres = resolution;
    session.params.yres = dev->settings.yres;
    session.params.startx = 0;
    session.params.starty = 0;
    session.params.pixels = num_pixels;
    session.params.lines = lines;
    session.params.depth = 16;
    session.params.channels = channels;
    session.params.scan_method = dev->settings.scan_method;
    session.params.scan_mode = ScanColorMode::COLOR_SINGLE_PASS;
    session.params.color_filter = dev->settings.color_filter;
    session.params.flags = SCAN_FLAG_DISABLE_SHADING |
                           SCAN_FLAG_DISABLE_GAMMA |
                           SCAN_FLAG_SINGLE_LINE |
                           SCAN_FLAG_IGNORE_LINE_DISTANCE;
    gl841_compute_session(dev, session, calib_sensor);

    gl841_init_scan_regs(dev, calib_sensor, &regs, session);

    dev->write_registers(regs);

  total_size = num_pixels * channels * 2 * lines;	/* colors * bytes_per_color * scan lines */

  std::vector<uint8_t> line(total_size);

    dev->cmd_set->begin_scan(dev, calib_sensor, &regs, true);
    sanei_genesys_read_data_from_scanner(dev, line.data(), total_size);

  if (DBG_LEVEL >= DBG_data)
    sanei_genesys_write_pnm_file("gl841_gain.pnm", line.data(), 16, channels, num_pixels, lines);

  /* average high level for each channel and compute gain
     to reach the target code
     we only use the central half of the CCD data         */
  for (j = 0; j < channels; j++)
    {
      max[j] = 0;
      for (i = 0; i < num_pixels; i++)
	{
	  if (dev->model->is_cis)
	      val =
		  line[i * 2 + j * 2 * num_pixels + 1] * 256 +
		  line[i * 2 + j * 2 * num_pixels];
	  else
	      val =
		  line[i * 2 * channels + 2 * j + 1] * 256 +
		  line[i * 2 * channels + 2 * j];

	  if (val > max[j])
	    max[j] = val;
	}

        gain[j] = 65535.0f / max[j];

      uint8_t out_gain = 0;

        if (dev->model->adc_id == AdcId::CANON_LIDE_35 ||
            dev->model->adc_id == AdcId::WOLFSON_XP300 ||
            dev->model->adc_id == AdcId::WOLFSON_DSM600)
        {
        gain[j] *= 0.69f; // seems we don't get the real maximum. empirically derived
	  if (283 - 208/gain[j] > 255)
              out_gain = 255;
	  else if (283 - 208/gain[j] < 0)
              out_gain = 0;
	  else
              out_gain = static_cast<std::uint8_t>(283 - 208 / gain[j]);
        } else if (dev->model->adc_id == AdcId::CANON_LIDE_80) {
              out_gain = static_cast<std::uint8_t>(gain[j] * 12);
        }
      dev->frontend.set_gain(j, out_gain);

      DBG(DBG_proc, "%s: channel %d, max=%d, gain = %f, setting:%d\n", __func__, j, max[j], gain[j],
          out_gain);
    }

  for (j = 0; j < channels; j++)
    {
      if(gain[j] > 10)
        {
	  DBG (DBG_error0, "**********************************************\n");
	  DBG (DBG_error0, "**********************************************\n");
	  DBG (DBG_error0, "****                                      ****\n");
	  DBG (DBG_error0, "****  Extremely low Brightness detected.  ****\n");
	  DBG (DBG_error0, "****  Check the scanning head is          ****\n");
	  DBG (DBG_error0, "****  unlocked and moving.                ****\n");
	  DBG (DBG_error0, "****                                      ****\n");
	  DBG (DBG_error0, "**********************************************\n");
	  DBG (DBG_error0, "**********************************************\n");
            throw SaneException(SANE_STATUS_JAMMED, "scanning head is locked");
        }

    }

    if (dev->model->is_cis) {
        uint8_t gain0 = dev->frontend.get_gain(0);
        if (gain0 > dev->frontend.get_gain(1)) {
            gain0 = dev->frontend.get_gain(1);
        }
        if (gain0 > dev->frontend.get_gain(2)) {
            gain0 = dev->frontend.get_gain(2);
        }
        dev->frontend.set_gain(0, gain0);
        dev->frontend.set_gain(1, gain0);
        dev->frontend.set_gain(2, gain0);
    }

    if (channels == 1) {
        dev->frontend.set_gain(0, dev->frontend.get_gain(1));
        dev->frontend.set_gain(2, dev->frontend.get_gain(1));
    }

  DBG(DBG_info, "%s: gain=(%d,%d,%d)\n", __func__,
      dev->frontend.get_gain(0),
      dev->frontend.get_gain(1),
      dev->frontend.get_gain(2));

    gl841_stop_action(dev);

    dev->cmd_set->slow_back_home(dev, true);
}

// wait for lamp warmup by scanning the same line until difference
// between 2 scans is below a threshold
void CommandSetGl841::init_regs_for_warmup(Genesys_Device* dev, const Genesys_Sensor& sensor,
                                           Genesys_Register_Set* local_reg, int* channels,
                                           int* total_size) const
{
    DBG_HELPER(dbg);
    int num_pixels = 4 * 300;
  *local_reg = dev->reg;

/* okay.. these should be defaults stored somewhere */
  dev->frontend.set_gain(0, 0);
  dev->frontend.set_gain(1, 0);
  dev->frontend.set_gain(2, 0);
  dev->frontend.set_offset(0, 0x80);
  dev->frontend.set_offset(1, 0x80);
  dev->frontend.set_offset(2, 0x80);

    ScanSession session;
    session.params.xres = sensor.optical_res;
    session.params.yres = dev->settings.yres;
    session.params.startx = sensor.dummy_pixel;
    session.params.starty = 0;
    session.params.pixels = num_pixels;
    session.params.lines = 1;
    session.params.depth = 16;
    session.params.channels = *channels;
    session.params.scan_method = dev->settings.scan_method;
    if (*channels == 3) {
        session.params.scan_mode = ScanColorMode::COLOR_SINGLE_PASS;
    } else {
        session.params.scan_mode = ScanColorMode::GRAY;
    }
    session.params.color_filter = dev->settings.color_filter;
    session.params.flags = SCAN_FLAG_DISABLE_SHADING |
                           SCAN_FLAG_DISABLE_GAMMA |
                           SCAN_FLAG_SINGLE_LINE |
                           SCAN_FLAG_IGNORE_LINE_DISTANCE;
    gl841_compute_session(dev, session, sensor);

    gl841_init_scan_regs(dev, sensor, local_reg, session);

  num_pixels = dev->current_setup.pixels;

  *total_size = num_pixels * 3 * 2 * 1;	/* colors * bytes_per_color * scan lines */

    dev->write_registers(*local_reg);
}


/*
 * this function moves head without scanning, forward, then backward
 * so that the head goes to park position.
 * as a by-product, also check for lock
 */
static void sanei_gl841_repark_head(Genesys_Device* dev)
{
    DBG_HELPER(dbg);

    gl841_feed(dev,232);

    // toggle motor flag, put an huge step number and redo move backward
    dev->cmd_set->slow_back_home(dev, true);
}

bool CommandSetGl841::is_compatible_calibration(Genesys_Device* dev, const Genesys_Sensor& sensor,
                                                Genesys_Calibration_Cache* cache,
                                                bool for_overwrite) const
{
#ifdef HAVE_SYS_TIME_H
  struct timeval time;
#endif

    DBG_HELPER(dbg);
  /* calibration cache not working yet for this model */
    if (dev->model->sensor_id == SensorId::CCD_PLUSTEK_OPTICPRO_3600) {
      return false;
    }

    calculate_current_setup(dev, sensor);

  if (dev->current_setup.ccd_size_divisor != cache->used_setup.ccd_size_divisor)
    return false;

  /* a cache entry expires after 30 minutes for non sheetfed scanners */
  /* this is not taken into account when overwriting cache entries    */
#ifdef HAVE_SYS_TIME_H
    if (!for_overwrite) {
      gettimeofday (&time, nullptr);
        if ((time.tv_sec - cache->last_calibration > 30 * 60) && (!dev->model->is_sheetfed))
        {
          DBG(DBG_proc, "%s: expired entry, non compatible cache\n", __func__);
          return false;
        }
    }
#endif

  return true;
}

/*
 * initialize ASIC : registers, motor tables, and gamma tables
 * then ensure scanner's head is at home
 */
void CommandSetGl841::init(Genesys_Device* dev) const
{
  uint8_t val;
  size_t size;

  DBG_INIT ();
    DBG_HELPER(dbg);

  dev->scanhead_position_in_steps = 0;

  /* Check if the device has already been initialized and powered up */
  if (dev->already_initialized)
    {
        val = sanei_genesys_get_status(dev);
      if (val & REG41_PWRBIT)
	{
	  DBG(DBG_info, "%s: already initialized\n", __func__);
      return;
	}
    }

  dev->dark_average_data.clear();
  dev->white_average_data.clear();

  dev->settings.color_filter = ColorFilter::RED;

    // ASIC reset
    dev->write_register(0x0e, 0x01);
    dev->write_register(0x0e, 0x00);

  /* Set default values for registers */
  gl841_init_registers (dev);

    // Write initial registers
    dev->write_registers(dev->reg);

    if (dev->model->flags & GENESYS_FLAG_TEST_ON_INIT) {
        sanei_gl841_asic_test(dev);
    }

  const auto& sensor = sanei_genesys_find_sensor_any(dev);

    // Set analog frontend
    dev->cmd_set->set_fe(dev, sensor, AFE_INIT);

    // FIXME: slow_back_home modifies dev->calib_reg and requires it to be filled
    dev->calib_reg = dev->reg;

    // Move home
    dev->cmd_set->slow_back_home(dev, true);

    // Init shading data
    sanei_genesys_init_shading_data(dev, sensor, sensor.sensor_pixels);

  /* ensure head is correctly parked, and check lock */
  if (dev->model->flags & GENESYS_FLAG_REPARK)
    {
        // FIXME: if repark fails, we should print an error message that the scanner is locked and
        // the user should unlock the lock. We should also rethrow with SANE_STATUS_JAMMED
        sanei_gl841_repark_head(dev);
    }

    // send gamma tables
    dev->cmd_set->send_gamma_table(dev, sensor);

  /* initial calibration reg values */
  Genesys_Register_Set& regs = dev->calib_reg;
  regs = dev->reg;

    unsigned resolution = sensor.get_logical_hwdpi(300);
    unsigned factor = sensor.optical_res / resolution;

    const auto& calib_sensor = sanei_genesys_find_sensor(dev, resolution, 3,
                                                         dev->settings.scan_method);

    unsigned num_pixels = 16 / factor;

    ScanSession session;
    session.params.xres = resolution;
    session.params.yres = 300;
    session.params.startx = 0;
    session.params.starty = 0;
    session.params.pixels = num_pixels;
    session.params.lines = 1;
    session.params.depth = 16;
    session.params.channels = 3;
    session.params.scan_method = dev->settings.scan_method;
    session.params.scan_mode = ScanColorMode::COLOR_SINGLE_PASS;
    session.params.color_filter = ColorFilter::RED;
    session.params.flags = SCAN_FLAG_DISABLE_SHADING |
                           SCAN_FLAG_DISABLE_GAMMA |
                           SCAN_FLAG_SINGLE_LINE |
                           SCAN_FLAG_IGNORE_LINE_DISTANCE;
    gl841_compute_session(dev, session, calib_sensor);

    gl841_init_scan_regs(dev, calib_sensor, &regs, session);

    dev->write_registers(regs);

    size = num_pixels * 3 * 2 * 1; // colors * bytes_per_color * scan lines

  std::vector<uint8_t> line(size);

  DBG(DBG_info, "%s: starting dummy data reading\n", __func__);
    dev->cmd_set->begin_scan(dev, calib_sensor, &regs, true);

  sanei_usb_set_timeout(1000);/* 1 second*/

    // ignore errors. next read will succeed
    sanei_genesys_read_data_from_scanner(dev, line.data(), size);

  sanei_usb_set_timeout(30 * 1000);/* 30 seconds*/

    end_scan(dev, &regs, true);

  regs = dev->reg;

    // Set powersaving(default = 15 minutes)
    set_powersaving(dev, 15);
    dev->already_initialized = true;
}

void CommandSetGl841::update_hardware_sensors(Genesys_Scanner* s) const
{
    DBG_HELPER(dbg);
  /* do what is needed to get a new set of events, but try to not lose
     any of them.
   */
  uint8_t val;

    if (s->dev->model->gpio_id == GpioId::CANON_LIDE_35
        || s->dev->model->gpio_id == GpioId::CANON_LIDE_80)
    {
        val = s->dev->read_register(REG6D);
        s->buttons[BUTTON_SCAN_SW].write((val & 0x01) == 0);
        s->buttons[BUTTON_FILE_SW].write((val & 0x02) == 0);
        s->buttons[BUTTON_EMAIL_SW].write((val & 0x04) == 0);
        s->buttons[BUTTON_COPY_SW].write((val & 0x08) == 0);
    }

    if (s->dev->model->gpio_id == GpioId::XP300 ||
        s->dev->model->gpio_id == GpioId::DP665 ||
        s->dev->model->gpio_id == GpioId::DP685)
    {
        val = s->dev->read_register(REG6D);

        s->buttons[BUTTON_PAGE_LOADED_SW].write((val & 0x01) == 0);
        s->buttons[BUTTON_SCAN_SW].write((val & 0x02) == 0);
    }
}

/** @brief search for a full width black or white strip.
 * This function searches for a black or white stripe across the scanning area.
 * When searching backward, the searched area must completely be of the desired
 * color since this area will be used for calibration which scans forward.
 * @param dev scanner device
 * @param forward true if searching forward, false if searching backward
 * @param black true if searching for a black strip, false for a white strip
 */
void CommandSetGl841::search_strip(Genesys_Device* dev, const Genesys_Sensor& sensor, bool forward,
                                   bool black) const
{
    DBG_HELPER_ARGS(dbg, "%s %s", black ? "black" : "white", forward ? "forward" : "reverse");
  unsigned int pixels, lines, channels;
  Genesys_Register_Set local_reg;
  size_t size;
  unsigned int pass, count, found, x, y, length;
  char title[80];
  GenesysRegister *r;
  uint8_t white_level=90;  /**< default white level to detect white dots */
  uint8_t black_level=60;  /**< default black level to detect black dots */

  /* use maximum gain when doing forward white strip detection
   * since we don't have calibrated the sensor yet */
  if(!black && forward)
    {
      dev->frontend.set_gain(0, 0xff);
      dev->frontend.set_gain(1, 0xff);
      dev->frontend.set_gain(2, 0xff);
    }

    dev->cmd_set->set_fe(dev, sensor, AFE_SET);
    gl841_stop_action(dev);

    // set up for a gray scan at lowest dpi
    unsigned dpi = *std::min_element(dev->model->xdpi_values.begin(),
                                     dev->model->xdpi_values.end());
  channels = 1;

  /* shading calibation is done with dev->motor.base_ydpi */
  /* lines = (dev->model->shading_lines * dpi) / dev->motor.base_ydpi; */
    lines = static_cast<unsigned>((10 * dpi) / MM_PER_INCH);

  pixels = (sensor.sensor_pixels * dpi) / sensor.optical_res;

  /* 20 cm max length for calibration sheet */
    length = static_cast<unsigned>(((200 * dpi) / MM_PER_INCH) / lines);

  dev->scanhead_position_in_steps = 0;

  local_reg = dev->reg;

    ScanSession session;
    session.params.xres = dpi;
    session.params.yres = dpi;
    session.params.startx = 0;
    session.params.starty = 0;
    session.params.pixels = pixels;
    session.params.lines = lines;
    session.params.depth = 8;
    session.params.channels = channels;
    session.params.scan_method = dev->settings.scan_method;
    session.params.scan_mode = ScanColorMode::GRAY;
    session.params.color_filter = ColorFilter::RED;
    session.params.flags = SCAN_FLAG_DISABLE_SHADING | SCAN_FLAG_DISABLE_GAMMA;
    gl841_compute_session(dev, session, sensor);

    size = pixels * channels * lines * (session.params.depth / 8);
    std::vector<uint8_t> data(size);

    gl841_init_scan_regs(dev, sensor, &local_reg, session);

  /* set up for reverse or forward */
  r = sanei_genesys_get_address(&local_reg, 0x02);
    if (forward) {
        r->value &= ~4;
    } else {
        r->value |= 4;
    }

    dev->write_registers(local_reg);

    dev->cmd_set->begin_scan(dev, sensor, &local_reg, true);

    // waits for valid data
    wait_until_buffer_non_empty(dev);

    // now we're on target, we can read data
    sanei_genesys_read_data_from_scanner(dev, data.data(), size);

    gl841_stop_action(dev);

  pass = 0;
  if (DBG_LEVEL >= DBG_data)
    {
      sprintf(title, "gl841_search_strip_%s_%s%02u.pnm", black ? "black" : "white",
              forward ? "fwd" : "bwd", pass);
      sanei_genesys_write_pnm_file(title, data.data(), session.params.depth,
                                   channels, pixels, lines);
    }

  /* loop until strip is found or maximum pass number done */
  found = 0;
  while (pass < length && !found)
    {
        dev->write_registers(local_reg);

        //now start scan
        dev->cmd_set->begin_scan(dev, sensor, &local_reg, true);

        // waits for valid data
        wait_until_buffer_non_empty(dev);

        // now we're on target, we can read data
        sanei_genesys_read_data_from_scanner(dev, data.data(), size);

        gl841_stop_action (dev);

      if (DBG_LEVEL >= DBG_data)
	{
          sprintf(title, "gl841_search_strip_%s_%s%02u.pnm",
                  black ? "black" : "white", forward ? "fwd" : "bwd", pass);
          sanei_genesys_write_pnm_file(title, data.data(), session.params.depth,
                                       channels, pixels, lines);
	}

      /* search data to find black strip */
      /* when searching forward, we only need one line of the searched color since we
       * will scan forward. But when doing backward search, we need all the area of the
       * same color */
      if (forward)
	{
	  for (y = 0; y < lines && !found; y++)
	    {
	      count = 0;
	      /* count of white/black pixels depending on the color searched */
	      for (x = 0; x < pixels; x++)
		{
		  /* when searching for black, detect white pixels */
		  if (black && data[y * pixels + x] > white_level)
		    {
		      count++;
		    }
		  /* when searching for white, detect black pixels */
		  if (!black && data[y * pixels + x] < black_level)
		    {
		      count++;
		    }
		}

	      /* at end of line, if count >= 3%, line is not fully of the desired color
	       * so we must go to next line of the buffer */
	      /* count*100/pixels < 3 */
	      if ((count * 100) / pixels < 3)
		{
		  found = 1;
		  DBG(DBG_data, "%s: strip found forward during pass %d at line %d\n", __func__,
		      pass, y);
		}
	      else
		{
		  DBG(DBG_data, "%s: pixels=%d, count=%d (%d%%)\n", __func__, pixels, count,
		      (100 * count) / pixels);
		}
	    }
	}
      else			/* since calibration scans are done forward, we need the whole area
				   to be of the required color when searching backward */
	{
	  count = 0;
	  for (y = 0; y < lines; y++)
	    {
	      /* count of white/black pixels depending on the color searched */
	      for (x = 0; x < pixels; x++)
		{
		  /* when searching for black, detect white pixels */
		  if (black && data[y * pixels + x] > white_level)
		    {
		      count++;
		    }
		  /* when searching for white, detect black pixels */
		  if (!black && data[y * pixels + x] < black_level)
		    {
		      count++;
		    }
		}
	    }

	  /* at end of area, if count >= 3%, area is not fully of the desired color
	   * so we must go to next buffer */
	  if ((count * 100) / (pixels * lines) < 3)
	    {
	      found = 1;
	      DBG(DBG_data, "%s: strip found backward during pass %d \n", __func__, pass);
	    }
	  else
	    {
	      DBG(DBG_data, "%s: pixels=%d, count=%d (%d%%)\n", __func__, pixels, count,
		  (100 * count) / pixels);
	    }
	}
      pass++;
    }

  if (found)
    {
      DBG(DBG_info, "%s: %s strip found\n", __func__, black ? "black" : "white");
    }
  else
    {
        throw SaneException(SANE_STATUS_UNSUPPORTED, "%s strip not found", black ? "black" : "white");
    }
}

/**
 * Send shading calibration data. The buffer is considered to always hold values
 * for all the channels.
 */
void CommandSetGl841::send_shading_data(Genesys_Device* dev, const Genesys_Sensor& sensor,
                                        uint8_t* data, int size) const
{
    DBG_HELPER_ARGS(dbg, "writing %d bytes of shading data", size);
  uint32_t length, x, factor, pixels, i;
    uint16_t dpiset, dpihw, beginpixel;
  uint8_t *ptr,*src;

  /* old method if no SHDAREA */
  if((dev->reg.find_reg(0x01).value & REG01_SHDAREA) == 0)
    {
        // start address
        sanei_genesys_set_buffer_address(dev, 0x0000);

        // shading data whole line
        dev->cmd_set->bulk_write_data(dev, 0x3c, data, size);
        return;
    }

  /* data is whole line, we extract only the part for the scanned area */
    length = static_cast<std::uint32_t>(size / 3);
    unsigned strpixel = dev->session.pixel_startx;
    unsigned endpixel = dev->session.pixel_endx;

  /* compute deletion/average factor */
    dpiset = dev->reg.get16(REG_DPISET);
  dpihw = gl841_get_dpihw(dev);
  unsigned ccd_size_divisor = dev->current_setup.ccd_size_divisor;
  factor=dpihw/dpiset;
  DBG(DBG_io2, "%s: dpihw=%d, dpiset=%d, ccd_size_divisor=%d, factor=%d\n", __func__, dpihw, dpiset,
      ccd_size_divisor, factor);

  /* turn pixel value into bytes 2x16 bits words */
  strpixel*=2*2; /* 2 words of 2 bytes */
  endpixel*=2*2;
  pixels=endpixel-strpixel;

  /* shading pixel begin is start pixel minus start pixel during shading
   * calibration. Currently only cases handled are full and half ccd resolution.
   */
    beginpixel = sensor.ccd_start_xoffset / ccd_size_divisor;
  beginpixel += sensor.dummy_pixel + 1;
  DBG(DBG_io2, "%s: ORIGIN PIXEL=%d\n", __func__, beginpixel);
  beginpixel = (strpixel-beginpixel*2*2)/factor;
  DBG(DBG_io2, "%s: BEGIN PIXEL=%d\n", __func__, beginpixel/4);

  DBG(DBG_io2, "%s: using chunks of %d bytes (%d shading data pixels)\n", __func__, length,
      length/4);
  std::vector<uint8_t> buffer(pixels, 0);

  /* write actual shading data contigously
   * channel by channel, starting at addr 0x0000
   * */
  for(i=0;i<3;i++)
    {
      /* copy data to work buffer and process it */
          /* coefficent destination */
      ptr=buffer.data();

      /* iterate on both sensor segment, data has been averaged,
       * so is in the right order and we only have to copy it */
      for(x=0;x<pixels;x+=4)
        {
          /* coefficient source */
          src=data+x+beginpixel+i*length;
          ptr[0]=src[0];
          ptr[1]=src[1];
          ptr[2]=src[2];
          ptr[3]=src[3];

          /* next shading coefficient */
          ptr+=4;
        }

        // 0x5400 alignment for LIDE80 internal memory
        sanei_genesys_set_buffer_address(dev, 0x5400*i);
        dev->cmd_set->bulk_write_data(dev, 0x3c, buffer.data(), pixels);
    }
}

bool CommandSetGl841::needs_home_before_init_regs_for_scan(Genesys_Device* dev) const
{
    (void) dev;
    return true;
}

void CommandSetGl841::wait_for_motor_stop(Genesys_Device* dev) const
{
    (void) dev;
}

void CommandSetGl841::rewind(Genesys_Device* dev) const
{
    (void) dev;
    throw SaneException("not implemented");
}

void CommandSetGl841::bulk_write_data(Genesys_Device* dev, uint8_t addr, uint8_t* data,
                                      size_t len) const
{
    sanei_genesys_bulk_write_data(dev, addr, data, len);
}

void CommandSetGl841::bulk_read_data(Genesys_Device* dev, uint8_t addr, uint8_t* data,
                                     size_t len) const
{
    sanei_genesys_bulk_read_data(dev, addr, data, len);
}

void CommandSetGl841::move_to_ta(Genesys_Device* dev) const
{
    (void) dev;
    throw SaneException("not implemented");
}

void CommandSetGl841::asic_boot(Genesys_Device *dev, bool cold) const
{
    (void) dev;
    (void) cold;
    throw SaneException("not implemented");
}

std::unique_ptr<CommandSet> create_gl841_cmd_set()
{
    return std::unique_ptr<CommandSet>(new CommandSetGl841{});
}

} // namespace genesys