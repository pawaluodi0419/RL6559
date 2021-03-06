#include "platform.h"

u8 _by_Pattern_check_crc32_dut1()
{
	u8 i;
	u8 Buff_dut1_XGPIO_0[8];
	u8 Loop_run_enable = 0;

	switch(dut1.g_pattern_step)
	{
	//取消Vmon满偏：XGPIO13设定输出low
	case 0x0000:
	{
		if(dut1.g_dut_pattern_status_buf[7] == 0x00)
		{
			Buff_dut1_XGPIO_0[0] = 0x31;                            //REG0005 ouput value[7:0]
			Buff_dut1_XGPIO_0[1] = 0xC2;							//REG0006
			Buff_dut1_XGPIO_0[2] = 0x40;							//REG0007 output value[15:8]
			Buff_dut1_XGPIO_0[3] = 0x9E;							//REG0008
			Buff_dut1_XGPIO_0[4] = 0x00|(dut1.g_uartPatternNum); 	//REG0009 output value[23:16]
			Buff_dut1_XGPIO_0[5] = 0x00;							//REG000a
			Buff_dut1_XGPIO_0[6] = 0x01;							//REG000b output value[31:24]
			Buff_dut1_XGPIO_0[7] = 0xFE;							//REG000c

			XGpio_dut1_Relay_WriteByte(XPAR_AXI_GPIO_dut1_1_BASEADDR,Buff_dut1_XGPIO_0);
			dut1.g_dut_pattern_status_buf[7] = 0x01;
			//xil_printf("PatternNum = %x\r\n",dut1.g_uartPatternNum);
			xil_printf("dut1.pattern_check_crc32_gpio_control_completed!\r\n");
			msdelay(10);
		}
		else if(dut1.g_dut_pattern_status_buf[7] == 0x01)
		{
			if(dut1.g_relay_control_timer == 0 )
			{
				dut1.g_dut_pattern_status_buf[7] = 0x00;
				dut1.g_pattern_step++;
				//msdelay(550);
			}
		}
		//output fail result
		else
		{
			dut1.g_result_fail = 0x01;
			dut1.g_result_fail_tmrcount = 0xffff;
		}
		break;
	}

	//MCU reboot: rh_smbusgeticstatus 0xcc 64 1
	case 0x0001:
	{
		dut1.g_pattern_smbus_control_buf[1] = smbus_cmd_type_geticstatus;
		dut1.g_pattern_smbus_control_buf[2] = 0x40;
		dut1.g_pattern_smbus_control_buf[3] = 0x00;
		dut1.g_pattern_smbus_control_buf[4] = 0x01;

		smbus1_irq_handle(dut1.g_pattern_smbus_control_buf);
		if(dut1.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut1.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut1.g_pattern_step++;
			msdelay(200);
		}
		break;
	}

	case 0x0002:
	{
		dut1.g_pattern_smbus_control_buf[1] = smbus_cmd_type_vdcmdenable;
		dut1.g_pattern_smbus_control_buf[2] = 0xda;
		dut1.g_pattern_smbus_control_buf[3] = 0x0b;
		dut1.g_pattern_smbus_control_buf[4] = 0x01;

		smbus1_irq_handle(dut1.g_pattern_smbus_control_buf);
		if(dut1.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=1; i<60; i++)
			{
				dut1.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut1.g_pattern_step++;
		}
		break;
	}

	case 0x0003:
	{
		dut1.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readmem;
		dut1.g_pattern_smbus_control_buf[2] = 0x07;
		dut1.g_pattern_smbus_control_buf[3] = 0xdf;
		dut1.g_pattern_smbus_control_buf[4] = 0x01;

		smbus1_irq_handle(dut1.g_pattern_smbus_control_buf);
		if(dut1.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			xil_printf("dut1 read 0xdf07 =%x\r\n", dut1.g_pattern_smbus_control_buf[10]);

			if(dut1.g_pattern_smbus_control_buf[10] == 0x40)
			{
				xil_printf("dut1.pattern_check_crc32_pass!\r\n\r\n");
				dut1.g_pattern_step++;
			}
			else
			{
				dut1.g_result_fail = 0x01;
				dut1.g_result_fail_tmrcount = 0xffff;
			}

			for(i=1; i<60; i++)
			{
				dut1.g_pattern_smbus_control_buf[i] = CLEAR_;
			}
			dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		}
		break;
	}

	//read efuse
	case 0x0004:
	{
		dut1.g_pattern_smbus_control_buf[1] = smbus_cmd_type_readefuse;
		dut1.g_pattern_smbus_control_buf[2] = 0x00;
		dut1.g_pattern_smbus_control_buf[3] = 0x00;
		dut1.g_pattern_smbus_control_buf[4] = 0x28;

		smbus1_irq_handle(dut1.g_pattern_smbus_control_buf);
		if(dut1.g_pattern_smbus_control_buf[0] != smbus_road_done_pass)
		{
			break;
		}
		else
		{
			for(i=0; i<40; i++)
			{
				xil_printf("dut1.efuse_data[%02d]= %02x\r\n", i, dut1.g_pattern_smbus_control_buf[i+10]);
			}

			for(i=1; i<60; i++)
			{
				dut1.g_pattern_smbus_control_buf[i] = CLEAR_;
			}

			dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut1.g_pattern_step++;
		}
		break;
	}

	case 0x0005:
	{
		dut1.g_dut_start_ready = 0;

		if(Loop_run_enable == 1)
		{
			dut1.g_dut_start_ready = 0x11;
			dut1.g_uartPatternNum = 0x00;
			dut1.g_test_count++;
			xil_printf("dut1.g_test_count =%x\r\n", dut1.g_test_count);
		}
		else
		{
			dut1.g_uartPatternNum = 0x17;
			dut1.g_pass_count++;
		}
		result_output_for_v50(XPAR_AXI_GPIO_dut1_1_BASEADDR,dut1.g_uartPatternNum);
		xil_printf("\r\ndut1.FT2_pass, dut1.g_pass_count = %d\r\n\r\n", dut1.g_pass_count);
		//xil_printf("dut1.g_ft2_test_done = %x\r\n",dut1.g_ft2_test_done);
		//xil_printf("dut1.g_dut_start_ready = %x\r\n\r\n",dut1.g_dut_start_ready);
		//xil_printf("dut1.g_result_fail = %x\r\n\r\n",dut1.g_result_fail);
		//xil_printf("dut1.g_pattern_step = %x\r\n\r\n",dut1.g_pattern_step);

		pattern_safe_state_dut1();

		break;
	}
	}

	if(dut1.g_result_fail == 0x01)
	{
		dut1.g_ack_bit_reread_timer = 20;
		xil_printf("dut1.pattern_check_crc32_fail! fail_step = %x\r\n",dut1.g_pattern_step);
		pattern_safe_state_dut1();
	}

	return 0;
}
