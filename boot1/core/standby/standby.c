/*
**********************************************************************************************************************
*											        eGon
*						                     the Embedded System
*									       script parser sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : standby.c
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
#include "standby.h"


static int eGon2_enter_standby(void);
static int eGon2_exit_standby(void);
static int eGon2_standby_detect(void);
static int eGon2_mod_enter_standby(void);
static int eGon2_mod_exit_standby(void);

//static int standby_flag = 0;
static int status;
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
int eGon2_standby_mode(void)
{
	__u32 dcin_exist, battery_exist;
	__s32 key_status;

	//����Ƿ��а�������
	key_status = eGon2_power_get_key();
	if(key_status & 0x01)			//������������£����ܵ�Դ�Ƿ��Ƴ���ֱ�ӽ���ϵͳ
	{
		//�˳����е�����ģ��
//		if(standby_flag)
//		{
//			eGon2_mod_exit_standby();
//		}
		return 3;
	}
	//����ⲿ��Դ�Ƿ����
	dcin_exist = 100;
	battery_exist = 100;
	eGon2_power_get_dcin_battery_exist(&dcin_exist, &battery_exist);
	if(!dcin_exist)							//�ⲿ��Դ�Ƴ�
	{
		//�˳����е�����ģ��
//		if(standby_flag)
//		{
//			eGon2_mod_exit_standby();
//		}
		return 6;
	}
//	charge_status = eGon2_power_battery_charge_status();		//������
//	if((charge_status > 0) && (battery_exist == 1))
//	{
//		//�˳����е�����ģ��
//		if(standby_flag)
//		{
//			eGon2_mod_exit_standby();
//		}
//		return 7;
//	}
	if(key_status & 0x02)			//���ⲿ��Դ���ڣ�����standby
	{
		//�˳����е�����ģ��
//		if(standby_flag)
//		{
//			eGon2_mod_exit_standby();
//		}
		return 2;
	}
	status = -1;
	//����ԭ����SP
	eGon2_store_sp();
	//�����µ�SP�������SRAM��
	eGon2_set_sp();

//	if(!standby_flag)
	{
		eGon2_mod_enter_standby();
	}
	eGon2_enter_standby();
	//���ڣ�clock������32k��
	do
	{
		//��ʼѭ������Ƿ�ʼ����
		eGon2_halt();
		status = eGon2_standby_detect();
	}
	while(status <= 0);
	//������Ҫ���ѣ��˳�standby
	eGon2_exit_standby();
	//�˳�ģ���standby
	eGon2_mod_exit_standby();
//	if((status != 8) && (status != 9))
//	{
//		eGon2_mod_exit_standby();
//		standby_flag = 0;
//	}
//	else
//	{
//		standby_flag = 1;
//	}
	eGon2_restore_sp();

	return status;
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
#define   DCDC2_STANDBY_VOL      (1250)
#define   DCDC3_STANDBY_VOL		 (1250)

static int eGon2_enter_standby(void)
{
	int i;
//	__u8  power_int_status[5];
	//����standby������
	eGon2_config_charge_current(1);
	//����VBUS��粻����
//	eGon2_power_vbus_cur_limit();
//	//���power���ж�pending
//	eGon2_power_int_query(power_int_status);
	//�����ж�
	standby_int_init();
	//����clock
	standby_clock_store();
	//����dram��֮�������ٷ���dram
	dram_power_save_process();
	//��ֹdrampll���ʹ��
	standby_clock_drampll_ouput(0);
	//�л���24M
	standby_clock_to_source(24000000);
	//�ر�����pll���
	standby_clock_plldisable();
	//���͵�Դ��ѹ���
	eGon2_power_set_dcdc2(DCDC2_STANDBY_VOL);
//	eGon2_power_set_dcdc3(DCDC3_STANDBY_VOL);
	//ʹ�ܵ�Դ�жϣ��ȴ�����
	eGon2_power_int_enable();
	//�л���Ƶ��ȫΪ0
	standby_clock_divsetto0();
	//�л�apb1��32k
	standby_clock_apb1_to_source(32000);
	//�л�ʱ�ӣ��ر�ʱ��
	for(i=0;i<2000;i++);
	standby_clock_to_source(32000);
	//�ر�24M����
	standby_clock_24m_op(0);

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
static int eGon2_exit_standby(void)
{
	int i;

	//�رյ�Դ�ж�
	eGon2_power_int_disable();
	//���õ�ѹ
	eGon2_power_set_dcdc2(-1);
//	eGon2_power_set_dcdc3(-1);
	//��ԭ����pll��ԭ���򿪵���򿪣�ԭ���رյĲ�����
	standby_clock_restore();
	for(i=0;i<80000;i++);//0x100000
	//�л�ʱ�ӵ�PLL1
	standby_clock_to_source(0);
	//��dram���ʹ��
	standby_clock_drampll_ouput(1);
	//����dram
	standby_tmr_enable_watchdog();
	dram_power_up_process();
	standby_tmr_disable_watchdog();
	//��ԭ�ж�״̬
	standby_int_exit();
	//��ԭ������
	eGon2_config_charge_current(0);
	//��ԭ���е�����ģ��
	//eGon2_timer_delay(50);

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
static int eGon2_standby_detect(void)
{
	int   i;
	__u32 dcin_exist, battery_exist;
	__u8  power_int_status[5];

	//����24M����
	standby_clock_24m_op(1);
	//�л�apb1��24M
	standby_clock_apb1_to_source(24000000);
	//����Ƶ�ʵ�24M
	standby_clock_to_source(24000000);
	standby_clock_divsetback();
	for(i=0;i<2000;i++);
	//����жϴ���
	eGon2_power_int_query(power_int_status);
	//����жϿ�������pending
	standby_int_query();
//	if(eGon2_key_get_status() == 1)			//��ͨADC��������
//	{
//		return 1;
//	}
//	eGon2_power_vbus_unlimit();
	if(power_int_status[2] & 0x02)			//��Դ�����̰�
	{
		return 2;
	}
	if(power_int_status[2] & 0x01)			//��Դ��������
	{
		return 3;
	}
	dcin_exist = 100;
	battery_exist = 100;
	eGon2_power_get_dcin_battery_exist(&dcin_exist, &battery_exist);
	if(!dcin_exist)							//�ⲿ��Դ�Ƴ�
	{
		return 4;
	}
//	if((power_int_status[1] & 0x04) && (battery_exist==1))			//������
//	{
//		return 5;
//	}
//	if(power_int_status[0] & 0x08)			//usb��ţ����
//	{
//		return 8;
//	}
//	if(power_int_status[0] & 0x04)			//usb��ţ�Ƴ�
//	{
//		return 9;
//	}

	//��ԭ��32K
	standby_clock_divsetto0();
	standby_clock_apb1_to_source(32000);
	standby_clock_to_source(32000);
	standby_clock_24m_op(0);

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
static int eGon2_mod_enter_standby(void)
{
	int i;

	for(i=0;i<EMOD_COUNT_MAX;i++)
	{
		eGon2_driver_standby(i, EGON2_MOD_ENTER_STANDBY, 0);
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
static int eGon2_mod_exit_standby(void)
{
	int i;

	for(i=0;i<EMOD_COUNT_MAX;i++)
	{
		eGon2_driver_standby(i, EGON2_MOD_EXIT_STANDBY, 0);
	}

	return 0;
}




