#include "platform.h"

u8 _by_PatternD_dut1()
{
	u8 i;
	u8 j;
	u8 Buff_dut1_XGPIO_0[8];
	//u8 cc_compare_temp_buf[20];
	//u8 CCDET_Tolerance_Compare_Data = 3;

	switch(dut1.g_ccdet_step)
	{
	//CCDet relay Control: XGPIO0设定输出low,  XGPIO2设定输出low
	case 0x0000:
	{
		if(dut1.g_dut_pattern_status_buf[7] == 0x00)
		{
			Buff_dut1_XGPIO_0[0] = 0x30;                            //REG0005 ouput value[7:0]
			Buff_dut1_XGPIO_0[1] = 0xC2;							//REG0006
			Buff_dut1_XGPIO_0[2] = 0x40;							//REG0007 output value[15:8]
			Buff_dut1_XGPIO_0[3] = 0x9E;							//REG0008
			Buff_dut1_XGPIO_0[4] = 0xC0|(dut1.g_uartPatternNum); 	//REG0009 output value[23:16]
			Buff_dut1_XGPIO_0[5] = 0x00;							//REG000a
			Buff_dut1_XGPIO_0[6] = 0x02;							//REG000b output value[31:24]
			Buff_dut1_XGPIO_0[7] = 0xFC;							//REG000c

			XGpio_dut1_Relay_WriteByte(XPAR_AXI_GPIO_dut1_1_BASEADDR,Buff_dut1_XGPIO_0);
			dut1.g_dut_pattern_status_buf[7] = 0x01;
			xil_printf("dut1.patternD_gpio_control_completed!\r\n");
			msdelay(10);
		}
		else if(dut1.g_dut_pattern_status_buf[7] == 0x01)
		{
			if(dut1.g_relay_control_timer == 0 )
			{
				dut1.g_dut_pattern_status_buf[7] = 0x00;
				dut1.g_ccdet_step++;
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

	/****************************************
	*CC_Det Calibration start!
	*6613 CC Det register
	*对应的电压档：0xC1A0
	*对应的电压档的偏移值：0x109/0x10A/0x10B/0x10C
	*对结果数值的读取：0xC1A9
	*共需校准的7个电压：0.2v/0.4v/0.66v/0.8v/1.23v/1.6v/2.6v
	******************************************/

	//CC1 0.2V calibration
	case 0x0001:
	{
		dut1.g_dac_data_buf[0] = 0x01;
		dut1.g_dac_data_buf[1] = 0x90;
		dut1.g_dac_data_buf[2] = 0x01;	//0x0190换算成十进制为400, 400/2000 = 0.2
		dut1.g_dac_data_buf[3] = 0x68;
		dut1.g_dac_data_buf[4] = 0x01;	//0x0168换算成十进制为360, 360/2000 = 0.18
		dut1.g_dac_data_buf[5] = 0xb8;
		dut1.g_dac_data_buf[6] = 0x01;	//0x01b8换算成十进制为440, 440/2000 = 0.22

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x09;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0x44;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[4];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[5];
		dut1.g_ccdet_phy_calibration_bit         = 3;
		dut1.g_ccdet_ana_result_bit              = 2;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[4] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[5] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
			xil_printf("dut1_CC1_0.2V_Calibration pass!\r\n");
			msdelay(5);
		}
		break;
	}

	//CC1 0.4V calibration
	case 0x0002:
	{
		dut1.g_dac_data_buf[0] = 0x01;
		dut1.g_dac_data_buf[1] = 0x20;
		dut1.g_dac_data_buf[2] = 0x03;	//0x0320换算成十进制为800, 800/2000 = 0.4
		dut1.g_dac_data_buf[3] = 0xe4;
		dut1.g_dac_data_buf[4] = 0x02;	//0x02e4换算成十进制为740, 740/2000 = 0.37
		dut1.g_dac_data_buf[5] = 0x5c;
		dut1.g_dac_data_buf[6] = 0x03;	//0x035c换算成十进制为2460, 860/2000 = 0.43

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x09;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0xaa;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[4];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[5];
		dut1.g_ccdet_phy_calibration_bit         = 2;
		dut1.g_ccdet_ana_result_bit              = 3;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[4] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[5] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;

			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
			xil_printf("dut1_CC1_0.4V_Calibration pass!\r\n");
			msdelay(5);
		}
		break;
	}

	//CC1 1.6V calibration
	case 0x0003:
	{
		dut1.g_dac_data_buf[0] = 0x01;
		dut1.g_dac_data_buf[1] = 0x80;
		dut1.g_dac_data_buf[2] = 0x0c;	//0x0c80换算成十进制为3200, 3200/2000 = 1.6
		dut1.g_dac_data_buf[3] = 0x1c;
		dut1.g_dac_data_buf[4] = 0x0c;	//0x0c1c换算成十进制为2460, 3100/2000 = 1.55
		dut1.g_dac_data_buf[5] = 0xbc;
		dut1.g_dac_data_buf[6] = 0x0c;	//0x0cbc换算成十进制为2460, 3260/2000 = 1.63

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x0a;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0xaa;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[6];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[7];
		dut1.g_ccdet_phy_calibration_bit         = 2;
		dut1.g_ccdet_ana_result_bit              = 4;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{
			break;
		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[6] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[7] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
			xil_printf("dut1_CC1_1.6V_Calibration pass!\r\n");
			msdelay(5);
		}
		break;
	}

	//CC1 0.66V calibration
	case 0x0004:
	{
		dut1.g_dac_data_buf[0] = 0x01;	//0x01: CC1 0x03: CC2	如果电压值超过2V, 需要写成0x05/0x07
		dut1.g_dac_data_buf[1] = 0x28;	//Data L
		dut1.g_dac_data_buf[2] = 0x05;	//Data H	0x0528换算成十进制为1320, 1320/2000 = 0.66
		dut1.g_dac_data_buf[3] = 0x00;	//Data L
		dut1.g_dac_data_buf[4] = 0x05;	//Data H	0x0500换算成十进制为1280, 1280/2000 = 0.64
		dut1.g_dac_data_buf[5] = 0x50;	//Data L
		dut1.g_dac_data_buf[6] = 0x05;	//Data H	0x0550换算成十进制为1360, 1280/2000 = 0.68

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x09;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0x00;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[4];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[5];
		dut1.g_ccdet_phy_calibration_bit         = 1;
		dut1.g_ccdet_ana_result_bit              = 3;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[4] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[5] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
			xil_printf("dut1_CC1_0.66V_Calibration pass!\r\n");
			msdelay(5);
		}
		break;
	}

	//CC1 1.23V calibration
	case 0x0005:
	{
		dut1.g_dac_data_buf[0] = 0x01;
		dut1.g_dac_data_buf[1] = 0x9c;
		dut1.g_dac_data_buf[2] = 0x09;	//0x099c换算成十进制为2460, 2460/2000 = 1.23
		dut1.g_dac_data_buf[3] = 0x60;
		dut1.g_dac_data_buf[4] = 0x09;	//0x0960换算成十进制为2400, 2400/2000 = 1.2
		dut1.g_dac_data_buf[5] = 0xd8;
		dut1.g_dac_data_buf[6] = 0x09;	//0x09d8换算成十进制为2520, 2520/2000 = 1.26

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x0a;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0x00;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[6];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[7];
		dut1.g_ccdet_phy_calibration_bit         = 3;
		dut1.g_ccdet_ana_result_bit              = 4;
		//xil_printf("dut1_pattern5_CC1_1.23V_Calibration_before\r\n");
		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[6] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[7] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
			xil_printf("dut1_CC1_1.23V_Calibration pass!\r\n");
			msdelay(5);
		}
		break;
	}

	//CC1 0.8V calibration
	case 0x0006:
	{
		dut1.g_dac_data_buf[0] = 0x01;
		dut1.g_dac_data_buf[1] = 0x40;
		dut1.g_dac_data_buf[2] = 0x06;	//0x0640换算成十进制为1600, 1600/2000 = 0.8
		dut1.g_dac_data_buf[3] = 0x04;
		dut1.g_dac_data_buf[4] = 0x06;	//0x0604换算成十进制为1540, 1540/2000 = 0.77
		dut1.g_dac_data_buf[5] = 0x08;
		dut1.g_dac_data_buf[6] = 0x07;	//0x0708换算成十进制为1800, 1800/2000 = 0.9

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x09;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0xff;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[4];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[5];
		dut1.g_ccdet_phy_calibration_bit         = 0;
		dut1.g_ccdet_ana_result_bit              = 3;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[4] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[5] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
			xil_printf("dut1_CC1_0.8V_Calibration pass!\r\n");
			msdelay(5);
		}
		break;
	}

	//CC1 2.6V calibration
	case 0x0007:
	{
		dut1.g_dac_data_buf[0] = 0x05;
		dut1.g_dac_data_buf[1] = 0x28;
		dut1.g_dac_data_buf[2] = 0x0a;	//0x0a28换算成十进制为2600, 2600/1000 = 2.6
		dut1.g_dac_data_buf[3] = 0xa6;
		dut1.g_dac_data_buf[4] = 0x09;	//0x09a6换算成十进制为2470, 2460/1000 = 2.47
		dut1.g_dac_data_buf[5] = 0xaa;
		dut1.g_dac_data_buf[6] = 0x0a;	//0x0aaa换算成十进制为2730, 2730/1000 = 2.73

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x0a;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0xff;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[6];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[7];
		dut1.g_ccdet_phy_calibration_bit         = 1;
		dut1.g_ccdet_ana_result_bit              = 4;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[6] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[7] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
			xil_printf("dut1_CC1_2.6V_Calibration pass!\r\n");
			msdelay(5);
		}
		break;
	}

	//CC2 0.2V calibration
	case 0x0008:
	{
		dut1.g_dac_data_buf[0] = 0x03;
		dut1.g_dac_data_buf[1] = 0x90;
		dut1.g_dac_data_buf[2] = 0x01;
		dut1.g_dac_data_buf[3] = 0x68;
		dut1.g_dac_data_buf[4] = 0x01;
		dut1.g_dac_data_buf[5] = 0xb8;
		dut1.g_dac_data_buf[6] = 0x01;

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x0b;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0x44;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[8];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[9];
		dut1.g_ccdet_phy_calibration_bit         = 3;
		dut1.g_ccdet_ana_result_bit              = 5;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[8] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[9] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
			xil_printf("dut1_CC2_0.2V_Calibration pass!\r\n");
			msdelay(5);
		}
		break;
	}

	//CC2 0.4V calibration
	case 0x0009:
	{
		dut1.g_dac_data_buf[0] = 0x03;
		dut1.g_dac_data_buf[1] = 0x20;
		dut1.g_dac_data_buf[2] = 0x03;
		dut1.g_dac_data_buf[3] = 0xe4;
		dut1.g_dac_data_buf[4] = 0x02;
		dut1.g_dac_data_buf[5] = 0x5c;
		dut1.g_dac_data_buf[6] = 0x03;

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x0b;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0xaa;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[8];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[9];
		dut1.g_ccdet_phy_calibration_bit         = 2;
		dut1.g_ccdet_ana_result_bit              = 6;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[8] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[9] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
			xil_printf("dut1_CC2_0.4V_Calibration pass!\r\n");
			msdelay(5);
		}
		break;
	}

	//CC2 1.6V calibration
	case 0x000a:
	{
		dut1.g_dac_data_buf[0] = 0x03;
		dut1.g_dac_data_buf[1] = 0x80;
		dut1.g_dac_data_buf[2] = 0x0c;
		dut1.g_dac_data_buf[3] = 0x1c;
		dut1.g_dac_data_buf[4] = 0x0c;
		dut1.g_dac_data_buf[5] = 0xbc;
		dut1.g_dac_data_buf[6] = 0x0c;

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x0c;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0xaa;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[10];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[11];
		dut1.g_ccdet_phy_calibration_bit         = 2;
		dut1.g_ccdet_ana_result_bit              = 7;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[10] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[11] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
			xil_printf("dut1_CC2_1.6V_Calibration pass!\r\n");
			msdelay(5);
		}
		break;
	}

	//CC2 0.66V calibration
	case 0x000b:
	{
		dut1.g_dac_data_buf[0] = 0x03;
		dut1.g_dac_data_buf[1] = 0x28;
		dut1.g_dac_data_buf[2] = 0x05;
		dut1.g_dac_data_buf[3] = 0x00;
		dut1.g_dac_data_buf[4] = 0x05;
		dut1.g_dac_data_buf[5] = 0x50;
		dut1.g_dac_data_buf[6] = 0x05;

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x0b;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0x00;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[8];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[9];
		dut1.g_ccdet_phy_calibration_bit         = 1;
		dut1.g_ccdet_ana_result_bit              = 6;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[8] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[9] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
			xil_printf("dut1_CC2_0.66V_Calibration pass!\r\n");
			msdelay(5);
		}
		break;
	}

	//CC2 1.23V calibration
	case 0x000c:
	{
		dut1.g_dac_data_buf[0] = 0x03;
		dut1.g_dac_data_buf[1] = 0x9c;
		dut1.g_dac_data_buf[2] = 0x09;
		dut1.g_dac_data_buf[3] = 0x60;
		dut1.g_dac_data_buf[4] = 0x09;
		dut1.g_dac_data_buf[5] = 0xd8;
		dut1.g_dac_data_buf[6] = 0x09;

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x0c;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0x00;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[10];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[11];
		dut1.g_ccdet_phy_calibration_bit         = 3;
		dut1.g_ccdet_ana_result_bit              = 7;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[10] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[11] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
			xil_printf("dut1_CC2_1.23V_Calibration pass!\r\n");
			msdelay(5);
		}
		break;
	}


	//CC2 0.8V calibration
	case 0x000d:
	{
		dut1.g_dac_data_buf[0] = 0x03;
		dut1.g_dac_data_buf[1] = 0x40;
		dut1.g_dac_data_buf[2] = 0x06;
		dut1.g_dac_data_buf[3] = 0x04;
		dut1.g_dac_data_buf[4] = 0x06;
		dut1.g_dac_data_buf[5] = 0x08;
		dut1.g_dac_data_buf[6] = 0x07;

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x0b;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0xff;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[8];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[9];
		dut1.g_ccdet_phy_calibration_bit         = 0;
		dut1.g_ccdet_ana_result_bit              = 6;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[8] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[9] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			dut1.g_ccdet_step++;
			dut1.g_pattern_step = 0;
			//xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
			xil_printf("dut1_CC2_0.8V_Calibration pass!\r\n");
			msdelay(5);
		}
		break;
	}

	//CC2 2.6V calibration
	case 0x00e:
	{
		dut1.g_dac_data_buf[0] = 0x07;
		dut1.g_dac_data_buf[1] = 0x28;
		dut1.g_dac_data_buf[2] = 0x0a;
		dut1.g_dac_data_buf[3] = 0xa6;
		dut1.g_dac_data_buf[4] = 0x09;
		dut1.g_dac_data_buf[5] = 0xaa;
		dut1.g_dac_data_buf[6] = 0x0a;

		dut1.g_ccdet_phy_calibration_addr_buf[0] = 0x0c;
		dut1.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut1.g_ccdet_voltage_sel = 0xff;
		dut1.g_ccdet_calibration_data_buf[0]     = dut1.g_ccdet_calibration_data_buf[10];
		dut1.g_ccdet_calibration_data_buf[1]     = dut1.g_ccdet_calibration_data_buf[11];
		dut1.g_ccdet_phy_calibration_bit         = 1;
		dut1.g_ccdet_ana_result_bit              = 7;

		_ccdet_calibration_dut1();
		if(dut1.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut1.g_ccdet_calibration_data_buf[10] = dut1.g_ccdet_calibration_data_buf[2];
			dut1.g_ccdet_calibration_data_buf[11] = dut1.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut1.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut1.g_ccdet_process_bit = ccdet_process_waiting;
			dut1.g_ccdet_retest_signbit = 0;
			//dut1.g_ccdet_step++;
			dut1.g_ccdet_step = 0x16;
			dut1.g_pattern_step = 0;
			//xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
			xil_printf("dut1_CC2_2.6V_Calibration pass!\r\n");
      		xil_printf("dut1_patternD_pass!\r\n\r\n");
		}
		break;
	}
/*
	//0.8v compare
	case 0x000f:
	{
		cc_compare_temp_buf[0] = (dut1.g_ccdet_calibration_data_buf[4]) & 0x0f;			//cc1 0.8v
		cc_compare_temp_buf[1] = (dut1.g_ccdet_calibration_data_buf[4] >> 4) & 0x0f;	//cc1 0.66v

		cc_compare_temp_buf[2] = (dut1.g_ccdet_calibration_data_buf[5]) & 0x0f;			//cc1 0.4v
		cc_compare_temp_buf[3] = (dut1.g_ccdet_calibration_data_buf[5] >> 4) & 0x0f;	//cc1 0.2v

		//cc_compare_temp_buf[4] = (dut1.g_ccdet_calibration_data_buf[6]) & 0x0f;
		cc_compare_temp_buf[5] = (dut1.g_ccdet_calibration_data_buf[6] >> 4) & 0x0f;	//cc1 2.6v

		cc_compare_temp_buf[6] = (dut1.g_ccdet_calibration_data_buf[7]) & 0x0f;			//cc1 1.6v
		cc_compare_temp_buf[7] = (dut1.g_ccdet_calibration_data_buf[7] >> 4) & 0x0f;	//cc1 1.23v

		cc_compare_temp_buf[8] = (dut1.g_ccdet_calibration_data_buf[8]) & 0x0f;			//cc2 0.8v
		cc_compare_temp_buf[9] = (dut1.g_ccdet_calibration_data_buf[8] >> 4) & 0x0f;	//cc2 0.66v

		cc_compare_temp_buf[10] = (dut1.g_ccdet_calibration_data_buf[9]) & 0x0f;		//cc2 0.4v
		cc_compare_temp_buf[11] = (dut1.g_ccdet_calibration_data_buf[9] >> 4) & 0x0f;	//cc2 0.2v

		//cc_compare_temp_buf[12] = (dut1.g_ccdet_calibration_data_buf[10]) & 0x0f;
		cc_compare_temp_buf[13] = (dut1.g_ccdet_calibration_data_buf[10] >> 4) & 0x0f;	//cc2 2.6v

		cc_compare_temp_buf[14] = (dut1.g_ccdet_calibration_data_buf[11]) & 0x0f;		//cc2 1.6v
		cc_compare_temp_buf[15] = (dut1.g_ccdet_calibration_data_buf[11] >> 4) & 0x0f;	//cc2 1.23v

		if(cc_compare_temp_buf[0] > cc_compare_temp_buf[8])
		{
			if((cc_compare_temp_buf[0] - cc_compare_temp_buf[8]) < CCDET_Tolerance_Compare_Data)
			{
				dut1.g_ccdet_step++;
			}
			else
			{
				dut1.g_result_fail = 0x01;
				dut1.g_result_fail_tmrcount = 0xffff;
			}
		}
		else
		{
			if((cc_compare_temp_buf[8] - cc_compare_temp_buf[0]) < CCDET_Tolerance_Compare_Data)
			{
				dut1.g_ccdet_step++;
			}
			else
			{
				dut1.g_result_fail = 0x01;
				dut1.g_result_fail_tmrcount = 0xffff;
			}
		}
	}

	//0.66v compare
	case 0x0010:
	{
		if(cc_compare_temp_buf[1] > cc_compare_temp_buf[9])
		{
			if((cc_compare_temp_buf[1] - cc_compare_temp_buf[9]) < CCDET_Tolerance_Compare_Data)
			{
				dut1.g_ccdet_step++;
			}
			else
			{
				dut1.g_result_fail = 0x01;
				dut1.g_result_fail_tmrcount = 0xffff;
			}
		}
		else
		{
			if((cc_compare_temp_buf[9] - cc_compare_temp_buf[1]) < CCDET_Tolerance_Compare_Data)
			{
				dut1.g_ccdet_step++;
			}
			else
			{
				dut1.g_result_fail = 0x01;
				dut1.g_result_fail_tmrcount = 0xffff;
			}
		}
	}

	//0.4v compare
	case 0x0011:
	{
		if(cc_compare_temp_buf[2] > cc_compare_temp_buf[10])
		{
			if((cc_compare_temp_buf[2] - cc_compare_temp_buf[10]) < CCDET_Tolerance_Compare_Data)
			{
				dut1.g_ccdet_step++;
			}
			else
			{
				dut1.g_result_fail = 0x01;
				dut1.g_result_fail_tmrcount = 0xffff;
			}
		}
		else
		{
			if((cc_compare_temp_buf[10] - cc_compare_temp_buf[2]) < CCDET_Tolerance_Compare_Data)
			{
				dut1.g_ccdet_step++;
			}
			else
			{
				dut1.g_result_fail = 0x01;
				dut1.g_result_fail_tmrcount = 0xffff;
			}
		}
	}

	//0.2v compare
	case 0x0012:
	{
		if(cc_compare_temp_buf[3] > cc_compare_temp_buf[11])
		{
			if((cc_compare_temp_buf[3] - cc_compare_temp_buf[11]) < CCDET_Tolerance_Compare_Data)
			{
				dut1.g_ccdet_step++;
			}
			else
			{
				dut1.g_result_fail = 0x01;
				dut1.g_result_fail_tmrcount = 0xffff;
			}
		}
		else
		{
			if((cc_compare_temp_buf[11] - cc_compare_temp_buf[3]) < CCDET_Tolerance_Compare_Data)
			{
				dut1.g_ccdet_step++;
			}
			else
			{
				dut1.g_result_fail = 0x01;
				dut1.g_result_fail_tmrcount = 0xffff;
			}
		}
	}

	//2.6v compare
	case 0x0013:
	{
		if(cc_compare_temp_buf[5] > cc_compare_temp_buf[13])
		{
			if((cc_compare_temp_buf[5] - cc_compare_temp_buf[13]) < CCDET_Tolerance_Compare_Data)
			{
				dut1.g_ccdet_step++;
			}
			else
			{
				dut1.g_result_fail = 0x01;
				dut1.g_result_fail_tmrcount = 0xffff;
			}
		}
		else
		{
			if((cc_compare_temp_buf[13] - cc_compare_temp_buf[5]) < CCDET_Tolerance_Compare_Data)
			{
				dut1.g_ccdet_step++;
			}
			else
			{
				dut1.g_result_fail = 0x01;
				dut1.g_result_fail_tmrcount = 0xffff;
			}
		}
	}

	//1.6v compare
	case 0x0014:
	{
		if(cc_compare_temp_buf[6] > cc_compare_temp_buf[14])
		{
			if((cc_compare_temp_buf[6] - cc_compare_temp_buf[14]) < CCDET_Tolerance_Compare_Data)
			{
				dut1.g_ccdet_step++;
			}
			else
			{
				dut1.g_result_fail = 0x01;
				dut1.g_result_fail_tmrcount = 0xffff;
			}
		}
		else
		{
			if((cc_compare_temp_buf[14] - cc_compare_temp_buf[6]) < CCDET_Tolerance_Compare_Data)
			{
				dut1.g_ccdet_step++;
			}
			else
			{
				dut1.g_result_fail = 0x01;
				dut1.g_result_fail_tmrcount = 0xffff;
			}
		}
	}

	//1.23v compare
	case 0x0015:
	{
		if(cc_compare_temp_buf[7] > cc_compare_temp_buf[15])
		{
			if((cc_compare_temp_buf[7] - cc_compare_temp_buf[15]) < CCDET_Tolerance_Compare_Data)
			{
				dut1.g_ccdet_step++;
			}
			else
			{
				dut1.g_result_fail = 0x01;
				dut1.g_result_fail_tmrcount = 0xffff;
			}
		}
		else
		{
			if((cc_compare_temp_buf[15] - cc_compare_temp_buf[7]) < CCDET_Tolerance_Compare_Data)
			{
				dut1.g_ccdet_step++;
			}
			else
			{
				dut1.g_result_fail = 0x01;
				dut1.g_result_fail_tmrcount = 0xffff;
			}
		}
	}
*/
	//CC_DET pass, write SCRATCH2(0xDF3C) bit[4]
	case 0x0016:
	{
		dut1.g_pattern_smbus_control_buf[1] = smbus_cmd_type_writemem;
		dut1.g_pattern_smbus_control_buf[2] = 0x3c;
		dut1.g_pattern_smbus_control_buf[3] = 0xdf;
		dut1.g_pattern_smbus_control_buf[4] = 0x01;
		dut1.g_pattern_smbus_control_buf[5] = 0x10;

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

			dut1.g_pattern_timer = 0x3fff;
			dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
			dut1.g_pattern_step = 0x00;
			//dut1.g_dut_pattern_status_buf[2]++;
			dut1.g_uartPatternEnable = 0x00;
			dut1.g_uartPatternNum++;
			result_output_for_v50(XPAR_AXI_GPIO_dut1_1_BASEADDR,dut1.g_uartPatternNum);
			//xil_printf("dut1.g_uartPatternNum = %x\r\n\r\n",dut1.g_uartPatternNum);

      		//XGpio_0_WriteBit(0,LED_D6_OFFSET,g_ledD6Tg);
      		//g_ledD6Tg=~g_ledD6Tg;
      		//xil_printf("dut1.g_ccdet_step = %x\r\n",dut1.g_ccdet_step);
      		//xil_printf("dut1.g_pattern_step = %x\r\n",dut1.g_pattern_step);
      		//xil_printf("dut1.VREF_CFG0_L =%x\r\n", dut1.g_ccdet_calibration_data_buf[4]);
      		//xil_printf("dut1.VREF_CFG0_H =%x\r\n", dut1.g_ccdet_calibration_data_buf[5]);
      		//xil_printf("dut1.VREF_CFG1_L =%x\r\n", dut1.g_ccdet_calibration_data_buf[6]);
      		//xil_printf("dut1.VREF_CFG1_H =%x\r\n", dut1.g_ccdet_calibration_data_buf[7]);
      		//xil_printf("dut1.VREF_CFG2_L =%x\r\n", dut1.g_ccdet_calibration_data_buf[8]);
      		//xil_printf("dut1.VREF_CFG2_H =%x\r\n", dut1.g_ccdet_calibration_data_buf[9]);
      		//xil_printf("dut1.VREF_CFG3_L =%x\r\n", dut1.g_ccdet_calibration_data_buf[10]);
      		//xil_printf("dut1.VREF_CFG3_H =%x\r\n", dut1.g_ccdet_calibration_data_buf[11]);
      		//XGpio_0_WriteBit(0,LED_D6_OFFSET,g_ledD6Tg);
			//g_ledD6Tg=~g_ledD6Tg;
		}
		break;
	}
	}

	if(dut1.g_result_fail == 0x01)
	{
		xil_printf("dut1_patternD_fail! patternD_fail_step = %x ccdet_calib_step = %x\r\n",dut1.g_pattern_step,dut1.g_ccdet_step);
		pattern_safe_state_dut1();
	}
	return 0;
}


