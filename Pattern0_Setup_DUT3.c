#include "platform.h"

u8 _by_Pattern0_Setup_dut3()
{
	u8 i;
	u8 Buff_dut3_XGPIO_0[8];
	u8 read_icstatus_data[20];
	u8 read_flashdata[40];

	switch(dut3.g_pattern_step)
	{
	//FT mode£ºVMONÂúÆ«(XGPIO13 -> H, XGPIO14 -> L), GPIO10Îªlow(XGPIO23 -> L)
	case 0x0000:
	{
		if(dut3.g_dut_pattern_status_buf[7] == 0x00)
		{
			Buff_dut3_XGPIO_0[0] = 0x00;                          	//REG0005 ouput value[7:0]
			Buff_dut3_XGPIO_0[1] = 0xFF;							//REG0006
			Buff_dut3_XGPIO_0[2] = 0x20;							//REG0007 output value[15:8]
			Buff_dut3_XGPIO_0[3] = 0xDE;							//REG0008
			Buff_dut3_XGPIO_0[4] = 0x40|(dut3.g_uartPatternNum); 	//REG0009 output value[23:16]
			Buff_dut3_XGPIO_0[5] = 0x00;							//REG000a
			Buff_dut3_XGPIO_0[6] = 0x02;							//REG000b output value[31:24]
			Buff_dut3_XGPIO_0[7] = 0xFC;							//REG000c

			XGpio_dut3_Relay_WriteByte(XPAR_AXI_GPIO_dut3_1_BASEADDR,Buff_dut3_XGPIO_0);
			dut3.g_dut_pattern_status_buf[7] = 0x01;
			xil_printf("dut3.pattern0_ft_mode_control_completed!\r\n");
			msdelay(10);
		}
		else if(dut3.g_dut_pattern_status_buf[7] == 0x01)
		{
			if(dut3.g_relay_control_timer == 0 )
			{
				dut3.g_dut_pattern_status_buf[7] = 0x00;
				dut3.g_pattern_step++;

				//XGpio_2_WriteBit(0,dut3_FT2_PWR_CTRL_OFFSET,0x00);	//S0_PWR_EN# output low, power on DUT
				i2c_mcp23008_output(AD7994_DEV3_ADDR, MCP23008_ADDR, 0xc0);
				xil_printf("dut3 power on!\r\n\r\n");
				msdelay(200);
			}
		}
		//output fail result
		else
		{
			dut3.g_result_fail = 0x01;
			dut3.g_result_fail_tmrcount = 0xffff;
		}
		break;
	}

	case 0x0001:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_vdcmdenable;
		dut3.g_pattern_smbus_control_buf[2] = 0xda;
		dut3.g_pattern_smbus_control_buf[3] = 0x0b;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut3.g_pattern_step++;
		}
		break;
	}

	//check FT mode
	case 0x0002:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut3.g_pattern_smbus_control_buf[2] = 0x07;
		dut3.g_pattern_smbus_control_buf[3] = 0xdf;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut3 read 0xdf07 =%x\r\n", dut3.g_pattern_smbus_control_buf[10]);

			if(dut3.g_pattern_smbus_control_buf[10] == 0x20)
			{
				xil_printf("dut3 check FT mode pass!\r\n\r\n");
				dut3.g_pattern_step = 0x04;
			}
			else
			{
				i2c_mcp23008_output(AD7994_DEV3_ADDR, MCP23008_ADDR, 0x00);
				msdelay(50);
				i2c_mcp23008_output(AD7994_DEV3_ADDR, MCP23008_ADDR, 0xc0);
				msdelay(200);
				xil_printf("dut3 check FT mode again!\r\n\r\n");
				dut3.g_pattern_step = 0x03;
			}

			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		}
		break;
	}

	//check FT mode retry
	case 0x0003:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut3.g_pattern_smbus_control_buf[2] = 0x07;
		dut3.g_pattern_smbus_control_buf[3] = 0xdf;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut3 read 0xdf07 =%x\r\n", dut3.g_pattern_smbus_control_buf[10]);

			if(dut3.g_pattern_smbus_control_buf[10] == 0x20)
			{
				xil_printf("dut3 check FT mode pass!\r\n\r\n");
				dut3.g_pattern_step++;
			}
			else
			{
				xil_printf("dut3 check FT mode fail!\r\n\r\n");

				dut3.g_result_fail = 0x01;
				dut3.g_result_fail_tmrcount = 0xffff;
			}

			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		}
		break;
	}

	//disconnect aux mux switch
	case 0x0004:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut3.g_pattern_smbus_control_buf[2] = 0x15;
		dut3.g_pattern_smbus_control_buf[3] = 0xc4;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;
		dut3.g_pattern_smbus_control_buf[5] = 0x00;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut3.g_pattern_step++;
		}
		break;
	}

//	//check IC information
//	case 0x000b:
//	{
//		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_geticstatus;
//		dut3.g_pattern_smbus_control_buf[2] = 0x00;
//		dut3.g_pattern_smbus_control_buf[3] = 0x00;
//		dut3.g_pattern_smbus_control_buf[4] = 0x14;
//
//		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
//		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
//		{
//			break;
//		}
//		else
//		{
//			for(i=0;i<21;i++)
//			{
//				read_icstatus_data[i] = dut3.g_pattern_smbus_control_buf[i+10];
//			}
//			xil_printf("dut3.read_icstatus_data[11] =0x%x\r\n", read_icstatus_data[11]);
//			xil_printf("dut3.read_icstatus_data[12] =0x%x\r\n", read_icstatus_data[12]);
//
//			if((read_icstatus_data[11] == 0x57) && (read_icstatus_data[12] == 0x54))
//			{
//			    xil_printf("dut3 check IC information pass!\r\n\r\n");
//
//				for(i=1; i<60; i++)
//				{
//					dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
//				}
//
//				dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
//				dut3.g_pattern_step++;
//			}
//			else
//			{
//				xil_printf("dut3 check IC information fail!\r\n\r\n");
//
//				dut3.g_result_fail = 0x01;
//				dut3.g_result_fail_tmrcount = 0xffff;
//			}
//		}
//		break;
//	}

	//check MCM flash ID
	case 0x0005:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_vdcmdenable;
		dut3.g_pattern_smbus_control_buf[2] = 0x04;
		dut3.g_pattern_smbus_control_buf[3] = 0x00;
		dut3.g_pattern_smbus_control_buf[4] = 0x02;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut3.get_mcmflash_id1 =0x%x\r\n", dut3.g_pattern_smbus_control_buf[10]);
			xil_printf("dut3.get_mcmflash_id2 =0x%x\r\n", dut3.g_pattern_smbus_control_buf[11]);

			if((dut3.g_pattern_smbus_control_buf[10] == 0xef) && (dut3.g_pattern_smbus_control_buf[11] == 0x11))
			{
			    xil_printf("dut3 check MCM flash ID pass!\r\n\r\n");

				for(i=1; i<60; i++)
				{
					dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
				}

				dut3.g_pattern_timer = 0x3fff;
				dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				dut3.g_pattern_step = 0x00;
				//dut3.g_dut_pattern_status_buf[2]++;
				dut3.g_uartPatternEnable = 0x00;
				dut3.g_uartPatternNum++;
				result_output_for_v50(XPAR_AXI_GPIO_dut3_1_BASEADDR,dut3.g_uartPatternNum);
				xil_printf("dut3_pattern0_setup_pass!\r\n\r\n");
			}
			else
			{
				xil_printf("dut3 check MCM flash ID fail!\r\n\r\n");

				dut3.g_result_fail = 0x01;
				dut3.g_result_fail_tmrcount = 0xffff;
			}
		}
		break;
	}
	}

	if(dut3.g_result_fail == 0x01)
	{
		xil_printf("dut3_pattern0_setup_fail! fail_step = %x\r\n",dut3.g_pattern_step);
		pattern_safe_state_dut3();
	}

	return 0;
}


