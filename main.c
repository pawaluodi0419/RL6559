#include "platform.h"

//function declare
//void uart0_intrq_handle();
//void uart1_intrq_handle();
//void uart2_intrq_handle();
//void uart3_intrq_handle();

void axi_tmr0_intrq_handle();
void axi_tmr1_intrq_handle();
void axi_tmr2_intrq_handle();

void XGPIO_0_Intrq_Handle();
void XGPIO_dut0_1_Intrq_Handle();
void XGPIO_dut1_1_Intrq_Handle();
void XGPIO_dut2_1_Intrq_Handle();
void XGPIO_dut3_1_Intrq_Handle();

void print(char *str);

//struct define
struct DUT dut0;
struct DUT dut1;
struct DUT dut2;
struct DUT dut3;

//globe define
u8 g_opaLedTg=0;
u8 g_ledD2Tg=0;
u8 g_ledD3Tg=0;
u8 g_ledD4Tg=0;
u8 g_ledD5Tg=0;
u8 g_ledD6Tg=0;
u8 g_ledD7Tg=0;
u8 g_u2_host_test1_flag = 0;

int main()
{
	xil_printf("FT2 start!\r\nFPGA Version:RL6559_FT_Merge_Release_2023011801\r\n");

	XGpio_WriteBit(XPAR_AXI_GPIO_1_BASEADDR,0,1, 0);	//reset usb host
	msdelay(100);
	XGpio_WriteBit(XPAR_AXI_GPIO_1_BASEADDR,0,1, 1);
	xil_printf("reset usb host!\r\n");
	msdelay(1000);

	pattern_initial_state_dut0();
	pattern_initial_state_dut1();
	pattern_initial_state_dut2();
	pattern_initial_state_dut3();

    u16 i=0;
	u16 j=0;
	u16 k=0;
	u8 h;

	u8 ADC_CHSel_Prev=0;

	init_platform();

    //initial the axi timer
    XTmrCtr_SetControlStatusReg(XPAR_AXI_TIMER_0_BASEADDR,0,XTC_CSR_AUTO_RELOAD_MASK|XTC_CSR_DOWN_COUNT_MASK); //config the TCSR as down counter, auto reload
    XTmrCtr_EnableIntr(XPAR_AXI_TIMER_0_BASEADDR,0);

    XTmrCtr_SetControlStatusReg(XPAR_AXI_TIMER_1_BASEADDR,0,XTC_CSR_AUTO_RELOAD_MASK|XTC_CSR_DOWN_COUNT_MASK); //config the TCSR as down counter, auto reload
    XTmrCtr_EnableIntr(XPAR_AXI_TIMER_1_BASEADDR,0);

    XTmrCtr_SetControlStatusReg(XPAR_AXI_TIMER_2_BASEADDR,0,XTC_CSR_AUTO_RELOAD_MASK|XTC_CSR_DOWN_COUNT_MASK); //config the TCSR as down counter, auto reload
    XTmrCtr_EnableIntr(XPAR_AXI_TIMER_2_BASEADDR,0);

    //initialize the Gpios
    GpioIniti();

    //initialize spi to Master
    SpiInitiToMaster();

    //no used on RL6558/RL6628 LoadBoard
    //initialize as 7705
    //AD7705Initial(AD7705_DEV0_ADDR);

    //initial SPI flash
    XGpio_0_WriteBit(1,MCM_FLASH_DBG_SEL_OFFSET, 0);  //select the MCM flash that isn't used for debug

    //initial AD7994
    AD7994_EN_LOW;
//  AD7994_EN_HIGH;

    //register the interrupt
    microblaze_enable_interrupts();
    //XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_UARTLITE_0_INTERRUPT_INTR,(XInterruptHandler)uart0_intrq_handle,(void*)NULL); //register the uart interrupt
    //XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_UARTLITE_1_INTERRUPT_INTR,(XInterruptHandler)uart1_intrq_handle,(void*)NULL); //register the uart interrupt
    //XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_UARTLITE_2_INTERRUPT_INTR,(XInterruptHandler)uart2_intrq_handle,(void*)NULL); //register the uart interrupt
    //XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_UARTLITE_3_INTERRUPT_INTR,(XInterruptHandler)uart3_intrq_handle,(void*)NULL); //register the uart interrupt

    XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_TIMER_0_INTERRUPT_INTR,(XInterruptHandler)axi_tmr0_intrq_handle,(void*)NULL); //register the axi timer interrupt
    XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_TIMER_1_INTERRUPT_INTR,(XInterruptHandler)axi_tmr1_intrq_handle,(void*)NULL); //register the axi timer interrupt
    XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_TIMER_2_INTERRUPT_INTR,(XInterruptHandler)axi_tmr2_intrq_handle,(void*)NULL); //register the axi timer interrupt

    XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_GPIO_0_IP2INTC_IRPT_INTR,(XInterruptHandler)XGPIO_0_Intrq_Handle,(void*)NULL); //register the GPIO 0 interrupt

    XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_GPIO_1_IP2INTC_IRPT_INTR,(XInterruptHandler)XGPIO_dut0_1_Intrq_Handle,(void*)NULL); //register the GPIO_DUT0_1 interrupt
    XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_GPIO_DUT1_1_IP2INTC_IRPT_INTR,(XInterruptHandler)XGPIO_dut1_1_Intrq_Handle,(void*)NULL); //register the GPIO_DUT1_1 interrupt
    XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_GPIO_DUT2_1_IP2INTC_IRPT_INTR,(XInterruptHandler)XGPIO_dut2_1_Intrq_Handle,(void*)NULL); //register the GPIO_DUT2_1 interrupt
    XIntc_RegisterHandler(XPAR_AXI_INTC_0_BASEADDR,XPAR_AXI_INTC_0_AXI_GPIO_DUT3_1_IP2INTC_IRPT_INTR,(XInterruptHandler)XGPIO_dut3_1_Intrq_Handle,(void*)NULL); //register the GPIO_DUT3_1 interrupt

    XIntc_MasterEnable(XPAR_AXI_INTC_0_BASEADDR); //enable intc
    XIntc_MasterEnable(XPAR_AXI_INTC_0_HIGHADDR);

    //load/enable the timer
    TmrIntvlLoad(XPAR_AXI_TIMER_0_BASEADDR,0,10000);
    XTmrCtr_Enable(XPAR_AXI_TIMER_0_BASEADDR, 0);

    TmrIntvlLoad(XPAR_AXI_TIMER_1_BASEADDR,0,100000);
    XTmrCtr_Enable(XPAR_AXI_TIMER_1_BASEADDR, 0);

    TmrIntvlLoad(XPAR_AXI_TIMER_2_BASEADDR,0,700);
    XTmrCtr_Enable(XPAR_AXI_TIMER_2_BASEADDR, 0);

    //enable the GPIO 0 interrupt
    XGpio_0_ReadBit(1,FT2_START_OFFSET); // configure the ft2_start gpio as input
    XGpio_WriteReg(XPAR_AXI_GPIO_0_BASEADDR,XGPIO_GIE_OFFSET,0x80000000); //Global Interrupt Enable
    XGpio_WriteReg(XPAR_AXI_GPIO_0_BASEADDR,XGPIO_IER_OFFSET,XGPIO_IR_CH2_MASK); //enable the GPIO_0 channel 2 interrupt

    //enable the GPIO_DUT0_1/GPIO_DUT1_1/GPIO_DUT2_1/GPIO_DUT3_1 interrupt
    XGpio_Dutx_ReadBit(XPAR_AXI_GPIO_1_BASEADDR,0,15);     // configure the dut0 ft2_start00 gpio as input
    XGpio_Dutx_ReadBit(XPAR_AXI_GPIO_DUT1_1_BASEADDR,0,15);// configure the dut1 ft2_start00 gpio as input
    XGpio_Dutx_ReadBit(XPAR_AXI_GPIO_DUT2_1_BASEADDR,0,15);// configure the dut2 ft2_start00 gpio as input
    XGpio_Dutx_ReadBit(XPAR_AXI_GPIO_DUT3_1_BASEADDR,0,15);// configure the dut3 ft2_start00 gpio as input

    XGpio_WriteReg(XPAR_AXI_GPIO_1_BASEADDR,XGPIO_GIE_OFFSET,0x80000000);      //Global Interrupt Enable
    XGpio_WriteReg(XPAR_AXI_GPIO_DUT1_1_BASEADDR,XGPIO_GIE_OFFSET,0x80000000); //Global Interrupt Enable
    XGpio_WriteReg(XPAR_AXI_GPIO_DUT2_1_BASEADDR,XGPIO_GIE_OFFSET,0x80000000); //Global Interrupt Enable
    XGpio_WriteReg(XPAR_AXI_GPIO_DUT3_1_BASEADDR,XGPIO_GIE_OFFSET,0x80000000); //Global Interrupt Enable

    XGpio_WriteReg(XPAR_AXI_GPIO_1_BASEADDR,XGPIO_IER_OFFSET,XGPIO_IR_CH1_MASK); //enable the GPIO_0 channel 1 interrupt
    XGpio_WriteReg(XPAR_AXI_GPIO_DUT1_1_BASEADDR,XGPIO_IER_OFFSET,XGPIO_IR_CH1_MASK); //enable the GPIO_0 channel 1 interrupt
    XGpio_WriteReg(XPAR_AXI_GPIO_DUT2_1_BASEADDR,XGPIO_IER_OFFSET,XGPIO_IR_CH1_MASK); //enable the GPIO_0 channel 1 interrupt
    XGpio_WriteReg(XPAR_AXI_GPIO_DUT3_1_BASEADDR,XGPIO_IER_OFFSET,XGPIO_IR_CH1_MASK); //enable the GPIO_0 channel 1 interrupt

    //enable the each int in the intc
    XIntc_EnableIntr(XPAR_AXI_INTC_0_BASEADDR, XPAR_AXI_TIMER_0_INTERRUPT_MASK|XPAR_AXI_TIMER_1_INTERRUPT_MASK|XPAR_AXI_TIMER_2_INTERRUPT_MASK|XPAR_AXI_GPIO_0_IP2INTC_IRPT_MASK
    		|XPAR_AXI_GPIO_1_IP2INTC_IRPT_MASK | XPAR_AXI_GPIO_DUT1_1_IP2INTC_IRPT_MASK | XPAR_AXI_GPIO_DUT2_1_IP2INTC_IRPT_MASK |XPAR_AXI_GPIO_DUT3_1_IP2INTC_IRPT_MASK);
    //XIntc_EnableIntr(XPAR_AXI_INTC_0_BASEADDR, XPAR_AXI_UARTLITE_0_INTERRUPT_MASK|XPAR_AXI_UARTLITE_1_INTERRUPT_MASK|XPAR_AXI_UARTLITE_2_INTERRUPT_MASK|XPAR_AXI_UARTLITE_3_INTERRUPT_MASK);

	dut0.g_test_count = 0;
	dut1.g_test_count = 0;
	dut2.g_test_count = 0;
	dut3.g_test_count = 0;

	dut0.g_pass_count = 0;
	dut1.g_pass_count = 0;
	dut2.g_pass_count = 0;
	dut3.g_pass_count = 0;

  	dut0.g_retest = 0;
  	dut1.g_retest = 0;
    dut2.g_retest = 0;
    dut3.g_retest = 0;

  	for(i=0;i<40;i++)
  	{
  		dut0.g_dut_pattern_status_buf[i] = 0x00;
  		dut1.g_dut_pattern_status_buf[i] = 0x00;
  		dut2.g_dut_pattern_status_buf[i] = 0x00;
  		dut3.g_dut_pattern_status_buf[i] = 0x00;
  	}

  	dut0.g_pattern_timer = 0x3fff;
  	dut1.g_pattern_timer = 0x3fff;
  	dut2.g_pattern_timer = 0x3fff;
  	dut3.g_pattern_timer = 0x3fff;

	dut0.g_relay_control_timer = 0xff;
	dut1.g_relay_control_timer = 0xff;
	dut2.g_relay_control_timer = 0xff;
	dut3.g_relay_control_timer = 0xff;

  	dut0.g_smbus_timer = 0xff;
  	dut1.g_smbus_timer = 0xff;
  	dut2.g_smbus_timer = 0xff;
  	dut3.g_smbus_timer = 0xff;

  	dut0.g_start_smbus_timer = 0xffffffff;
  	dut1.g_start_smbus_timer = 0xffffffff;
  	dut2.g_start_smbus_timer = 0xffffffff;
  	dut3.g_start_smbus_timer = 0xffffffff;

	dut0.g_smbus_status_buf[0] = 0x00;
	dut1.g_smbus_status_buf[0] = 0x00;
	dut2.g_smbus_status_buf[0] = 0x00;
	dut3.g_smbus_status_buf[0] = 0x00;

	dut0.g_smbus_status_buf[1] = 0xff;
	dut1.g_smbus_status_buf[1] = 0xff;
	dut2.g_smbus_status_buf[1] = 0xff;
	dut3.g_smbus_status_buf[1] = 0xff;

	dut0.g_smbus_style = 0x00;
	dut1.g_smbus_style = 0x00;
	dut2.g_smbus_style = 0x00;
	dut3.g_smbus_style = 0x00;

	dut0.g_dut_start_ready = 0;
	dut1.g_dut_start_ready = 0;
	dut2.g_dut_start_ready = 0;
	dut3.g_dut_start_ready = 0;

	dut0.g_uartPatternEnable = 0;
	dut1.g_uartPatternEnable = 0;
	dut2.g_uartPatternEnable = 0;
	dut3.g_uartPatternEnable = 0;

	dut0.g_uartPatternNum = 0x18;
	dut1.g_uartPatternNum = 0x18;
	dut2.g_uartPatternNum = 0x18;
	dut3.g_uartPatternNum = 0x18;

	dut0.g_ack_bit_reread_timer = 20;
	dut1.g_ack_bit_reread_timer = 20;
	dut2.g_ack_bit_reread_timer = 20;
	dut3.g_ack_bit_reread_timer = 20;

	dut0.g_result_polling_tmrcount = 20;
	dut1.g_result_polling_tmrcount = 20;
	dut2.g_result_polling_tmrcount = 20;
	dut3.g_result_polling_tmrcount = 20;

	dut0.g_start_test_flag1 = 0x00;
	dut1.g_start_test_flag1 = 0x00;
	dut2.g_start_test_flag1 = 0x00;
	dut3.g_start_test_flag1 = 0x00;

	dut0.g_start_test_flag2 = 0x00;
	dut1.g_start_test_flag2 = 0x00;
	dut2.g_start_test_flag2 = 0x00;
	dut3.g_start_test_flag2 = 0x00;

	dut0.g_ft2_test_done = 0x00;
	dut1.g_ft2_test_done = 0x00;
	dut2.g_ft2_test_done = 0x00;
	dut3.g_ft2_test_done = 0x00;

  	for(i=0;i<12;i++)
  	{
  		dut0.g_ccdet_calibration_data_buf[i] = 0x88;
  	    dut1.g_ccdet_calibration_data_buf[i] = 0x88;
        dut2.g_ccdet_calibration_data_buf[i] = 0x88;
  		dut3.g_ccdet_calibration_data_buf[i] = 0x88;
  	}

  	dut0.g_ccdet_step = 0;
  	dut1.g_ccdet_step = 0;
  	dut2.g_ccdet_step = 0;
  	dut3.g_ccdet_step = 0;

  	dut0.g_ccdet_retest_signbit = 0;
  	dut1.g_ccdet_retest_signbit = 0;
  	dut2.g_ccdet_retest_signbit = 0;
  	dut3.g_ccdet_retest_signbit = 0;

	for(i=0; i<20; i++)
	{
		dut0.g_current_data_buf[i] = 0;
		dut1.g_current_data_buf[i] = 0;
		dut2.g_current_data_buf[i] = 0;
		dut3.g_current_data_buf[i] = 0;
	}

//	dut0.g_current_data_sum = 0;
//	dut1.g_current_data_sum = 0;
//	dut2.g_current_data_sum = 0;
//	dut3.g_current_data_sum = 0;

	dut0.g_current_data_centre = 0;
	dut1.g_current_data_centre = 0;
	dut2.g_current_data_centre = 0;
	dut3.g_current_data_centre = 0;

	while(1)
	{
		//dut0.g_dut_pattern_status_buf[2] = (dut0.g_uartPatternNum & 0xff);
		//dut1.g_dut_pattern_status_buf[2] = (dut1.g_uartPatternNum & 0xff);
		//dut2.g_dut_pattern_status_buf[2] = (dut2.g_uartPatternNum & 0xff);
		//dut3.g_dut_pattern_status_buf[2] = (dut3.g_uartPatternNum & 0xff);

		u8 progress_initial_start = 0x0;
		progress_initial_start = (dut0.g_dut_start_ready | dut1.g_dut_start_ready | dut2.g_dut_start_ready | dut3.g_dut_start_ready);

		if(progress_initial_start==0)
		{
			//xil_printf("wait FT start signal...\r\n");
		}

		if(progress_initial_start == 0x11)
		{
			xil_printf("FT start signal start!\r\n\r\n");
			platform_initial();
		}

		if(dut0.g_dut_start_ready == 1)
		{
			//if any fail issue//
			if(dut0.g_result_fail != 0x00)
			{
				//break;
				//xil_printf("\r\n dut1_run_fail!\r\n\r\n");
			}
			else
			{
				switch(dut0.g_uartPatternNum)
				{
				case 0x00:
				{
					_by_Pattern0_Setup_dut0();
					break;
				}
				case 0x01:
				{
					_by_Pattern1_dut0();
					break;
				}
				case 0x02:
				{
					_by_Pattern2_dut0();
					break;
				}
				case 0x03:
				{
					_by_Pattern3_dut0();
					break;
				}
				case 0x04:
				{
					_by_Pattern4_dut0();
					break;
				}
				case 0x05:
				{
					_by_Pattern5_dut0();
					break;
				}
				case 0x06:
				{
					_by_Pattern6_dut0();
					break;
				}
				case 0x07:
				{
					_by_Pattern7_dut0();
					break;
				}
				case 0x08:
				{
					//CC OVP
					_by_Pattern8_dut0();
					break;
				}
				case 0x09:
				{
					//SBU OVP
					_by_Pattern9_dut0();
					break;
				}
				case 0x0A:
				{
					//BMC RX
					_by_PatternA_dut0();
					break;
				}
				case 0x0B:
				{
					//VFRS
					_by_PatternB_dut0();
					break;
				}
				case 0x0C:
				{
					//CC detect
					_by_PatternC_dut0();
					break;
				}
				case 0x0D:
				{
					//_by_PatternD_dut0();
					break;
				}
				case 0x0E:
				{
//					_by_Pattern_U2_host_test1_dut0();
					if(g_u2_host_test1_flag)
					{
						dut0.g_uartPatternNum++;
					}
					break;
				}
				case 0x0F:
				{
					_by_PatternF_efuse_dut0();
					break;
				}
				case 0x10:
				{
					//_by_Pattern_U2_host_test1_dut0();
					break;
				}
				case 0x11:
				{
					_by_Pattern_PD_msg_dut0();
					break;
				}
				case 0x14:
				{
					_by_Pattern_U2_host_test2_dut0();
					break;
				}
				case 0x15:
				{
					_by_Pattern_Softdelink_current_test_dut0();
					break;
				}
				case 0x16:
				{
					//_by_Pattern_check_crc32_dut0();
					break;
				}
				case 0x18:
				{
					dut0.g_uartPatternNum = 0x00;
					break;
				}
				}
			}
		}

		if(dut1.g_dut_start_ready == 1)
		{
			//if any fail issue//
			if(dut1.g_result_fail != 0x00)
			{
				//break;
				//xil_printf("\r\n dut1_run_fail!\r\n\r\n");
			}
			else
			{
				switch(dut1.g_uartPatternNum)
				{
				case 0x00:
				{
					_by_Pattern0_Setup_dut1();
					break;
				}
				case 0x01:
				{
					_by_Pattern1_dut1();
					break;
				}
				case 0x02:
				{
					_by_Pattern2_dut1();
					break;
				}
				case 0x03:
				{
					_by_Pattern3_dut1();
					break;
				}
				case 0x04:
				{
					_by_Pattern4_dut1();
					break;
				}
				case 0x05:
				{
					_by_Pattern5_dut1();
					break;
				}
				case 0x06:
				{
					_by_Pattern6_dut1();
					break;
				}
				case 0x07:
				{
					_by_Pattern7_dut1();
					break;
				}
				case 0x08:
				{
					//CC OVP
					_by_Pattern8_dut1();
					break;
				}
				case 0x09:
				{
					//SBU OVP
					_by_Pattern9_dut1();
					break;
				}
				case 0x0A:
				{
					//BMC RX
					_by_PatternA_dut1();
					break;
				}
				case 0x0B:
				{
					//VFRS
					_by_PatternB_dut1();
					break;
				}
				case 0x0C:
				{
					//CC detect
					_by_PatternC_dut1();
					break;
				}
				case 0x0D:
				{
					//_by_PatternD_dut1();
					break;
				}
				case 0x0E:
				{
//					_by_Pattern_U2_host_test1_dut1();
					if(g_u2_host_test1_flag)
					{
						dut1.g_uartPatternNum++;
					}
					break;
				}
				case 0x0F:
				{
					_by_PatternF_efuse_dut1();
					break;
				}
				case 0x10:
				{
					//_by_Pattern_U2_host_test1_dut1();
					break;
				}
				case 0x11:
				{
					_by_Pattern_PD_msg_dut1();
					break;
				}
				case 0x14:
				{
					_by_Pattern_U2_host_test2_dut1();
					break;
				}
				case 0x15:
				{
					_by_Pattern_Softdelink_current_test_dut1();
					break;
				}
				case 0x16:
				{
					//_by_Pattern_check_crc32_dut1();
					break;
				}
				case 0x18:
				{
					dut1.g_uartPatternNum = 0x00;
					break;
				}
				}
			}
		}

		if(dut2.g_dut_start_ready == 1)
		{
			//if any fail issue//
			if(dut2.g_result_fail != 0x00)
			{
				//break;
				//xil_printf("\r\n dut2_run_fail!\r\n\r\n");
			}
			else
			{
				switch(dut2.g_uartPatternNum)
				{
				case 0x00:
				{
					_by_Pattern0_Setup_dut2();
					break;
				}
				case 0x01:
				{
					_by_Pattern1_dut2();
					break;
				}
				case 0x02:
				{
					_by_Pattern2_dut2();
					break;
				}
				case 0x03:
				{
					_by_Pattern3_dut2();
					break;
				}
				case 0x04:
				{
					_by_Pattern4_dut2();
					break;
				}
				case 0x05:
				{
					_by_Pattern5_dut2();
					break;
				}
				case 0x06:
				{
					_by_Pattern6_dut2();
					break;
				}
				case 0x07:
				{
					_by_Pattern7_dut2();
					break;
				}
				case 0x08:
				{
					//CC OVP
					_by_Pattern8_dut2();
					break;
				}
				case 0x09:
				{
					//SBU OVP
					_by_Pattern9_dut2();
					break;
				}
				case 0x0A:
				{
					//BMC RX
					_by_PatternA_dut2();
					break;
				}
				case 0x0B:
				{
					//VFRS
					_by_PatternB_dut2();
					break;
				}
				case 0x0C:
				{
					//CC detect
					_by_PatternC_dut2();
					break;
				}
				case 0x0D:
				{
					//_by_PatternD_dut2();
					break;
				}
				case 0x0E:
				{
//					_by_Pattern_U2_host_test1_dut2();
					if(g_u2_host_test1_flag)
					{
						dut2.g_uartPatternNum++;
					}
					break;
				}
				case 0x0F:
				{
					_by_PatternF_efuse_dut2();
					break;
				}
				case 0x10:
				{
					//_by_Pattern_U2_host_test1_dut2();
					break;
				}
				case 0x11:
				{
					_by_Pattern_PD_msg_dut2();
					break;
				}
				case 0x14:
				{
					_by_Pattern_U2_host_test2_dut2();
					break;
				}
				case 0x15:
				{
					_by_Pattern_Softdelink_current_test_dut2();
					break;
				}
				case 0x16:
				{
					//_by_Pattern_check_crc32_dut2();
					break;
				}
				case 0x18:
				{
					dut2.g_uartPatternNum = 0x00;
					break;
				}
				}
			}
		}

		if(dut3.g_dut_start_ready == 1)
		{
			//if any fail issue//
			if(dut3.g_result_fail != 0x00)
			{
				//break;
				//xil_printf("\r\n dut3_run_fail!\r\n\r\n");
			}
			else
			{
				switch(dut3.g_uartPatternNum)
				{
				case 0x00:
				{
					_by_Pattern0_Setup_dut3();
					break;
				}
				case 0x01:
				{
					_by_Pattern1_dut3();
					break;
				}
				case 0x02:
				{
					_by_Pattern2_dut3();
					break;
				}
				case 0x03:
				{
					_by_Pattern3_dut3();
					break;
				}
				case 0x04:
				{
					_by_Pattern4_dut3();
					break;
				}
				case 0x05:
				{
					_by_Pattern5_dut3();
					break;
				}
				case 0x06:
				{
					_by_Pattern6_dut3();
					break;
				}
				case 0x07:
				{
					_by_Pattern7_dut3();
					break;
				}
				case 0x08:
				{
					//CC OVP
					_by_Pattern8_dut3();
					break;
				}
				case 0x09:
				{
					//SBU OVP
					_by_Pattern9_dut3();
					break;
				}
				case 0x0A:
				{
					//BMC RX
					_by_PatternA_dut3();
					break;
				}
				case 0x0B:
				{
					//VFRS
					_by_PatternB_dut3();
					break;
				}
				case 0x0C:
				{
					//CC detect
					_by_PatternC_dut3();
					break;
				}
				case 0x0D:
				{
					//_by_PatternD_dut3();
					break;
				}
				case 0x0E:
				{
//					_by_Pattern_U2_host_test1_dut3();
					if(g_u2_host_test1_flag)
					{
						dut3.g_uartPatternNum++;
					}
					break;
				}
				case 0x0F:
				{
					_by_PatternF_efuse_dut3();
					break;
				}
				case 0x10:
				{
					//_by_Pattern_U2_host_test1_dut3();
					break;
				}
				case 0x11:
				{
					_by_Pattern_PD_msg_dut3();
					break;
				}
				case 0x14:
				{
					_by_Pattern_U2_host_test2_dut3();
					break;
				}
				case 0x15:
				{
					_by_Pattern_Softdelink_current_test_dut3();
					break;
				}
				case 0x16:
				{
					//_by_Pattern_check_crc32_dut3();
					break;
				}
				case 0x18:
				{
					dut3.g_uartPatternNum = 0x00;
					break;
				}
				}
			}
		}

		if((dut0.g_start_test_flag1 == 0x01)||(dut1.g_start_test_flag1 == 0x01)||(dut2.g_start_test_flag1 == 0x01)||(dut3.g_start_test_flag1 == 0x01))
		{
			dut0.g_start_test_flag1 = 0x00;
			dut1.g_start_test_flag1 = 0x00;
			dut2.g_start_test_flag1 = 0x00;
			dut3.g_start_test_flag1 = 0x00;

			i2c_send_vendorcmd(AD7994_DEV0_ADDR, USB_HOST_ADDR, FIRST_TEST);
			msdelay(5);
			//Resolve the problem: data out of sync
			i2c_send_vendorcmd(AD7994_DEV0_ADDR, USB_HOST_ADDR, 0x05);
			msdelay(5);
			xil_printf("usb host start to test billboard/DP reverse/U2 mux B!\r\n\r\n");
		}

		if((dut0.g_start_test_flag2 == 0x02)||(dut1.g_start_test_flag2 == 0x02)||(dut2.g_start_test_flag2 == 0x02)||(dut3.g_start_test_flag2 == 0x02))
		{
			dut0.g_start_test_flag2 = 0x00;
			dut1.g_start_test_flag2 = 0x00;
			dut2.g_start_test_flag2 = 0x00;
			dut3.g_start_test_flag2 = 0x00;

			i2c_send_vendorcmd(AD7994_DEV0_ADDR, USB_HOST_ADDR, SECOND_TEST);
			msdelay(5);
			//Resolve the problem: data out of sync
			i2c_send_vendorcmd(AD7994_DEV0_ADDR, USB_HOST_ADDR, 0x05);
			msdelay(5);
			xil_printf("usb host start to test DP obverse/U2 mux A!\r\n\r\n");
		}

		if((dut0.g_ft2_test_done == 0x00) && (dut1.g_ft2_test_done == 0x00) && (dut2.g_ft2_test_done == 0x00) && (dut3.g_ft2_test_done == 0x00))
		{
			i2c_send_vendorcmd(AD7994_DEV0_ADDR, USB_HOST_ADDR, FINISH_TEST);
			msdelay(5);
			//Resolve the problem: data out of sync
			i2c_send_vendorcmd(AD7994_DEV0_ADDR, USB_HOST_ADDR, 0x05);
			msdelay(2000);
			xil_printf("without start signal or ft2 test done, send uart data 0x03!\r\n\r\n");

			XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR, 0, 1, 1);
		}
   }
   return 0;
}


