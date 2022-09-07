/******************************************************************************
*
* (c) Copyright 2010-2012 Xilinx, Inc. All rights reserved.
*
* This file contains confidential and proprietary information of Xilinx, Inc.
* and is protected under U.S. and international copyright and other
* intellectual property laws.
*
* DISCLAIMER
* This disclaimer is not a license and does not grant any rights to the
* materials distributed herewith. Except as otherwise provided in a valid
* license issued to you by Xilinx, and to the maximum extent permitted by
* applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL
* FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,
* IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE;
* and (2) Xilinx shall not be liable (whether in contract or tort, including
* negligence, or under any other theory of liability) for any loss or damage
* of any kind or nature related to, arising under or in connection with these
* materials, including for any direct, or any indirect, special, incidental,
* or consequential loss or damage (including loss of data, profits, goodwill,
* or any type of loss or damage suffered as a result of any action brought by
* a third party) even if such damage or loss was reasonably foreseeable or
* Xilinx had been advised of the possibility of the same.
*
* CRITICAL APPLICATIONS
* Xilinx products are not designed or intended to be fail-safe, or for use in
* any application requiring fail-safe performance, such as life-support or
* safety devices or systems, Class III medical devices, nuclear facilities,
* applications related to the deployment of airbags, or any other applications
* that could lead to death, personal injury, or severe property or
* environmental damage (individually and collectively, "Critical
* Applications"). Customer assumes the sole risk and liability of any use of
* Xilinx products in Critical Applications, subject only to applicable laws
* and regulations governing limitations on product liability.
*
* THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE
* AT ALL TIMES.
*
******************************************************************************/

#include "platform.h"
#include "xparameters.h"

void platform_initial()
{
    u16 i=0;
	//u16 j=0;
	//u16 k=0;

	if(dut0.g_dut_start_ready == 0x11)
	{
		pattern_safe_state_dut0();
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR, 0, 1, 0);

		dut0.g_result_fail = 0x00;
		dut0.g_result_output_ch = 0;
		dut0.g_uartPatternEnable = 0x01;
		dut0.g_uartPatternNum = 0x18;
		dut0.g_dut_start_ready = 1;
		dut0.g_ft2_test_done = 0x01;
		//xil_printf("dut0.platform_initial_print_finish!\r\n");
		dut0.g_test_count++;
		xil_printf("---------- dut0.g_test_count ---------- = %d\r\n\r\n", dut0.g_test_count);
	}

	if(dut1.g_dut_start_ready == 0x11)
	{
		pattern_safe_state_dut1();
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR, 0, 1, 0);

		dut1.g_result_fail = 0x00;
		dut1.g_result_output_ch = 0;
		dut1.g_uartPatternEnable = 0x01;
		dut1.g_uartPatternNum = 0x18;
		dut1.g_dut_start_ready = 1;
		dut1.g_ft2_test_done = 0x01;
		//xil_printf("dut1.platform_initial_print_finish!\r\n");
		dut1.g_test_count++;
		xil_printf("---------- dut1.g_test_count ---------- = %d\r\n\r\n", dut1.g_test_count);
	}

	if(dut2.g_dut_start_ready == 0x11)
	{
		pattern_safe_state_dut2();
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR, 0, 1, 0);

		dut2.g_result_fail = 0x00;
		dut2.g_result_output_ch = 0;
		dut2.g_uartPatternEnable = 0x01;
		dut2.g_uartPatternNum = 0x18;
		dut2.g_dut_start_ready = 1;
		dut2.g_ft2_test_done = 0x01;
		//xil_printf("dut2.platform_initial_print_finish!\r\n");
		dut2.g_test_count++;
		xil_printf("---------- dut2.g_test_count ---------- = %d\r\n\r\n", dut2.g_test_count);
	}

	if(dut3.g_dut_start_ready == 0x11)
	{
		pattern_safe_state_dut3();
		XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR, 0, 1, 0);

		dut3.g_result_fail = 0x00;
		dut3.g_result_output_ch = 0;
		dut3.g_uartPatternEnable = 0x01;
		dut3.g_uartPatternNum = 0x18;
		dut3.g_dut_start_ready = 1;
		dut3.g_ft2_test_done = 0x01;
		//xil_printf("dut3.platform_initial_print_finish!\r\n");
		dut3.g_test_count++;
		xil_printf("---------- dut3.g_test_count ---------- = %d\r\n\r\n", dut3.g_test_count);
	}

	//xil_printf("dut0.g_dut_start_ready = %x\r\n\r\n", dut0.g_dut_start_ready);
	//xil_printf("dut1.g_dut_start_ready = %x\r\n\r\n", dut1.g_dut_start_ready);
	//xil_printf("dut2.g_dut_start_ready = %x\r\n\r\n", dut2.g_dut_start_ready);
	//xil_printf("dut3.g_dut_start_ready = %x\r\n\r\n", dut3.g_dut_start_ready);
	//xil_printf("dut0.g_result_fail = %x\r\n\r\n", dut0.g_result_fail);
	//xil_printf("dut1.g_result_fail = %x\r\n\r\n", dut1.g_result_fail);
	//xil_printf("dut2.g_result_fail = %x\r\n\r\n", dut2.g_result_fail);
	//xil_printf("dut3.g_result_fail = %x\r\n\r\n", dut3.g_result_fail);
	//xil_printf("dut0.g_dut_pattern_status_buf[2] = %x\r\n\r\n", dut0.g_dut_pattern_status_buf[2]);
	//xil_printf("dut1.g_dut_pattern_status_buf[2] = %x\r\n\r\n", dut1.g_dut_pattern_status_buf[2]);
	//xil_printf("dut2.g_dut_pattern_status_buf[2] = %x\r\n\r\n", dut2.g_dut_pattern_status_buf[2]);
	//xil_printf("dut3.g_dut_pattern_status_buf[2] = %x\r\n\r\n", dut3.g_dut_pattern_status_buf[2]);
	//xil_printf("dut0.g_pattern_step = %x\r\n", dut0.g_pattern_step);
	//xil_printf("dut1.g_pattern_step = %x\r\n", dut1.g_pattern_step);
	//xil_printf("dut2.g_pattern_step = %x\r\n", dut2.g_pattern_step);
	//xil_printf("dut3.g_pattern_step = %x\r\n", dut3.g_pattern_step);
	//xil_printf("platform initial over!\r\n")
}
