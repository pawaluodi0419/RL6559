#include "platform.h"

void pattern_safe_state_dut1()
{
	u8 i;
	u8 Buff_dut1_XGPIO_0[8];
	//relay control
	Buff_dut1_XGPIO_0[0] = 0x00;                            //REG0005 ouput value[7:0]
	Buff_dut1_XGPIO_0[1] = 0xFA;							//REG0006
	Buff_dut1_XGPIO_0[2] = 0x40;							//REG0007 output value[15:8]
	Buff_dut1_XGPIO_0[3] = 0x9E;							//REG0008
	Buff_dut1_XGPIO_0[4] = 0x00|(dut1.g_uartPatternNum); 	//REG0009 output value[23:16]
	Buff_dut1_XGPIO_0[5] = 0xE0;							//REG000a
	Buff_dut1_XGPIO_0[6] = 0x02;							//REG000b output value[31:24]
	Buff_dut1_XGPIO_0[7] = 0xFC;							//REG000c
	XGpio_dut1_Relay_WriteByte(XPAR_AXI_GPIO_dut1_1_BASEADDR,Buff_dut1_XGPIO_0);
	msdelay(10);
	//xil_printf("\r\ndut1 relay control setup completed!\r\n");

	//DAC output 0v
	dut1.g_dac_data_buf[0] = 0x01;	//0x01: CC1 0x03: CC2
	dut1.g_dac_data_buf[1] = 0x00;	//Data L
	dut1.g_dac_data_buf[2] = 0x00;	//Data H
	dut1.g_dac_data_buf[3] = 0x00;	//Data L
	dut1.g_dac_data_buf[4] = 0x00;	//Data H
	dut1.g_dac_data_buf[5] = 0x00;	//Data L
	dut1.g_dac_data_buf[6] = 0x00;	//Data H
	_by_mcp4822_output_control_dut1(dut1.g_dac_data_buf);
	msdelay(10);

	dut1.g_dac_data_buf[0] = 0x03;	//0x01: CC1 0x03: CC2
	dut1.g_dac_data_buf[1] = 0x00;	//Data L
	dut1.g_dac_data_buf[2] = 0x00;	//Data H
	dut1.g_dac_data_buf[3] = 0x00;	//Data L
	dut1.g_dac_data_buf[4] = 0x00;	//Data H
	dut1.g_dac_data_buf[5] = 0x00;	//Data L
	dut1.g_dac_data_buf[6] = 0x00;	//Data H
	_by_mcp4822_output_control_dut1(dut1.g_dac_data_buf);
	msdelay(10);
	//xil_printf("dut1 DAC output 0v setup completed!\r\n");

	//DUT IC power off
	XGpio_2_WriteBit(0,dut1_FT2_PWR_CTRL_OFFSET,0x01);
	msdelay(50);
	//xil_printf("dut1 power off setup completed!\r\n\r\n");

	i2c_mcp23008_output(AD7994_DEV1_ADDR, MCP23008_ADDR, 0x00);
	msdelay(10);

	for(i=0;i<40;i++)
	{
		dut1.g_dut_pattern_status_buf[i] = 0;
	}

	for(i=1; i<60; i++)
	{
		dut1.g_pattern_smbus_control_buf[i] = CLEAR_;
	}
	dut1.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
	dut1.g_smbus_status_buf[0] = 0x00;
	dut1.g_smbus_status_buf[1] = 0xff;
	dut1.g_smbus_style = 0x00;

	for(i=0;i<12;i++)
	{
		dut1.g_ccdet_calibration_data_buf[i] = 0x88;
	}
	dut1.g_ccdet_step = 0;
	dut1.g_ccdet_retest_signbit = 0;

   	dut1.g_pattern_timer = 0xfff;
   	dut1.g_relay_control_timer = 0xff;
	dut1.g_smbus_timer = 0xff;

	dut1.g_efuse_status = 0;
	dut1.g_retest = 0;

	dut1.g_pattern_step = 0x00;
	dut1.g_ft2_test_done = 0x00;
	dut1.g_uartPatternEnable = 0x00;
	dut1.g_dut_start_ready = 0;
}


