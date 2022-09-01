#include "platform.h"

u8 _by_Pattern_PD_msg_dut3()
{
	u8 i;
	u8 Buff_dut3_XGPIO_0[8];

	switch(dut3.g_pattern_step)
	{
	//reset to flash前需满足进FT mode条件：XGPIO13设定输出high, XGPIO23设定输出low
	//PD msg test relay control：XGPIO0设定输出high, XGPIO24设定输出high
	//DP obverse/U2 mux A: XGPIO22 -> L
	case 0x0000:
	{
		if(dut3.g_dut_pattern_status_buf[7] == 0x00)
		{
			Buff_dut3_XGPIO_0[0] = 0x31;                            //REG0005 ouput value[7:0]
			Buff_dut3_XGPIO_0[1] = 0xC2;							//REG0006
			Buff_dut3_XGPIO_0[2] = 0x60;							//REG0007 output value[15:8]
			Buff_dut3_XGPIO_0[3] = 0x9E;							//REG0008
			Buff_dut3_XGPIO_0[4] = 0x00|(dut3.g_uartPatternNum); 	//REG0009 output value[23:16]
			Buff_dut3_XGPIO_0[5] = 0x00;							//REG000a
			Buff_dut3_XGPIO_0[6] = 0x03;							//REG000b output value[31:24]
			Buff_dut3_XGPIO_0[7] = 0xFC;							//REG000c

			XGpio_dut3_Relay_WriteByte(XPAR_AXI_GPIO_dut3_1_BASEADDR,Buff_dut3_XGPIO_0);
			dut3.g_dut_pattern_status_buf[7] = 0x01;
			//xil_printf("PatternNum = %x\r\n",dut3.g_uartPatternNum);
			xil_printf("dut3.pattern_PD_msg_gpio_control_completed!\r\n");
			msdelay(10);
		}
		else if(dut3.g_dut_pattern_status_buf[7] == 0x01)
		{
			if(dut3.g_relay_control_timer == 0 )
			{
				dut3.g_dut_pattern_status_buf[7] = 0x00;
				dut3.g_pattern_step++;
				//msdelay(550);
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

	//MCU reboot: rh_smbusgeticstatus 0xcc 64 1
	case 0x0001:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_geticstatus;
		dut3.g_pattern_smbus_control_buf[2] = 0x40;
		dut3.g_pattern_smbus_control_buf[3] = 0x00;
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
			msdelay(200);
		}
		break;
	}

	case 0x0002:
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
/*
	//configure DP reverse
	case 0x0004:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut3.g_pattern_smbus_control_buf[2] = 0x15;
		dut3.g_pattern_smbus_control_buf[3] = 0xc4;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;
		dut3.g_pattern_smbus_control_buf[5] = 0x34;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
//			xil_printf("dut3.read_0xC415 =0x%x\r\n", dut3.g_pattern_smbus_control_buf[10]);

			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut3.g_pattern_step++;
		}
		break;
	}

	//configure U2 mux B
	case 0x0005:
	{
		dut3.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writephy;
		dut3.g_pattern_smbus_control_buf[2] = 0x01;
		dut3.g_pattern_smbus_control_buf[3] = 0x00;
		dut3.g_pattern_smbus_control_buf[4] = 0x01;
		dut3.g_pattern_smbus_control_buf[5] = 0x00;
		dut3.g_pattern_smbus_control_buf[6] = 0x03;

		smbus3_irq_handle(dut3.g_pattern_smbus_control_buf);
		if(dut3.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
//			xil_printf("dut3.read_0x0001 =0x%x\r\n", dut3.g_pattern_smbus_control_buf[10]);
//			xil_printf("dut3.read_0x0001 =0x%x\r\n\r\n", dut3.g_pattern_smbus_control_buf[11]);

			for(i=1; i<60; i++)
			{
				dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;

			dut3.g_start_test_flag2 = 0x02;
			dut3.g_pattern_step++;
		}
		break;
	}
*/
	//polling ack bit
	case 0x0003:
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

			if(dut3.g_pattern_smbus_control_buf[10] == 0x80)
			{
				for(i=1; i<60; i++)
				{
					dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
				}

				dut3.g_ack_bit_reread_timer = 20;
				dut3.g_pattern_timer = 0x3fff;
				dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
				dut3.g_pattern_step = 0x00;
				//dut3.g_dut_pattern_status_buf[2]++;
				dut3.g_uartPatternEnable = 0x00;
				dut3.g_uartPatternNum = 0x14;
				//dut3.g_uartPatternNum = (dut3.g_uartPatternNum++ & 0xff);
				result_output_for_v50(XPAR_AXI_GPIO_dut3_1_BASEADDR,dut3.g_uartPatternNum);
				xil_printf("dut3.pattern_PD_msg_pass!\r\n\r\n");
			}
			else
			{
				if(dut3.g_ack_bit_reread_timer > 0)
				{
					for(i=1; i<60; i++)
					{
						dut3.g_pattern_smbus_control_buf[i] = CLEAR_;
					}
					dut3.g_pattern_smbus_control_buf[0] = smbus_road_waiting;

					//xil_printf("dut3.g_ack_bit_reread_timer =%x\r\n", dut3.g_ack_bit_reread_timer);
					dut3.g_ack_bit_reread_timer--;
					msdelay(100);	//每100ms polling一次结果，timeout时间2s
				}
				else
				{
					dut3.g_result_fail = 0x01;
					dut3.g_result_fail_tmrcount = 0xffff;
				}
			}
		}
		break;
	}
	}

	if(dut3.g_result_fail == 0x01)
	{
		dut3.g_ack_bit_reread_timer = 20;
		xil_printf("dut3.pattern_PD_msg_fail! fail_step = %x\r\n",dut3.g_pattern_step);
		pattern_safe_state_dut3();
	}

	return 0;
}
