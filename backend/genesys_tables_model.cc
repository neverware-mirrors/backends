/* sane - Scanner Access Now Easy.

   Copyright (C) 2003 Oliver Rauch
   Copyright (C) 2003-2005 Henning Meier-Geinitz <henning@meier-geinitz.de>
   Copyright (C) 2004, 2005 Gerhard Jaeger <gerhard@gjaeger.de>
   Copyright (C) 2004-2013 Stéphane Voltz <stef.dev@free.fr>
   Copyright (C) 2005-2009 Pierre Willenbrock <pierre@pirsoft.dnsalias.org>
   Copyright (C) 2007 Luke <iceyfor@gmail.com>
   Copyright (C) 2010 Jack McGill <jmcgill85258@yahoo.com>
   Copyright (C) 2010 Andrey Loginov <avloginov@gmail.com>,
                   xerox travelscan device entry
   Copyright (C) 2010 Chris Berry <s0457957@sms.ed.ac.uk> and Michael Rickmann <mrickma@gwdg.de>
                 for Plustek Opticbook 3600 support
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

#define DEBUG_DECLARE_ONLY

#include "genesys_low.h"

StaticInit<std::vector<Genesys_USB_Device_Entry>> s_usb_devices;

void genesys_init_usb_device_tables()
{
    s_usb_devices.init();

    Genesys_Model model;
    model.name = "umax-astra-4500";
    model.vendor = "UMAX";
    model.model = "Astra 4500";
    model.model_id = MODEL_UMAX_ASTRA_4500;
    model.asic_type = AsicType::GL646;

    model.xdpi_values = { 1200, 600, 300, 150, 75 };
    model.ydpi_values = { 2400, 1200, 600, 300, 150, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(3.5);
    model.y_offset = SANE_FIX(7.5);
    model.x_size = SANE_FIX(218.0);
    model.y_size = SANE_FIX(299.0);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(1.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 8;
    model.ld_shift_b = 16;

    model.line_mode_color_order = ColorOrder::BGR;

    model.is_cis = SANE_FALSE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CCD_UMAX;
    model.dac_type = DAC_WOLFSON_UMAX;
    model.gpo_type = GPO_UMAX;
    model.motor_type = MOTOR_UMAX;
    model.flags = GENESYS_FLAG_UNTESTED;
    model.buttons = GENESYS_HAS_NO_BUTTONS;
    model.shading_lines = 20;
    model.shading_ta_lines = 0;
    model.search_lines = 200;

    s_usb_devices->emplace_back(0x0638, 0x0a10, model);


    model = Genesys_Model();
    model.name = "canon-lide-50";
    model.vendor = "Canon";
    model.model = "LiDE 35/40/50";
    model.model_id = MODEL_CANON_LIDE_50;
    model.asic_type = AsicType::GL841;

    model.xdpi_values = { 1200, 600, 400, 300, 240, 200, 150, 75 };
    model.ydpi_values = { 2400, 1200, 600, 400, 300, 240, 200, 150, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(0.42);
    model.y_offset = SANE_FIX(7.9);
    model.x_size = SANE_FIX(218.0);
    model.y_size = SANE_FIX(299.0);

    model.y_offset_calib_white = SANE_FIX(6.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CCD_CANONLIDE35;
    model.dac_type = DAC_CANONLIDE35;
    model.gpo_type = GPO_CANONLIDE35;
    model.motor_type = MOTOR_CANONLIDE35;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_DARK_WHITE_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_SCAN_SW |
                    GENESYS_HAS_FILE_SW |
                    GENESYS_HAS_EMAIL_SW |
                    GENESYS_HAS_COPY_SW;
    model.shading_lines = 280;
    model.shading_ta_lines = 0;
    model.search_lines = 400;

    s_usb_devices->emplace_back(0x04a9, 0x2213, model);


    model = Genesys_Model();
    model.name = "panasonic-kv-ss080";
    model.vendor = "Panasonic";
    model.model = "KV-SS080";
    model.model_id = MODEL_PANASONIC_KV_SS080;
    model.asic_type = AsicType::GL843;

    model.xdpi_values = { 600, /* 500, 400,*/ 300, 200, 150, 100, 75 };
    model.ydpi_values = { 1200, 600, /* 500, 400, */ 300, 200, 150, 100, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(7.2);
    model.y_offset = SANE_FIX(14.7);
    model.x_size = SANE_FIX(217.7);
    model.y_size = SANE_FIX(300.0);

    model.y_offset_calib_white = SANE_FIX(9.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(0.0);
    model.y_size_ta = SANE_FIX(0.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 8;
    model.ld_shift_b = 16;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_FALSE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CCD_KVSS080;
    model.dac_type = DAC_KVSS080;
    model.gpo_type = GPO_KVSS080;
    model.motor_type = MOTOR_KVSS080;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_SCAN_SW;
    model.shading_lines = 100;
    model.shading_ta_lines = 0;
    model.search_lines = 100;

    s_usb_devices->emplace_back(0x04da, 0x100f, model);


    model = Genesys_Model();
    model.name = "hewlett-packard-scanjet-4850c";
    model.vendor = "Hewlett Packard";
    model.model = "ScanJet 4850C";
    model.model_id = MODEL_HP_SCANJET_4850C;
    model.asic_type = AsicType::GL843;

    model.xdpi_values = { 2400, 1200, 600, 400, 300, 200, 150, 100 };
    model.ydpi_values = { 2400, 1200, 600, 400, 300, 200, 150, 100 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(7.9);
    model.y_offset = SANE_FIX(5.9);
    model.x_size = SANE_FIX(219.6);
    model.y_size = SANE_FIX(314.5);

    model.y_offset_calib_white = SANE_FIX(3.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 24;
    model.ld_shift_b = 48;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_FALSE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CCD_G4050;
    model.dac_type = DAC_G4050;
    model.gpo_type = GPO_G4050;
    model.motor_type = MOTOR_G4050;
    model.flags = GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_STAGGERED_LINE |
                  GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_SCAN_SW | GENESYS_HAS_FILE_SW | GENESYS_HAS_COPY_SW;
    model.shading_lines = 100;
    model.shading_ta_lines = 0;
    model.search_lines = 100;
    s_usb_devices->emplace_back(0x03f0, 0x1b05, model);


    model = Genesys_Model();
    model.name = "hewlett-packard-scanjet-g4010";
    model.vendor = "Hewlett Packard";
    model.model = "ScanJet G4010";
    model.model_id = MODEL_HP_SCANJET_G4010;
    model.asic_type = AsicType::GL843;

    model.xdpi_values = { 2400, 1200, 600, 400, 300, 200, 150, 100 };
    model.ydpi_values = { 2400, 1200, 600, 400, 300, 200, 150, 100 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(8.0);
    model.y_offset = SANE_FIX(13.00);
    model.x_size = SANE_FIX(217.9);
    model.y_size = SANE_FIX(315.0);

    model.y_offset_calib_white = SANE_FIX(3.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 24;
    model.ld_shift_b = 48;
    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_FALSE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CCD_G4050;
    model.dac_type = DAC_G4050;
    model.gpo_type = GPO_G4050;
    model.motor_type = MOTOR_G4050;
    model.flags = GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_STAGGERED_LINE |
                  GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_SCAN_SW | GENESYS_HAS_FILE_SW | GENESYS_HAS_COPY_SW;
    model.shading_lines = 100;
    model.shading_ta_lines = 0;
    model.search_lines = 100;

    s_usb_devices->emplace_back(0x03f0, 0x4505, model);


    model = Genesys_Model();
    model.name = "hewlett-packard-scanjet-g4050";
    model.vendor = "Hewlett Packard";
    model.model = "ScanJet G4050";
    model.model_id = MODEL_HP_SCANJET_G4050;
    model.asic_type = AsicType::GL843;

    model.xdpi_values = { 2400, 1200, 600, 400, 300, 200, 150, 100 };
    model.ydpi_values = { 2400, 1200, 600, 400, 300, 200, 150, 100 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(8.0);
    model.y_offset = SANE_FIX(13.00);
    model.x_size = SANE_FIX(217.9);
    model.y_size = SANE_FIX(315.0);

    model.y_offset_calib_white = SANE_FIX(3.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(8.0);
    model.y_offset_ta = SANE_FIX(13.00);
    model.x_size_ta = SANE_FIX(217.9);
    model.y_size_ta = SANE_FIX(250.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(40.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 24;
    model.ld_shift_b = 48;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_FALSE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CCD_G4050;
    model.dac_type = DAC_G4050;
    model.gpo_type = GPO_G4050;
    model.motor_type = MOTOR_G4050;
    model.flags = GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_STAGGERED_LINE |
                  GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_SCAN_SW | GENESYS_HAS_FILE_SW | GENESYS_HAS_COPY_SW;
    model.shading_lines = 100;
    model.shading_ta_lines = 0;
    model.search_lines = 100;

    s_usb_devices->emplace_back(0x03f0, 0x4605, model);


    model = Genesys_Model();
    model.name = "canon-canoscan-4400f";
    model.vendor = "Canon";
    model.model = "Canoscan 4400f";
    model.model_id = MODEL_CANON_CANOSCAN_4400F;
    model.asic_type = AsicType::GL843;

    model.xdpi_values = { 4800, 2400, 1200, 600, 400, 300, 200, 150, 100 };
    model.ydpi_values = { 4800, 2400, 1200, 600, 400, 300, 200, 150, 100 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(6.0);
    model.y_offset = SANE_FIX(13.00);
    model.x_size = SANE_FIX(217.9);
    model.y_size = SANE_FIX(315.0);

    model.y_offset_calib_white = SANE_FIX(3.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(8.0);
    model.y_offset_ta = SANE_FIX(13.00);
    model.x_size_ta = SANE_FIX(217.9);
    model.y_size_ta = SANE_FIX(250.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(40.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 24;
    model.ld_shift_b = 48;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_FALSE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CCD_CS4400F;
    model.dac_type = DAC_G4050;
    model.gpo_type = GPO_CS4400F;
    model.motor_type = MOTOR_G4050;
    model.flags = GENESYS_FLAG_NO_CALIBRATION |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_STAGGERED_LINE |
                  GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_FULL_HWDPI_MODE |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_SCAN_SW | GENESYS_HAS_FILE_SW | GENESYS_HAS_COPY_SW;
    model.shading_lines = 100;
    model.shading_ta_lines = 0;
    model.search_lines = 100;

    s_usb_devices->emplace_back(0x04a9, 0x2228, model);


    model = Genesys_Model();
    model.name = "canon-canoscan-8400f";
    model.vendor = "Canon";
    model.model = "Canoscan 8400f";
    model.model_id = MODEL_CANON_CANOSCAN_8400F;
    model.asic_type = AsicType::GL843;

    model.xdpi_values = { 4800, 2400, 1200, 600, 400, 300, 200, 150, 100 };
    model.ydpi_values = { 4800, 2400, 1200, 600, 400, 300, 200, 150, 100 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(3.5);
    model.y_offset = SANE_FIX(17.00);
    model.x_size = SANE_FIX(219.9);
    model.y_size = SANE_FIX(315.0);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(10.0);

    model.x_offset_ta = SANE_FIX(100.0);
    model.y_offset_ta = SANE_FIX(50.00);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(230.0);

    model.y_offset_sensor_to_ta = SANE_FIX(22.0);
    model.y_offset_calib_white_ta = SANE_FIX(25.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 24;
    model.ld_shift_b = 48;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_FALSE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CCD_CS8400F;
    model.dac_type = DAC_CS8400F;
    model.gpo_type = GPO_CS8400F;
    model.motor_type = MOTOR_CS8400F;
    model.flags = GENESYS_FLAG_HAS_UTA |
                  GENESYS_FLAG_HAS_UTA_INFRARED |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_STAGGERED_LINE |
                  GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_FULL_HWDPI_MODE |
                  GENESYS_FLAG_CUSTOM_GAMMA |
                  GENESYS_FLAG_SHADING_REPARK;
    model.buttons = GENESYS_HAS_SCAN_SW | GENESYS_HAS_FILE_SW | GENESYS_HAS_COPY_SW;
    model.shading_lines = 100;
    model.shading_ta_lines = 50;
    model.search_lines = 100;

    s_usb_devices->emplace_back(0x04a9, 0x221e, model);


    model = Genesys_Model();
    model.name = "canon-canoscan-8600f";
    model.vendor = "Canon";
    model.model = "Canoscan 8600f";
    model.model_id = MODEL_CANON_CANOSCAN_8600F;
    model.asic_type = AsicType::GL843;

    model.xdpi_values = { 4800, 2400, 1200, 600, 400, 300 }; // TODO: resolutions for non-XPA mode
    model.ydpi_values = { 4800, 2400, 1200, 600, 400, 300 }; // TODO: resolutions for non-XPA mode
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(24.0);
    model.y_offset = SANE_FIX(10.0);
    model.x_size = SANE_FIX(216.0);
    model.y_size = SANE_FIX(297.0);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(8.0);

    model.x_offset_ta = SANE_FIX(85.0);
    model.y_offset_ta = SANE_FIX(26.0);
    model.x_size_ta = SANE_FIX(70.0);
    model.y_size_ta = SANE_FIX(230.0);

    model.y_offset_sensor_to_ta = SANE_FIX(11.5);
    model.y_offset_calib_white_ta = SANE_FIX(14.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 48;
    model.ld_shift_b = 96;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_FALSE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CCD_CS8600F;
    model.dac_type = DAC_CS8600F;
    model.gpo_type = GPO_CS8600F;
    model.motor_type = MOTOR_CS8600F;
    model.flags = GENESYS_FLAG_HAS_UTA |
                  GENESYS_FLAG_HAS_UTA_INFRARED |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_STAGGERED_LINE |
                  GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_FULL_HWDPI_MODE |
                  GENESYS_FLAG_CUSTOM_GAMMA |
                  GENESYS_FLAG_SHADING_REPARK;
    model.buttons = GENESYS_HAS_SCAN_SW | GENESYS_HAS_FILE_SW | GENESYS_HAS_COPY_SW;
    model.shading_lines = 50;
    model.shading_ta_lines = 50;
    model.search_lines = 100;

    s_usb_devices->emplace_back(0x04a9, 0x2229, model);


    model = Genesys_Model();
    model.name = "canon-lide-100";
    model.vendor = "Canon";
    model.model = "LiDE 100";
    model.model_id = MODEL_CANON_LIDE_100;
    model.asic_type = AsicType::GL847;

    model.xdpi_values = { 4800, 2400, 1200, 600, 300, 200, 150, 100, 75 };
    model.ydpi_values = { 4800, 2400, 1200, 600, 300, 200, 150, 100, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(1.1);
    model.y_offset = SANE_FIX(8.3);
    model.x_size = SANE_FIX(216.07);
    model.y_size = SANE_FIX(299.0);

    model.y_offset_calib_white = SANE_FIX(1.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CIS_CANONLIDE100;
    model.dac_type = DAC_CANONLIDE200;
    model.gpo_type = GPO_CANONLIDE200;
    model.motor_type = MOTOR_CANONLIDE100;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_SIS_SENSOR |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_SHADING_REPARK |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_SCAN_SW |
                    GENESYS_HAS_COPY_SW |
                    GENESYS_HAS_EMAIL_SW |
                    GENESYS_HAS_FILE_SW;
    model.shading_lines = 50;
    model.shading_ta_lines = 0;
    model.search_lines = 400;

    s_usb_devices->emplace_back(0x04a9, 0x1904, model);


    model = Genesys_Model();
    model.name = "canon-lide-110";
    model.vendor = "Canon";
    model.model = "LiDE 110";
    model.model_id = MODEL_CANON_LIDE_110;
    model.asic_type = AsicType::GL124;

    model.xdpi_values = { 2400, 1200, 600, /* 400,*/ 300, 150, 100, 75 };
    model.ydpi_values = { 4800, 2400, 1200, 600, /* 400,*/ 300, 150, 100, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(2.2);
    model.y_offset = SANE_FIX(9.0);
    model.x_size = SANE_FIX(216.70);
    model.y_size = SANE_FIX(300.0);

    model.y_offset_calib_white = SANE_FIX(1.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;
    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CIS_CANONLIDE110;
    model.dac_type = DAC_CANONLIDE110;
    model.gpo_type = GPO_CANONLIDE110;
    model.motor_type = MOTOR_CANONLIDE110;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_SHADING_REPARK |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_SCAN_SW |
                    GENESYS_HAS_COPY_SW |
                    GENESYS_HAS_EMAIL_SW |
                    GENESYS_HAS_FILE_SW;
    model.shading_lines = 50;
    model.shading_ta_lines = 0;
    model.search_lines = 400;

    s_usb_devices->emplace_back(0x04a9, 0x1909, model);


    model = Genesys_Model();
    model.name = "canon-lide-120";
    model.vendor = "Canon";
    model.model = "LiDE 120";
    model.model_id = MODEL_CANON_LIDE_120;
    model.asic_type = AsicType::GL124;

    model.xdpi_values = { 4800, 2400, 1200, 600, 300, 150, 100, 75 };
    model.ydpi_values = { 4800, 2400, 1200, 600, 300, 150, 100, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(0.0);
    model.y_offset = SANE_FIX(8.0);
    model.x_size = SANE_FIX(216.0);
    model.y_size = SANE_FIX(300.0);

    model.y_offset_calib_white = SANE_FIX(1.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;
    model.line_mode_color_order = ColorOrder::RGB;
    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CIS_CANONLIDE120;
    model.dac_type = DAC_CANONLIDE120;
    model.gpo_type = GPO_CANONLIDE120;
    model.motor_type = MOTOR_CANONLIDE120;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_SHADING_REPARK |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_SCAN_SW |
                    GENESYS_HAS_COPY_SW |
                    GENESYS_HAS_EMAIL_SW |
                    GENESYS_HAS_FILE_SW;
    model.shading_lines = 50;
    model.shading_ta_lines = 0;
    model.search_lines = 400;

    s_usb_devices->emplace_back(0x04a9, 0x190e, model);


    model = Genesys_Model();
    model.name = "canon-lide-210";
    model.vendor = "Canon";
    model.model = "LiDE 210";
    model.model_id = MODEL_CANON_LIDE_210;
    model.asic_type = AsicType::GL124;

    model.xdpi_values = { 4800, 2400, 1200, 600, /* 400,*/ 300, 150, 100, 75 };
    model.ydpi_values = { 4800, 2400, 1200, 600, /* 400,*/ 300, 150, 100, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(2.2);
    model.y_offset = SANE_FIX(8.7);
    model.x_size = SANE_FIX(216.70);
    model.y_size = SANE_FIX(297.5);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CIS_CANONLIDE210;
    model.dac_type = DAC_CANONLIDE110;
    model.gpo_type = GPO_CANONLIDE210;
    model.motor_type = MOTOR_CANONLIDE210;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_SHADING_REPARK |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_SCAN_SW |
                    GENESYS_HAS_COPY_SW |
                    GENESYS_HAS_EMAIL_SW |
                    GENESYS_HAS_FILE_SW |
                    GENESYS_HAS_EXTRA_SW;
    model.shading_lines = 60;
    model.shading_ta_lines = 0;
    model.search_lines = 400;

    s_usb_devices->emplace_back(0x04a9, 0x190a, model);


    model = Genesys_Model();
    model.name = "canon-lide-220";
    model.vendor = "Canon";
    model.model = "LiDE 220";
    model.model_id = MODEL_CANON_LIDE_220;
    model.asic_type = AsicType::GL124; // or a compatible one

    model.xdpi_values = { 4800, 2400, 1200, 600, 300, 150, 100, 75 };
    model.ydpi_values = { 4800, 2400, 1200, 600, 300, 150, 100, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(2.2);
    model.y_offset = SANE_FIX(8.7);
    model.x_size = SANE_FIX(216.70);
    model.y_size = SANE_FIX(297.5);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;

    model.line_mode_color_order = ColorOrder::RGB;
    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CIS_CANONLIDE220;
    model.dac_type = DAC_CANONLIDE110;
    model.gpo_type = GPO_CANONLIDE210;
    model.motor_type = MOTOR_CANONLIDE210;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_SHADING_REPARK |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_SCAN_SW |
                    GENESYS_HAS_COPY_SW |
                    GENESYS_HAS_EMAIL_SW |
                    GENESYS_HAS_FILE_SW |
                    GENESYS_HAS_EXTRA_SW;
    model.shading_lines = 60;
    model.shading_ta_lines = 0;
    model.search_lines = 400;

    s_usb_devices->emplace_back(0x04a9, 0x190f, model);


    model = Genesys_Model();
    model.name = "canon-5600f";
    model.vendor = "Canon";
    model.model = "5600F";
    model.model_id = MODEL_CANON_CANOSCAN_5600F;
    model.asic_type = AsicType::GL847;

    model.xdpi_values = { 1200, 600, 400, 300, 200, 150, 100, 75 };
    model.ydpi_values = { 1200, 600, 400, 300, 200, 150, 100, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(1.1);
    model.y_offset = SANE_FIX(8.3);
    model.x_size = SANE_FIX(216.07);
    model.y_size = SANE_FIX(299.0);

    model.y_offset_calib_white = SANE_FIX(3.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CIS_CANONLIDE200;
    model.dac_type = DAC_CANONLIDE200;
    model.gpo_type = GPO_CANONLIDE200;
    model.motor_type = MOTOR_CANONLIDE200;
    model.flags = GENESYS_FLAG_UNTESTED |
                  GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_SIS_SENSOR |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_SCAN_SW |
                    GENESYS_HAS_COPY_SW |
                    GENESYS_HAS_EMAIL_SW |
                    GENESYS_HAS_FILE_SW;
    model.shading_lines = 50;
    model.shading_ta_lines = 0;
    model.search_lines = 400;

    s_usb_devices->emplace_back(0x04a9, 0x1906, model);


    model = Genesys_Model();
    model.name = "canon-lide-700f";
    model.vendor = "Canon";
    model.model = "LiDE 700F";
    model.model_id = MODEL_CANON_LIDE_700F;
    model.asic_type = AsicType::GL847;

    model.xdpi_values = { 4800, 2400, 1200, 600, 300, 200, 150, 100, 75 };
    model.ydpi_values = { 4800, 2400, 1200, 600, 300, 200, 150, 100, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(3.1);
    model.y_offset = SANE_FIX(8.1);
    model.x_size = SANE_FIX(216.07);
    model.y_size = SANE_FIX(297.0);

    model.y_offset_calib_white = SANE_FIX(1.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);
    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CIS_CANONLIDE700;
    model.dac_type = DAC_CANONLIDE700;
    model.gpo_type = GPO_CANONLIDE700;
    model.motor_type = MOTOR_CANONLIDE700;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_SIS_SENSOR |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_SHADING_REPARK |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_SCAN_SW |
                    GENESYS_HAS_COPY_SW |
                    GENESYS_HAS_EMAIL_SW |
                    GENESYS_HAS_FILE_SW;
    model.shading_lines = 70;
    model.shading_ta_lines = 0;
    model.search_lines = 400;

    s_usb_devices->emplace_back(0x04a9, 0x1907, model);


    model = Genesys_Model();
    model.name = "canon-lide-200";
    model.vendor = "Canon";
    model.model = "LiDE 200";
    model.model_id = MODEL_CANON_LIDE_200;
    model.asic_type = AsicType::GL847;

    model.xdpi_values = { 4800, 2400, 1200, 600, 300, 200, 150, 100, 75 };
    model.ydpi_values = { 4800, 2400, 1200, 600, 300, 200, 150, 100, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(1.1);
    model.y_offset = SANE_FIX(8.3);
    model.x_size = SANE_FIX(216.07);
    model.y_size = SANE_FIX(299.0);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;

    model.line_mode_color_order = ColorOrder::RGB;
    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CIS_CANONLIDE200;
    model.dac_type = DAC_CANONLIDE200;
    model.gpo_type = GPO_CANONLIDE200;
    model.motor_type = MOTOR_CANONLIDE200;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_SIS_SENSOR |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_SHADING_REPARK |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_SCAN_SW |
                    GENESYS_HAS_COPY_SW |
                    GENESYS_HAS_EMAIL_SW |
                    GENESYS_HAS_FILE_SW;
    model.shading_lines = 50;
    model.shading_ta_lines = 0;
    model.search_lines = 400;

    s_usb_devices->emplace_back(0x04a9, 0x1905, model);


    model = Genesys_Model();
    model.name = "canon-lide-60";
    model.vendor = "Canon";
    model.model = "LiDE 60";
    model.model_id = MODEL_CANON_LIDE_60;
    model.asic_type = AsicType::GL841;

    model.xdpi_values = { 1200, 600, 300, 150, 75 };
    model.ydpi_values = { 2400, 1200, 600, 300, 150, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(0.42);
    model.y_offset = SANE_FIX(7.9);
    model.x_size = SANE_FIX(218.0);
    model.y_size = SANE_FIX(299.0);

    model.y_offset_calib_white = SANE_FIX(6.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;
    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CCD_CANONLIDE35;
    model.dac_type = DAC_CANONLIDE35;
    model.gpo_type = GPO_CANONLIDE35;
    model.motor_type = MOTOR_CANONLIDE35;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_DARK_WHITE_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA;

    model.buttons = GENESYS_HAS_COPY_SW |
                    GENESYS_HAS_SCAN_SW |
                    GENESYS_HAS_FILE_SW |
                    GENESYS_HAS_EMAIL_SW;
    model.shading_lines = 300;
    model.shading_ta_lines = 0;
    model.search_lines = 400;
    // this is completely untested
    s_usb_devices->emplace_back(0x04a9, 0x221c, model);


    model = Genesys_Model();
    model.name = "canon-lide-80";
    model.vendor = "Canon";
    model.model = "LiDE 80";
    model.model_id = MODEL_CANON_LIDE_80;
    model.asic_type = AsicType::GL841;

    model.xdpi_values = {      1200, 600, 400, 300, 240, 150, 100, 75 };
    model.ydpi_values = { 2400, 1200, 600, 400, 300, 240, 150, 100, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };
    model.x_offset = SANE_FIX(0.42);
    model.y_offset = SANE_FIX(7.90);
    model.x_size = SANE_FIX(216.07);
    model.y_size = SANE_FIX(299.0);

    model.y_offset_calib_white = SANE_FIX(4.5);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CIS_CANONLIDE80;
    model.dac_type = DAC_CANONLIDE80;
    model.gpo_type = GPO_CANONLIDE80;
    model.motor_type = MOTOR_CANONLIDE80;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_DARK_WHITE_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_SCAN_SW |
                    GENESYS_HAS_FILE_SW |
                    GENESYS_HAS_EMAIL_SW |
                    GENESYS_HAS_COPY_SW;
    model.shading_lines = 160;
    model.shading_ta_lines = 0;
    model.search_lines = 400;

    s_usb_devices->emplace_back(0x04a9, 0x2214, model);


    model = Genesys_Model();
    model.name = "hewlett-packard-scanjet-2300c";
    model.vendor = "Hewlett Packard";
    model.model = "ScanJet 2300c";
    model.model_id = MODEL_HP_SCANJET_2300C;
    model.asic_type = AsicType::GL646;

    // FIXME: the scanner supports 1200 ydpi, but we never scanned at this resolution so for now
    // it's not supported
    model.xdpi_values = { 600, 300, 150, 75 };
    model.ydpi_values = { /* 1200, */600, 300, 150, 75 };

    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(2.0);
    model.y_offset = SANE_FIX(7.5);
    model.x_size = SANE_FIX(215.9);
    model.y_size = SANE_FIX(295.0);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(1.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 16;
    model.ld_shift_g = 8;
    model.ld_shift_b = 0;

    model.line_mode_color_order = ColorOrder::RGB;
    model.is_cis = SANE_FALSE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CCD_HP2300;
    model.dac_type = DAC_WOLFSON_HP2300;
    model.gpo_type = GPO_HP2300;
    model.motor_type = MOTOR_HP2300;
    model.flags = GENESYS_FLAG_14BIT_GAMMA |
                  GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_SEARCH_START |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_SCAN_SW | GENESYS_HAS_COPY_SW;
    model.shading_lines = 40;
    model.shading_ta_lines = 0;
    model.search_lines = 132;

    s_usb_devices->emplace_back(0x03f0, 0x0901, model);


    model = Genesys_Model();
    model.name = "hewlett-packard-scanjet-2400c";
    model.vendor = "Hewlett Packard";
    model.model = "ScanJet 2400c";
    model.model_id = MODEL_HP_SCANJET_2400C;
    model.asic_type = AsicType::GL646;

    model.xdpi_values = { 1200, 600, 300, 150, 100, 50 };
    model.ydpi_values = { 1200, 600, 300, 150, 100, 50 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(6.5);
    model.y_offset = SANE_FIX(2.5);
    model.x_size = SANE_FIX(220.0);
    model.y_size = SANE_FIX(297.2);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(1.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 24;
    model.ld_shift_b = 48;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_FALSE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CCD_HP2400;
    model.dac_type = DAC_WOLFSON_HP2400;
    model.gpo_type = GPO_HP2400;
    model.motor_type = MOTOR_HP2400;
    model.flags = GENESYS_FLAG_14BIT_GAMMA |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_STAGGERED_LINE |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_COPY_SW | GENESYS_HAS_EMAIL_SW | GENESYS_HAS_SCAN_SW;
    model.shading_lines = 20;
    model.shading_ta_lines = 0;
    model.search_lines = 132;

    s_usb_devices->emplace_back(0x03f0, 0x0a01, model);


    model = Genesys_Model();
    model.name = "visioneer-strobe-xp200";
    model.vendor = "Visioneer";
    model.model = "Strobe XP200";
    model.model_id = MODEL_VISIONEER_STROBE_XP200;
    model.asic_type = AsicType::GL646;

    model.xdpi_values = { 600, 300, 200, 100, 75 };
    model.ydpi_values = { 600, 300, 200, 100, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(0.5);
    model.y_offset = SANE_FIX(16.0);
    model.x_size = SANE_FIX(215.9);
    model.y_size = SANE_FIX(297.2);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_TRUE;
    model.ccd_type = CIS_XP200;
    model.dac_type = DAC_AD_XP200;
    model.gpo_type = GPO_XP200;
    model.motor_type = MOTOR_XP200;
    model.flags = GENESYS_FLAG_14BIT_GAMMA |
                  GENESYS_FLAG_CUSTOM_GAMMA |
                  GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_OFFSET_CALIBRATION;
    model.buttons = GENESYS_HAS_SCAN_SW | GENESYS_HAS_PAGE_LOADED_SW | GENESYS_HAS_CALIBRATE;
    model.shading_lines = 120;
    model.shading_ta_lines = 0;
    model.search_lines = 132;

    s_usb_devices->emplace_back(0x04a7, 0x0426, model);


    model = Genesys_Model();
    model.name = "hewlett-packard-scanjet-3670c";
    model.vendor = "Hewlett Packard";
    model.model = "ScanJet 3670c";
    model.model_id = MODEL_HP_SCANJET_3670C;
    model.asic_type = AsicType::GL646;

    model.xdpi_values = { 1200, 600, 300, 150, 100, 75 };
    model.ydpi_values = { 1200, 600, 300, 150, 100, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(8.5);
    model.y_offset = SANE_FIX(11.0);
    model.x_size = SANE_FIX(215.9);
    model.y_size = SANE_FIX(300.0);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(1.0);

    model.x_offset_ta = SANE_FIX(104.0);
    model.y_offset_ta = SANE_FIX(55.6);
    model.x_size_ta = SANE_FIX(25.6);
    model.y_size_ta = SANE_FIX(78.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(76.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 24;
    model.ld_shift_b = 48;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_FALSE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CCD_HP3670;
    model.dac_type = DAC_WOLFSON_HP3670;
    model.gpo_type = GPO_HP3670;
    model.motor_type = MOTOR_HP3670;
    model.flags = GENESYS_FLAG_14BIT_GAMMA |
                  GENESYS_FLAG_XPA |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_STAGGERED_LINE |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_COPY_SW | GENESYS_HAS_EMAIL_SW | GENESYS_HAS_SCAN_SW;
    model.shading_lines = 20;
    model.shading_ta_lines = 0;
    model.search_lines = 200;

    s_usb_devices->emplace_back(0x03f0, 0x1405, model);


    model = Genesys_Model();
    model.name = "plustek-opticpro-st12";
    model.vendor = "Plustek";
    model.model = "OpticPro ST12";
    model.model_id = MODEL_PLUSTEK_OPTICPRO_ST12;
    model.asic_type = AsicType::GL646;

    model.xdpi_values = { 600, 300, 150, 75 };
    model.ydpi_values = { 1200, 600, 300, 150, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(3.5);
    model.y_offset = SANE_FIX(7.5);
    model.x_size = SANE_FIX(218.0);
    model.y_size = SANE_FIX(299.0);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(1.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 8;
    model.ld_shift_b = 16;

    model.line_mode_color_order = ColorOrder::BGR;

    model.is_cis = SANE_FALSE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CCD_ST12;
    model.dac_type = DAC_WOLFSON_ST12;
    model.gpo_type = GPO_ST12;
    model.motor_type = MOTOR_UMAX;
    model.flags = GENESYS_FLAG_UNTESTED | GENESYS_FLAG_14BIT_GAMMA;
    model.buttons = GENESYS_HAS_NO_BUTTONS,
    model.shading_lines = 20;
    model.shading_ta_lines = 0;
    model.search_lines = 200;

    s_usb_devices->emplace_back(0x07b3, 0x0600, model);

    model = Genesys_Model();
    model.name = "plustek-opticpro-st24";
    model.vendor = "Plustek";
    model.model = "OpticPro ST24";
    model.model_id = MODEL_PLUSTEK_OPTICPRO_ST24;
    model.asic_type = AsicType::GL646;

    model.xdpi_values = { 1200, 600, 300, 150, 75 };
    model.ydpi_values = { 2400, 1200, 600, 300, 150, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(3.5);
    model.y_offset = SANE_FIX(7.5);
    model.x_size = SANE_FIX(218.0);
    model.y_size = SANE_FIX(299.0);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(1.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 8;
    model.ld_shift_b = 16;

    model.line_mode_color_order = ColorOrder::BGR;

    model.is_cis = SANE_FALSE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CCD_ST24;
    model.dac_type = DAC_WOLFSON_ST24;
    model.gpo_type = GPO_ST24;
    model.motor_type = MOTOR_ST24;
    model.flags = GENESYS_FLAG_UNTESTED |
                  GENESYS_FLAG_14BIT_GAMMA |
                  GENESYS_FLAG_CUSTOM_GAMMA |
                  GENESYS_FLAG_SEARCH_START |
                  GENESYS_FLAG_OFFSET_CALIBRATION;
    model.buttons = GENESYS_HAS_NO_BUTTONS,
    model.shading_lines = 20;
    model.shading_ta_lines = 0;
    model.search_lines = 200;

    s_usb_devices->emplace_back(0x07b3, 0x0601, model);

    model = Genesys_Model();
    model.name = "medion-md5345-model";
    model.vendor = "Medion";
    model.model = "MD5345/MD6228/MD6471";
    model.model_id = MODEL_MEDION_MD5345;
    model.asic_type = AsicType::GL646;

    model.xdpi_values = { 1200, 600, 400, 300, 200, 150, 100, 75, 50 };
    model.ydpi_values = { 2400, 1200, 600, 400, 300, 200, 150, 100, 75, 50 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(0.30);
    model.y_offset = SANE_FIX(0.80);
    model.x_size = SANE_FIX(220.0);
    model.y_size = SANE_FIX(296.4);

    model.y_offset_calib_white = SANE_FIX(0.00);
    model.x_offset_calib_black = SANE_FIX(0.00);

    model.x_offset_ta = SANE_FIX(0.00);
    model.y_offset_ta = SANE_FIX(0.00);
    model.x_size_ta = SANE_FIX(0.00);
    model.y_size_ta = SANE_FIX(0.00);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.00);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 48;
    model.ld_shift_g = 24;
    model.ld_shift_b = 0;
    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_FALSE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CCD_5345;
    model.dac_type = DAC_WOLFSON_5345;
    model.gpo_type = GPO_5345;
    model.motor_type = MOTOR_5345;
    model.flags = GENESYS_FLAG_14BIT_GAMMA |
                  GENESYS_FLAG_SEARCH_START |
                  GENESYS_FLAG_STAGGERED_LINE |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_SHADING_NO_MOVE |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_COPY_SW |
                    GENESYS_HAS_EMAIL_SW |
                    GENESYS_HAS_POWER_SW |
                    GENESYS_HAS_OCR_SW |
                    GENESYS_HAS_SCAN_SW;
    model.shading_lines = 40;
    model.shading_ta_lines = 0;
    model.search_lines = 200;

    s_usb_devices->emplace_back(0x0461, 0x0377, model);

    model = Genesys_Model();
    model.name = "visioneer-strobe-xp300";
    model.vendor = "Visioneer";
    model.model = "Strobe XP300";
    model.model_id = MODEL_VISIONEER_STROBE_XP300;
    model.asic_type = AsicType::GL841;

    model.xdpi_values = { 600, 300, 150, 75 };
    model.ydpi_values = { 600, 300, 150, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(0.0);
    model.y_offset = SANE_FIX(1.0);
    model.x_size = SANE_FIX(435.0);
    model.y_size = SANE_FIX(511);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(26.5);
    // this is larger than needed -- accounts for second sensor head, which is a calibration item
    model.eject_feed = SANE_FIX(0.0);
    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_TRUE;
    model.ccd_type = CCD_XP300;
    model.dac_type = DAC_WOLFSON_XP300;
    model.gpo_type = GPO_XP300;
    model.motor_type = MOTOR_XP300;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_SCAN_SW | GENESYS_HAS_PAGE_LOADED_SW | GENESYS_HAS_CALIBRATE;
    model.shading_lines = 100;
    model.shading_ta_lines = 0;
    model.search_lines = 400;

    s_usb_devices->emplace_back(0x04a7, 0x0474, model);

    model = Genesys_Model();
    model.name = "syscan-docketport-665";
    model.vendor = "Syscan/Ambir";
    model.model = "DocketPORT 665";
    model.model_id = MODEL_SYSCAN_DOCKETPORT_665;
    model.asic_type = AsicType::GL841;

    model.xdpi_values = { 600, 300, 150, 75 };
    model.ydpi_values = { 1200, 600, 300, 150, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(0.0);
    model.y_offset = SANE_FIX(0.0);
    model.x_size = SANE_FIX(108.0);
    model.y_size = SANE_FIX(511);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(17.5);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_TRUE;
    model.ccd_type = CCD_DP665;
    model.dac_type = DAC_WOLFSON_XP300;
    model.gpo_type = GPO_DP665;
    model.motor_type = MOTOR_DP665;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_SCAN_SW | GENESYS_HAS_PAGE_LOADED_SW | GENESYS_HAS_CALIBRATE;
    model.shading_lines = 100;
    model.shading_ta_lines = 0;
    model.search_lines = 400;

    s_usb_devices->emplace_back(0x0a82, 0x4803, model);

    model = Genesys_Model();
    model.name = "visioneer-roadwarrior";
    model.vendor = "Visioneer";
    model.model = "Readwarrior";
    model.model_id = MODEL_VISIONEER_ROADWARRIOR;
    model.asic_type = AsicType::GL841;

    model.xdpi_values = { 600, 300, 150, 75 };
    model.ydpi_values = { 1200, 600, 300, 150, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(0.0);
    model.y_offset = SANE_FIX(0.0);
    model.x_size = SANE_FIX(220.0);
    model.y_size = SANE_FIX(511);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(16.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_TRUE;
    model.ccd_type = CCD_ROADWARRIOR;
    model.dac_type = DAC_WOLFSON_XP300;
    model.gpo_type = GPO_DP665;
    model.motor_type = MOTOR_ROADWARRIOR;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA |
                  GENESYS_FLAG_DARK_CALIBRATION;
    model.buttons = GENESYS_HAS_SCAN_SW | GENESYS_HAS_PAGE_LOADED_SW | GENESYS_HAS_CALIBRATE;
    model.shading_lines = 100;
    model.shading_ta_lines = 0;
    model.search_lines = 400;

    s_usb_devices->emplace_back(0x04a7, 0x0494, model);

    model = Genesys_Model();
    model.name = "syscan-docketport-465";
    model.vendor = "Syscan";
    model.model = "DocketPORT 465";
    model.model_id = MODEL_SYSCAN_DOCKETPORT_465;
    model.asic_type = AsicType::GL841;

    model.xdpi_values = { 600, 300, 150, 75 };
    model.ydpi_values = { 1200, 600, 300, 150, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(0.0);
    model.y_offset = SANE_FIX(0.0);
    model.x_size = SANE_FIX(220.0);
    model.y_size = SANE_FIX(511);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(16.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_TRUE;
    model.ccd_type = CCD_ROADWARRIOR;
    model.dac_type = DAC_WOLFSON_XP300;
    model.gpo_type = GPO_DP665;
    model.motor_type = MOTOR_ROADWARRIOR;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_NO_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA |
                  GENESYS_FLAG_UNTESTED;
    model.buttons = GENESYS_HAS_SCAN_SW | GENESYS_HAS_PAGE_LOADED_SW;
    model.shading_lines = 300;
    model.shading_ta_lines = 0;
    model.search_lines = 400;

    s_usb_devices->emplace_back(0x0a82, 0x4802, model);


    model = Genesys_Model();
    model.name = "visioneer-xp100-revision3";
    model.vendor = "Visioneer";
    model.model = "XP100 Revision 3";
    model.model_id = MODEL_VISIONEER_STROBE_XP100_REVISION3;
    model.asic_type = AsicType::GL841;

    model.xdpi_values = { 600, 300, 150, 75 };
    model.ydpi_values = { 1200, 600, 300, 150, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(0.0);
    model.y_offset = SANE_FIX(0.0);
    model.x_size = SANE_FIX(220.0);
    model.y_size = SANE_FIX(511);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(16.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_TRUE;
    model.ccd_type = CCD_ROADWARRIOR;
    model.dac_type = DAC_WOLFSON_XP300;
    model.gpo_type = GPO_DP665;
    model.motor_type = MOTOR_ROADWARRIOR;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA |
                  GENESYS_FLAG_DARK_CALIBRATION;
    model.buttons = GENESYS_HAS_SCAN_SW | GENESYS_HAS_PAGE_LOADED_SW | GENESYS_HAS_CALIBRATE;
    model.shading_lines = 100;
    model.shading_ta_lines = 0;
    model.search_lines = 400;

    s_usb_devices->emplace_back(0x04a7, 0x049b, model);

    model = Genesys_Model();
    model.name = "pentax-dsmobile-600";
    model.vendor = "Pentax";
    model.model = "DSmobile 600";
    model.model_id = MODEL_PENTAX_DSMOBILE_600;
    model.asic_type = AsicType::GL841;

    model.xdpi_values = { 600, 300, 150, 75 };
    model.ydpi_values = { 1200, 600, 300, 150, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(0.0);
    model.y_offset = SANE_FIX(0.0);
    model.x_size = SANE_FIX(220.0);
    model.y_size = SANE_FIX(511);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(16.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_TRUE;
    model.ccd_type = CCD_DSMOBILE600;
    model.dac_type = DAC_WOLFSON_DSM600;
    model.gpo_type = GPO_DP665;
    model.motor_type = MOTOR_DSMOBILE_600;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA |
                  GENESYS_FLAG_DARK_CALIBRATION;
    model.buttons = GENESYS_HAS_SCAN_SW | GENESYS_HAS_PAGE_LOADED_SW | GENESYS_HAS_CALIBRATE;
    model.shading_lines = 100;
    model.shading_ta_lines = 0;
    model.search_lines = 400;

    s_usb_devices->emplace_back(0x0a17, 0x3210, model);
    // clone, only usb id is different
    s_usb_devices->emplace_back(0x04f9, 0x2038, model);

    model = Genesys_Model();
    model.name = "syscan-docketport-467";
    model.vendor = "Syscan";
    model.model = "DocketPORT 467";
    model.model_id = MODEL_SYSCAN_DOCKETPORT_467;
    model.asic_type = AsicType::GL841;

    model.xdpi_values = { 600, 300, 150, 75 };
    model.ydpi_values = { 1200, 600, 300, 150, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(0.0);
    model.y_offset = SANE_FIX(0.0);
    model.x_size = SANE_FIX(220.0);
    model.y_size = SANE_FIX(511);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(16.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;
    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_TRUE;
    model.ccd_type = CCD_DSMOBILE600;
    model.dac_type = DAC_WOLFSON_DSM600;
    model.gpo_type = GPO_DP665;
    model.motor_type = MOTOR_DSMOBILE_600;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA |
                  GENESYS_FLAG_DARK_CALIBRATION;
    model.buttons = GENESYS_HAS_SCAN_SW | GENESYS_HAS_PAGE_LOADED_SW | GENESYS_HAS_CALIBRATE;
    model.shading_lines = 100;
    model.shading_ta_lines = 0;
    model.search_lines = 400;

    s_usb_devices->emplace_back(0x1dcc, 0x4812, model);

    model = Genesys_Model();
    model.name = "syscan-docketport-685";
    model.vendor = "Syscan/Ambir";
    model.model = "DocketPORT 685";
    model.model_id = MODEL_SYSCAN_DOCKETPORT_685;
    model.asic_type = AsicType::GL841;

    model.xdpi_values = { 600, 300, 150, 75 };
    model.ydpi_values = { 600, 300, 150, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(0.0);
    model.y_offset = SANE_FIX(1.0);
    model.x_size = SANE_FIX(212.0);
    model.y_size = SANE_FIX(500);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(26.5);
    // this is larger than needed -- accounts for second sensor head, which is a calibration item
    model.eject_feed = SANE_FIX(0.0);
    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_TRUE;
    model.ccd_type = CCD_DP685;
    model.dac_type = DAC_WOLFSON_DSM600;
    model.gpo_type = GPO_DP685;
    model.motor_type = MOTOR_XP300;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA |
                  GENESYS_FLAG_DARK_CALIBRATION;
    model.buttons = GENESYS_HAS_SCAN_SW | GENESYS_HAS_PAGE_LOADED_SW | GENESYS_HAS_CALIBRATE;
    model.shading_lines = 100;
    model.shading_ta_lines = 0;
    model.search_lines = 400;


    s_usb_devices->emplace_back(0x0a82, 0x480c, model);


    model = Genesys_Model();
    model.name = "syscan-docketport-485";
    model.vendor = "Syscan/Ambir";
    model.model = "DocketPORT 485";
    model.model_id = MODEL_SYSCAN_DOCKETPORT_485;
    model.asic_type = AsicType::GL841;

    model.xdpi_values = { 600, 300, 150, 75 };
    model.ydpi_values = { 600, 300, 150, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(0.0);
    model.y_offset = SANE_FIX(1.0);
    model.x_size = SANE_FIX(435.0);
    model.y_size = SANE_FIX(511);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(26.5);
    // this is larger than needed -- accounts for second sensor head, which is a calibration item
    model.eject_feed = SANE_FIX(0.0);
    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_TRUE;
    model.ccd_type = CCD_XP300;
    model.dac_type = DAC_WOLFSON_XP300;
    model.gpo_type = GPO_XP300;
    model.motor_type = MOTOR_XP300;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA |
                  GENESYS_FLAG_DARK_CALIBRATION;
    model.buttons = GENESYS_HAS_SCAN_SW | GENESYS_HAS_PAGE_LOADED_SW | GENESYS_HAS_CALIBRATE;
    model.shading_lines = 100;
    model.shading_ta_lines = 0;
    model.search_lines = 400;

    s_usb_devices->emplace_back(0x0a82, 0x4800, model);


    model = Genesys_Model();
    model.name = "dct-docketport-487";
    model.vendor = "DCT";
    model.model = "DocketPORT 487";
    model.model_id = MODEL_DCT_DOCKETPORT_487;
    model.asic_type = AsicType::GL841;

    model.xdpi_values = { 600, 300, 150, 75 };
    model.ydpi_values = { 600, 300, 150, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(0.0);
    model.y_offset = SANE_FIX(1.0);
    model.x_size = SANE_FIX(435.0);
    model.y_size = SANE_FIX(511);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(26.5);
    // this is larger than needed -- accounts for second sensor head, which is a calibration item
    model.eject_feed = SANE_FIX(0.0);
    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_TRUE;
    model.ccd_type = CCD_XP300;
    model.dac_type = DAC_WOLFSON_XP300;
    model.gpo_type = GPO_XP300;
    model.motor_type = MOTOR_XP300;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA |
                  GENESYS_FLAG_UNTESTED;
    model.buttons = GENESYS_HAS_SCAN_SW | GENESYS_HAS_PAGE_LOADED_SW | GENESYS_HAS_CALIBRATE;
    model.shading_lines = 100;
    model.shading_ta_lines = 0;
    model.search_lines = 400;

    s_usb_devices->emplace_back(0x1dcc, 0x4810, model);


    model = Genesys_Model();
    model.name = "visioneer-7100-model";
    model.vendor = "Visioneer";
    model.model = "OneTouch 7100";
    model.model_id = MODEL_VISIONEER_7100;
    model.asic_type = AsicType::GL646;

    model.xdpi_values = { 1200, 600, 400, 300, 200, 150, 100, 75, 50 };
    model.ydpi_values = { 2400, 1200, 600, 400, 300, 200, 150, 100, 75, 50 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(4.00);
    model.y_offset = SANE_FIX(0.80);
    model.x_size = SANE_FIX(215.9);
    model.y_size = SANE_FIX(296.4);

    model.y_offset_calib_white = SANE_FIX(0.00);
    model.x_offset_calib_black = SANE_FIX(0.00);

    model.x_offset_ta = SANE_FIX(0.00);
    model.y_offset_ta = SANE_FIX(0.00);
    model.x_size_ta = SANE_FIX(0.00);
    model.y_size_ta = SANE_FIX(0.00);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.00);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 48;
    model.ld_shift_g = 24;
    model.ld_shift_b = 0;
    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_FALSE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CCD_5345;
    model.dac_type = DAC_WOLFSON_5345;
    model.gpo_type = GPO_5345;
    model.motor_type = MOTOR_5345;
    model.flags = GENESYS_FLAG_14BIT_GAMMA |
                  GENESYS_FLAG_SEARCH_START |
                  GENESYS_FLAG_STAGGERED_LINE |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_COPY_SW |
                    GENESYS_HAS_EMAIL_SW |
                    GENESYS_HAS_POWER_SW |
                    GENESYS_HAS_OCR_SW |
                    GENESYS_HAS_SCAN_SW;
    model.shading_lines = 40;
    model.shading_ta_lines = 0;
    model.search_lines = 200;

    s_usb_devices->emplace_back(0x04a7, 0x0229, model);


    model = Genesys_Model();
    model.name = "xerox-2400-model";
    model.vendor = "Xerox";
    model.model = "OneTouch 2400";
    model.model_id = MODEL_XEROX_2400;
    model.asic_type = AsicType::GL646;

    model.xdpi_values = { 1200, 600, 400, 300, 200, 150, 100, 75, 50 };
    model.ydpi_values = { 2400, 1200, 600, 400, 300, 200, 150, 100, 75, 50 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(4.00);
    model.y_offset = SANE_FIX(0.80);
    model.x_size = SANE_FIX(215.9);
    model.y_size = SANE_FIX(296.4);

    model.y_offset_calib_white = SANE_FIX(0.00);
    model.x_offset_calib_black = SANE_FIX(0.00);

    model.x_offset_ta = SANE_FIX(0.00);
    model.y_offset_ta = SANE_FIX(0.00);
    model.x_size_ta = SANE_FIX(0.00);
    model.y_size_ta = SANE_FIX(0.00);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.00);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 48;
    model.ld_shift_g = 24;
    model.ld_shift_b = 0;
    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_FALSE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CCD_5345;
    model.dac_type = DAC_WOLFSON_5345;
    model.gpo_type = GPO_5345;
    model.motor_type = MOTOR_5345;
    model.flags = GENESYS_FLAG_14BIT_GAMMA |
                  GENESYS_FLAG_SEARCH_START |
                  GENESYS_FLAG_STAGGERED_LINE |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_COPY_SW |
                    GENESYS_HAS_EMAIL_SW |
                    GENESYS_HAS_POWER_SW |
                    GENESYS_HAS_OCR_SW |
                    GENESYS_HAS_SCAN_SW;
    model.shading_lines = 40;
    model.shading_ta_lines = 0;
    model.search_lines = 200;

    s_usb_devices->emplace_back(0x0461, 0x038b, model);


    model = Genesys_Model();
    model.name = "xerox-travelscanner";
    model.vendor = "Xerox";
    model.model = "Travelscanner 100";
    model.model_id = MODEL_XEROX_TRAVELSCANNER_100;
    model.asic_type = AsicType::GL841;

    model.xdpi_values = { 600, 300, 150, 75 };
    model.ydpi_values = { 1200, 600, 300, 150, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(4.0);
    model.y_offset = SANE_FIX(0.0);
    model.x_size = SANE_FIX(220.0);
    model.y_size = SANE_FIX(511);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(16.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_TRUE;
    model.is_sheetfed = SANE_TRUE;
    model.ccd_type = CCD_ROADWARRIOR;
    model.dac_type = DAC_WOLFSON_XP300;
    model.gpo_type = GPO_DP665;
    model.motor_type = MOTOR_ROADWARRIOR;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA |
                  GENESYS_FLAG_DARK_CALIBRATION;
    model.buttons = GENESYS_HAS_SCAN_SW | GENESYS_HAS_PAGE_LOADED_SW | GENESYS_HAS_CALIBRATE;
    model.shading_lines = 100;
    model.shading_ta_lines = 0;
    model.search_lines = 400;

    s_usb_devices->emplace_back(0x04a7, 0x04ac, model);


    model = Genesys_Model();
    model.name = "plustek-opticbook-3600";
    model.vendor = "PLUSTEK";
    model.model = "OpticBook 3600";
    model.model_id = MODEL_PLUSTEK_OPTICPRO_3600;
    model.asic_type = AsicType::GL841;
    model.xdpi_values = { /*1200,*/ 600, 400, 300, 200, 150, 100, 75 };
    model.ydpi_values = { /*2400,*/ 1200, 600, 400, 300, 200, 150, 100, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(0.42);
    model.y_offset = SANE_FIX(6.75);
    model.x_size = SANE_FIX(216.0);
    model.y_size = SANE_FIX(297.0);

    model.y_offset_calib_white = SANE_FIX(0.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(0.0);
    model.y_size_ta = SANE_FIX(0.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 24;
    model.ld_shift_b = 48;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_FALSE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CCD_PLUSTEK_3600;
    model.dac_type = DAC_PLUSTEK_3600;
    model.gpo_type = GPO_PLUSTEK_3600;
    model.motor_type = MOTOR_PLUSTEK_3600;
    model.flags = GENESYS_FLAG_UNTESTED |                // not fully working yet
                  GENESYS_FLAG_CUSTOM_GAMMA |
                  GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_OFFSET_CALIBRATION;
    model.buttons = GENESYS_HAS_NO_BUTTONS;
    model.shading_lines = 7;
    model.shading_ta_lines = 0;
    model.search_lines = 200;

    s_usb_devices->emplace_back(0x07b3, 0x0900, model);


    model = Genesys_Model();
    model.name = "hewlett-packard-scanjet-N6310";
    model.vendor = "Hewlett Packard";
    model.model = "ScanJet N6310";
    model.model_id = MODEL_HP_SCANJET_N6310;
    model.asic_type = AsicType::GL847;

    model.xdpi_values = { 2400, 1200, 600, 400, 300, 200, 150, 100, 75 };
    model.ydpi_values = { 2400, 1200, 600, 400, 300, 200, 150, 100, 75 };

    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(6);
    model.y_offset = SANE_FIX(2);
    model.x_size = SANE_FIX(216);
    model.y_size = SANE_FIX(511);

    model.y_offset_calib_white = SANE_FIX(3.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(100.0);
    model.y_size_ta = SANE_FIX(100.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0);

    model.post_scan = SANE_FIX(0);
    model.eject_feed = SANE_FIX(0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 0;
    model.ld_shift_b = 0;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_FALSE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CCD_HP_N6310;
    model.dac_type = DAC_CANONLIDE200;        // Not defined yet for N6310
    model.gpo_type = GPO_HP_N6310;
    model.motor_type = MOTOR_CANONLIDE200,    // Not defined yet for N6310
    model.flags = GENESYS_FLAG_UNTESTED |
                  GENESYS_FLAG_14BIT_GAMMA |
                  GENESYS_FLAG_DARK_CALIBRATION |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA |
                  GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_NO_CALIBRATION;

    model.buttons = GENESYS_HAS_NO_BUTTONS;
    model.shading_lines = 100;
    model.shading_ta_lines = 0;
    model.search_lines = 100;

    s_usb_devices->emplace_back(0x03f0, 0x4705, model);


    model = Genesys_Model();
    model.name = "plustek-opticbook-3800";
    model.vendor = "PLUSTEK";
    model.model = "OpticBook 3800";
    model.model_id = MODEL_PLUSTEK_OPTICBOOK_3800;
    model.asic_type = AsicType::GL845;

    model.xdpi_values = { 1200, 600, 300, 150, 100, 75 };
    model.ydpi_values = { 1200, 600, 300, 150, 100, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(7.2);
    model.y_offset = SANE_FIX(14.7);
    model.x_size = SANE_FIX(217.7);
    model.y_size = SANE_FIX(300.0);

    model.y_offset_calib_white = SANE_FIX(9.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(0.0);
    model.y_size_ta = SANE_FIX(0.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 24;
    model.ld_shift_b = 48;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_FALSE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CCD_PLUSTEK3800;
    model.dac_type = DAC_PLUSTEK3800;
    model.gpo_type = GPO_PLUSTEK3800;
    model.motor_type = MOTOR_PLUSTEK3800;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_NO_BUTTONS;  // TODO there are 4 buttons to support
    model.shading_lines = 100;
    model.shading_ta_lines = 0;
    model.search_lines = 100;

    s_usb_devices->emplace_back(0x07b3, 0x1300, model);


    model = Genesys_Model();
    model.name = "canon-image-formula-101";
    model.vendor = "Canon";
    model.model = "Image Formula 101";
    model.model_id = MODEL_CANON_IMAGE_FORMULA_101;
    model.asic_type = AsicType::GL846;

    model.xdpi_values = { 1200, 600, 300, 150, 100, 75 };
    model.ydpi_values = { 1200, 600, 300, 150, 100, 75 };
    model.bpp_gray_values = { 16, 8 };
    model.bpp_color_values = { 16, 8 };

    model.x_offset = SANE_FIX(7.2);
    model.y_offset = SANE_FIX(14.7);
    model.x_size = SANE_FIX(217.7);
    model.y_size = SANE_FIX(300.0);

    model.y_offset_calib_white = SANE_FIX(9.0);
    model.x_offset_calib_black = SANE_FIX(0.0);

    model.x_offset_ta = SANE_FIX(0.0);
    model.y_offset_ta = SANE_FIX(0.0);
    model.x_size_ta = SANE_FIX(0.0);
    model.y_size_ta = SANE_FIX(0.0);

    model.y_offset_sensor_to_ta = SANE_FIX(0.0);
    model.y_offset_calib_white_ta = SANE_FIX(0.0);

    model.post_scan = SANE_FIX(0.0);
    model.eject_feed = SANE_FIX(0.0);

    model.ld_shift_r = 0;
    model.ld_shift_g = 24;
    model.ld_shift_b = 48;

    model.line_mode_color_order = ColorOrder::RGB;

    model.is_cis = SANE_FALSE;
    model.is_sheetfed = SANE_FALSE;
    model.ccd_type = CCD_IMG101;
    model.dac_type = DAC_IMG101;
    model.gpo_type = GPO_IMG101;
    model.motor_type = MOTOR_IMG101;
    model.flags = GENESYS_FLAG_SKIP_WARMUP |
                  GENESYS_FLAG_OFFSET_CALIBRATION |
                  GENESYS_FLAG_CUSTOM_GAMMA;
    model.buttons = GENESYS_HAS_NO_BUTTONS ;
    model.shading_lines = 100;
    model.shading_ta_lines = 0;
    model.search_lines = 100;

    s_usb_devices->emplace_back(0x1083, 0x162e, model);
 }