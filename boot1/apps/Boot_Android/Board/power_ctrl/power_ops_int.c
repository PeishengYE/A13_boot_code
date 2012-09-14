/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON driver power sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : drv_power.c
*
* By      : Jerry
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
*/
#include "include.h"
#include  "axp_i.h"


static  __u8  power_int_value[5];
static  __u32 power_int_working = 0;

extern  __u32  tmr_hd;
extern  int    usb_working;
extern  int    power_ops_int_status;

__s32 BOOT_TWI_Read(__u32 arg1, __u8 *arg2, __u8 *arg3)
{
    _twi_arg_t  twi_data;

    twi_data.slave_addr = arg1;
    twi_data.slave_addr_flag = 0;
    twi_data.byte_addr  = arg2;
    twi_data.byte_addr_width = 1;
    twi_data.byte_count = 1;
    twi_data.data = arg3;
    twi_data.if_restart = 1;

    return wBoot_twi_read((void *)&twi_data);
}

__s32  BOOT_TWI_Write(__u32 arg1, __u8 *arg2, __u8 *arg3)
{
    _twi_arg_t  twi_data;

    twi_data.slave_addr = arg1;
    twi_data.slave_addr_flag = 0;
    twi_data.byte_addr  = arg2;
    twi_data.byte_addr_width = 1;
    twi_data.byte_count = 1;
    twi_data.data = arg3;

    return wBoot_twi_write((void *)&twi_data);
}
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
__s32 boot_power_int_enable(void)
{
    __u8  reg_addr;
    __u8  int_enable[5];
    int	  i;

    for(i=0;i<5;i++)
    {
    	reg_addr = BOOT_POWER20_INTEN1 + i;
    	if(BOOT_TWI_Read(AXP20_ADDR, &reg_addr, power_int_value + i))
    	{
    		__inf("twi read err\n");
        	return -1;
    	}
    }

	int_enable[0] = 0x0C;	//������VBUS�Ƴ���VBUS����
	int_enable[1] = 0;		//
	int_enable[2] = 0x03;	//��������Դ�����̰�������
	int_enable[3] = 0;
	int_enable[4] = 0;

	for(i=0;i<5;i++)
	{
		reg_addr = BOOT_POWER20_INTEN1 + i;
    	if(BOOT_TWI_Write(AXP20_ADDR, &reg_addr, int_enable + i))
    	{
    		__inf("twi write err\n");
        	return -1;
    	}
    }

	return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
__s32 boot_power_int_disable(void)
{
    __u8  reg_addr;
    int   i;

	for(i=0;i<5;i++)
	{
		reg_addr = BOOT_POWER20_INTEN1 + i;
    	if(BOOT_TWI_Write(AXP20_ADDR, &reg_addr, power_int_value + i))
    	{
    		__inf("twi write err\n");
        	return -1;
    	}
    }

	return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
__s32 eGon2_power_int_query(__u8 *int_status)
{
    __u8  reg_addr;
    int	  i;

	for(i=0;i<5;i++)
	{
		reg_addr = BOOT_POWER20_INTSTS1 + i;
    	if(BOOT_TWI_Read(AXP20_ADDR, &reg_addr, int_status + i))
    	{
    		__inf("twi read err\n");
        	return -1;
    	}
    	if(BOOT_TWI_Write(AXP20_ADDR, &reg_addr, int_status + i))
    	{
    		__inf("twi write err\n");
        	return -1;
    	}
    }
    INTC_REG_IRQ_PENDCLR0 |= 1;

	return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
void power_int_handler(void *p_arg)
{
	__u8 status[5];

	status[0] = 0;
	eGon2_power_int_query(status);
	if(status[0] & 0x04)		//vbus�Ƴ�
	{
		__inf("vbus remove\n");
		usb_working = 0;
		usb_detect_exit();
		power_set_usbpc();
	}
	else if(status[0] & 0x08)	//vbus���룬����usbȥ���
	{
		__inf("vbus insert\n");
		power_ops_int_status |= 0x04;
		usb_detect_enter();
	}
	if(status[2] & 0x02)		//�̰�
	{
		power_ops_int_status |= 0x02;
	}
	else if(status[2] & 0x01)	//����
	{
		power_ops_int_status |= 0x01;
	}
}
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
void power_int_reg(void)
{
	__inf("power start detect\n");
	if(!power_int_working)
	{
		__inf("power enter detect\n");
		power_ops_int_status = 0;
		power_int_working = 1;
		boot_power_int_enable();
		wBoot_InsINT_Func(0, (int *)power_int_handler, 0);
		wBoot_EnableInt(0);
	}
}
/*
************************************************************************************************************
*
*                                             function
*
*    �������ƣ�
*
*    �����б�
*
*    ����ֵ  ��
*
*    ˵��    ��
*
*
************************************************************************************************************
*/
void power_int_rel(void)
{
	__inf("power exit detect\n");
	wBoot_DisableInt(0);
	boot_power_int_disable();
	power_int_working = 0;
	power_ops_int_status = 0;
}

