/*
;*****************************************************************************************************************
;                                                         eGON
;                                         the Embedded GO-ON Bootloader System
;
;                             Copyright(C), 2006-2008, SoftWinners Microelectronic Co., Ltd.
;											       All Rights Reserved
;
; File Name : asm_misc.s
;
; Author : Gary.Wang
;
; Version : 1.1.0
;
; Date : 2009.09.14
;
; Description :
;
; Others : None at present.
;
;
; History :
;
;  <Author>        <time>       <version>      <description>
;
; Gary.Wang       2009.09.14      1.1.0        build the file
;
;******************************************************************************************************************
*/


#include "arm_a8.h"



/*
;*******************************************************************************
;��������: open_sys_int
;����ԭ��: void open_sys_int( void )
;��������: open system irq interrupt
;��ڲ���: void
;�� �� ֵ: void
;��    ע:
;*******************************************************************************
*/
	.globl open_sys_int
open_sys_int:
	mrs r0, cpsr
	bic r0, r0, #( ARMV7_IRQ_MASK | ARMV7_FIQ_MASK )
	msr cpsr_c, r0
	mov pc, lr


/*
;*******************************************************************************
;��������: close_sys_int
;����ԭ��: void close_sys_int( void )
;��������: close system irq interrupt
;��ڲ���: void
;�� �� ֵ: void
;��    ע:
;*******************************************************************************
*/
	.globl close_sys_int
close_sys_int:
	mrs r0, cpsr
	orr r0, r0, #( ARMV7_IRQ_MASK | ARMV7_FIQ_MASK )
	msr cpsr_c, r0
	mov pc, lr

