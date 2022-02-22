/*
 * smbus_drv.h
 *
 *  Created on: 2018-3-6
 *      Author: Ashley_su
 */

#ifndef SMBUS_IRQ_HANDLE_
#define SMBUS_IRQ_HANDLE_

#include <xparameters.h>
#include "platform.h"

#define smbus_master_communication_fail	                0x1f
#define smbus_master_communication_pass                 0x3f
#define smbus_master_write_blockwrite_timer	            0x05
#define smbus_master_write_before_pinstatus_timer	    0x07
#define smbus_master_write_pinstatus_timer	            0x07
#define smbus_master_read_before_readblock_timer	    0x01
#define smbus_master_read_after_readblock_timer	        0x04

#define clear_	                                        0x00

#define smbus_cmd_type_writemem	             0x18
#define smbus_cmd_type_writephy	             0x1B
#define smbus_cmd_type_writeefuse	         0x19

#define smbus_cmd_type_readmem	             0x38
#define smbus_cmd_type_readphy	             0x3B
#define smbus_cmd_type_readefuse	         0x39
#define smbus_cmd_type_geticstatus	         0x3A
#define smbus_cmd_type_vdcmdenable	         0x01
#define smbus_cmd_type_getflashid	         0x01

#define smbus_cmd_type_reset_to_flash	     0x05
#define smbus_cmd_type_reset_to_rom		     0x02
#define smbus_cmd_type_writeflash		     0x04
#define smbus_cmd_type_readflash		     0x24
#define smbus_cmd_type_eraseflash		     0x03

#define SMBUS_DEV0_ADDR XPAR_AXI_IIC_DUT0_1_BASEADDR
#define SMBUS_DEV1_ADDR XPAR_AXI_IIC_DUT1_1_BASEADDR
#define SMBUS_DEV2_ADDR XPAR_AXI_IIC_DUT2_1_BASEADDR
#define SMBUS_DEV3_ADDR XPAR_AXI_IIC_DUT3_1_BASEADDR

u8 smbus0_irq_handle(u8 smbus_control_buf[]);
u8 smbus1_irq_handle(u8 smbus_control_buf[]);
u8 smbus2_irq_handle(u8 smbus_control_buf[]);
u8 smbus3_irq_handle(u8 smbus_control_buf[]);

#endif /* SMBUS_DRV_H_*/

