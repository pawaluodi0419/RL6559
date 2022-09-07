#include "platform.h"

u8 _by_PatternC_dut2()
{
	u8 i;
	u8 Buff_dut2_XGPIO_0[8];

	switch(dut2.g_pattern_step)
	{
	//write pattern index
	case 0x0000:
	{
		dut2.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut2.g_pattern_smbus_control_buf[2] = 0x33;
		dut2.g_pattern_smbus_control_buf[3] = 0xdf;
		dut2.g_pattern_smbus_control_buf[4] = 0x01;
		dut2.g_pattern_smbus_control_buf[5] = 0x0c;

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
	case 0x0001:
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
			if(dut2.g_pattern_smbus_control_buf[10] == 0x8C)
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
				dut2.g_uartPatternNum = 0x20;
				result_output_for_v50(XPAR_AXI_GPIO_dut2_1_BASEADDR,dut2.g_uartPatternNum);
				xil_printf("dut2.patternC_pass!\r\n\r\n");
				dut2.g_start_test_flag1 = 0x01;
			}
			else
			{
				dut2.g_result_fail = 0x01;
				dut2.g_result_fail_tmrcount = 0xffff;
			}
		}
		break;
	}
	}

	if(dut2.g_result_fail == 0x01)
	{
		xil_printf("dut2.patternC_fail! fail_step = %x\r\n",dut2.g_pattern_step);
		pattern_safe_state_dut2();
	}

	return 0;
}
