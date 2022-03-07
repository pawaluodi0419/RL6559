/*
 * smbus_drv.c
 *
 *  Created on: 2018-3-6
 *      Author: Ashley_su
 */

#include "platform.h"

/******************************************************************************
*
* Config the smbus
*
* @param    devAddr :
*
* @param    chNum :
*
* @param    avgPoints :
*
* @return	adc conversion data
*
* @note		None
*
*
*Note:
*Smbus_process_state:             g_pattern_smbus_control_buf[0]
*Smbus_w/r_m/p/e    :             g_pattern_smbus_control_buf[1]
*Smbus_mem_addrL    :             g_pattern_smbus_control_buf[2]
*Smbus_mem_addrH    :             g_pattern_smbus_control_buf[3]
*Data count         :             g_pattern_smbus_control_buf[4]
*Data               :             g_pattern_smbus_control_buf[5]
*
*
*Note:
*smbus process step:              g_smbus_status_buf[0]
*smbus timer:                     g_smbus_status_buf[1]
*smbus get_pinstatus:             g_smbus_status_buf[2]

*******************************************************************************/

u8 smbus2_irq_handle(u8 smbus_control_buf[])
{

	//u8  smbus_writemem_type;
	//u32 statusreg;
	//u8  smbus_get_pinstatus_result = 0x01;
	//u16 smbus_memAddr;

	u8  i;
	u8  j;
	u8  statusreg;
	u8  Ackbit;
	u8  isr_reg = 0xff;


	u8  smbus_writedata_count = smbus_control_buf[4];
	u8  smbus_writecmd_count;
	u8  smbus_writecmd_buf[30];

	u8  smbus_readcmd_count;

	u8  smbus2_process_state = dut2.g_pattern_smbus_control_buf[0];
	u8  smbus2_cmd_code = smbus_control_buf[1];
	u8  smbus2_cmd_code_add = smbus_control_buf[2];
	u8  smbus_get_pinstatus_result = dut2.g_smbus_status_buf[2];

	//////////xil_printf("smbus_get_pinstatus_result = %d\r\n", smbus_get_pinstatus_result);

	//dut2.g_smbus_style = dut2.g_smbus_status_buf[0];
	//dut2.g_smbus_timer = dut2.g_smbus_status_buf[1];

	//////////xil_printf("smbus2_timer_initial\r\n");
	//////////xil_printf("dut2_smbus_style = %x\r\n", dut2.g_smbus_style);
	//////////xil_printf("dut2_smbus_timer = %x\r\n", dut2.g_smbus_timer);


	//smbus_write//
	if(((smbus2_cmd_code & 0x30) == 0x10) | (smbus2_cmd_code == 0x04))
	{
		switch(smbus2_process_state)
		{
		case smbus_road_waiting:
		{
			//xil_printf("\r\n smbus_road_waiting_doing_write_case00_\r\n");
			//smbus_write_step1_writeblock
			dut2.g_smbus_status_buf[0] = CLEAR_;
			dut2.g_smbus_status_buf[1] = CLEAR_;
			dut2.g_smbus_status_buf[2] = CLEAR_;

			switch(smbus_control_buf[1])
			{
			case smbus_cmd_type_writemem:
			{
				smbus_writecmd_count = smbus_writedata_count + 5;

				smbus_writecmd_buf[0] = smbus_control_buf[1];
				smbus_writecmd_buf[1] = smbus_writedata_count+3;
				smbus_writecmd_buf[2] = smbus_control_buf[2];
				smbus_writecmd_buf[3] = smbus_control_buf[3];
				smbus_writecmd_buf[4] = smbus_control_buf[4];

				for(i=0; i<smbus_writedata_count; i++)
				{
					smbus_writecmd_buf[i+5] = smbus_control_buf[i+5];
				}

				for(j=0;j<smbus_writecmd_count;j++)
				{
					//xil_printf("smbus_writecmd_buf[%x] = %x\r\n", j,smbus_writecmd_buf[j]);
				}
				break;
			}

			case smbus_cmd_type_writephy:
			{
				smbus_writecmd_count = smbus_writedata_count *2 + 4;

				smbus_writecmd_buf[0] = smbus_control_buf[1];
				smbus_writecmd_buf[1] = smbus_writedata_count *2 + 2;
				smbus_writecmd_buf[2] = smbus_control_buf[2];
				smbus_writecmd_buf[3] = smbus_control_buf[3];
				//smbus_writecmd_buf[4] = smbus_control_buf[4];

				for(i=0; i<(smbus_writedata_count*2); i++)
				{
					smbus_writecmd_buf[i+4] = smbus_control_buf[i+5];
				}
				break;
			}

			case smbus_cmd_type_writeefuse:
			{
				smbus_writecmd_count = smbus_writedata_count + 5;

				smbus_writecmd_buf[0] = smbus_control_buf[1];
				smbus_writecmd_buf[1] = smbus_writedata_count+3;
				smbus_writecmd_buf[2] = smbus_control_buf[2];
				smbus_writecmd_buf[3] = smbus_control_buf[3];
				smbus_writecmd_buf[4] = smbus_control_buf[4];

				for(i=0; i<smbus_writedata_count; i++)
				{
					smbus_writecmd_buf[i+5] = smbus_control_buf[i+5];
				}
				break;
			}

			case smbus_cmd_type_writeflash:
			{
				smbus_writecmd_count = smbus_writedata_count + 5;

				smbus_writecmd_buf[0] = smbus_control_buf[1];
				smbus_writecmd_buf[1] = smbus_writedata_count+3;
				smbus_writecmd_buf[2] = smbus_control_buf[2];
				smbus_writecmd_buf[3] = smbus_control_buf[3];
				smbus_writecmd_buf[4] = smbus_control_buf[4];

				for(i=0; i<smbus_writedata_count; i++)
				{
					smbus_writecmd_buf[i+5] = smbus_control_buf[i+5];
				}

				for(j=0;j<smbus_writecmd_count;j++)
				{
					//xil_printf("smbus_writecmd_buf[%x] = %x\r\n", j,smbus_writecmd_buf[j]);
				}
				break;
			}
			}

			smbus_master_write_blockwrite(SMBUS_DEV2_ADDR, smbus_writecmd_buf, smbus_writecmd_count);

			isr_reg = XIic_ReadReg(SMBUS_DEV2_ADDR,  XIIC_IISR_OFFSET);
			if((isr_reg &0x02) != 0)
			{
				dut2.g_result_fail=0x01;
				dut2.g_result_fail_tmrcount = 0xffff;
			}
			//smbus_writedata_buf[smbus_writedata_count] = clear_;
			dut2.g_smbus_status_buf[0]++;
			dut2.g_smbus_status_buf[1] = smbus_master_write_blockwrite_timer;
			dut2.g_smbus_style++;
			dut2.g_smbus_timer = smbus_master_write_blockwrite_timer;

			//xil_printf("waiting_write\r\n");
			//xil_printf("dut2_smbus_style = %x\r\n", dut2.g_smbus_style);
			//xil_printf("dut2_smbus_timer = %x\r\n", dut2.g_smbus_timer);

			dut2.g_pattern_smbus_control_buf[0] = smbus_road_doing;
			smbus2_process_state = smbus_road_doing;
			break;
		}


		case smbus_road_doing:
		{
//			if(dut2.g_smbus_timer != 0)
//			{
//				if((dut2.g_axiTmr2Cnt&0x7f)==0x64)
//				{
//					XGpio_0_WriteBit(0,LED_D7_OFFSET,g_ledD7Tg);
//					g_ledD7Tg=~g_ledD7Tg;
//				}
//			}
//			else
//			{
//				if((dut2.g_axiTmr2Cnt&0x7f)==0x64)
//				{
//					XGpio_0_WriteBit(0,LED_D5_OFFSET,g_ledD5Tg);
//					g_ledD5Tg=~g_ledD5Tg;
//				}

				switch(dut2.g_smbus_style)
				{
				case 0x00:
				{
					//xil_printf("\r\n smbus_road_doing_doing_write_case00_\r\n");
					//smbus_write_step1_writeblock

					dut2.g_smbus_status_buf[0] = CLEAR_;
					dut2.g_smbus_status_buf[1] = CLEAR_;
					dut2.g_smbus_status_buf[2] = CLEAR_;

					switch(smbus_control_buf[1])
					{
					case smbus_cmd_type_writemem:
					{
						smbus_writecmd_count = smbus_writedata_count + 5;

						smbus_writecmd_buf[0] = smbus_control_buf[1];
						smbus_writecmd_buf[1] = smbus_writedata_count+3;
						smbus_writecmd_buf[2] = smbus_control_buf[2];
						smbus_writecmd_buf[3] = smbus_control_buf[3];
						smbus_writecmd_buf[4] = smbus_control_buf[4];

						for(i=0; i<smbus_writedata_count; i++)
						{
							smbus_writecmd_buf[i+5] = smbus_control_buf[i+5];
						}
						//xil_printf("smbus_writecmd_buf[5] = %x\r\n", smbus_writecmd_buf[5]);
						//xil_printf("smbus_writecmd_buf[6] = %x\r\n", smbus_writecmd_buf[6]);
						break;
					}

					case smbus_cmd_type_writephy:
					{
						smbus_writecmd_count = smbus_writedata_count *2 + 4;
						smbus_writecmd_buf[0] = smbus_control_buf[1];
						smbus_writecmd_buf[1] = smbus_writedata_count *2 + 2;
						smbus_writecmd_buf[2] = smbus_control_buf[2];
						smbus_writecmd_buf[3] = smbus_control_buf[3];
						//smbus_writecmd_buf[4] = smbus_control_buf[4];

						for(i=0; i<(smbus_writedata_count*2); i++)
						{
							smbus_writecmd_buf[i+4] = smbus_control_buf[i+5];
						}
						break;
					}

					case smbus_cmd_type_writeefuse:
					{
						smbus_writecmd_count = smbus_writedata_count + 5;
						smbus_writecmd_buf[0] = smbus_control_buf[1];
						smbus_writecmd_buf[1] = smbus_writedata_count+3;
						smbus_writecmd_buf[2] = smbus_control_buf[2];
						smbus_writecmd_buf[3] = smbus_control_buf[3];
						smbus_writecmd_buf[4] = smbus_control_buf[4];

						for(i=0; i<smbus_writedata_count; i++)
						{
							smbus_writecmd_buf[i+5] = smbus_control_buf[i+5];
						}
						break;
					}

					case smbus_cmd_type_writeflash:
					{
						smbus_writecmd_count = smbus_writedata_count + 5;
						smbus_writecmd_buf[0] = smbus_control_buf[1];
						smbus_writecmd_buf[1] = smbus_writedata_count+3;
						smbus_writecmd_buf[2] = smbus_control_buf[2];
						smbus_writecmd_buf[3] = smbus_control_buf[3];
						smbus_writecmd_buf[4] = smbus_control_buf[4];

						for(i=0; i<smbus_writedata_count; i++)
						{
							smbus_writecmd_buf[i+5] = smbus_control_buf[i+5];
						}
						break;
					}
					}

					smbus_master_write_blockwrite(SMBUS_DEV2_ADDR, smbus_writecmd_buf, smbus_writecmd_count);
					isr_reg = XIic_ReadReg(SMBUS_DEV2_ADDR,  XIIC_IISR_OFFSET);
					if((isr_reg &0x02) != 0)
					{
						dut2.g_result_fail=0x01;
						dut2.g_result_fail_tmrcount = 0xffff;
					}
					//smbus_writedata_buf[smbus_writedata_count] = clear_;
					dut2.g_smbus_status_buf[0]++;
					dut2.g_smbus_status_buf[1] = smbus_master_write_blockwrite_timer;
					//dut2.g_smbus_style = dut2.g_smbus_status_buf[0];
					//dut2.g_smbus_timer = dut2.g_smbus_status_buf[1];
					dut2.g_smbus_style++;
					dut2.g_smbus_timer = smbus_master_write_blockwrite_timer;

					dut2.g_pattern_smbus_control_buf[0] = smbus_road_doing;
					smbus2_process_state = smbus_road_doing;
					//xil_printf("doing_write_case00\r\n");
					//xil_printf("dut2_smbus_style = %x\r\n", dut2.g_smbus_style);
					//xil_printf("dut2_smbus_timer = %x\r\n", dut2.g_smbus_timer);
					break;
				}

				case 0x01:
				{
					//xil_printf("\r\n doing_write_case01\r\n");
					statusreg = XIic_ReadReg(SMBUS_DEV2_ADDR,  XIIC_SR_REG_OFFSET);
					if((statusreg & XIIC_SR_BUS_BUSY_MASK) == 1)
					{
						dut2.g_pattern_smbus_control_buf[0] = smbus_master_communication_fail;
						return 0;
					}
					else
					{
						dut2.g_smbus_status_buf[0]++;
						dut2.g_smbus_status_buf[1] = smbus_master_write_before_pinstatus_timer;
						dut2.g_smbus_style++;
						dut2.g_smbus_timer = smbus_master_write_before_pinstatus_timer;

						dut2.g_smbus_retry_timer = 0x32;
						dut2.g_pattern_smbus_control_buf[0] = smbus_road_doing;
						smbus2_process_state = smbus_road_doing;

						//xil_printf("doing_write_case01\r\n");
						//xil_printf("dut2_smbus_style = %x\r\n", dut2.g_smbus_style);
						//xil_printf("dut2_smbus_timer = %x\r\n", dut2.g_smbus_timer);
					}
					break;
				}

				case 0x02:
				{
					usdelay(10);
					//xil_printf("\r\n doing_write_case02\r\n");
					smbus_get_pinstatus_result = smbus_master_get_pinstate_dut2(SMBUS_DEV2_ADDR, smbus_get_pinstatus_result);
					//xil_printf("smbus_get_pinstatus_result = %d\r\n", smbus_get_pinstatus_result);

					//dut2.g_smbus_status_buf[0]++;
					dut2.g_smbus_status_buf[1] = smbus_master_write_pinstatus_timer;
					//dut2.g_smbus_style++;
					dut2.g_smbus_timer = smbus_master_write_pinstatus_timer;

					dut2.g_pattern_smbus_control_buf[0] = smbus_road_doing;
					smbus2_process_state = smbus_road_doing;

					switch(smbus_get_pinstatus_result)
					{
					case 0x00:
					{
						if(dut2.g_smbus_retry_timer >0)
						{
							dut2.g_smbus_retry_timer--;
						}
						else
						{
							dut2.g_result_fail=0x01;
							dut2.g_result_fail_tmrcount = 0xffff;
							dut2.g_smbus_status_buf[0] = clear_;
							dut2.g_smbus_status_buf[1] = clear_;
							dut2.g_smbus_status_buf[2] = clear_;
							dut2.g_smbus_style = clear_;
							dut2.g_smbus_timer = clear_;
						}
						break;
					}

					case 0x01:
					{
						dut2.g_smbus_status_buf[0]++;
						dut2.g_smbus_status_buf[2] = 0x01;
						dut2.g_smbus_style++;
						break;
					}

					case 0x02:
					{
						if(dut2.g_smbus_retry_timer >0)
						{
							dut2.g_smbus_retry_timer--;
						}
						else
						{
							dut2.g_result_fail=0x01;
							dut2.g_result_fail_tmrcount = 0xffff;
							dut2.g_smbus_status_buf[0] = clear_;
							dut2.g_smbus_status_buf[1] = clear_;
							dut2.g_smbus_status_buf[2] = clear_;
							dut2.g_smbus_style = clear_;
							dut2.g_smbus_timer = clear_;
						}
						break;
					}

					case 0x03:
					{
						dut2.g_result_fail=0x01;
						dut2.g_result_fail_tmrcount = 0xffff;
						dut2.g_smbus_status_buf[0] = clear_;
						dut2.g_smbus_status_buf[1] = clear_;
						dut2.g_smbus_status_buf[2] = clear_;
						dut2.g_smbus_style = clear_;
						dut2.g_smbus_timer = clear_;

						break;
					}
					}

					//xil_printf("doing_write_case02\r\n");
					//xil_printf("smbus_get_pinstatus_result = %d\r\n", smbus_get_pinstatus_result);
					//xil_printf("dut2_smbus_style = %x\r\n", dut2.g_smbus_style);
					//xil_printf("dut2_smbus_timer = %x\r\n", dut2.g_smbus_timer);
					break;
				}

				case 0x03:
				{
					//xil_printf("\r\n doing_write_case03\r\n");
					statusreg = XIic_ReadReg(SMBUS_DEV2_ADDR,  XIIC_SR_REG_OFFSET);

					//xil_printf("smbus_get_pinstatus_result = %d\r\n", smbus_get_pinstatus_result);
					//xil_printf("dut2_statusreg = %d\r\n", statusreg);

					if((statusreg & XIIC_SR_BUS_BUSY_MASK) == 0x01)
					{
						dut2.g_pattern_smbus_control_buf[0] = smbus_master_communication_fail;
						smbus2_process_state = smbus_master_communication_fail;

						dut2.g_smbus_status_buf[0] = clear_;
						dut2.g_smbus_status_buf[1] = clear_;
						dut2.g_smbus_style = clear_;
						dut2.g_smbus_timer = clear_;
						dut2.g_smbus_status_buf[2] = clear_;
					}
					else if((statusreg & XIIC_SR_BUS_BUSY_MASK) == 0)
					{
						dut2.g_pattern_smbus_control_buf[0] = smbus_master_communication_pass;
						smbus2_process_state = smbus_master_communication_pass;

						dut2.g_smbus_status_buf[0] = clear_;
						dut2.g_smbus_status_buf[1] = clear_;
						dut2.g_smbus_style = clear_;
						dut2.g_smbus_timer = clear_;
						dut2.g_smbus_status_buf[2] = clear_;
					}
					else
					{
						dut2.g_pattern_smbus_control_buf[0] = smbus_master_communication_fail;

						dut2.g_smbus_status_buf[0] = clear_;
						dut2.g_smbus_status_buf[1] = clear_;
						dut2.g_smbus_status_buf[2] = clear_;
						dut2.g_smbus_style = clear_;
						dut2.g_smbus_timer = clear_;
					}
					break;
				}
				}
//			}
			break;
		}
		}
	}

	//smbus_read//
	if(((smbus2_cmd_code & 0x30) == 0x30) | (smbus2_cmd_code == 0x24)  | (smbus2_cmd_code == 0x26)| ((smbus2_cmd_code == 0x01) && (smbus2_cmd_code_add == 0x04)))
	{
		switch(smbus2_process_state)
		{
		case smbus_road_waiting:
		{
			//xil_printf("dut2_smbus_road_waiting_doing_read_case0000\r\n");
			dut2.g_smbus_status_buf[0] = CLEAR_;
			dut2.g_smbus_status_buf[1] = CLEAR_;
			dut2.g_smbus_status_buf[2] = CLEAR_;

		    smbus_writecmd_buf[0] = smbus_control_buf[1];
		    smbus_writecmd_buf[1] = smbus_writedata_count+2;
		    smbus_writecmd_buf[2] = smbus_control_buf[2];
		    smbus_writecmd_buf[3] = smbus_control_buf[3];
		    smbus_writecmd_buf[4] = smbus_control_buf[4];

		    if(smbus_control_buf[1] == smbus_cmd_type_geticstatus)
		    {
		    	smbus_writecmd_buf[1] = 0x03;
		    }
		    if(smbus_control_buf[1] == smbus_cmd_type_readefuse)
		    {
		    	smbus_writecmd_buf[1] = 0x03;
		    }
		    if(smbus_control_buf[1] == smbus_cmd_type_getflashid)
		    {
		    	smbus_writecmd_buf[1] = 0x03;
		    }
		    if(smbus_control_buf[1] == smbus_cmd_type_readflash_64K)
		    {
		    	smbus_writecmd_buf[1] = 0x03;
		    }
		    if(smbus_control_buf[1] == smbus_cmd_type_readflash_128K)
		    {
		    	smbus_writecmd_buf[1] = 0x03;
		    }

		    smbus_master_write_blockwrite(SMBUS_DEV2_ADDR, smbus_writecmd_buf, 5);
			isr_reg = XIic_ReadReg(SMBUS_DEV2_ADDR,  XIIC_IISR_OFFSET);	//interrupt status register
			if((isr_reg &0x02) != 0)	//bit1: transmit error
			{
				dut2.g_result_fail=0x01;
				dut2.g_result_fail_tmrcount = 0xffff;
			}
			//smbus_writedata_buf[smbus_writedata_count] = clear_;
			dut2.g_smbus_status_buf[0]++;
			dut2.g_smbus_status_buf[1] = smbus_master_write_blockwrite_timer;
			dut2.g_smbus_style++;
			dut2.g_smbus_timer = smbus_master_write_blockwrite_timer;

			//xil_printf("waiting_write\r\n");
			//xil_printf("dut2_smbus_style = %x\r\n", dut2.g_smbus_style);
			//xil_printf("dut2_smbus_timer = %x\r\n", dut2.g_smbus_timer);

			dut2.g_pattern_smbus_control_buf[0] = smbus_road_doing;
			smbus2_process_state = smbus_road_doing;
			break;
		}

		case smbus_road_doing:
		{
			switch(dut2.g_smbus_style)
			{
			case 0x00:
			{
				//smbus_read_step1_writeblock
				//xil_printf("\r\n dut2_smbus_road_doing_doing_read_case00\r\n");

				dut2.g_smbus_status_buf[0] = CLEAR_;
				dut2.g_smbus_status_buf[1] = CLEAR_;
				dut2.g_smbus_status_buf[2] = CLEAR_;

			    smbus_writecmd_buf[0] = smbus_control_buf[1];
			    smbus_writecmd_buf[1] = smbus_writedata_count+2;
			    smbus_writecmd_buf[2] = smbus_control_buf[2];
			    smbus_writecmd_buf[3] = smbus_control_buf[3];
			    smbus_writecmd_buf[4] = smbus_control_buf[4];

				smbus_master_write_blockwrite(SMBUS_DEV2_ADDR, smbus_writecmd_buf, 5);
				isr_reg = XIic_ReadReg(SMBUS_DEV2_ADDR,  XIIC_IISR_OFFSET);
				if((isr_reg &0x02) != 0)
				{
					dut2.g_result_fail=0x01;
					dut2.g_result_fail_tmrcount = 0xffff;
				}
				dut2.g_smbus_status_buf[0]++;
				dut2.g_smbus_status_buf[1] = smbus_master_write_blockwrite_timer;
				dut2.g_smbus_style++;
				dut2.g_smbus_timer = smbus_master_write_blockwrite_timer;

				dut2.g_pattern_smbus_control_buf[0] = smbus_road_doing;
				smbus2_process_state = smbus_road_doing;
				//////////xil_printf("doing_read_case00\r\n");
				//////////xil_printf("dut2_smbus_style = %x\r\n", dut2.g_smbus_style);
				//////////xil_printf("dut2_smbus_timer = %x\r\n", dut2.g_smbus_timer);
				break;
			}

			case 0x01:
			{
		        //xil_printf("\r\n doing_read_case01\r\n");
				statusreg = XIic_ReadReg(SMBUS_DEV2_ADDR,  XIIC_SR_REG_OFFSET);	//status register bit2
				if((statusreg & XIIC_SR_BUS_BUSY_MASK) == 1)
				{
					dut2.g_pattern_smbus_control_buf[0] = smbus_master_communication_fail;
					return 0;

				}
				else
				{
					dut2.g_smbus_status_buf[0]++;
					dut2.g_smbus_status_buf[1] = smbus_master_write_before_pinstatus_timer;
					dut2.g_smbus_style++;
					dut2.g_smbus_timer = smbus_master_write_before_pinstatus_timer;

					dut2.g_smbus_retry_timer = 0x4e20;
					dut2.g_pattern_smbus_control_buf[0] = smbus_road_doing;
					smbus2_process_state = smbus_road_doing;

					//xil_printf("dut2.g_smbus_status_buf[2] = %x\r\n", dut2.g_smbus_status_buf[2]);
					//xil_printf("doing_write_case01\r\n");
					//xil_printf("dut2_smbus_style = %x\r\n", dut2.g_smbus_style);
					//xil_printf("dut2_smbus_timer = %x\r\n", dut2.g_smbus_timer);
				}
				break;
			}

			case 0x02:
			{
		        usdelay(100);
				//dut2.g_smbus_status_buf[2] = smbus_master_get_pinstate(SMBUS_DEV2_ADDR, smbus_get_pinstatus_result);
				//xil_printf("dut2_smbus_pinstatus = %x\r\n", dut2.g_smbus_status_buf[2]);
				//xil_printf("\r\n doing_read_case02\r\n");

				smbus_get_pinstatus_result = smbus_master_get_pinstate_dut2(SMBUS_DEV2_ADDR, smbus_get_pinstatus_result);
				//xil_printf("smbus_get_pinstatus_result = %x\r\n", smbus_get_pinstatus_result);
				usdelay(20);
				//dut2.g_smbus_status_buf[0]++;
				dut2.g_smbus_status_buf[1] = smbus_master_write_pinstatus_timer;
				//dut2.g_smbus_style++;
				dut2.g_smbus_timer = smbus_master_write_pinstatus_timer;

				dut2.g_pattern_smbus_control_buf[0] = smbus_road_doing;
				smbus2_process_state = smbus_road_doing;
				//dut2.g_smbus_retry_timer--;

				switch(smbus_get_pinstatus_result)
				{
				case 0x00:
				{
					if(dut2.g_smbus_retry_timer >0)
					{
						dut2.g_smbus_retry_timer--;
					}
					else
					{
						dut2.g_result_fail=0x01;
						dut2.g_result_fail_tmrcount = 0xffff;
						dut2.g_smbus_status_buf[0] = clear_;
						dut2.g_smbus_status_buf[1] = clear_;
						dut2.g_smbus_status_buf[2] = clear_;
						dut2.g_smbus_style = clear_;
						dut2.g_smbus_timer = clear_;
					}
					break;
				}

				case 0x01:
				{
					dut2.g_smbus_status_buf[0]++;
					dut2.g_smbus_status_buf[2] = 0x01;
					dut2.g_smbus_style++;
					break;
				}

				case 0x02:
				{
					if(dut2.g_smbus_retry_timer >0)
					{
						dut2.g_smbus_retry_timer--;
					}
					else
					{
						dut2.g_result_fail=0x01;
						dut2.g_result_fail_tmrcount = 0xffff;
						dut2.g_smbus_status_buf[0] = clear_;
						dut2.g_smbus_status_buf[1] = clear_;
						dut2.g_smbus_status_buf[2] = clear_;
						dut2.g_smbus_style = clear_;
						dut2.g_smbus_timer = clear_;
					}
					break;
				}

				case 0x03:
				{
					dut2.g_result_fail=0x01;
					dut2.g_result_fail_tmrcount = 0xffff;
					dut2.g_smbus_status_buf[0] = clear_;
					dut2.g_smbus_status_buf[1] = clear_;
					dut2.g_smbus_status_buf[2] = clear_;
					dut2.g_smbus_style = clear_;
					dut2.g_smbus_timer = clear_;

					break;
				}
				}

				//xil_printf("dut2.g_smbus_status_buf[2] = %x\r\n", dut2.g_smbus_status_buf[2]);
				//xil_printf("doing_write_case02\r\n");
				//xil_printf("dut2_smbus_style = %x\r\n", dut2.g_smbus_style);
				//xil_printf("dut2_smbus_timer = %x\r\n", dut2.g_smbus_timer);
				break;
			}

			case 0x03:
			{
				//xil_printf("\r\n doing_read_case03\r\n");
				statusreg = XIic_ReadReg(SMBUS_DEV2_ADDR,  XIIC_SR_REG_OFFSET);
				//xil_printf("dut2_smbus_pinstatus = %x\r\n", dut2.g_smbus_status_buf[2]);

				if((statusreg & XIIC_SR_BUS_BUSY_MASK) == 0x01)
				{
					dut2.g_pattern_smbus_control_buf[0] = smbus_master_communication_fail;
					smbus2_process_state = smbus_master_communication_fail;

					dut2.g_smbus_status_buf[0]++;
					dut2.g_smbus_status_buf[1] = smbus_master_read_before_readblock_timer;
					dut2.g_smbus_style++;
					dut2.g_smbus_timer = smbus_master_read_before_readblock_timer;
				}
				else if((statusreg & XIIC_SR_BUS_BUSY_MASK) == 0)
				{
					dut2.g_pattern_smbus_control_buf[0] = smbus_road_doing;

					dut2.g_smbus_status_buf[0]++;
					dut2.g_smbus_status_buf[1] = smbus_master_read_before_readblock_timer;
					dut2.g_smbus_style++;
					dut2.g_smbus_timer = smbus_master_read_before_readblock_timer;
				}

				//xil_printf("dut2.g_smbus_status_buf[2] = %x\r\n", dut2.g_smbus_status_buf[2]);
				//xil_printf("doing_write_case03\r\n");
				//xil_printf("dut2_smbus_style = %x\r\n", dut2.g_smbus_style);
				//xil_printf("dut2_smbus_timer = %x\r\n", dut2.g_smbus_timer);
				break;
			}

			case 0x04:
			{
				//xil_printf("\r\n doing_read_case04\r\n");
				switch(smbus_control_buf[1])
				{
				case smbus_cmd_type_readmem:
				{
					smbus_readcmd_count = smbus_control_buf[4];
					break;
				}
				case smbus_cmd_type_readflash_64K:
				{
					smbus_readcmd_count = smbus_control_buf[4];
					break;
				}
				case smbus_cmd_type_readflash_128K:
				{
					smbus_readcmd_count = smbus_control_buf[4];
					break;
				}
				case smbus_cmd_type_readphy:
				{
					smbus_readcmd_count = smbus_control_buf[4]*2;
					break;
				}
				case smbus_cmd_type_readefuse:
				{
					smbus_readcmd_count = smbus_control_buf[4];
					break;
				}
				case smbus_cmd_type_geticstatus:
				{
					smbus_readcmd_count = smbus_control_buf[4];
					break;
				}
				case smbus_cmd_type_getflashid:
				{
					smbus_readcmd_count = smbus_control_buf[4];
					break;
				}
				}

				smbus_master_read_blockread_dut2(SMBUS_DEV2_ADDR, smbus_readcmd_count);
				//xil_printf("dut2_smbus_read_datacount = %x\r\n", smbus_control_buf[4]);
				dut2.g_smbus_status_buf[0]++;
				dut2.g_smbus_status_buf[1] = smbus_master_write_pinstatus_timer;
				dut2.g_smbus_style++;
				dut2.g_smbus_timer = smbus_master_read_after_readblock_timer;
				//xil_printf("dut2.g_smbus_status_buf[2] = %x\r\n", dut2.g_smbus_status_buf[2]);
				break;
			}

			case 0x05:
			{
				//xil_printf("\r\n doing_read_case05\r\n");
				statusreg = XIic_ReadReg(SMBUS_DEV2_ADDR,  XIIC_SR_REG_OFFSET);
				//xil_printf("statusreg = %x\r\n", statusreg);
				if(dut2.g_smbus_status_buf[2] == 0x01)
				{
					if((statusreg & XIIC_SR_BUS_BUSY_MASK) == 0x01)
					{
						dut2.g_pattern_smbus_control_buf[0] = smbus_master_communication_fail;
						smbus2_process_state = smbus_master_communication_fail;
					}
					else if((statusreg & XIIC_SR_BUS_BUSY_MASK) == 0)
					{
						dut2.g_pattern_smbus_control_buf[0] = smbus_master_communication_pass;
					}
				}
				else
				{
					dut2.g_pattern_smbus_control_buf[0] = smbus_master_communication_fail;
				}
				dut2.g_smbus_status_buf[0] = clear_;
				dut2.g_smbus_status_buf[1] = clear_;
				dut2.g_smbus_status_buf[2] = clear_;
				dut2.g_smbus_style = clear_;
				dut2.g_smbus_timer = clear_;

				//xil_printf("doing_write_case03\r\n");
				//xil_printf("dut2_smbus_style = %x\r\n", dut2.g_smbus_style);
				//xil_printf("dut2_smbus_timer = %x\r\n", dut2.g_smbus_timer);
				//xil_printf("dut2.g_smbus_status_buf[2] = %x\r\n", dut2.g_smbus_status_buf[2]);
				break;
			}
			}
			break;
		}
		}
	}

	//smbus_reset_to_flash//
	if(smbus2_cmd_code == 0x05)
	{
		switch(smbus2_process_state)
		{
		case smbus_road_waiting:
		{
			dut2.g_smbus_status_buf[0] = CLEAR_;
			dut2.g_smbus_status_buf[1] = CLEAR_;
			dut2.g_smbus_status_buf[2] = CLEAR_;

		    smbus_writecmd_buf[0] = smbus_control_buf[1];
		    smbus_writecmd_buf[1] = 0x03;
		    smbus_writecmd_buf[2] = smbus_control_buf[2];
		    smbus_writecmd_buf[3] = smbus_control_buf[3];
		    smbus_writecmd_buf[4] = smbus_control_buf[4];

		    smbus_master_write_blockwrite(SMBUS_DEV2_ADDR, smbus_writecmd_buf, 5);
			isr_reg = XIic_ReadReg(SMBUS_DEV2_ADDR,  XIIC_IISR_OFFSET);
			if((isr_reg &0x02) != 0)
			{
				dut2.g_result_fail=0x01;
				dut2.g_result_fail_tmrcount = 0xffff;
			}
			//smbus_writedata_buf[smbus_writedata_count] = clear_;
			dut2.g_smbus_status_buf[0]++;
			dut2.g_smbus_status_buf[1] = smbus_master_write_blockwrite_timer;
			dut2.g_smbus_style++;
			dut2.g_smbus_timer = smbus_master_write_blockwrite_timer;

			//xil_printf("waiting_write\r\n");
			//xil_printf("dut2_smbus_style = %x\r\n", dut2.g_smbus_style);
			//xil_printf("dut2_smbus_timer = %x\r\n", dut2.g_smbus_timer);

			dut2.g_pattern_smbus_control_buf[0] = smbus_road_doing;
			smbus2_process_state = smbus_road_doing;
			break;
		}

		case smbus_road_doing:
		{
//			if(dut2.g_smbus_timer != 0)
//			{
//				if((dut2.g_axiTmr2Cnt&0x7f)==0x64)
//				{
//					XGpio_0_WriteBit(0,LED_D7_OFFSET,g_ledD7Tg);
//					g_ledD7Tg=~g_ledD7Tg;
//				}
//			}
//			else
//			{
//				if((dut2.g_axiTmr2Cnt&0x7f)==0x64)
//				{
//					XGpio_0_WriteBit(0,LED_D5_OFFSET,g_ledD5Tg);
//					g_ledD5Tg=~g_ledD5Tg;
//				}

				switch(dut2.g_smbus_style)
				{
				case 0x00:
				{
					dut2.g_smbus_status_buf[0] = CLEAR_;
					dut2.g_smbus_status_buf[1] = CLEAR_;
					dut2.g_smbus_status_buf[2] = CLEAR_;

				    smbus_writecmd_buf[0] = smbus_control_buf[1];
				    smbus_writecmd_buf[1] = 0x03;
				    smbus_writecmd_buf[2] = smbus_control_buf[2];
				    smbus_writecmd_buf[3] = smbus_control_buf[3];
				    smbus_writecmd_buf[4] = smbus_control_buf[4];

				    smbus_master_write_blockwrite(SMBUS_DEV2_ADDR, smbus_writecmd_buf, 5);
					isr_reg = XIic_ReadReg(SMBUS_DEV2_ADDR,  XIIC_IISR_OFFSET);
					if((isr_reg &0x02) != 0)
					{
						dut2.g_result_fail=0x01;
						dut2.g_result_fail_tmrcount = 0xffff;
					}
					//smbus_writedata_buf[smbus_writedata_count] = clear_;
					dut2.g_smbus_status_buf[0]++;
					dut2.g_smbus_status_buf[1] = smbus_master_write_blockwrite_timer;
					//dut2.g_smbus_style = dut2.g_smbus_status_buf[0];
					//dut2.g_smbus_timer = dut2.g_smbus_status_buf[1];
					dut2.g_smbus_style++;
					dut2.g_smbus_timer = smbus_master_write_blockwrite_timer;

					dut2.g_pattern_smbus_control_buf[0] = smbus_road_doing;
					smbus2_process_state = smbus_road_doing;
					//xil_printf("doing_write_case00\r\n");
					//xil_printf("dut2_smbus_style = %x\r\n", dut2.g_smbus_style);
					//xil_printf("dut2_smbus_timer = %x\r\n", dut2.g_smbus_timer);
					break;
				}
				/*
				case 0x01:
				{
					//xil_printf("\r\n doing_write_case01\r\n");
					statusreg = XIic_ReadReg(SMBUS_DEV2_ADDR,  XIIC_SR_REG_OFFSET);
					if((statusreg & XIIC_SR_BUS_BUSY_MASK) == 1)
					{
						dut2.g_pattern_smbus_control_buf[0] = smbus_master_communication_fail;
						return 0;
					}
					else
					{
						dut2.g_smbus_status_buf[0]++;
						dut2.g_smbus_status_buf[1] = smbus_master_write_before_pinstatus_timer;
						dut2.g_smbus_style++;
						dut2.g_smbus_timer = smbus_master_write_before_pinstatus_timer;

						dut2.g_smbus_retry_timer = 0x32;
						dut2.g_pattern_smbus_control_buf[0] = smbus_road_doing;
						smbus2_process_state = smbus_road_doing;

						//xil_printf("doing_write_case01\r\n");
						//xil_printf("dut2_smbus_style = %x\r\n", dut2.g_smbus_style);
						//xil_printf("dut2_smbus_timer = %x\r\n", dut2.g_smbus_timer);
					}
					break;
				}

				case 0x02:
				{
					usdelay(10);
					//xil_printf("\r\n doing_write_case02\r\n");
					smbus_get_pinstatus_result = smbus_master_get_pinstate_dut2(SMBUS_DEV2_ADDR, smbus_get_pinstatus_result);
					//xil_printf("smbus_get_pinstatus_result = %d\r\n", smbus_get_pinstatus_result);

					//dut2.g_smbus_status_buf[0]++;
					dut2.g_smbus_status_buf[1] = smbus_master_write_pinstatus_timer;
					//dut2.g_smbus_style++;
					dut2.g_smbus_timer = smbus_master_write_pinstatus_timer;

					dut2.g_pattern_smbus_control_buf[0] = smbus_road_doing;
					smbus2_process_state = smbus_road_doing;

					switch(smbus_get_pinstatus_result)
					{
					case 0x00:
					{
						if(dut2.g_smbus_retry_timer >0)
						{
							dut2.g_smbus_retry_timer--;
						}
						else
						{
							dut2.g_result_fail=0x01;
							dut2.g_result_fail_tmrcount = 0xffff;
							dut2.g_smbus_status_buf[0] = clear_;
							dut2.g_smbus_status_buf[1] = clear_;
							dut2.g_smbus_status_buf[2] = clear_;
							dut2.g_smbus_style = clear_;
							dut2.g_smbus_timer = clear_;
						}
						break;
					}
					case 0x01:
					{
						dut2.g_smbus_status_buf[0]++;
						dut2.g_smbus_status_buf[2] = 0x01;
						dut2.g_smbus_style++;
						break;
					}
					case 0x02:
					{
						if(dut2.g_smbus_retry_timer >0)
						{
							dut2.g_smbus_retry_timer--;
						}
						else
						{
							dut2.g_result_fail=0x01;
							dut2.g_result_fail_tmrcount = 0xffff;
							dut2.g_smbus_status_buf[0] = clear_;
							dut2.g_smbus_status_buf[1] = clear_;
							dut2.g_smbus_status_buf[2] = clear_;
							dut2.g_smbus_style = clear_;
							dut2.g_smbus_timer = clear_;
						}
						break;
					}
					case 0x03:
					{
						dut2.g_result_fail=0x01;
						dut2.g_result_fail_tmrcount = 0xffff;
						dut2.g_smbus_status_buf[0] = clear_;
						dut2.g_smbus_status_buf[1] = clear_;
						dut2.g_smbus_status_buf[2] = clear_;
						dut2.g_smbus_style = clear_;
						dut2.g_smbus_timer = clear_;
						break;
					}
					}

					//xil_printf("doing_write_case02\r\n");
					//xil_printf("smbus_get_pinstatus_result = %d\r\n", smbus_get_pinstatus_result);
					//xil_printf("dut2_smbus_style = %x\r\n", dut2.g_smbus_style);
					//xil_printf("dut2_smbus_timer = %x\r\n", dut2.g_smbus_timer);
					break;
				}
				*/
				case 0x01:
				{
					//xil_printf("\r\n doing_write_case03\r\n");
					statusreg = XIic_ReadReg(SMBUS_DEV2_ADDR,  XIIC_SR_REG_OFFSET);

					//xil_printf("smbus_get_pinstatus_result = %d\r\n", smbus_get_pinstatus_result);
					//xil_printf("dut2_statusreg = %d\r\n", statusreg);

					if((statusreg & XIIC_SR_BUS_BUSY_MASK) == 0x01)
					{
						dut2.g_pattern_smbus_control_buf[0] = smbus_master_communication_fail;
						smbus2_process_state = smbus_master_communication_fail;

						dut2.g_smbus_status_buf[0] = clear_;
						dut2.g_smbus_status_buf[1] = clear_;
						dut2.g_smbus_style = clear_;
						dut2.g_smbus_timer = clear_;
						dut2.g_smbus_status_buf[2] = clear_;
					}
					else if((statusreg & XIIC_SR_BUS_BUSY_MASK) == 0)
					{
						dut2.g_pattern_smbus_control_buf[0] = smbus_master_communication_pass;
						smbus2_process_state = smbus_master_communication_pass;

						dut2.g_smbus_status_buf[0] = clear_;
						dut2.g_smbus_status_buf[1] = clear_;
						dut2.g_smbus_style = clear_;
						dut2.g_smbus_timer = clear_;
						dut2.g_smbus_status_buf[2] = clear_;
					}
					else
					{
						dut2.g_pattern_smbus_control_buf[0] = smbus_master_communication_fail;

						dut2.g_smbus_status_buf[0] = clear_;
						dut2.g_smbus_status_buf[1] = clear_;
						dut2.g_smbus_status_buf[2] = clear_;
						dut2.g_smbus_style = clear_;
						dut2.g_smbus_timer = clear_;
					}
					break;
				}
				}
//			}
			break;
		}
		}
	}

	//smbus_reset_to_rom/smbus_erase_flash//
	if(((smbus2_cmd_code == 0x01) && (smbus2_cmd_code_add == 0xda)) | (smbus2_cmd_code == 0x02) | (smbus2_cmd_code == 0x03))
	{
		switch(smbus2_process_state)
		{
		case smbus_road_waiting:
		{
			dut2.g_smbus_status_buf[0] = CLEAR_;
			dut2.g_smbus_status_buf[1] = CLEAR_;
			dut2.g_smbus_status_buf[2] = CLEAR_;

		    smbus_writecmd_buf[0] = smbus_control_buf[1];
		    smbus_writecmd_buf[1] = 0x03;
		    smbus_writecmd_buf[2] = smbus_control_buf[2];
		    smbus_writecmd_buf[3] = smbus_control_buf[3];
		    smbus_writecmd_buf[4] = smbus_control_buf[4];

		    smbus_master_write_blockwrite(SMBUS_DEV2_ADDR, smbus_writecmd_buf, 5);
			isr_reg = XIic_ReadReg(SMBUS_DEV2_ADDR,  XIIC_IISR_OFFSET);
			if((isr_reg &0x02) != 0)
			{
				dut2.g_result_fail=0x01;
				dut2.g_result_fail_tmrcount = 0xffff;
			}
			//smbus_writedata_buf[smbus_writedata_count] = clear_;
			dut2.g_smbus_status_buf[0]++;
			dut2.g_smbus_status_buf[1] = smbus_master_write_blockwrite_timer;
			dut2.g_smbus_style++;
			dut2.g_smbus_timer = smbus_master_write_blockwrite_timer;

			//xil_printf("waiting_write\r\n");
			//xil_printf("dut2_smbus_style = %x\r\n", dut2.g_smbus_style);
			//xil_printf("dut2_smbus_timer = %x\r\n", dut2.g_smbus_timer);

			dut2.g_pattern_smbus_control_buf[0] = smbus_road_doing;
			smbus2_process_state = smbus_road_doing;
			break;
		}

		case smbus_road_doing:
		{
//			if(dut2.g_smbus_timer != 0)
//			{
//				if((dut2.g_axiTmr2Cnt&0x7f)==0x64)
//				{
//					XGpio_0_WriteBit(0,LED_D7_OFFSET,g_ledD7Tg);
//					g_ledD7Tg=~g_ledD7Tg;
//				}
//			}
//			else
//			{
//				if((dut2.g_axiTmr2Cnt&0x7f)==0x64)
//				{
//					XGpio_0_WriteBit(0,LED_D5_OFFSET,g_ledD5Tg);
//					g_ledD5Tg=~g_ledD5Tg;
//				}

				switch(dut2.g_smbus_style)
				{
				case 0x00:
				{
					dut2.g_smbus_status_buf[0] = CLEAR_;
					dut2.g_smbus_status_buf[1] = CLEAR_;
					dut2.g_smbus_status_buf[2] = CLEAR_;

				    smbus_writecmd_buf[0] = smbus_control_buf[1];
				    smbus_writecmd_buf[1] = 0x03;
				    smbus_writecmd_buf[2] = smbus_control_buf[2];
				    smbus_writecmd_buf[3] = smbus_control_buf[3];
				    smbus_writecmd_buf[4] = smbus_control_buf[4];

				    smbus_master_write_blockwrite(SMBUS_DEV2_ADDR, smbus_writecmd_buf, 5);
					isr_reg = XIic_ReadReg(SMBUS_DEV2_ADDR,  XIIC_IISR_OFFSET);
					if((isr_reg &0x02) != 0)
					{
						dut2.g_result_fail=0x01;
						dut2.g_result_fail_tmrcount = 0xffff;
					}
					//smbus_writedata_buf[smbus_writedata_count] = clear_;
					dut2.g_smbus_status_buf[0]++;
					dut2.g_smbus_status_buf[1] = smbus_master_write_blockwrite_timer;
					//dut2.g_smbus_style = dut2.g_smbus_status_buf[0];
					//dut2.g_smbus_timer = dut2.g_smbus_status_buf[1];
					dut2.g_smbus_style++;
					dut2.g_smbus_timer = smbus_master_write_blockwrite_timer;

					dut2.g_pattern_smbus_control_buf[0] = smbus_road_doing;
					smbus2_process_state = smbus_road_doing;
					//xil_printf("doing_write_case00\r\n");
					//xil_printf("dut2_smbus_style = %x\r\n", dut2.g_smbus_style);
					//xil_printf("dut2_smbus_timer = %x\r\n", dut2.g_smbus_timer);
					break;
				}

				case 0x01:
				{
					//xil_printf("\r\n doing_write_case01\r\n");
					statusreg = XIic_ReadReg(SMBUS_DEV2_ADDR,  XIIC_SR_REG_OFFSET);
					if((statusreg & XIIC_SR_BUS_BUSY_MASK) == 1)
					{
						dut2.g_pattern_smbus_control_buf[0] = smbus_master_communication_fail;
						return 0;
					}
					else
					{
						dut2.g_smbus_status_buf[0]++;
						dut2.g_smbus_status_buf[1] = smbus_master_write_before_pinstatus_timer;
						dut2.g_smbus_style++;
						dut2.g_smbus_timer = smbus_master_write_before_pinstatus_timer;

						dut2.g_smbus_retry_timer = 0x64;
						dut2.g_pattern_smbus_control_buf[0] = smbus_road_doing;
						smbus2_process_state = smbus_road_doing;

						//xil_printf("doing_write_case01\r\n");
						//xil_printf("dut2_smbus_style = %x\r\n", dut2.g_smbus_style);
						//xil_printf("dut2_smbus_timer = %x\r\n", dut2.g_smbus_timer);
					}
					break;
				}

				case 0x02:
				{
					msdelay(50);
					//xil_printf("\r\n doing_write_case02\r\n");
					smbus_get_pinstatus_result = smbus_master_get_pinstate_dut2(SMBUS_DEV2_ADDR, smbus_get_pinstatus_result);
					//xil_printf("smbus_get_pinstatus_result = %d\r\n", smbus_get_pinstatus_result);

					//dut2.g_smbus_status_buf[0]++;
					dut2.g_smbus_status_buf[1] = smbus_master_write_pinstatus_timer;
					//dut2.g_smbus_style++;
					dut2.g_smbus_timer = smbus_master_write_pinstatus_timer;

					dut2.g_pattern_smbus_control_buf[0] = smbus_road_doing;
					smbus2_process_state = smbus_road_doing;

					switch(smbus_get_pinstatus_result)
					{
					case 0x00:
					{
						if(dut2.g_smbus_retry_timer >0)
						{
							dut2.g_smbus_retry_timer--;
						}
						else
						{
							dut2.g_result_fail=0x01;
							dut2.g_result_fail_tmrcount = 0xffff;
							dut2.g_smbus_status_buf[0] = clear_;
							dut2.g_smbus_status_buf[1] = clear_;
							dut2.g_smbus_status_buf[2] = clear_;
							dut2.g_smbus_style = clear_;
							dut2.g_smbus_timer = clear_;
						}
						break;
					}
					case 0x01:
					{
						dut2.g_smbus_status_buf[0]++;
						dut2.g_smbus_status_buf[2] = 0x01;
						dut2.g_smbus_style++;
						break;
					}
					case 0x02:
					{
						if(dut2.g_smbus_retry_timer >0)
						{
							dut2.g_smbus_retry_timer--;
						}
						else
						{
							dut2.g_result_fail=0x01;
							dut2.g_result_fail_tmrcount = 0xffff;
							dut2.g_smbus_status_buf[0] = clear_;
							dut2.g_smbus_status_buf[1] = clear_;
							dut2.g_smbus_status_buf[2] = clear_;
							dut2.g_smbus_style = clear_;
							dut2.g_smbus_timer = clear_;
						}
						break;
					}
					case 0x03:
					{
						dut2.g_result_fail=0x01;
						dut2.g_result_fail_tmrcount = 0xffff;
						dut2.g_smbus_status_buf[0] = clear_;
						dut2.g_smbus_status_buf[1] = clear_;
						dut2.g_smbus_status_buf[2] = clear_;
						dut2.g_smbus_style = clear_;
						dut2.g_smbus_timer = clear_;
						break;
					}
					}

					//xil_printf("doing_write_case02\r\n");
					//xil_printf("smbus_get_pinstatus_result = %d\r\n", smbus_get_pinstatus_result);
					//xil_printf("dut2_smbus_style = %x\r\n", dut2.g_smbus_style);
					//xil_printf("dut2_smbus_timer = %x\r\n", dut2.g_smbus_timer);
					break;
				}

				case 0x03:
				{
					//xil_printf("\r\n doing_write_case03\r\n");
					statusreg = XIic_ReadReg(SMBUS_DEV2_ADDR,  XIIC_SR_REG_OFFSET);

					//xil_printf("smbus_get_pinstatus_result = %d\r\n", smbus_get_pinstatus_result);
					//xil_printf("dut2_statusreg = %d\r\n", statusreg);

					if((statusreg & XIIC_SR_BUS_BUSY_MASK) == 0x01)
					{
						dut2.g_pattern_smbus_control_buf[0] = smbus_master_communication_fail;
						smbus2_process_state = smbus_master_communication_fail;

						dut2.g_smbus_status_buf[0] = clear_;
						dut2.g_smbus_status_buf[1] = clear_;
						dut2.g_smbus_style = clear_;
						dut2.g_smbus_timer = clear_;
						dut2.g_smbus_status_buf[2] = clear_;
					}
					else if((statusreg & XIIC_SR_BUS_BUSY_MASK) == 0)
					{
						dut2.g_pattern_smbus_control_buf[0] = smbus_master_communication_pass;
						smbus2_process_state = smbus_master_communication_pass;

						dut2.g_smbus_status_buf[0] = clear_;
						dut2.g_smbus_status_buf[1] = clear_;
						dut2.g_smbus_style = clear_;
						dut2.g_smbus_timer = clear_;
						dut2.g_smbus_status_buf[2] = clear_;
					}
					else
					{
						dut2.g_pattern_smbus_control_buf[0] = smbus_master_communication_fail;

						dut2.g_smbus_status_buf[0] = clear_;
						dut2.g_smbus_status_buf[1] = clear_;
						dut2.g_smbus_status_buf[2] = clear_;
						dut2.g_smbus_style = clear_;
						dut2.g_smbus_timer = clear_;
					}
					break;
				}
				}
//			}
			break;
		}
		}
	}

	return 0;
}

