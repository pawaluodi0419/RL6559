#include "platform.h"

u8 _by_PatternF_efuse_dut2()
{
	u8 i;
	u8 Buff_dut2_XGPIO_0[8];

	switch(dut2.g_pattern_step)
	{
	//if this is a new ic, program efuse
	//power rail: S2_5V_SW(XGPIO8 -> L)
	case 0x0000:
	{
		if(dut2.g_dut_pattern_status_buf[7] == 0x00)
		{
			Buff_dut2_XGPIO_0[0] = 0x30;                            //REG0005 ouput value[7:0]
			Buff_dut2_XGPIO_0[1] = 0xC2;							//REG0006
			Buff_dut2_XGPIO_0[2] = 0x40;							//REG0007 output value[15:8]
			Buff_dut2_XGPIO_0[3] = 0x9E;							//REG0008
			Buff_dut2_XGPIO_0[4] = 0xC0|(dut2.g_uartPatternNum); 	//REG0009 output value[23:16]
			Buff_dut2_XGPIO_0[5] = 0x00;							//REG000a
			Buff_dut2_XGPIO_0[6] = 0x02;							//REG000b output value[31:24]
			Buff_dut2_XGPIO_0[7] = 0xFC;							//REG000c

			XGpio_dut2_Relay_WriteByte(XPAR_AXI_GPIO_dut2_1_BASEADDR,Buff_dut2_XGPIO_0);
			dut2.g_dut_pattern_status_buf[7] = 0x01;
			xil_printf("dut2.patternF_gpio_control_completed!\r\n");
			msdelay(10);
		}
		else if(dut2.g_dut_pattern_status_buf[7] == 0x01)
		{
			if(dut2.g_relay_control_timer == 0 )
			{
				dut2.g_dut_pattern_status_buf[7] = 0x00;
				dut2.g_pattern_step++;
			}
		}
		//output fail result
		else
		{
			dut2.g_result_fail = 0x01;
			dut2.g_result_fail_tmrcount = 0xffff;
		}
		break;
	}

	//write pattern index
	case 0x0001:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut2.g_pattern_smbus_control_buf[2] = 0x33;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;
		dut2.g_pattern_smbus_control_buf[5] = 0x0f;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut2 write pattern index =%x\r\n", dut2.g_pattern_smbus_control_buf[5]);
			for(i=1; i<60; i++)
			{
				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut2.g_pattern_step++;
			//msdelay(50);
		}
		break;
	}

	//polling ack bit
	case 0x0002:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut2.g_pattern_smbus_control_buf[2] = 0x33;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;

		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut2 polling pattern ack bit =%x\r\n", dut2.g_pattern_smbus_control_buf[10]);
			if(dut2.g_pattern_smbus_control_buf[10] == 0x8F)
			{
				for(i=1; i<60; i++)
				{
					dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
				}
				dut2.g_pattern_timer = 0x3fff;
				dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				dut2.g_pattern_step = 0x00;
				//dut2.g_dut_pattern_status_buf[2]++;
				dut2.g_uartPatternEnable = 0x00;
				dut2.g_uartPatternNum++;
				dut2.g_uartPatternNum++;
				result_output_for_v50(XPAR_AXI_GPIO_dut2_1_BASEADDR,dut2.g_uartPatternNum);
				xil_printf("dut2.patternF_pass!\r\n\r\n");
			}
			else
			{
				dut2.g_result_fail = 0x01;
				dut2.g_result_fail_tmrcount = 0xffff;
			}
		}
		break;
	}

//	//read efuse
//	case 0x0003:
//	{
//		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readefuse;
//		dut2.g_pattern_smbus_control_buf[2] = 0x00;
//		dut2.g_pattern_smbus_control_buf[3] = 0x00;
//		dut2.g_pattern_smbus_control_buf[4] = 0x28;
//
//		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
//		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
//		{
//			break;
//		}
//		else
//		{
//			for(i=0; i<40; i++)
//			{
//				xil_printf("dut2.efuse_data[%02d]= %02x\r\n", i, dut2.g_pattern_smbus_control_buf[i+10]);
//			}
//
//			for(i=1; i<60; i++)
//			{
//				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
//			}
//			dut2.g_pattern_timer = 0x3fff;
//			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
//			dut2.g_pattern_step = 0x00;
//			//dut2.g_dut_pattern_status_buf[2]++;
//			dut2.g_uartPatternEnable = 0x00;
//			dut2.g_uartPatternNum++;
//			result_output_for_v50(XPAR_AXI_GPIO_dut2_1_BASEADDR,dut2.g_uartPatternNum);
//			xil_printf("dut2.patternF_pass!\r\n\r\n");
//		}
//		break;
//	}

//	//read ft2 data(22 byte): rh_smbusgeticstatus 0xcc 32 22
//	case 0x0003:
//	{
//		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_geticstatus;
//		dut2.g_pattern_smbus_control_buf[2] = 0x20;
//		dut2.g_pattern_smbus_control_buf[3] = 0x00;
//		dut2.g_pattern_smbus_control_buf[4] = 0x16;
//
//		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
//		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
//		{
//			break;
//		}
//		else
//		{
//			for(i=0;i<60;i++)
//			{
//				dut2.g_read_ft2_data[i] = 0;
//			}
//
//			for(i=0;i<22;i++)
//			{
//				dut2.g_read_ft2_data[i] = dut2.g_pattern_smbus_control_buf[i+10];
//			}
//
//			for(i=0;i<22;i++)
//			{
//				xil_printf("dut2.ft2_data[%02d]= %02x\r\n", i, dut2.g_read_ft2_data[i]);
//			}
//
//			xil_printf("\r\n");
//
//			for(i=1; i<60; i++)
//			{
//				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
//			}
//
//			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
//			dut2.g_pattern_step++;
//		}
//		break;
//	}
//
//	//read flash data(22 byte): rh_smbusgeticstatus 0xcc 33 22
//	case 0x0004:
//	{
//		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_geticstatus;
//		dut2.g_pattern_smbus_control_buf[2] = 0x21;
//		dut2.g_pattern_smbus_control_buf[3] = 0x00;
//		dut2.g_pattern_smbus_control_buf[4] = 0x16;
//
//		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
//		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
//		{
//			break;
//		}
//		else
//		{
//			for(i=0;i<60;i++)
//			{
//				dut2.g_read_flash_data[i] = 0;
//			}
//
//			for(i=0;i<22;i++)
//			{
//				dut2.g_read_flash_data[i] = dut2.g_pattern_smbus_control_buf[i+10];
//			}
//
//			for(i=0;i<22;i++)
//			{
//				xil_printf("dut2.flash_data[%02d]= %02x\r\n", i, dut2.g_read_flash_data[i]);
//			}
//
//			xil_printf("\r\n");
//
//			for(i=1; i<60; i++)
//			{
//				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
//			}
//
//			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
//			dut2.g_pattern_step++;
//		}
//		break;
//	}
//
//	//read efuse data(20 byte): rh_smbusgeticstatus 0xcc 35 20
//	case 0x0005:
//	{
//		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_geticstatus;
//		dut2.g_pattern_smbus_control_buf[2] = 0x23;
//		dut2.g_pattern_smbus_control_buf[3] = 0x00;
//		dut2.g_pattern_smbus_control_buf[4] = 0x14;
//
//		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
//		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
//		{
//			break;
//		}
//		else
//		{
//			for(i=0;i<60;i++)
//			{
//				dut2.g_read_efuse_data[i] = 0;
//			}
//
//			for(i=0;i<20;i++)
//			{
//				dut2.g_read_efuse_data[i] = dut2.g_pattern_smbus_control_buf[i+10];
//			}
//
//			for(i=0;i<20;i++)
//			{
//				xil_printf("dut2.efuse_data[%02d]= %02x\r\n", i, dut2.g_read_efuse_data[i]);
//			}
//
//			for(i=1; i<60; i++)
//			{
//				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
//			}
//
//			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
//			dut2.g_pattern_step++;
//		}
//		break;
//	}
//
//	//read efuse data(20 byte): rh_smbusgeticstatus 0xcc 36 20
//	case 0x0006:
//	{
//		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_geticstatus;
//		dut2.g_pattern_smbus_control_buf[2] = 0x24;
//		dut2.g_pattern_smbus_control_buf[3] = 0x00;
//		dut2.g_pattern_smbus_control_buf[4] = 0x14;
//
//		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
//		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
//		{
//			break;
//		}
//		else
//		{
//			for(i=0;i<20;i++)
//			{
//				dut2.g_read_efuse_data[i+20] = dut2.g_pattern_smbus_control_buf[i+10];
//			}
//
//			for(i=0;i<20;i++)
//			{
//				xil_printf("dut2.efuse_data[%02d]= %02x\r\n", i+20, dut2.g_read_efuse_data[i+20]);
//			}
//
//			for(i=1; i<60; i++)
//			{
//				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
//			}
//			dut2.g_pattern_timer = 0x3fff;
//			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
//			dut2.g_pattern_step = 0x00;
//			//dut2.g_dut_pattern_status_buf[2]++;
//			dut2.g_uartPatternEnable = 0x00;
//			dut2.g_uartPatternNum = 0x10;
//			result_output_for_v50(XPAR_AXI_GPIO_dut2_1_BASEADDR,dut2.g_uartPatternNum);
//			xil_printf("dut2.patternF_pass!\r\n\r\n");
//		}
//		break;
//	}
//
//	//if this is a new ic, compare ft2 data and flash data
//	case 0x0006:
//	{
//		if(dut2.g_retest == 0)
//		{
//			for(i=0;i<22;i++)
//			{
//				if(dut2.g_read_ft2_data[i] == dut2.g_read_flash_data[i])
//				{
//					if(i == 21)
//					{
//						dut2.g_pattern_step++;
//					}
//					else
//					{
//
//					}
//				}
//				else
//				{
//					i = 22;
//					dut2.g_result_fail = 0x01;
//					dut2.g_result_fail_tmrcount = 0xffff;
//					xil_printf("dut2 compare ft2 data and flash data fail!\r\n\r\n");
//				}
//			}
//		}
//
//		if(dut2.g_retest == 1)
//		{
//			for(i=1; i<60; i++)
//			{
//				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
//			}
//			dut2.g_pattern_timer = 0x3fff;
//			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
//			dut2.g_pattern_step = 0x00;
//			//dut2.g_dut_pattern_status_buf[2]++;
//			dut2.g_uartPatternEnable = 0x00;
//			dut2.g_uartPatternNum = 0x10;
//			result_output_for_v50(XPAR_AXI_GPIO_dut2_1_BASEADDR,dut2.g_uartPatternNum);
//			xil_printf("dut2.patternF_pass!\r\n\r\n");
//		}
//		break;
//	}
//
//	//if this is a new ic, program efuse
//	case 0x0007:
//	{
//		//LDO output 5.55V
//		i2c_mcp23008_output(AD7994_DEV2_ADDR, MCP23008_ADDR, 0xc0);
//		msdelay(10);
//
//		//power rail: S2_5V_SW(XGPIO8 -> L)
//		if(dut2.g_dut_pattern_status_buf[7] == 0x00)
//		{
//			Buff_dut2_XGPIO_0[0] = 0x30;                            //REG0005 ouput value[7:0]
//			Buff_dut2_XGPIO_0[1] = 0xC2;							//REG0006
//			Buff_dut2_XGPIO_0[2] = 0x40;							//REG0007 output value[15:8]
//			Buff_dut2_XGPIO_0[3] = 0x9E;							//REG0008
//			Buff_dut2_XGPIO_0[4] = 0xC0|(dut2.g_uartPatternNum); 	//REG0009 output value[23:16]
//			Buff_dut2_XGPIO_0[5] = 0x00;							//REG000a
//			Buff_dut2_XGPIO_0[6] = 0x00;							//REG000b output value[31:24]
//			Buff_dut2_XGPIO_0[7] = 0xFF;							//REG000c
//
//			XGpio_dut2_Relay_WriteByte(XPAR_AXI_GPIO_dut2_1_BASEADDR,Buff_dut2_XGPIO_0);
//			dut2.g_dut_pattern_status_buf[7] = 0x01;
//			//xil_printf("PatternNum = %x\r\n",dut2.g_uartPatternNum);
//			xil_printf("dut2.patternE_gpio_control_completed!\r\n");
//			msdelay(10);
//		}
//		else if(dut2.g_dut_pattern_status_buf[7] == 0x01)
//		{
//			if(dut2.g_relay_control_timer == 0 )
//			{
//				dut2.g_dut_pattern_status_buf[7] = 0x00;
//				dut2.g_pattern_step++;
//			}
//		}
//		//output fail result
//		else
//		{
//			dut2.g_result_fail = 0x01;
//			dut2.g_result_fail_tmrcount = 0xffff;
//		}
//		break;
//	}
//
//	//if this is a new ic, program efuse
//	case 0x0008:
//	{
//		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writeefuse;
//		dut2.g_pattern_smbus_control_buf[2] = 0x00;
//		dut2.g_pattern_smbus_control_buf[3] = 0x00;
//		dut2.g_pattern_smbus_control_buf[4] = 0x28;
//
//		for(i=0;i<40;i++)
//		{
//			dut2.g_pattern_smbus_control_buf[i+5] = dut2.g_read_efuse_data[i];
//		}
//
//		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
//		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
//		{
//			break;
//		}
//		else
//		{
//			for(i=1; i<60; i++)
//			{
//				dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
//			}
//
//			dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
//			dut2.g_pattern_step++;
//		}
//		break;
//	}
//
//	//read efuse and compare
//	case 0x0009:
//	{
//		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readefuse;
//		dut2.g_pattern_smbus_control_buf[2] = 0x00;
//		dut2.g_pattern_smbus_control_buf[3] = 0x00;
//		dut2.g_pattern_smbus_control_buf[4] = 0x28;
//
//		smbus2_irq_handle(dut2.g_pattern_smbus_control_buf);
//		if(dut2.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
//		{
//			break;
//		}
//		else
//		{
//			for(i=0;i<40;i++)
//			{
//				if(dut2.g_read_efuse_data[i] != dut2.g_pattern_smbus_control_buf[i+10])
//				{
//					dut2.g_result_fail = 0x01;
//					dut2.g_result_fail_tmrcount = 0xffff;
//					xil_printf("dut2 compare efuse data fail!\r\n\r\n");
//				}
//				else
//				{
//					for(i=1; i<60; i++)
//					{
//						dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
//					}
//					dut2.g_pattern_timer = 0x3fff;
//					dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
//					dut2.g_pattern_step = 0x00;
//					//dut2.g_dut_pattern_status_buf[2]++;
//					dut2.g_uartPatternEnable = 0x00;
//					dut2.g_uartPatternNum = 0x10;
//					result_output_for_v50(XPAR_AXI_GPIO_dut2_1_BASEADDR,dut2.g_uartPatternNum);
//					xil_printf("dut2 compare efuse data pass!\r\n\r\n");
//					xil_printf("dut2.patternF_pass!\r\n\r\n");
//				}
//
//			}
//		}
//		break;
//	}
	}

	if(dut2.g_result_fail == 0x01)
	{
		xil_printf("dut2.patternF_fail! fail_step = %x\r\n",dut2.g_pattern_step);
		pattern_safe_state_dut2();
	}

	return 0;
}
