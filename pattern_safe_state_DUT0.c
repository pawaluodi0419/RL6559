#include "platform.h"

void pattern_safe_state_dut0()
{
	u8 i;
	u8 Buff_dut0_XGPIO_0[8];
	//relay control
	Buff_dut0_XGPIO_0[0] = 0x00;                            //REG0005 ouput value[7:0]
	Buff_dut0_XGPIO_0[1] = 0xFA;							//REG0006
	Buff_dut0_XGPIO_0[2] = 0x40;							//REG0007 output value[15:8]
	Buff_dut0_XGPIO_0[3] = 0x9E;							//REG0008
	Buff_dut0_XGPIO_0[4] = 0x00|(dut0.g_uartPatternNum); 	//REG0009 output value[23:16]
	Buff_dut0_XGPIO_0[5] = 0xE0;							//REG000a
	Buff_dut0_XGPIO_0[6] = 0x02;							//REG000b output value[31:24]
	Buff_dut0_XGPIO_0[7] = 0xFC;							//REG000c
	XGpio_dut0_Relay_WriteByte(XPAR_AXI_GPIO_dut0_1_BASEADDR,Buff_dut0_XGPIO_0);
	msdelay(10);
	//xil_printf("\r\ndut0 relay control setup completed!\r\n");

	//DAC output 0v
	dut0.g_dac_data_buf[0] = 0x01;	//0x01: CC1 0x03: CC2
	dut0.g_dac_data_buf[1] = 0x00;	//Data L
	dut0.g_dac_data_buf[2] = 0x00;	//Data H
	dut0.g_dac_data_buf[3] = 0x00;	//Data L
	dut0.g_dac_data_buf[4] = 0x00;	//Data H
	dut0.g_dac_data_buf[5] = 0x00;	//Data L
	dut0.g_dac_data_buf[6] = 0x00;	//Data H
	_by_mcp4822_output_control_dut0(dut0.g_dac_data_buf);
	msdelay(10);

	dut0.g_dac_data_buf[0] = 0x03;	//0x01: CC1 0x03: CC2
	dut0.g_dac_data_buf[1] = 0x00;	//Data L
	dut0.g_dac_data_buf[2] = 0x00;	//Data H
	dut0.g_dac_data_buf[3] = 0x00;	//Data L
	dut0.g_dac_data_buf[4] = 0x00;	//Data H
	dut0.g_dac_data_buf[5] = 0x00;	//Data L
	dut0.g_dac_data_buf[6] = 0x00;	//Data H
	_by_mcp4822_output_control_dut0(dut0.g_dac_data_buf);
	msdelay(10);
	//xil_printf("dut0 DAC output 0v setup completed!\r\n");

	//DUT IC power off
	XGpio_2_WriteBit(0,dut0_FT2_PWR_CTRL_OFFSET,0x01);
	msdelay(50);
	//xil_printf("dut0 power off setup completed!\r\n\r\n");

	i2c_mcp23008_output(AD7994_DEV0_ADDR, MCP23008_ADDR, 0x00);
	msdelay(10);

	for(i=0;i<40;i++)
	{
		dut0.g_dut_pattern_status_buf[i] = 0;
	}

	for(i=1; i<60; i++)
	{
		dut0.g_pattern_smbus_control_buf[i] = CLEAR_;
	}
	dut0.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
	dut0.g_smbus_status_buf[0] = 0x00;
	dut0.g_smbus_status_buf[1] = 0xff;
	dut0.g_smbus_style = 0x00;

	for(i=0;i<12;i++)
	{
		dut0.g_ccdet_calibration_data_buf[i] = 0x88;
	}
	dut0.g_ccdet_step = 0;
	dut0.g_ccdet_retest_signbit = 0;

   	dut0.g_pattern_timer = 0x3fff;
   	dut0.g_relay_control_timer = 0xff;
	dut0.g_smbus_timer = 0xff;

	dut0.g_efuse_status = 0;
	dut0.g_retest = 0;

	dut0.g_pattern_step = 0x00;
	dut0.g_ft2_test_done = 0x00;
	dut0.g_uartPatternEnable = 0x00;
	dut0.g_dut_start_ready = 0;
}


