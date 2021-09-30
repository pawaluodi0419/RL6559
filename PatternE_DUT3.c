#include "platform.h"

u8 _by_PatternE_dut3()
{
	u8 i;
	u8 Buff_dut3_XGPIO_0[8];

	switch(dut3.g_pattern_step)
	{
	//CC1/CC2切换到CC1_TRH/CC2_TRH: XGPIO0设定输出low, XGPIO2设定输出low
	case 0x0000:
	{
		if(dut3.g_dut_pattern_status_buf[7] == 0x00)
		{
			Buff_dut3_XGPIO_0[0] = 0x30;                            //REG0005 ouput value[7:0]
			Buff_dut3_XGPIO_0[1] = 0xC2;							//REG0006
			Buff_dut3_XGPIO_0[2] = 0x40;							//REG0007 output value[15:8]
			Buff_dut3_XGPIO_0[3] = 0x9E;							//REG0008
			Buff_dut3_XGPIO_0[4] = 0xC0|(dut3.g_uartPatternNum); 	//REG0009 output value[23:16]
			Buff_dut3_XGPIO_0[5] = 0x00;							//REG000a
			Buff_dut3_XGPIO_0[6] = 0x02;							//REG000b output value[31:24]
			Buff_dut3_XGPIO_0[7] = 0xFC;							//REG000c

			XGpio_dut3_Relay_WriteByte(XPAR_AXI_GPIO_dut3_1_BASEADDR,Buff_dut3_XGPIO_0);
			dut3.g_dut_pattern_status_buf[7] = 0x01;
			xil_printf("dut3.patternE_gpio_control_completed!\r\n");
			msdelay(10);
		}
		else if(dut3.g_dut_pattern_status_buf[7] == 0x01)
		{
			if(dut3.g_relay_control_timer == 0 )
			{
				dut3.g_dut_pattern_status_buf[7] = 0x00;
				dut3.g_pattern_step++;
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

	//驱动DAC MCP4822 VOUTA(channel CC1)输出0.52V
	case 0x0001:
	{
		dut3.g_dac_data_buf[0] = 0x01;	//0x01: CC1 0x03: CC2	如果电压值超过2V, 需要写成0x05/0x07
		dut3.g_dac_data_buf[1] = 0x10;	//Data L
		dut3.g_dac_data_buf[2] = 0x04;	//Data H
		dut3.g_dac_data_buf[3] = 0xd4;	//Data L
		dut3.g_dac_data_buf[4] = 0x03;	//Data H
		dut3.g_dac_data_buf[5] = 0x4c;	//Data L
		dut3.g_dac_data_buf[6] = 0x04;	//Data H

		_by_mcp4822_output_control_dut3(dut3.g_dac_data_buf);
		dut3.g_pattern_step++;
		//xil_printf("dut3_dac_writen =%x,%x,%x\r\n",dut3.g_dac_data_buf[0],dut3.g_dac_data_buf[1],dut3.g_dac_data_buf[2]);
		break;
	}

	//驱动DAC MCP4822 VOUTB(channel CC2)输出0.52V
	case 0x0002:
	{
		dut3.g_dac_data_buf[0] = 0x03;	//0x01: CC1 0x03: CC2	如果电压值超过2V, 需要写成0x05/0x07
		dut3.g_dac_data_buf[1] = 0x10;	//Data L
		dut3.g_dac_data_buf[2] = 0x04;	//Data H
		dut3.g_dac_data_buf[3] = 0xd4;	//Data L
		dut3.g_dac_data_buf[4] = 0x03;	//Data H
		dut3.g_dac_data_buf[5] = 0x4c;	//Data L
		dut3.g_dac_data_buf[6] = 0x04;	//Data H

		_by_mcp4822_output_control_dut3(dut3.g_dac_data_buf);
		dut3.g_pattern_step++;
		//xil_printf("dut3_dac_writen =%x,%x,%x\r\n",dut3.g_dac_data_buf[0],dut3.g_dac_data_buf[1],dut3.g_dac_data_buf[2]);
		break;
	}

	//write pattern index
	case 0x0003:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut3.g_pattern_smbus_control_buf[2] = 0x33;
		dut3.g_pattern_smbus_control_buf[3] = 0xdf;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;
		dut3.g_pattern_smbus_control_buf[5] = 0x0e;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut3 write pattern index =%x\r\n", dut3.g_pattern_smbus_control_buf[5]);
			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut3.g_pattern_step++;
			//msdelay(220);
		}
		break;
	}

	//polling ack bit
	case 0x0004:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut3.g_pattern_smbus_control_buf[2] = 0x33;
		dut3.g_pattern_smbus_control_buf[3] = 0xdf;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut3 polling pattern ack bit =%x\r\n", dut3.g_pattern_smbus_control_buf[10]);
			if(dut3.g_pattern_smbus_control_buf[10] == 0x8E)
			{
				xil_printf("dut3.patternE_pass!\r\n\r\n");

				for(i=1; i<60; i++)
				{
					dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
				}

				dut3.g_pattern_timer = 0xfff;
				dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				dut3.g_pattern_step = 0x00;
				//dut3.g_dut_pattern_status_buf[2]++;
				dut3.g_uartPatternEnable = 0x00;
				dut3.g_uartPatternNum++;
				result_output_for_v50(XPAR_AXI_GPIO_dut3_1_BASEADDR,dut3.g_uartPatternNum);
			}
			else
			{
				dut3.g_result_fail = 0x01;
				dut3.g_result_fail_tmrcount = 0xffff;
			}
		}
		break;
	}

//	//read flash data: rh_smbusgeticstatus 0xcc 33 22
//	case 0x0005:
//	{
//		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_geticstatus;
//		dut3.g_pattern_smbus_control_buf[2] = 0x21;
//		dut3.g_pattern_smbus_control_buf[3] = 0x00;
//		dut3.g_pattern_smbus_control_buf[4] = 0x16;
//
//		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
//		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
//		{
//			break;
//		}
//		else
//		{
//			for(i=0;i<22;i++)
//			{
//				//xil_printf("dut3.flash_data[%02d]= %02x\r\n", i, dut3.g_pattern_smbus_control_buf[i+10]);
//			}
//
//			if(dut3.g_pattern_smbus_control_buf[31] == 0x5A)
//			{
//				dut3.g_retest = 1;
//				xil_printf("dut3 is a retest ic!\r\n\r\n");
//			}
//			else
//			{
//				dut3.g_retest = 0;
//				xil_printf("dut3 is a new ic!\r\n\r\n");
//			}
//
//			for(i=1; i<60; i++)
//			{
//				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
//			}
//
//			dut3.g_pattern_timer = 0xfff;
//			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
//			dut3.g_pattern_step = 0x00;
//			//dut3.g_dut_pattern_status_buf[2]++;
//			dut3.g_uartPatternEnable = 0x00;
//			dut3.g_uartPatternNum = 0x0f;
//			result_output_for_v50(XPAR_AXI_GPIO_dut3_1_BASEADDR,dut3.g_uartPatternNum);
//		}
//		break;
//	}
	}

	if(dut3.g_result_fail == 0x01)
	{
		xil_printf("dut3.patternE_fail! fail_step = %x\r\n",dut3.g_pattern_step);
		pattern_safe_state_dut3();
	}

	return 0;
}
