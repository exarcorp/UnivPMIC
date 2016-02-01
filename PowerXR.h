/*
 * XR Programmer - standalone programmer to program a hex file
 *
 * Copyright (c) 2014 Exar Corporation, Inc.
 *
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef POWERXR_h
#define POWERXR_h

#include "Arduino.h"
// typedef unsigned int word;
// typedef uint8_t byte;

class Xrp7704{
    public:
        Xrp7704();
        Xrp7704(int i2c_addr);
        ~Xrp7704();

        byte standard_read(byte);
        void standard_write(byte, byte);
        byte otp_read(byte);
        void otp_write(byte, byte);

        byte program_flash(byte[]);
        byte download_ram(byte[]);

    protected:
        int _i2c_addr;
};

class Xrp7714 : public Xrp7704{ 
    public:
        Xrp7714(int i2c_addr);
        ~Xrp7714();
};

class Xrp7724{ 
    public:
        Xrp7724();
        Xrp7724(int i2c_addr);
        ~Xrp7724();
        
        word standard_read(byte);
        void standard_write(byte, word);
        byte register_read(word);
        void register_write(word, byte);
        
        String get_chip_name();
        word get_host_status();
        void set_host_int_mask(word);
        void clear_host_int(word);
        word get_fault_status();
        void clear_fault_status(word);
        word power_get_status();
        byte get_power_chip_ready();
        void set_power_chip_ready(word);
        void power_restart();
        void chip_reset();
        float power_read_voltage_ch(byte);
        float power_read_voltage_vin();
        word power_read_temp_vtj();
        float power_read_lowside_voltage_ch(byte);
        byte get_ife_gain(byte);
        float get_current_ch(byte, float, float);
        void power_enable_supply_group(byte, byte);
        void power_enable_supply(byte, byte);
        void power_set_voltage_ch(byte, float);
        void power_set_voltage_native_fine_adj_ch(byte, float);
        float get_prescale_voltage(byte);
        void power_set_current_ch(byte, float, float);
        void power_ok_cfg(byte, byte);
        byte read_gpio();
        void set_gpio(byte, byte);
        void polarity_gpio(byte, byte);
        word get_flash_program_address();
        void set_flash_program_address(word);
        word get_flash_program_data();
        void set_flash_program_data(word);
        word get_flash_program_data_inc_addr();
        void set_flash_program_data_inc_addr(word);
        void flash_init(word);
        word get_flash_page_clear();
        void set_flash_page_clear(byte, byte);
        word get_flash_page_erase();
        void set_flash_page_erase(byte, byte);
        
        byte clear_flash_page(byte);
        byte erase_flash_page(byte);
        byte program_flash(byte[], word);

        void invalidate_flash();
        byte download_ram(byte[]);
    protected:
        int _i2c_addr;
};

class Xrp7725 : public Xrp7724{ 
    public:
        Xrp7725(int i2c_addr);
        ~Xrp7725();

        float power_read_vil_ch(byte, word);
        float get_average_current_ch(byte, float, float, word);
        byte initialize_accumulator();
};

class Xr77129 : public Xrp7724{ 
    public:
        Xr77129(int i2c_addr);
        ~Xr77129();

        void power_set_voltage_ch(byte, float);
};

class Xr77128 : public Xrp7724{ 
    public:
        Xr77128(int i2c_addr);
        ~Xr77128();

        void power_set_voltage_ch(byte, float);
};



#endif