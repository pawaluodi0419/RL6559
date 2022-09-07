#include "platform.h"

u8 _by_Pattern_U2_host_test2_dut2()
{
	u8 i;

	switch(dut2.g_pattern_step)
	{
	case 0x0000:
	{
		dut2.g_start_test_flag2 = 0x02;
		dut2.g_result_polling_tmrcount = 20;

		i2c_get_result_dut2(AD7994_DEV0_ADDR, USB_HOST_ADDR);
		msdelay(5);

		for(i=0;i<5;i++)
		{
			xil_printf("dut2.g_i2cRecBuf[%02d]= %02x\r\n", i, dut2.g_i2cRecBuf[i]);
		}
		xil_printf("\r\n");

		dut2.g_pattern_step++;

		break;
	}

	case 0x0001:
	{
		if((dut2.g_i2cRecBuf[3] & 0x1f) == 0x1F)
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
			result_output_for_v50(XPAR_AXI_GPIO_dut2_1_BASEADDR,dut2.g_uartPatternNum);
			dut2.g_uartPatternNum++;
			xil_printf("dut2.pattern_u2_host_test2_pass!\r\n\r\n");
		}
		else
		{
			if(dut2.g_result_polling_tmrcount > 0)
			{
				i2c_get_result_dut2(AD7994_DEV0_ADDR, USB_HOST_ADDR);
				msdelay(5);

//				for(i=0;i<5;i++)
//				{
//					dut2.g_i2cRecBuf[i] = get_result_databuf[i];
//				}

				for(i=0;i<5;i++)
				{
					xil_printf("dut2.g_i2cRecBuf[%02d]= %02x\r\n", i, dut2.g_i2cRecBuf[i]);
				}

				dut2.g_result_polling_tmrcount--;
				xil_printf("dut2.g_result_polling_tmrcount = %d\r\n", dut2.g_result_polling_tmrcount);
				msdelay(100);	//每100ms polling一次结果
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
		xil_printf("dut2.pattern_u2_host_test2_fail! fail_step = %x\r\n",dut2.g_pattern_step);
		pattern_safe_state_dut2();
	}

	return 0;
}
