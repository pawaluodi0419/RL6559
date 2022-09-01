/*
 * timer_irq_handle.c
 *
 *  Created on: 2017-6-19
 *      Author: kay_yin
 */
#include "platform.h"

// ------------------------------------------------------------
//axi timer 0 interrupt handle
//
//enter this interrupt every 1 ms
// ------------------------------------------------------------
void axi_tmr0_intrq_handle()
{
	u32 a;

	//disable the count0
	//XTmrCtr_Disable(XPAR_AXI_TIMER_0_BASEADDR,0);

	//enter interrupt count ++
	if((dut0.g_dut_start_ready == 1) || (dut1.g_dut_start_ready == 1) || (dut2.g_dut_start_ready == 1) ||(dut3.g_dut_start_ready == 1))
	{
		dut0.g_axiTmr0Cnt++;
	}

	//clear the axi timer0 count0 interrupt flag
	a=XTmrCtr_GetControlStatusReg(XPAR_AXI_TIMER_0_BASEADDR,0);
    XTmrCtr_SetControlStatusReg(XPAR_AXI_TIMER_0_BASEADDR,0,a);
}

void axi_tmr1_intrq_handle()
{
	u8 i;
	u32 a;

	//disable the count0
	//XTmrCtr_Disable(XPAR_AXI_TIMER_0_BASEADDR,0);

	//enter interrupt count ++
	dut0.g_axiTmr1Cnt++;

	if(XGpio_Dutx_ReadBit(XPAR_AXI_GPIO_dut1_1_BASEADDR, 0, 1) == 1)
	{
		g_u2_host_test1_flag = 1;
	}
	else
	{
		g_u2_host_test1_flag = 0;
	}

	//clear the axi timer1 count0 interrupt flag
	a=XTmrCtr_GetControlStatusReg(XPAR_AXI_TIMER_1_BASEADDR,0);
    XTmrCtr_SetControlStatusReg(XPAR_AXI_TIMER_1_BASEADDR,0,a);
}

void axi_tmr2_intrq_handle()
{
	u32 a;

	//disable the count0
	//XTmrCtr_Disable(XPAR_AXI_TIMER_0_BASEADDR,0);

	//LED blink
	if((dut0.g_axiTmr2Cnt&0x7f)==0x64){
		XGpio_0_WriteBit(0,LED_D2_OFFSET,g_ledD2Tg);
		g_ledD2Tg=~g_ledD2Tg;
	}

	if(dut0.g_dut_start_ready == 1)
	{
		////////////////smbus_timer/////////////
		if(dut0.g_smbus_timer&0xff == 0x00)
		{

		}
		else if((dut0.g_smbus_timer&0xff)> 0x00)
		{
			if((dut0.g_smbus_timer&0xff)>0x0f)
			{

			}
			else
			{
				dut0.g_smbus_timer--;
			}
		}

		////////////////relay control_timer/////////////
		if(dut0.g_relay_control_timer&0xffff == 0x00)
		{

		}
		else if((dut0.g_relay_control_timer&0xffff)> 0x00)
		{
			if((dut0.g_relay_control_timer&0xff)>0xffff)
			{
				//something wrong
			}
			else
			{
				dut0.g_relay_control_timer--;
				//xil_printf("dut0_relay_control_timer = %d\r\n", dut0.g_relay_control_timer);
			}
		}

		////////////////smbus_start_timer/////////////
		if(dut0.g_start_smbus_timer&0xffffffff == 0x00)
		{

		}
		else if((dut0.g_start_smbus_timer&0xffffffff)> 0x00)
		{
			dut0.g_start_smbus_timer--;
		}

		////////////////pattern_timer/////////////
		if(dut0.g_pattern_timer == 0)
		{
			pattern_safe_state_dut0();
			xil_printf("fpga no response, please check environment!\r\n\r\n");
		}
		else
		{
			//xil_printf("dut0.g_pattern_timer = %d\r\n", dut0.g_pattern_timer);
			dut0.g_pattern_timer--;
		}
	}

	if(dut1.g_dut_start_ready == 1)
	{
		////////////////smbus_timer/////////////
		if(dut1.g_smbus_timer&0xff == 0x00)
		{

		}
		else if((dut1.g_smbus_timer&0xff)> 0x00)
		{
			if((dut1.g_smbus_timer&0xff)>0x0f)
			{

			}
			else
			{
				dut1.g_smbus_timer--;
			}
		}

		////////////////relay_control_timer/////////////
		if(dut1.g_relay_control_timer&0xffff == 0x00)
		{

		}
		else if((dut1.g_relay_control_timer&0xffff)> 0x00)
		{
			if((dut1.g_relay_control_timer&0xff)>0xffff)
			{
				//something wrong
			}
			else
			{
				dut1.g_relay_control_timer--;
				//xil_printf("dut1_relay_control_timer = %d\r\n", dut2.g_relay_control_timer);
			}
		}

		////////////////smbus_start_timer/////////////
		if(dut1.g_start_smbus_timer&0xffffffff == 0x00)
		{

		}
		else if((dut1.g_start_smbus_timer&0xffffffff)> 0x00)
		{
			dut1.g_start_smbus_timer--;
		}

		////////////////pattern_timer/////////////
		if(dut1.g_pattern_timer == 0)
		{
			pattern_safe_state_dut1();
			xil_printf("fpga no response, please check environment!\r\n\r\n");
		}
		else
		{
			dut1.g_pattern_timer--;
		}
	}

	if(dut2.g_dut_start_ready == 1)
	{
		////////////////smbus_timer/////////////
		if(dut2.g_smbus_timer&0xff == 0x00)
		{

		}
		else if((dut2.g_smbus_timer&0xff)> 0x00)
		{
			if((dut2.g_smbus_timer&0xff)>0x0f)
			{

			}
			else
			{
				dut2.g_smbus_timer--;
			}
		}

		////////////////relay_control_timer/////////////
		if(dut2.g_relay_control_timer&0xffff == 0x00)
		{

		}
		else if((dut2.g_relay_control_timer&0xffff)> 0x00)
		{
			if((dut2.g_relay_control_timer&0xff)>0xffff)
			{
				//something wrong
			}
			else
			{
				dut2.g_relay_control_timer--;
				//xil_printf("dut2_relay_control_timer = %d\r\n", dut2.g_relay_control_timer);
			}
		}

		////////////////smbus_start_timer/////////////
		if(dut2.g_start_smbus_timer&0xffffffff == 0x00)
		{

		}
		else if((dut2.g_start_smbus_timer&0xffffffff)> 0x00)
		{
			dut2.g_start_smbus_timer--;
		}

		////////////////pattern_timer/////////////
		if(dut2.g_pattern_timer == 0)
		{
			pattern_safe_state_dut2();
			xil_printf("fpga no response, please check environment!\r\n\r\n");
		}
		else
		{
			dut2.g_pattern_timer--;
		}
	}

	if(dut3.g_dut_start_ready == 1)
	{
		////////////////smbus_timer/////////////
		if(dut3.g_smbus_timer&0xff == 0x00)
		{

		}
		else if((dut3.g_smbus_timer&0xff)> 0x00)
		{
			if((dut3.g_smbus_timer&0xff)>0x0f)
			{

			}
			else
			{
				dut3.g_smbus_timer--;
			}
		}

		////////////////relay_control_timer/////////////
		if(dut3.g_relay_control_timer&0xffff == 0x00)
		{

		}
		else if((dut3.g_relay_control_timer&0xffff)> 0x00)
		{
			if((dut3.g_relay_control_timer&0xff)>0xffff)
			{
				//something wrong
			}
			else
			{
				dut3.g_relay_control_timer--;
				//xil_printf("dut3_relay_control_timer = %d\r\n", dut3.g_relay_control_timer);
			}
		}

		////////////////smbus_start_timer/////////////
		if(dut3.g_start_smbus_timer&0xffffffff == 0x00)
		{

		}
		else if((dut3.g_start_smbus_timer&0xffffffff)> 0x00)
		{
			dut3.g_start_smbus_timer--;
		}

		////////////////pattern_timer/////////////
		if(dut3.g_pattern_timer == 0)
		{
			pattern_safe_state_dut3();
			xil_printf("fpga no response, please check environment!\r\n\r\n");
		}
		else
		{
			dut3.g_pattern_timer--;
		}
	}

	//enter interrupt count ++
	dut0.g_axiTmr2Cnt++;
	//clear the axi timer0 count0 interrupt flag
	a=XTmrCtr_GetControlStatusReg(XPAR_AXI_TIMER_2_BASEADDR,0);
    XTmrCtr_SetControlStatusReg(XPAR_AXI_TIMER_2_BASEADDR,0,a);
}
