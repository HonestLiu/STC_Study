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

#ifndef __UART_H
#define __UART_H	 

#include	"config.h"

//========================================================================
//                              锟斤拷锟斤拷锟斤拷锟斤拷
//========================================================================

#define	UART1	1       //使锟斤拷锟斤拷些锟斤拷锟节就匡拷锟斤拷应锟侥讹拷锟藉，锟斤拷锟矫的达拷锟节匡拷锟斤拷锟轿碉拷锟斤拷锟藉，锟斤拷省锟斤拷源
#define	UART2	2
#define	UART3	3
#define	UART4	4

#define	UART_QUEUE_MODE	0           //锟斤拷锟矫达拷锟节凤拷锟斤拷模式锟斤拷0锟斤拷锟斤拷锟斤拷模式锟斤拷1锟斤拷锟斤拷锟斤拷模式

#define	PRINTF_SELECT  UART2		//选锟斤拷 printf 锟斤拷锟斤拷锟斤拷使锟矫的达拷锟节ｏ拷锟斤拷锟斤拷 UART1~UART4

#ifdef UART1
#define	COM_TX1_Lenth	128
#define	COM_RX1_Lenth	128
#endif
#ifdef UART2
#define	COM_TX2_Lenth	128
#define	COM_RX2_Lenth	128
#endif
#ifdef UART3
#define	COM_TX3_Lenth	64
#define	COM_RX3_Lenth	64
#endif
#ifdef UART4
#define	COM_TX4_Lenth	32
#define	COM_RX4_Lenth	32
#endif

#define	UART_ShiftRight	0		//同锟斤拷锟斤拷位锟斤拷锟�
#define	UART_8bit_BRTx	(1<<6)	//8位锟斤拷锟斤拷,锟缴变波锟斤拷锟斤拷
#define	UART_9bit		(2<<6)	//9位锟斤拷锟斤拷,锟教讹拷锟斤拷锟斤拷锟斤拷
#define	UART_9bit_BRTx	(3<<6)	//9位锟斤拷锟斤拷,锟缴变波锟斤拷锟斤拷

#define	TimeOutSet1		5
#define	TimeOutSet2		5
#define	TimeOutSet3		5
#define	TimeOutSet4		5

#define	BRT_Timer1	1
#define	BRT_Timer2	2
#define	BRT_Timer3	3
#define	BRT_Timer4	4

//========================================================================
//                              UART锟斤拷锟斤拷
//========================================================================

#define		TI2					(S2CON & 2) 		/* 锟叫讹拷TI2锟角凤拷锟斤拷锟斤拷锟� */
#define		RI2					(S2CON & 1) 		/* 锟叫讹拷RI2锟角凤拷锟斤拷锟斤拷锟斤拷 */
#define		SET_TI2()			S2CON |=  (1<<1)	/* 锟斤拷锟斤拷TI2(锟斤拷锟斤拷锟叫讹拷) */
#define		CLR_TI2()			S2CON &= ~(1<<1)	/* 锟斤拷锟絋I2 */
#define		CLR_RI2()			S2CON &= ~1			/* 锟斤拷锟絉I2 */

#define		TI3					(S3CON & 2) != 0	/* 锟叫讹拷TI3锟角凤拷锟斤拷锟斤拷锟� */
#define		RI3					(S3CON & 1) != 0	/* 锟叫讹拷RI3锟角凤拷锟斤拷锟斤拷锟斤拷 */
#define		SET_TI3()			S3CON |=  (1<<1)	/* 锟斤拷锟斤拷TI3(锟斤拷锟斤拷锟叫讹拷) */
#define		CLR_TI3()			S3CON &= ~(1<<1)	/* 锟斤拷锟絋I3 */
#define		CLR_RI3()			S3CON &= ~1			/* 锟斤拷锟絉I3 */

#define		TI4					(S4CON & 2) != 0	/* 锟叫讹拷TI3锟角凤拷锟斤拷锟斤拷锟� */
#define		RI4					(S4CON & 1) != 0	/* 锟叫讹拷RI3锟角凤拷锟斤拷锟斤拷锟斤拷 */
#define		SET_TI4()			S4CON |=  2			/* 锟斤拷锟斤拷TI3(锟斤拷锟斤拷锟叫讹拷) */
#define		CLR_TI4()			S4CON &= ~2			/* 锟斤拷锟絋I3 */
#define		CLR_RI4()			S4CON &= ~1			/* 锟斤拷锟絉I3 */

#define		UART1_RxEnable(n)	(n==0?(REN = 0):(REN = 1))			/* UART1锟斤拷锟斤拷使锟斤拷 */
#define		UART2_RxEnable(n)	S2CON = (S2CON & ~0x10) | (n << 4)	/* UART2锟斤拷锟斤拷使锟斤拷 */
#define		UART3_RxEnable(n)	S3CON = (S3CON & ~0x10) | (n << 4)	/* UART3锟斤拷锟斤拷使锟斤拷 */
#define		UART4_RxEnable(n)	S4CON = (S4CON & ~0x10) | (n << 4)	/* UART4锟斤拷锟斤拷使锟斤拷 */

typedef struct
{ 
	u8	TX_send;		//锟窖凤拷锟斤拷指锟斤拷
	u8	TX_write;		//锟斤拷锟斤拷写指锟斤拷
	u8	B_TX_busy;		//忙锟斤拷志

	u8 	RX_Cnt;			//锟斤拷锟斤拷锟街节硷拷锟斤拷
	u8	RX_TimeOut;		//锟斤拷锟秸筹拷时
} COMx_Define; 

typedef struct
{ 
	u8	UART_Mode;			//模式,         UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	u8	UART_BRT_Use;		//使锟矫诧拷锟斤拷锟斤拷,   BRT_Timer1,BRT_Timer2,BRT_Timer3,BRT_Timer4
	u32	UART_BaudRate;		//锟斤拷锟斤拷锟斤拷, 	   一锟斤拷 110 ~ 115200
	u8	Morecommunicate;	//锟斤拷锟酵ㄑ讹拷锟斤拷锟�, ENABLE,DISABLE
	u8	UART_RxEnable;		//锟斤拷锟斤拷锟斤拷锟斤拷,   ENABLE,DISABLE
	u8	BaudRateDouble;		//锟斤拷锟斤拷锟绞加憋拷, ENABLE,DISABLE
} COMx_InitDefine; 

#ifdef UART1
extern	COMx_Define	COM1;
extern	u8	xdata TX1_Buffer[COM_TX1_Lenth];	//锟斤拷锟酵伙拷锟斤拷
extern	u8 	xdata RX1_Buffer[COM_RX1_Lenth];	//锟斤拷锟秸伙拷锟斤拷
#endif
#ifdef UART2
extern	COMx_Define	COM2;
extern	u8	xdata TX2_Buffer[COM_TX2_Lenth];	//锟斤拷锟酵伙拷锟斤拷
extern	u8 	xdata RX2_Buffer[COM_RX2_Lenth];	//锟斤拷锟秸伙拷锟斤拷
#endif
#ifdef UART3
extern	COMx_Define	COM3;
extern	u8	xdata TX3_Buffer[COM_TX3_Lenth];	//锟斤拷锟酵伙拷锟斤拷
extern	u8 	xdata RX3_Buffer[COM_RX3_Lenth];	//锟斤拷锟秸伙拷锟斤拷
#endif
#ifdef UART4
extern	COMx_Define	COM4;
extern	u8	xdata TX4_Buffer[COM_TX4_Lenth];	//锟斤拷锟酵伙拷锟斤拷
extern	u8 	xdata RX4_Buffer[COM_RX4_Lenth];	//锟斤拷锟秸伙拷锟斤拷
#endif

u8	UART_Configuration(u8 UARTx, COMx_InitDefine *COMx);
#ifdef UART1
void TX1_write2buff(u8 dat);	//锟斤拷锟斤拷1锟斤拷锟酵猴拷锟斤拷
void PrintString1(u8 *puts);
#endif
#ifdef UART2
void TX2_write2buff(u8 dat);	//锟斤拷锟斤拷2锟斤拷锟酵猴拷锟斤拷
void PrintString2(u8 *puts);
#endif
#ifdef UART3
void TX3_write2buff(u8 dat);	//锟斤拷锟斤拷3锟斤拷锟酵猴拷锟斤拷
void PrintString3(u8 *puts);
#endif
#ifdef UART4
void TX4_write2buff(u8 dat);	//锟斤拷锟斤拷4锟斤拷锟酵猴拷锟斤拷
void PrintString4(u8 *puts);
#endif

//void COMx_write2buff(u8 UARTx, u8 dat);	//锟斤拷锟节凤拷锟酵猴拷锟斤拷
//void PrintString(u8 UARTx, u8 *puts);

#endif

