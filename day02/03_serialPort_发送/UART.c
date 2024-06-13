/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCAI.com ---------------------------------------------*/
/* --- BBS: www.STCAIMCU.com  -----------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 锟斤拷锟揭拷诔锟斤拷锟斤拷锟绞癸拷么舜锟斤拷锟�,锟斤拷锟节筹拷锟斤拷锟斤拷注锟斤拷使锟斤拷锟斤拷STC锟斤拷锟斤拷锟较硷拷锟斤拷锟斤拷            */
/*---------------------------------------------------------------------*/

#include "UART.h"

//========================================================================
//                               锟斤拷锟截憋拷锟斤拷锟斤拷锟斤拷
//========================================================================

#ifdef UART1
COMx_Define	COM1;
u8	xdata TX1_Buffer[COM_TX1_Lenth];	//锟斤拷锟酵伙拷锟斤拷
u8 	xdata RX1_Buffer[COM_RX1_Lenth];	//锟斤拷锟秸伙拷锟斤拷
#endif
#ifdef UART2
COMx_Define	COM2;
u8	xdata TX2_Buffer[COM_TX2_Lenth];	//锟斤拷锟酵伙拷锟斤拷
u8 	xdata RX2_Buffer[COM_RX2_Lenth];	//锟斤拷锟秸伙拷锟斤拷
#endif
#ifdef UART3
COMx_Define	COM3;
u8	xdata TX3_Buffer[COM_TX3_Lenth];	//锟斤拷锟酵伙拷锟斤拷
u8 	xdata RX3_Buffer[COM_RX3_Lenth];	//锟斤拷锟秸伙拷锟斤拷
#endif
#ifdef UART4
COMx_Define	COM4;
u8	xdata TX4_Buffer[COM_TX4_Lenth];	//锟斤拷锟酵伙拷锟斤拷
u8 	xdata RX4_Buffer[COM_RX4_Lenth];	//锟斤拷锟秸伙拷锟斤拷
#endif

//========================================================================
// 锟斤拷锟斤拷: UART_Configuration
// 锟斤拷锟斤拷: UART锟斤拷始锟斤拷锟斤拷锟斤拷.
// 锟斤拷锟斤拷: UARTx: UART锟斤拷锟�, COMx锟结构锟斤拷锟斤拷,锟斤拷慰锟経ART.h锟斤拷亩锟斤拷锟�.
// 锟斤拷锟斤拷: none.
// 锟芥本: V1.0, 2012-10-22
//========================================================================
u8 UART_Configuration(u8 UARTx, COMx_InitDefine *COMx)
{
#if  defined( UART1 ) || defined( UART2 ) || defined( UART3 ) || defined( UART4 )
	u16	i;
	u32	j;
#else
	UARTx = NULL;
	COMx = NULL;
#endif

#ifdef UART1
	if(UARTx == UART1)
	{
		COM1.TX_send    = 0;
		COM1.TX_write   = 0;
		COM1.B_TX_busy  = 0;
		COM1.RX_Cnt     = 0;
		COM1.RX_TimeOut = 0;
		
		for(i=0; i<COM_TX1_Lenth; i++)	TX1_Buffer[i] = 0;
		for(i=0; i<COM_RX1_Lenth; i++)	RX1_Buffer[i] = 0;

		SCON = (SCON & 0x3f) | COMx->UART_Mode;	//模式锟斤拷锟斤拷
		if((COMx->UART_Mode == UART_9bit_BRTx) || (COMx->UART_Mode == UART_8bit_BRTx))	//锟缴变波锟斤拷锟斤拷
		{
			j = (MAIN_Fosc / 4) / COMx->UART_BaudRate;	//锟斤拷1T锟斤拷锟斤拷
			if(j >= 65536UL)	return FAIL;	//锟斤拷锟斤拷
			j = 65536UL - j;
			if(COMx->UART_BRT_Use == BRT_Timer1)
			{
				TR1 = 0;
				AUXR &= ~0x01;		//S1 BRT Use Timer1;
				TMOD &= ~(1<<6);	//Timer1 set As Timer
				TMOD &= ~0x30;		//Timer1_16bitAutoReload;
				AUXR |=  (1<<6);	//Timer1 set as 1T mode
				TH1 = (u8)(j>>8);
				TL1 = (u8)j;
				ET1 = 0;	//锟斤拷止锟叫讹拷
				TMOD &= ~0x40;	//锟斤拷时
				INTCLKO &= ~0x02;	//锟斤拷锟斤拷锟绞憋拷锟�
				TR1  = 1;
			}
			else if(COMx->UART_BRT_Use == BRT_Timer2)
			{
				AUXR &= ~(1<<4);	//Timer stop
				AUXR |= 0x01;		//S1 BRT Use Timer2;
				AUXR &= ~(1<<3);	//Timer2 set As Timer
				AUXR |=  (1<<2);	//Timer2 set as 1T mode
				T2H = (u8)(j>>8);
				T2L = (u8)j;
				IE2  &= ~(1<<2);	//锟斤拷止锟叫讹拷
				AUXR |=  (1<<4);	//Timer run enable
			}
			else return FAIL;	//锟斤拷锟斤拷
		}
		else if(COMx->UART_Mode == UART_ShiftRight)
		{
			if(COMx->BaudRateDouble == ENABLE)	AUXR |=  (1<<5);	//锟教讹拷锟斤拷锟斤拷锟斤拷SysClk/2
			else								AUXR &= ~(1<<5);	//锟教讹拷锟斤拷锟斤拷锟斤拷SysClk/12
		}
		else if(COMx->UART_Mode == UART_9bit)	//锟教讹拷锟斤拷锟斤拷锟斤拷SysClk*2^SMOD/64
		{
			if(COMx->BaudRateDouble == ENABLE)	PCON |=  (1<<7);	//锟教讹拷锟斤拷锟斤拷锟斤拷SysClk/32
			else								PCON &= ~(1<<7);	//锟教讹拷锟斤拷锟斤拷锟斤拷SysClk/64
		}
		UART1_RxEnable(COMx->UART_RxEnable);	//UART锟斤拷锟斤拷使锟斤拷

		return SUCCESS;
	}
#endif
#ifdef UART2
	if(UARTx == UART2)
	{
		COM2.TX_send    = 0;
		COM2.TX_write   = 0;
		COM2.B_TX_busy  = 0;
		COM2.RX_Cnt     = 0;
		COM2.RX_TimeOut = 0;

		for(i=0; i<COM_TX2_Lenth; i++)	TX2_Buffer[i] = 0;
		for(i=0; i<COM_RX2_Lenth; i++)	RX2_Buffer[i] = 0;

		if((COMx->UART_Mode == UART_9bit_BRTx) ||(COMx->UART_Mode == UART_8bit_BRTx))	//锟缴变波锟斤拷锟斤拷
		{
			if(COMx->UART_Mode == UART_9bit_BRTx)	S2CON |=  (1<<7);	//9bit
			else									S2CON &= ~(1<<7);	//8bit
			j = (MAIN_Fosc / 4) / COMx->UART_BaudRate;	//锟斤拷1T锟斤拷锟斤拷
			if(j >= 65536UL)	return FAIL;	//锟斤拷锟斤拷
			j = 65536UL - j;
			AUXR &= ~(1<<4);	//Timer stop
			AUXR &= ~(1<<3);	//Timer2 set As Timer
			AUXR |=  (1<<2);	//Timer2 set as 1T mode
			T2H = (u8)(j>>8);
			T2L = (u8)j;
			IE2  &= ~(1<<2);	//锟斤拷止锟叫讹拷
			AUXR |=  (1<<4);	//Timer run enable
		}
		else	return FAIL;	//模式锟斤拷锟斤拷
		UART2_RxEnable(COMx->UART_RxEnable);	//UART锟斤拷锟斤拷使锟斤拷

		return SUCCESS;
	}
#endif
#ifdef UART3
	if(UARTx == UART3)
	{
		COM3.TX_send    = 0;
		COM3.TX_write   = 0;
		COM3.B_TX_busy  = 0;
		COM3.RX_Cnt     = 0;
		COM3.RX_TimeOut = 0;
		for(i=0; i<COM_TX3_Lenth; i++)	TX3_Buffer[i] = 0;
		for(i=0; i<COM_RX3_Lenth; i++)	RX3_Buffer[i] = 0;

		if((COMx->UART_Mode == UART_9bit_BRTx) || (COMx->UART_Mode == UART_8bit_BRTx))	//锟缴变波锟斤拷锟斤拷
		{
			if(COMx->UART_Mode == UART_9bit_BRTx)	S3CON |=  (1<<7);	//9bit
			else									S3CON &= ~(1<<7);	//8bit
			j = (MAIN_Fosc / 4) / COMx->UART_BaudRate;	//锟斤拷1T锟斤拷锟斤拷
			if(j >= 65536UL)	return FAIL;	//锟斤拷锟斤拷
			j = 65536UL - j;
			if(COMx->UART_BRT_Use == BRT_Timer3)
			{
				S3CON |=  (1<<6);		//S3 BRT Use Timer3;
				T3H = (u8)(j>>8);
				T3L = (u8)j;
				T4T3M &= 0xf0;
				T4T3M |= 0x0a;			//Timer3 set As Timer, 1T mode, Start timer3
			}
			else if(COMx->UART_BRT_Use == BRT_Timer2)
			{
				AUXR &= ~(1<<4);		//Timer stop
				S3CON &= ~(1<<6);		//S3 BRT Use Timer2;
				AUXR &= ~(1<<3);		//Timer2 set As Timer
				AUXR |=  (1<<2);		//Timer2 set as 1T mode
				T2H = (u8)(j>>8);
				T2L = (u8)j;
				IE2  &= ~(1<<2);	//锟斤拷止锟叫讹拷
				AUXR |=  (1<<4);	//Timer run enable
			}
			else return FAIL;	//锟斤拷锟斤拷
		}
		else	return FAIL;	//模式锟斤拷锟斤拷
		UART3_RxEnable(COMx->UART_RxEnable);	//UART锟斤拷锟斤拷使锟斤拷

		return SUCCESS;
	}
#endif
#ifdef UART4
	if(UARTx == UART4)
	{
		COM4.TX_send    = 0;
		COM4.TX_write   = 0;
		COM4.B_TX_busy  = 0;
		COM4.RX_Cnt     = 0;
		COM4.RX_TimeOut = 0;
		for(i=0; i<COM_TX4_Lenth; i++)	TX4_Buffer[i] = 0;
		for(i=0; i<COM_RX4_Lenth; i++)	RX4_Buffer[i] = 0;

		if((COMx->UART_Mode == UART_9bit_BRTx) || (COMx->UART_Mode == UART_8bit_BRTx))	//锟缴变波锟斤拷锟斤拷
		{
			if(COMx->UART_Mode == UART_9bit_BRTx)	S4CON |=  (1<<7);	//9bit
			else									S4CON &= ~(1<<7);	//8bit
			j = (MAIN_Fosc / 4) / COMx->UART_BaudRate;	//锟斤拷1T锟斤拷锟斤拷
			if(j >= 65536UL)	return FAIL;	//锟斤拷锟斤拷
			j = 65536UL - j;
			if(COMx->UART_BRT_Use == BRT_Timer4)
			{
				S4CON |=  (1<<6);		//S4 BRT Use Timer4;
				T4H = (u8)(j>>8);
				T4L = (u8)j;
				T4T3M &= 0x0f;
				T4T3M |= 0xa0;			//Timer4 set As Timer, 1T mode, Start timer4
			}
			else if(COMx->UART_BRT_Use == BRT_Timer2)
			{
				AUXR &= ~(1<<4);		//Timer stop
				S4CON &= ~(1<<6);		//S4 BRT Use Timer2;
				AUXR &= ~(1<<3);		//Timer2 set As Timer
				AUXR |=  (1<<2);		//Timer2 set as 1T mode
				T2H = (u8)(j>>8);
				T2L = (u8)j;
				IE2  &= ~(1<<2);	//锟斤拷止锟叫讹拷
				AUXR |=  (1<<4);	//Timer run enable
			}
			else return FAIL;	//锟斤拷锟斤拷
		}
		else	return FAIL;	//模式锟斤拷锟斤拷
		UART4_RxEnable(COMx->UART_RxEnable);	//UART锟斤拷锟斤拷使锟斤拷
		
		return SUCCESS;
	}
#endif
	return FAIL;	//锟斤拷锟斤拷
}

/*********************************************************/

/********************* UART1 锟斤拷锟斤拷 ************************/
#ifdef UART1
void TX1_write2buff(u8 dat)	//锟斤拷锟斤拷1锟斤拷锟酵猴拷锟斤拷
{
    #if(UART_QUEUE_MODE == 1)
	TX1_Buffer[COM1.TX_write] = dat;	//装锟斤拷锟酵伙拷锟藉，使锟矫讹拷锟斤拷式锟斤拷锟捷凤拷锟酵ｏ拷一锟斤拷锟皆凤拷锟斤拷锟斤拷锟捷筹拷锟饺诧拷要锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷小锟斤拷COM_TXn_Lenth锟斤拷
	if(++COM1.TX_write >= COM_TX1_Lenth)	COM1.TX_write = 0;

	if(COM1.B_TX_busy == 0)		//锟斤拷锟斤拷
	{  
		COM1.B_TX_busy = 1;		//锟斤拷志忙
		TI = 1;					//锟斤拷锟斤拷锟斤拷锟斤拷锟叫讹拷
	}
    #else
    //锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷式锟斤拷锟酵凤拷锟斤拷
	SBUF = dat;
	COM1.B_TX_busy = 1;		//锟斤拷志忙
	while(COM1.B_TX_busy);
    #endif
}

void PrintString1(u8 *puts)
{
    for (; *puts != 0;	puts++)  TX1_write2buff(*puts); 	//锟斤拷锟斤拷停止锟斤拷0锟斤拷锟斤拷
}

#endif

/********************* UART2 锟斤拷锟斤拷 ************************/
#ifdef UART2
void TX2_write2buff(u8 dat)	//锟斤拷锟斤拷2锟斤拷锟酵猴拷锟斤拷
{
    #if(UART_QUEUE_MODE == 1)
	TX2_Buffer[COM2.TX_write] = dat;	//装锟斤拷锟酵伙拷锟藉，使锟矫讹拷锟斤拷式锟斤拷锟捷凤拷锟酵ｏ拷一锟斤拷锟皆凤拷锟斤拷锟斤拷锟捷筹拷锟饺诧拷要锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷小锟斤拷COM_TXn_Lenth锟斤拷
	if(++COM2.TX_write >= COM_TX2_Lenth)	COM2.TX_write = 0;

	if(COM2.B_TX_busy == 0)		//锟斤拷锟斤拷
	{  
		COM2.B_TX_busy = 1;		//锟斤拷志忙
		SET_TI2();				//锟斤拷锟斤拷锟斤拷锟斤拷锟叫讹拷
	}
    #else
    //锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷式锟斤拷锟酵凤拷锟斤拷
	S2BUF = dat;
	COM2.B_TX_busy = 1;		//锟斤拷志忙
	while(COM2.B_TX_busy);
    #endif
}

void PrintString2(u8 *puts)
{
    for (; *puts != 0;	puts++)  TX2_write2buff(*puts); 	//锟斤拷锟斤拷停止锟斤拷0锟斤拷锟斤拷
}

#endif

/********************* UART3 锟斤拷锟斤拷 ************************/
#ifdef UART3
void TX3_write2buff(u8 dat)	//锟斤拷锟斤拷3锟斤拷锟酵猴拷锟斤拷
{
    #if(UART_QUEUE_MODE == 1)
	TX3_Buffer[COM3.TX_write] = dat;	//装锟斤拷锟酵伙拷锟藉，使锟矫讹拷锟斤拷式锟斤拷锟捷凤拷锟酵ｏ拷一锟斤拷锟皆凤拷锟斤拷锟斤拷锟捷筹拷锟饺诧拷要锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷小锟斤拷COM_TXn_Lenth锟斤拷
	if(++COM3.TX_write >= COM_TX3_Lenth)	COM3.TX_write = 0;

	if(COM3.B_TX_busy == 0)		//锟斤拷锟斤拷
	{  
		COM3.B_TX_busy = 1;		//锟斤拷志忙
		SET_TI3();				//锟斤拷锟斤拷锟斤拷锟斤拷锟叫讹拷
	}
    #else
    //锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷式锟斤拷锟酵凤拷锟斤拷
	S3BUF = dat;
	COM3.B_TX_busy = 1;		//锟斤拷志忙
	while(COM3.B_TX_busy);
    #endif
}

void PrintString3(u8 *puts)
{
    for (; *puts != 0;	puts++)  TX3_write2buff(*puts); 	//锟斤拷锟斤拷停止锟斤拷0锟斤拷锟斤拷
}

#endif

/********************* UART4 锟斤拷锟斤拷 ************************/
#ifdef UART4
void TX4_write2buff(u8 dat)	//锟斤拷锟斤拷4锟斤拷锟酵猴拷锟斤拷
{
    #if(UART_QUEUE_MODE == 1)
	TX4_Buffer[COM4.TX_write] = dat;	//装锟斤拷锟酵伙拷锟藉，使锟矫讹拷锟斤拷式锟斤拷锟捷凤拷锟酵ｏ拷一锟斤拷锟皆凤拷锟斤拷锟斤拷锟捷筹拷锟饺诧拷要锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷小锟斤拷COM_TXn_Lenth锟斤拷
	if(++COM4.TX_write >= COM_TX4_Lenth)	COM4.TX_write = 0;

	if(COM4.B_TX_busy == 0)		//锟斤拷锟斤拷
	{  
		COM4.B_TX_busy = 1;		//锟斤拷志忙
		SET_TI4();				//锟斤拷锟斤拷锟斤拷锟斤拷锟叫讹拷
	}
    #else
    //锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷式锟斤拷锟酵凤拷锟斤拷
	S4BUF = dat;
	COM4.B_TX_busy = 1;		//锟斤拷志忙
	while(COM4.B_TX_busy);
    #endif
}

void PrintString4(u8 *puts)
{
    for (; *puts != 0;	puts++)  TX4_write2buff(*puts); 	//锟斤拷锟斤拷停止锟斤拷0锟斤拷锟斤拷
}

#endif

/*********************************************************/
/*
void COMx_write2buff(u8 UARTx, u8 dat)  //UART1/UART2/UART3/UART4
{
	if(UARTx == UART1)	TX1_write2buff(dat);
	if(UARTx == UART2)	TX2_write2buff(dat);
	if(UARTx == UART3)	TX3_write2buff(dat);
	if(UARTx == UART4)	TX4_write2buff(dat);
}

void PrintString(u8 UARTx, u8 *puts)
{
    for (; *puts != 0;	puts++)  COMx_write2buff(UARTx,*puts); 	//锟斤拷锟斤拷停止锟斤拷0锟斤拷锟斤拷
}
*/

/********************* Printf 锟斤拷锟斤拷 ************************/
#if(PRINTF_SELECT == 1)

char putchar(char c)
{
	TX1_write2buff(c);
	return c;
}

#elif(PRINTF_SELECT == 2)

char putchar(char c)
{
	TX2_write2buff(c);
	return c;
}

#elif(PRINTF_SELECT == 3)

char putchar(char c)
{
	TX3_write2buff(c);
	return c;
}

#elif(PRINTF_SELECT == 4)

char putchar(char c)
{
	TX4_write2buff(c);
	return c;
}

#endif
