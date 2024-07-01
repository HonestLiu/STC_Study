/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCAI.com ---------------------------------------------*/
/* --- BBS: www.STCAIMCU.com  -----------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
/*---------------------------------------------------------------------*/

#ifndef	__EXTI_H
#define	__EXTI_H

#include	"config.h"

//========================================================================
//                             外部中断设置
//========================================================================

#define 	INT0_Mode(n)		(n==0?(IT0 = 0):(IT0 = 1))	/* INT0中断模式  下降沿/上升,下降沿中断 */
#define 	INT1_Mode(n)		(n==0?(IT1 = 0):(IT1 = 1))	/* INT0中断模式  下降沿/上升,下降沿中断 */

//========================================================================
//                              定义声明
//========================================================================

sbit INT0 = P3^2;
sbit INT1 = P3^3;
sbit INT2 = P3^6;
sbit INT3 = P3^7;
sbit INT4 = P3^0;

#define	EXT_INT0			0	//外中断0
#define	EXT_INT1			1	//外中断1
#define	EXT_INT2			2	//外中断2
#define	EXT_INT3			3	//外中断3
#define	EXT_INT4			4	//外中断4

#define	EXT_MODE_RiseFall	0	//上升沿/下降沿中断 ; 上升下降沿全监听
#define	EXT_MODE_Fall			1	//下降沿中断 ； 只监听下降沿

typedef struct
{
	u8	EXTI_Mode;			//中断模式,  	EXT_MODE_RiseFall, EXT_MODE_Fall
} EXTI_InitTypeDef;

extern u8 WakeUpSource;

u8	Ext_Inilize(u8 EXT, EXTI_InitTypeDef *INTx);

#endif
