#include "platform.h"

u8 _by_PatternF_efuse_dut0()
{
	u8 i;
	u8 Buff_dut0_XGPIO_0[8];

	switch(dut0.g_pattern_step)
	{
	//if this is a new ic, program efuse
	//power rail: S0_5V_SW(XGPIO8 -> L)
	case 0x0000:
	{
		if(dut0.g_dut_pattern_status_buf[7] == 0x00)
		{
			Buff_dut0_XGPIO_0[0] = 0x30;                            //REG0005 ouput value[7:0]
			Buff_dut0_XGPIO_0[1] = 0xC2;							//REG0006
			Buff_dut0_XGPIO_0[2] = 0x40;							//REG0007 output value[15:8]
			Buff_dut0_XGPIO_0[3] = 0x9E;							//REG0008
			Buff_dut0_XGPIO_0[4] = 0xC0|(dut0.g_uartPatternNum); 	//REG0009 output value[23:16]
			Buff_dut0_XGPIO_0[5] = 0x00;							//REG000a
			Buff_dut0_XGPIO_0[6] = 0x02;							//REG000b output value[31:24]
			Buff_dut0_XGPIO_0[7] = 0xFC;							//REG000c

			XGpio_dut0_Relay_WriteByte(XPAR_AXI_GPIO_dut0_1_BASEADDR,Buff_dut0_XGPIO_0);
			dut0.g_dut_pattern_status_buf[7] = 0x01;
			xil_printf("dut0.patternF_gpio_control_completed!\r\n");
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

	//write pattern index
	case 0x0001:
	{
		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut0.g_pattern_smbus_control_buf[2] = 0x33;
		dut0.g_pattern_smbus_control_buf[3] = 0xdf;
		dut0.g_pattern_smbus_control_buf[4] = 0x01;
		dut0.g_pattern_smbus_control_buf[5] = 0x0f;

		smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
		if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut0 write pattern index =%x\r\n", dut0.g_pattern_smbus_control_buf[5]);
			for(i=1; i<60; i++)
			{
				dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut0.g_pattern_step++;
			//msdelay(50);
		}
		break;
	}

	//polling ack bit
	case 0x0002:
	{
		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut0.g_pattern_smbus_control_buf[2] = 0x33;
		dut0.g_pattern_smbus_control_buf[3] = 0xdf;
		dut0.g_pattern_smbus_control_buf[4] = 0x01;

		smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
		if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut0 polling pattern ack bit =%x\r\n", dut0.g_pattern_smbus_control_buf[10]);
			if(dut0.g_pattern_smbus_control_buf[10] == 0x8F)
			{
				for(i=1; i<60; i++)
				{
					dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
				}
				dut0.g_pattern_timer = 0x3fff;
				dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				dut0.g_pattern_step = 0x00;
				//dut0.g_dut_pattern_status_buf[2]++;
				dut0.g_uartPatternEnable = 0x00;
				dut0.g_uartPatternNum++;
				dut0.g_uartPatternNum++;
				result_output_for_v50(XPAR_AXI_GPIO_dut0_1_BASEADDR,dut0.g_uartPatternNum);
				xil_printf("dut0.patternF_pass!\r\n\r\n");
			}
			else
			{
				dut0.g_result_fail = 0x01;
				dut0.g_result_fail_tmrcount = 0xffff;
			}
		}
		break;
	}

//	//read efuse
//	case 0x0003:
//	{
//		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readefuse;
//		dut0.g_pattern_smbus_control_buf[2] = 0x00;
//		dut0.g_pattern_smbus_control_buf[3] = 0x00;
//		dut0.g_pattern_smbus_control_buf[4] = 0x28;
//
//		smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
//		if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
//		{
//			break;
//		}
//		else
//		{
//			for(i=0; i<40; i++)
//			{
//				xil_printf("dut0.efuse_data[%02d]= %02x\r\n", i, dut0.g_pattern_smbus_control_buf[i+10]);
//			}
//
//			for(i=1; i<60; i++)
//			{
//				dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
//			}
//			dut0.g_pattern_timer = 0x3fff;
//			dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
//			dut0.g_pattern_step = 0x00;
//			//dut0.g_dut_pattern_status_buf[2]++;
//			dut0.g_uartPatternEnable = 0x00;
//			dut0.g_uartPatternNum++;
//			result_output_for_v50(XPAR_AXI_GPIO_dut0_1_BASEADDR,dut0.g_uartPatternNum);
//			xil_printf("dut0.patternF_pass!\r\n\r\n");
//		}
//		break;
//	}

//	//read ft2 data(22 byte): rh_smbusgeticstatus 0xcc 32 22
//	case 0x0003:
//	{
//		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_geticstatus;
//		dut0.g_pattern_smbus_control_buf[2] = 0x20;
//		dut0.g_pattern_smbus_control_buf[3] = 0x00;
//		dut0.g_pattern_smbus_control_buf[4] = 0x16;
//
//		smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
//		if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
//		{
//			break;
//		}
//		else
//		{
//			for(i=0;i<60;i++)
//			{
//				dut0.g_read_ft2_data[i] = 0;
//			}
//
//			for(i=0;i<22;i++)
//			{
//				dut0.g_read_ft2_data[i] = dut0.g_pattern_smbus_control_buf[i+10];
//			}
//
//			for(i=0;i<22;i++)
//			{
//				xil_printf("dut0.ft2_data[%02d]= %02x\r\n", i, dut0.g_read_ft2_data[i]);
//			}
//
//			xil_printf("\r\n");
//
//			for(i=1; i<60; i++)
//			{
//				dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
//			}
//
//			dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
//			dut0.g_pattern_step++;
//		}
//		break;
//	}
//
//	//read flash data(22 byte): rh_smbusgeticstatus 0xcc 33 22
//	case 0x0004:
//	{
//		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_geticstatus;
//		dut0.g_pattern_smbus_control_buf[2] = 0x21;
//		dut0.g_pattern_smbus_control_buf[3] = 0x00;
//		dut0.g_pattern_smbus_control_buf[4] = 0x16;
//
//		smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
//		if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
//		{
//			break;
//		}
//		else
//		{
//			for(i=0;i<60;i++)
//			{
//				dut0.g_read_flash_data[i] = 0;
//			}
//
//			for(i=0;i<22;i++)
//			{
//				dut0.g_read_flash_data[i] = dut0.g_pattern_smbus_control_buf[i+10];
//			}
//
//			for(i=0;i<22;i++)
//			{
//				xil_printf("dut0.flash_data[%02d]= %02x\r\n", i, dut0.g_read_flash_data[i]);
//			}
//
//			xil_printf("\r\n");
//
//			for(i=1; i<60; i++)
//			{
//				dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
//			}
//
//			dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
//			dut0.g_pattern_step++;
//		}
//		break;
//	}
//
//	//read efuse data(20 byte): rh_smbusgeticstatus 0xcc 35 20
//	case 0x0005:
//	{
//		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_geticstatus;
//		dut0.g_pattern_smbus_control_buf[2] = 0x23;
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
//			for(i=0;i<60;i++)
//			{
//				dut0.g_read_efuse_data[i] = 0;
//			}
//
//			for(i=0;i<20;i++)
//			{
//				dut0.g_read_efuse_data[i] = dut0.g_pattern_smbus_control_buf[i+10];
//			}
//
//			for(i=0;i<20;i++)
//			{
//				xil_printf("dut0.efuse_data[%02d]= %02x\r\n", i, dut0.g_read_efuse_data[i]);
//			}
//
//			for(i=1; i<60; i++)
//			{
//				dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
//			}
//
//			dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
//			dut0.g_pattern_step++;
//		}
//		break;
//	}
//
//	//read efuse data(20 byte): rh_smbusgeticstatus 0xcc 36 20
//	case 0x0006:
//	{
//		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_geticstatus;
//		dut0.g_pattern_smbus_control_buf[2] = 0x24;
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
//			for(i=0;i<20;i++)
//			{
//				dut0.g_read_efuse_data[i+20] = dut0.g_pattern_smbus_control_buf[i+10];
//			}
//
//			for(i=0;i<20;i++)
//			{
//				xil_printf("dut0.efuse_data[%02d]= %02x\r\n", i+20, dut0.g_read_efuse_data[i+20]);
//			}
//
//			for(i=1; i<60; i++)
//			{
//				dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
//			}
//			dut0.g_pattern_timer = 0x3fff;
//			dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
//			dut0.g_pattern_step = 0x00;
//			//dut0.g_dut_pattern_status_buf[2]++;
//			dut0.g_uartPatternEnable = 0x00;
//			dut0.g_uartPatternNum = 0x10;
//			result_output_for_v50(XPAR_AXI_GPIO_dut0_1_BASEADDR,dut0.g_uartPatternNum);
//			xil_printf("dut0.patternF_pass!\r\n\r\n");
//		}
//		break;
//	}
//
//	//if this is a new ic, compare ft2 data and flash data
//	case 0x0006:
//	{
//		if(dut0.g_retest == 0)
//		{
//			for(i=0;i<22;i++)
//			{
//				if(dut0.g_read_ft2_data[i] == dut0.g_read_flash_data[i])
//				{
//					if(i == 21)
//					{
//						dut0.g_pattern_step++;
//					}
//					else
//					{
//
//					}
//				}
//				else
//				{
//					i = 22;
//					dut0.g_result_fail = 0x01;
//					dut0.g_result_fail_tmrcount = 0xffff;
//					xil_printf("dut0 compare ft2 data and flash data fail!\r\n\r\n");
//				}
//			}
//		}
//
//		if(dut0.g_retest == 1)
//		{
//			for(i=1; i<60; i++)
//			{
//				dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
//			}
//			dut0.g_pattern_timer = 0x3fff;
//			dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
//			dut0.g_pattern_step = 0x00;
//			//dut0.g_dut_pattern_status_buf[2]++;
//			dut0.g_uartPatternEnable = 0x00;
//			dut0.g_uartPatternNum = 0x10;
//			result_output_for_v50(XPAR_AXI_GPIO_dut0_1_BASEADDR,dut0.g_uartPatternNum);
//			xil_printf("dut0.patternF_pass!\r\n\r\n");
//		}
//		break;
//	}
//
//	//if this is a new ic, program efuse
//	case 0x0007:
//	{
//		//LDO output 5.55V
//		i2c_mcp23008_output(AD7994_DEV0_ADDR, MCP23008_ADDR, 0xc0);
//		msdelay(10);
//
//		//power rail: S0_5V_SW(XGPIO8 -> L)
//		if(dut0.g_dut_pattern_status_buf[7] == 0x00)
//		{
//			Buff_dut0_XGPIO_0[0] = 0x30;                            //REG0005 ouput value[7:0]
//			Buff_dut0_XGPIO_0[1] = 0xC2;							//REG0006
//			Buff_dut0_XGPIO_0[2] = 0x40;							//REG0007 output value[15:8]
//			Buff_dut0_XGPIO_0[3] = 0x9E;							//REG0008
//			Buff_dut0_XGPIO_0[4] = 0xC0|(dut0.g_uartPatternNum); 	//REG0009 output value[23:16]
//			Buff_dut0_XGPIO_0[5] = 0x00;							//REG000a
//			Buff_dut0_XGPIO_0[6] = 0x00;							//REG000b output value[31:24]
//			Buff_dut0_XGPIO_0[7] = 0xFF;							//REG000c
//
//			XGpio_dut0_Relay_WriteByte(XPAR_AXI_GPIO_dut0_1_BASEADDR,Buff_dut0_XGPIO_0);
//			dut0.g_dut_pattern_status_buf[7] = 0x01;
//			//xil_printf("PatternNum = %x\r\n",dut0.g_uartPatternNum);
//			xil_printf("dut0.patternE_gpio_control_completed!\r\n");
//			msdelay(10);
//		}
//		else if(dut0.g_dut_pattern_status_buf[7] == 0x01)
//		{
//			if(dut0.g_relay_control_timer == 0 )
//			{
//				dut0.g_dut_pattern_status_buf[7] = 0x00;
//				dut0.g_pattern_step++;
//			}
//		}
//		//output fail result
//		else
//		{
//			dut0.g_result_fail = 0x01;
//			dut0.g_result_fail_tmrcount = 0xffff;
//		}
//		break;
//	}
//
//	//if this is a new ic, program efuse
//	case 0x0008:
//	{
//		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writeefuse;
//		dut0.g_pattern_smbus_control_buf[2] = 0x00;
//		dut0.g_pattern_smbus_control_buf[3] = 0x00;
//		dut0.g_pattern_smbus_control_buf[4] = 0x28;
//
//		for(i=0;i<40;i++)
//		{
//			dut0.g_pattern_smbus_control_buf[i+5] = dut0.g_read_efuse_data[i];
//		}
//
//		smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
//		if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
//		{
//			break;
//		}
//		else
//		{
//			for(i=1; i<60; i++)
//			{
//				dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
//			}
//
//			dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
//			dut0.g_pattern_step++;
//		}
//		break;
//	}
//
//	//read efuse and compare
//	case 0x0009:
//	{
//		dut0.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readefuse;
//		dut0.g_pattern_smbus_control_buf[2] = 0x00;
//		dut0.g_pattern_smbus_control_buf[3] = 0x00;
//		dut0.g_pattern_smbus_control_buf[4] = 0x28;
//
//		smbus0_irq_handle(dut0.g_pattern_smbus_control_buf);
//		if(dut0.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
//		{
//			break;
//		}
//		else
//		{
//			for(i=0;i<40;i++)
//			{
//				if(dut0.g_read_efuse_data[i] != dut0.g_pattern_smbus_control_buf[i+10])
//				{
//					dut0.g_result_fail = 0x01;
//					dut0.g_result_fail_tmrcount = 0xffff;
//					xil_printf("dut0 compare efuse data fail!\r\n\r\n");
//				}
//				else
//				{
//					for(i=1; i<60; i++)
//					{
//						dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
//					}
//					dut0.g_pattern_timer = 0x3fff;
//					dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
//					dut0.g_pattern_step = 0x00;
//					//dut0.g_dut_pattern_status_buf[2]++;
//					dut0.g_uartPatternEnable = 0x00;
//					dut0.g_uartPatternNum = 0x10;
//					result_output_for_v50(XPAR_AXI_GPIO_dut0_1_BASEADDR,dut0.g_uartPatternNum);
//					xil_printf("dut0 compare efuse data pass!\r\n\r\n");
//					xil_printf("dut0.patternF_pass!\r\n\r\n");
//				}
//
//			}
//		}
//		break;
//	}
	}

	if(dut0.g_result_fail == 0x01)
	{
		xil_printf("dut0.patternF_fail! fail_step = %x\r\n",dut0.g_pattern_step);
		pattern_safe_state_dut0();
	}

	return 0;
}
