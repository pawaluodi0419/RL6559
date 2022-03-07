#include "platform.h"

u8 _by_Pattern0_Setup_dut0()
{
	u8 i;
	u8 Buff_dut0_XGPIO_0[8];
	u8 read_icstatus_data[20];
	u8 read_flashdata[40];

	switch(dut0.g_pattern_step)
	{
	//normal mode£ºVMON·ÇÂúÆ«(XGPIO13 -> L, XGPIO14 -> H), GPIO10Îªlow(XGPIO23 -> L)
	//power rail: S0_5V_SW(XGPIO8 -> L)
	//DP reverse/U2 mux B: XGPIO22 -> H
	case 0x0000:
	{
		if(dut0.g_dut_pattern_status_buf[7] == 0x00)
		{
			Buff_dut0_XGPIO_0[0] = 0x00;                          	//REG0005 ouput value[7:0]
			Buff_dut0_XGPIO_0[1] = 0xFF;							//REG0006
			Buff_dut0_XGPIO_0[2] = 0x40;							//REG0007 output value[15:8]
			Buff_dut0_XGPIO_0[3] = 0xDE;							//REG0008
			Buff_dut0_XGPIO_0[4] = 0x40|(dut0.g_uartPatternNum); 	//REG0009 output value[23:16]
			Buff_dut0_XGPIO_0[5] = 0x00;							//REG000a
			Buff_dut0_XGPIO_0[6] = 0x02;							//REG000b output value[31:24]
			Buff_dut0_XGPIO_0[7] = 0xFC;							//REG000c

			XGpio_dut0_Relay_WriteByte(XPAR_AXI_GPIO_dut0_1_BASEADDR,Buff_dut0_XGPIO_0);
			dut0.g_dut_pattern_status_buf[7] = 0x01;
			//xil_printf("dut0.pattern0_gpio_control_completed!\r\n");
			msdelay(10);
		}
		else if(dut0.g_dut_pattern_status_buf[7] == 0x01)
		{
			if(dut0.g_relay_control_timer == 0 )
			{
				dut0.g_dut_pattern_status_buf[7] = 0x00;
				dut0.g_pattern_step++;

				//XGpio_2_WriteBit(0,dut0_FT2_PWR_CTRL_OFFSET,0x00);	//S0_PWR_EN# output low, power on DUT
				i2c_mcp23008_output(AD7994_DEV0_ADDR, MCP23008_ADDR, 0x80);
				xil_printf("dut0 power on!\r\n\r\n");
				msdelay(200);
			}
		}
		//output fail result
		else
		{
			dut0.g_result_fail = 0x01;
			dut0.g_result_fail_tmrcount = 0xffff;
		}
		break;
	}

	case 0x0001:
	{
		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_vdcmdenable;
		dut0.g_pattern_smbus_control_buf[2] = 0xda;
		dut0.g_pattern_smbus_control_buf[3] = 0x0b;
		dut0.g_pattern_smbus_control_buf[4] = 0x01;

		smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
		if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut0.g_pattern_step++;
		}
		break;
	}

	//check crc32
	case 0x0002:
	{
		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut0.g_pattern_smbus_control_buf[2] = 0x07;
		dut0.g_pattern_smbus_control_buf[3] = 0xdf;
		dut0.g_pattern_smbus_control_buf[4] = 0x01;

		smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
		if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut0 read 0xdf07 =%x\r\n", dut0.g_pattern_smbus_control_buf[10]);

			if(dut0.g_pattern_smbus_control_buf[10] == 0x40)
			{
				xil_printf("dut0.pattern_check_crc32_pass!\r\n\r\n");
				dut0.g_pattern_step++;
			}
			else
			{
				xil_printf("dut0.pattern_check_crc32_fail!\r\n\r\n");

				dut0.g_result_fail = 0x01;
				dut0.g_result_fail_tmrcount = 0xffff;
			}

			for(i=1; i<60; i++)
			{
				dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		}
		break;
	}

	//check FW version
	case 0x0003:
	{
		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_geticstatus;
		dut0.g_pattern_smbus_control_buf[2] = 0x00;
		dut0.g_pattern_smbus_control_buf[3] = 0x00;
		dut0.g_pattern_smbus_control_buf[4] = 0x14;

		smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
		if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=0;i<21;i++)
			{
				read_icstatus_data[i] = dut0.g_pattern_smbus_control_buf[i+10];
			}
			xil_printf("dut0.read_icstatus_data[3] =0x%02x\r\n", read_icstatus_data[3]);
			xil_printf("dut0.read_icstatus_data[4] =0x%02x\r\n", read_icstatus_data[4]);

			if((read_icstatus_data[3] == 0x01) && (read_icstatus_data[4] == 0x02))
			{
			    xil_printf("dut0 check FW version pass!\r\n\r\n");

				for(i=1; i<60; i++)
				{
					dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
				}

				dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				dut0.g_pattern_step++;
			}
			else
			{
				xil_printf("dut0 check FW version fail!\r\n\r\n");

				dut0.g_result_fail = 0x01;
				dut0.g_result_fail_tmrcount = 0xffff;
			}
		}
		break;
	}

	//check crc32 data
	case 0x0004:
	{
		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readflash_128K;
		dut0.g_pattern_smbus_control_buf[2] = 0xe6;
		dut0.g_pattern_smbus_control_buf[3] = 0xff;
		dut0.g_pattern_smbus_control_buf[4] = 0x04;

		smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
		if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=0;i<4;i++)
			{
				read_flashdata[i] = dut0.g_pattern_smbus_control_buf[i+10];
			}
			xil_printf("dut0.crc32_byte0 =0x%02x\r\n", read_flashdata[0]);
			xil_printf("dut0.crc32_byte1 =0x%02x\r\n", read_flashdata[1]);
			xil_printf("dut0.crc32_byte2 =0x%02x\r\n", read_flashdata[2]);
			xil_printf("dut0.crc32_byte3 =0x%02x\r\n", read_flashdata[3]);

			if((read_flashdata[0] == 0x3f) && (read_flashdata[1] == 0x93) && (read_flashdata[2] == 0xd0) && (read_flashdata[3] == 0xf2))
			{
			    xil_printf("dut0 check crc32 data pass!\r\n\r\n");

				for(i=1; i<60; i++)
				{
					dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
				}

				dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				dut0.g_pattern_step++;
			}
			else
			{
				xil_printf("dut0 check crc32 data fail!\r\n\r\n");

				dut0.g_result_fail = 0x01;
				dut0.g_result_fail_tmrcount = 0xffff;
			}
		}
		break;
	}

	//FT mode£ºVMONÂúÆ«(XGPIO13 -> H, XGPIO14 -> L), GPIO10Îªlow(XGPIO23 -> L)
	case 0x0005:
	{
		if(dut0.g_dut_pattern_status_buf[7] == 0x00)
		{
			Buff_dut0_XGPIO_0[0] = 0x00;                          	//REG0005 ouput value[7:0]
			Buff_dut0_XGPIO_0[1] = 0xFF;							//REG0006
			Buff_dut0_XGPIO_0[2] = 0x20;							//REG0007 output value[15:8]
			Buff_dut0_XGPIO_0[3] = 0xDE;							//REG0008
			Buff_dut0_XGPIO_0[4] = 0x40|(dut0.g_uartPatternNum); 	//REG0009 output value[23:16]
			Buff_dut0_XGPIO_0[5] = 0x00;							//REG000a
			Buff_dut0_XGPIO_0[6] = 0x00;							//REG000b output value[31:24]
			Buff_dut0_XGPIO_0[7] = 0xFF;							//REG000c

			XGpio_dut0_Relay_WriteByte(XPAR_AXI_GPIO_dut0_1_BASEADDR,Buff_dut0_XGPIO_0);
			dut0.g_dut_pattern_status_buf[7] = 0x01;
			xil_printf("dut0.pattern0_ft_mode_control_completed!\r\n");
			msdelay(10);
		}
		else if(dut0.g_dut_pattern_status_buf[7] == 0x01)
		{
			if(dut0.g_relay_control_timer == 0 )
			{
				dut0.g_dut_pattern_status_buf[7] = 0x00;
				dut0.g_pattern_step++;
			}
		}
		//output fail result
		else
		{
			dut0.g_result_fail = 0x01;
			dut0.g_result_fail_tmrcount = 0xffff;
		}
		break;
	}

	//reset to flash
	case 0x0006:
	{
		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_reset_to_flash;
		dut0.g_pattern_smbus_control_buf[2] = 0xda;
		dut0.g_pattern_smbus_control_buf[3] = 0x0b;
		dut0.g_pattern_smbus_control_buf[4] = 0x01;

		smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
		if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<31; i++)
			{
				dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut0.g_pattern_step++;
			msdelay(200);
		}
		break;
	}

	case 0x0007:
	{
		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_vdcmdenable;
		dut0.g_pattern_smbus_control_buf[2] = 0xda;
		dut0.g_pattern_smbus_control_buf[3] = 0x0b;
		dut0.g_pattern_smbus_control_buf[4] = 0x01;

		smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
		if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut0.g_pattern_step++;
		}
		break;
	}

	//check FT mode
	case 0x0008:
	{
		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut0.g_pattern_smbus_control_buf[2] = 0x07;
		dut0.g_pattern_smbus_control_buf[3] = 0xdf;
		dut0.g_pattern_smbus_control_buf[4] = 0x01;

		smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
		if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut0 read 0xdf07 =%x\r\n", dut0.g_pattern_smbus_control_buf[10]);

			if(dut0.g_pattern_smbus_control_buf[10] == 0x20)
			{
				xil_printf("dut0 check FT mode pass!\r\n\r\n");
				dut0.g_pattern_step = 0x0a;
			}
			else
			{
				i2c_mcp23008_output(AD7994_DEV0_ADDR, MCP23008_ADDR, 0x00);
				msdelay(50);
				i2c_mcp23008_output(AD7994_DEV0_ADDR, MCP23008_ADDR, 0x80);
				msdelay(200);
				xil_printf("dut0 check FT mode again!\r\n\r\n");
				dut0.g_pattern_step = 0x09;
			}

			for(i=1; i<60; i++)
			{
				dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		}
		break;
	}

	//check FT mode retry
	case 0x0009:
	{
		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut0.g_pattern_smbus_control_buf[2] = 0x07;
		dut0.g_pattern_smbus_control_buf[3] = 0xdf;
		dut0.g_pattern_smbus_control_buf[4] = 0x01;

		smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
		if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut0 read 0xdf07 =%x\r\n", dut0.g_pattern_smbus_control_buf[10]);

			if(dut0.g_pattern_smbus_control_buf[10] == 0x20)
			{
				xil_printf("dut0 check FT mode pass!\r\n\r\n");
				dut0.g_pattern_step++;
			}
			else
			{
				xil_printf("dut0 check FT mode fail!\r\n\r\n");

				dut0.g_result_fail = 0x01;
				dut0.g_result_fail_tmrcount = 0xffff;
			}

			for(i=1; i<60; i++)
			{
				dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		}
		break;
	}

	//disconnect aux mux switch
	case 0x000a:
	{
		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut0.g_pattern_smbus_control_buf[2] = 0x15;
		dut0.g_pattern_smbus_control_buf[3] = 0xc4;
		dut0.g_pattern_smbus_control_buf[4] = 0x01;
		dut0.g_pattern_smbus_control_buf[5] = 0x00;

		smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
		if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut0.g_pattern_step++;
			dut0.g_pattern_step++;
		}
		break;
	}

//	//check IC information
//	case 0x000b:
//	{
//		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_geticstatus;
//		dut0.g_pattern_smbus_control_buf[2] = 0x00;
//		dut0.g_pattern_smbus_control_buf[3] = 0x00;
//		dut0.g_pattern_smbus_control_buf[4] = 0x14;
//
//		smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
//		if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
//		{
//			break;
//		}
//		else
//		{
//			for(i=0;i<21;i++)
//			{
//				read_icstatus_data[i] = dut0.g_pattern_smbus_control_buf[i+10];
//			}
//			xil_printf("dut0.read_icstatus_data[11] =0x%x\r\n", read_icstatus_data[11]);
//			xil_printf("dut0.read_icstatus_data[12] =0x%x\r\n", read_icstatus_data[12]);
//
//			if((read_icstatus_data[11] == 0x57) && (read_icstatus_data[12] == 0x54))
//			{
//			    xil_printf("dut0 check IC information pass!\r\n\r\n");
//
//				for(i=1; i<60; i++)
//				{
//					dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
//				}
//
//				dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
//				dut0.g_pattern_step++;
//			}
//			else
//			{
//				xil_printf("dut0 check IC information fail!\r\n\r\n");
//
//				dut0.g_result_fail = 0x01;
//				dut0.g_result_fail_tmrcount = 0xffff;
//			}
//		}
//		break;
//	}

	//check MCM flash ID
	case 0x000c:
	{
		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_vdcmdenable;
		dut0.g_pattern_smbus_control_buf[2] = 0x04;
		dut0.g_pattern_smbus_control_buf[3] = 0x00;
		dut0.g_pattern_smbus_control_buf[4] = 0x02;

		smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
		if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut0.get_mcmflash_id1 =0x%x\r\n", dut0.g_pattern_smbus_control_buf[10]);
			xil_printf("dut0.get_mcmflash_id2 =0x%x\r\n", dut0.g_pattern_smbus_control_buf[11]);

			if((dut0.g_pattern_smbus_control_buf[10] == 0xef) && (dut0.g_pattern_smbus_control_buf[11] == 0x11))
			{
			    xil_printf("dut0 check MCM flash ID pass!\r\n\r\n");

				for(i=1; i<60; i++)
				{
					dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
				}

				dut0.g_pattern_timer = 0xfff;
				dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				dut0.g_pattern_step = 0x00;
				//dut0.g_dut_pattern_status_buf[2]++;
				dut0.g_uartPatternEnable = 0x00;
				dut0.g_uartPatternNum++;
				result_output_for_v50(XPAR_AXI_GPIO_dut0_1_BASEADDR,dut0.g_uartPatternNum);
				xil_printf("dut0_pattern0_setup_pass!\r\n\r\n");
			}
			else
			{
				xil_printf("dut0 check MCM flash ID fail!\r\n\r\n");

				dut0.g_result_fail = 0x01;
				dut0.g_result_fail_tmrcount = 0xffff;
			}
		}
		break;
	}
	}

	if(dut0.g_result_fail == 0x01)
	{
		xil_printf("dut0_pattern0_setup_fail! fail_step = %x\r\n",dut0.g_pattern_step);
		pattern_safe_state_dut0();
	}

	return 0;
}


