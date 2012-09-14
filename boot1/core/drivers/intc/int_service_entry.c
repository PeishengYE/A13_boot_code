/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name   : int_service_entry.c
*
* Author      : Gary.Wang
*
* Version     : 1.1.0
*
* Date        : 2009.04.06
*
* Description :
*
* Others      : None at present.
*
*
* History     :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2009.04.06       1.1.0        build the file
*
************************************************************************************************************************
*/
#include "include.h"

extern void  open_sys_int( void );
extern void  close_sys_int( void );
extern __s32 timer0_int_func(void *arg);
extern __s32 timer1_int_func(void *arg);

typedef __s32  (* __int_func )(void* /*p_arg*/);     /* isr function pointer                                         */

static __s32 esIRQHandler_default( void * pArg );

typedef struct
{
    void 	       *pArg;               //�жϺ�������
    __int_func      pIsr;               //�жϺ�����ַ

}__int_func_t;

__int_func_t  eGon2_IRQVectorTable[84] =
{
        { 0, esIRQHandler_default },       //index  0
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },       //index 9

        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },       //index 19

        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, timer0_int_func      },   	   //timer0 22
        { 0, timer1_int_func      },   	   //timer1 23
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },       //index 29

        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },       //index 39

        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },       //index 49

        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },      //index 59

        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },      //index 69

        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },      //index 79

        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default },
        { 0, esIRQHandler_default }		  //index 83
};




/*
*********************************************************************************************************
*										   IRQHandler_default
*
* Description: �����е�û�а�װ���������жϵ�Ĭ�ϴ�����, һ������˺�����ϵͳ�������κ����飬
*               �ȴ�watch dog reset system
* Arguments	 : void
* Returns	 : void
*********************************************************************************************************
*/
static __s32 esIRQHandler_default(void * pArg)
{
	eGon2_printf("int not support\n");
    while(1);
}

/*
*********************************************************************************************************
*										   eGon2_Int_Init
*
* Description: ���жϽ��г�ʼ���������ر������ж�ʹ�ܣ���mask����������ж�pengding
*
* Arguments	 : void
* Returns	 : void
*********************************************************************************************************
*/
void eGon2_Int_Init(void)
{
	//�ر������ж�ʹ��
	INTC_REG_ENABLE0 = 0;
	INTC_REG_ENABLE1 = 0;
	INTC_REG_ENABLE2 = 0;
	//�������ж�mask
	INTC_REG_MASK0 = 0;
	INTC_REG_MASK1 = 0;
	INTC_REG_MASK2 = 0;
	//��������ж�pengding
	INTC_REG_FIQ_PENDCLR0 = 0xffffffff;
	INTC_REG_FIQ_PENDCLR1 = 0xffffffff;
	INTC_REG_FIQ_PENDCLR2 = 0xffffffff;

	INTC_REG_IRQ_PENDCLR0 = 0xffffffff;
	INTC_REG_IRQ_PENDCLR1 = 0xffffffff;
	INTC_REG_IRQ_PENDCLR2 = 0xffffffff;

	return;
}
/*
*********************************************************************************************************
*										   eGon2_Int_Exit
*
* Description: �ж��˳�ʱ����еĲ����������ر������ж�ʹ�ܣ��ر�mask����������ж�pengding
*
* Arguments	 : void
* Returns	 : void
*********************************************************************************************************
*/
void eGon2_Int_Exit(void)
{
    //�ر�TIMER�ж�
    *(volatile unsigned int *)(0x01c20c00 + 0x00) = 0;
    *(volatile unsigned int *)(0x01c20c00 + 0x04) |= 0x043f;
    *(volatile unsigned int *)(0x01c20c00 + 0x10) = 0;
    *(volatile unsigned int *)(0x01c20c00 + 0x20) = 0;
    //�ر�DMA�ж�
    *(volatile unsigned int *)(0x01c02000 + 0x00) = 0;
    *(volatile unsigned int *)(0x01c02000 + 0x04) = 0xffffffff;

	//�ر������ж�ʹ��
	INTC_REG_ENABLE0 = 0;
	INTC_REG_ENABLE1 = 0;
	INTC_REG_ENABLE2 = 0;
	//�������ж�mask
	INTC_REG_MASK0 = 0;
	INTC_REG_MASK1 = 0;
	INTC_REG_MASK2 = 0;
	//��������ж�pengding
	INTC_REG_FIQ_PENDCLR0 = 0xffffffff;
	INTC_REG_FIQ_PENDCLR1 = 0xffffffff;
	INTC_REG_FIQ_PENDCLR2 = 0xffffffff;

	INTC_REG_IRQ_PENDCLR0 = 0xffffffff;
	INTC_REG_IRQ_PENDCLR1 = 0xffffffff;
	INTC_REG_IRQ_PENDCLR2 = 0xffffffff;

    return;
}
/*
*********************************************************************************************************
*										   eGon2_InsINT_Func
*
* Description:  Ϊ�ж�Դ���ö�Ӧ���жϷ������װ�ú���
*
* Arguments	 : irq_no     �жϺ�
*              finc_addr  �жϷ�����
* Returns	 : 0  ע��ɹ�
*              -1 ע��ʧ��
*********************************************************************************************************
*/
__s32 eGon2_InsINT_Func(__u32 irq_no, int *func_addr, void *p_arg)
{
    close_sys_int();
    if(irq_no < 80)
    {
        if(eGon2_IRQVectorTable[irq_no].pIsr == esIRQHandler_default)    //��û�н���ע��
        {
            eGon2_IRQVectorTable[irq_no].pIsr = (__int_func )func_addr;
            eGon2_IRQVectorTable[irq_no].pArg = p_arg;
            open_sys_int();

            return 0;
        }
    }
    open_sys_int();

    return -1;
}

/*
*********************************************************************************************************
*										   eGon2_UnsInt_Func
*
* Description:  Ϊ�ж�Դ���ö�Ӧ���жϷ������ж�غ���
*
* Arguments	 : irq_no     �жϺ�
*
* Returns	 : 0  ж�سɹ�
*              -1 ж��ʧ��
* Note       : û��ע������жϷ�����Ҳ���Խ���ж��?
*********************************************************************************************************
*/
__s32 eGon2_UnsInt_Func(__u32 irq_no)
{
    close_sys_int();
    if(irq_no < 80)
    {
        if(eGon2_IRQVectorTable[irq_no].pIsr != esIRQHandler_default)    //��û�н���ע��
        {
            eGon2_IRQVectorTable[irq_no].pIsr = esIRQHandler_default;
            eGon2_IRQVectorTable[irq_no].pArg = 0;
            open_sys_int();

            return 0;
        }
    }
    open_sys_int();

    return -1;
}
/*
*********************************************************************************************************
*										   EnableInt
*
* Description:  ʹ���ж�
*
* Arguments	 : irq_no     �жϺ�
*
* Returns	 :
*
*********************************************************************************************************
*/
__s32 eGon2_EnableInt(__u32 irq_no)
{
    if(irq_no < 32)
    {
        INTC_REG_ENABLE0    |=  (1 << irq_no);
        INTC_REG_MASK0 	    &= ~(1 << irq_no);
        if(irq_no == INTC_IRQNO_FIQ) /* must clear pending bit when enabled */
            INTC_REG_FIQ_PENDCLR0 = (1 << INTC_IRQNO_FIQ);
    }
    else if(irq_no < 64)
    {
    	irq_no              -= 32;
        INTC_REG_ENABLE1    |=  (1 << irq_no);
        INTC_REG_MASK1 	    &= ~(1 << irq_no);
    }
    else
    {
        irq_no               -= 64;
        INTC_REG_ENABLE2    |=  (1 << irq_no);
        INTC_REG_MASK2 	    &= ~(1 << irq_no);
    }

    return 0;
}

/*
*********************************************************************************************************
*										   DisableInt
*
* Description:  ��ֹ�ж�
*
* Arguments	 : irq_no     �жϺ�
*
* Returns	 :
*
*********************************************************************************************************
*/
__s32 eGon2_DisableInt(__u32 irq_no)
{
    if(irq_no < 32)
    {
        INTC_REG_ENABLE0    &= ~(1 << irq_no);
        INTC_REG_MASK0 	    |=  (1 << irq_no);
    }
    else if(irq_no < 64)
    {
    	irq_no -= 32;
        INTC_REG_ENABLE1    &= ~(1 << irq_no);
        INTC_REG_MASK1 	    |=  (1 << irq_no);
    }
    else
    {
        irq_no -= 64;
        INTC_REG_ENABLE2    &= ~(1 << irq_no);
        INTC_REG_MASK2 	    |=  (1 << irq_no);
    }

    return 0;
}


