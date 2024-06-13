/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCAI.com ---------------------------------------------*/
/* --- BBS: www.STCAIMCU.com  -----------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 閿熸枻鎷烽敓鎻亷鎷疯瘮閿熸枻鎷烽敓鏂ゆ嫹閿熺粸鐧告嫹涔堣垳閿熸枻鎷烽敓锟�,閿熸枻鎷烽敓鑺傜鎷烽敓鏂ゆ嫹閿熸枻鎷锋敞閿熸枻鎷蜂娇閿熸枻鎷烽敓鏂ゆ嫹STC閿熸枻鎷烽敓鏂ゆ嫹閿熻緝纭锋嫹閿熸枻鎷烽敓鏂ゆ嫹            */
/*---------------------------------------------------------------------*/

#include	"GPIO.h"

//========================================================================
// 閿熸枻鎷烽敓鏂ゆ嫹: u8	GPIO_Inilize(u8 GPIO, GPIO_InitTypeDef *GPIOx)
// 閿熸枻鎷烽敓鏂ゆ嫹: 閿熸枻鎷峰閿熸枻鎷稩O閿熸枻鎷�.
// 閿熸枻鎷烽敓鏂ゆ嫹: GPIOx: 閿熺粨鏋勯敓鏂ゆ嫹閿熸枻鎷�,閿熸枻鎷锋叞閿熺但imer.h閿熸枻鎷蜂憨閿熸枻鎷烽敓锟�.
// 閿熸枻鎷烽敓鏂ゆ嫹: 閿熺即鐧告嫹閿熸枻鎷烽敓鏂ゆ嫹 SUCCESS, 閿熸枻鎷烽敓瑗熻繑浼欐嫹 FAIL.
// 閿熻姤鏈�: V1.0, 2012-10-22
//========================================================================
u8	GPIO_Inilize(u8 GPIO, GPIO_InitTypeDef *GPIOx)
{
    if(GPIO > GPIO_P7)				return FAIL;	//閿熸枻鎷烽敓鏂ゆ嫹
    if(GPIOx->Mode > GPIO_OUT_PP)	return FAIL;	//閿熸枻鎷烽敓鏂ゆ嫹
    if(GPIO == GPIO_P0)
    {
        if(GPIOx->Mode == GPIO_PullUp)		P0M1 &= ~GPIOx->Pin,	P0M0 &= ~GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹鍑嗗弻閿熸枻鎷烽敓锟�
        if(GPIOx->Mode == GPIO_HighZ)			P0M1 |=  GPIOx->Pin,	P0M0 &= ~GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹
        if(GPIOx->Mode == GPIO_OUT_OD)		P0M1 |=  GPIOx->Pin,	P0M0 |=  GPIOx->Pin;	 //閿熸枻鎷锋紡閿熸枻鎷烽敓锟�
        if(GPIOx->Mode == GPIO_OUT_PP)		P0M1 &= ~GPIOx->Pin,	P0M0 |=  GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓锟�
    }
    if(GPIO == GPIO_P1)
    {
        if(GPIOx->Mode == GPIO_PullUp)		P1M1 &= ~GPIOx->Pin,	P1M0 &= ~GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹鍑嗗弻閿熸枻鎷烽敓锟�
        if(GPIOx->Mode == GPIO_HighZ)			P1M1 |=  GPIOx->Pin,	P1M0 &= ~GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹
        if(GPIOx->Mode == GPIO_OUT_OD)		P1M1 |=  GPIOx->Pin,	P1M0 |=  GPIOx->Pin;	 //閿熸枻鎷锋紡閿熸枻鎷烽敓锟�
        if(GPIOx->Mode == GPIO_OUT_PP)		P1M1 &= ~GPIOx->Pin,	P1M0 |=  GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓锟�
    }
    if(GPIO == GPIO_P2)
    {
        if(GPIOx->Mode == GPIO_PullUp)		P2M1 &= ~GPIOx->Pin,	P2M0 &= ~GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹鍑嗗弻閿熸枻鎷烽敓锟�
        if(GPIOx->Mode == GPIO_HighZ)			P2M1 |=  GPIOx->Pin,	P2M0 &= ~GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹
        if(GPIOx->Mode == GPIO_OUT_OD)		P2M1 |=  GPIOx->Pin,	P2M0 |=  GPIOx->Pin;	 //閿熸枻鎷锋紡閿熸枻鎷烽敓锟�
        if(GPIOx->Mode == GPIO_OUT_PP)		P2M1 &= ~GPIOx->Pin,	P2M0 |=  GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓锟�
    }
    if(GPIO == GPIO_P3)
    {
        if(GPIOx->Mode == GPIO_PullUp)		P3M1 &= ~GPIOx->Pin,	P3M0 &= ~GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹鍑嗗弻閿熸枻鎷烽敓锟�
        if(GPIOx->Mode == GPIO_HighZ)			P3M1 |=  GPIOx->Pin,	P3M0 &= ~GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹
        if(GPIOx->Mode == GPIO_OUT_OD)		P3M1 |=  GPIOx->Pin,	P3M0 |=  GPIOx->Pin;	 //閿熸枻鎷锋紡閿熸枻鎷烽敓锟�
        if(GPIOx->Mode == GPIO_OUT_PP)		P3M1 &= ~GPIOx->Pin,	P3M0 |=  GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓锟�
    }
    if(GPIO == GPIO_P4)
    {
        if(GPIOx->Mode == GPIO_PullUp)		P4M1 &= ~GPIOx->Pin,	P4M0 &= ~GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹鍑嗗弻閿熸枻鎷烽敓锟�
        if(GPIOx->Mode == GPIO_HighZ)			P4M1 |=  GPIOx->Pin,	P4M0 &= ~GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹
        if(GPIOx->Mode == GPIO_OUT_OD)		P4M1 |=  GPIOx->Pin,	P4M0 |=  GPIOx->Pin;	 //閿熸枻鎷锋紡閿熸枻鎷烽敓锟�
        if(GPIOx->Mode == GPIO_OUT_PP)		P4M1 &= ~GPIOx->Pin,	P4M0 |=  GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓锟�
    }
    if(GPIO == GPIO_P5)
    {
        if(GPIOx->Mode == GPIO_PullUp)		P5M1 &= ~GPIOx->Pin,	P5M0 &= ~GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹鍑嗗弻閿熸枻鎷烽敓锟�
        if(GPIOx->Mode == GPIO_HighZ)			P5M1 |=  GPIOx->Pin,	P5M0 &= ~GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹
        if(GPIOx->Mode == GPIO_OUT_OD)		P5M1 |=  GPIOx->Pin,	P5M0 |=  GPIOx->Pin;	 //閿熸枻鎷锋紡閿熸枻鎷烽敓锟�
        if(GPIOx->Mode == GPIO_OUT_PP)		P5M1 &= ~GPIOx->Pin,	P5M0 |=  GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓锟�
    }
    if(GPIO == GPIO_P6)
    {
        if(GPIOx->Mode == GPIO_PullUp)		P6M1 &= ~GPIOx->Pin,	P6M0 &= ~GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹鍑嗗弻閿熸枻鎷烽敓锟�
        if(GPIOx->Mode == GPIO_HighZ)			P6M1 |=  GPIOx->Pin,	P6M0 &= ~GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹
        if(GPIOx->Mode == GPIO_OUT_OD)		P6M1 |=  GPIOx->Pin,	P6M0 |=  GPIOx->Pin;	 //閿熸枻鎷锋紡閿熸枻鎷烽敓锟�
        if(GPIOx->Mode == GPIO_OUT_PP)		P6M1 &= ~GPIOx->Pin,	P6M0 |=  GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓锟�
    }
    if(GPIO == GPIO_P7)
    {
        if(GPIOx->Mode == GPIO_PullUp)		P7M1 &= ~GPIOx->Pin,	P7M0 &= ~GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹鍑嗗弻閿熸枻鎷烽敓锟�
        if(GPIOx->Mode == GPIO_HighZ)			P7M1 |=  GPIOx->Pin,	P7M0 &= ~GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹
        if(GPIOx->Mode == GPIO_OUT_OD)		P7M1 |=  GPIOx->Pin,	P7M0 |=  GPIOx->Pin;	 //閿熸枻鎷锋紡閿熸枻鎷烽敓锟�
        if(GPIOx->Mode == GPIO_OUT_PP)		P7M1 &= ~GPIOx->Pin,	P7M0 |=  GPIOx->Pin;	 //閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓锟�
    }
    return SUCCESS;	//閿熺即鐧告嫹
}
