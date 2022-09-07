#include "platform.h"

u8 _by_Pattern_U2_host_test2_dut0()
{
	u8 i;

	switch(dut0.g_pattern_step)
	{
	case 0x0000:
	{
		dut0.g_start_test_flag2 = 0x02;
		dut0.g_result_polling_tmrcount = 20;

		i2c_get_result_dut0(AD7994_DEV0_ADDR, USB_HOST_ADDR);
		msdelay(5);

		for(i=0;i<5;i++)
		{
			xil_printf("dut0.g_i2cRecBuf[%02d]= %02x\r\n", i, dut0.g_i2cRecBuf[i]);
		}
		xil_printf("\r\n");

		dut0.g_pattern_step++;

		break;
	}

	case 0x0001:
	{
		if((dut0.g_i2cRecBuf[1] & 0x1f) == 0x1F)
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
			result_output_for_v50(XPAR_AXI_GPIO_dut0_1_BASEADDR,dut0.g_uartPatternNum);
			dut0.g_uartPatternNum++;
			xil_printf("dut0.pattern_u2_host_test2_pass!\r\n\r\n");
		}
		else
		{
			if(dut0.g_result_polling_tmrcount > 0)
			{
				i2c_get_result_dut0(AD7994_DEV0_ADDR, USB_HOST_ADDR);
				msdelay(5);

//				for(i=0;i<5;i++)
//				{
//					dut0.g_i2cRecBuf[i] = get_result_databuf[i];
//				}

				for(i=0;i<5;i++)
				{
					xil_printf("dut0.g_i2cRecBuf[%02d]= %02x\r\n", i, dut0.g_i2cRecBuf[i]);
				}

				dut0.g_result_polling_tmrcount--;
				xil_printf("dut0.g_result_polling_tmrcount = %d\r\n", dut0.g_result_polling_tmrcount);
				msdelay(100);	//每100ms polling一次结果
			}
			else
			{
				dut0.g_result_fail = 0x01;
				dut0.g_result_fail_tmrcount = 0xffff;
			}
		}
		break;
	}
	}

	if(dut0.g_result_fail == 0x01)
	{
		xil_printf("dut0.pattern_u2_host_test2_fail! fail_step = %x\r\n",dut0.g_pattern_step);
		pattern_safe_state_dut0();
	}

	return 0;
}
