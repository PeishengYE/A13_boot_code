/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    :
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
#include "common_res.h"
#include "axp_i.h"

#define   BAT_VOL_ACTIVE     (10)

int    power_ops_int_status = 0;
__u32  pic_layer_para;

static int show_battery_full(int *status);
static int shut_battery_full(void);
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
__s32 check_file_to_fel(char *name)
{
    H_FILE  hfile = NULL;

    hfile = wBoot_fopen(name, "r");
    if(hfile)
    {
        wBoot_fclose(hfile);
        wBoot_rmfile(name);
        hfile = NULL;
    	hfile = wBoot_fopen(name, "r");
    	if(!hfile)
    	{
    		__inf("dest file is not exist\n");
    	}
    	else
    	{
    		wBoot_fclose(hfile);
    	}

    	return 0;
    }

    return -1;
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
*    ˵��    �������������ɣ�ֱ�ӹػ�
*
*
************************************************************************************************************
*/
__s32 check_natch_time(char *file_name, __u32 work_mode)
{
    H_FILE  hfile = NULL;
    __s32   ret = -1;

    hfile = wBoot_fopen(file_name, "r");
    if(hfile)
    {
        if(work_mode == WORK_MODE_PRODUCT)
        {
            ret = 0;
        }
        wBoot_fclose(hfile);
        wBoot_rmfile(file_name);
    }

    return ret;
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
*    ����ֵ  �� 0:��������
*			   -1:�ػ�
*
*    ˵��    �������������ɣ�ֱ�ӹػ�
*
*
************************************************************************************************************
*/
__s32 check_power_status(void)
{
	__s32 status;
	__s32 power_start;
    __u32 dcin, bat_exist;
	/* YEP inside: 
	 * power off the board whenever there is no AC charger connected 
	 * Disadvantage: failed to reboot by sw */
	  
	/*
		__inf("YEP inside: power off the board. \n");
		return -1;
		*/
	
	status = wBoot_power_get_level();
	//�͵�״̬�£����ⲿ��Դ��ֱ�ӹػ� 
	if(status == 1){
		__inf("battery low power with no dc or ac, should set power off\n");
		ShowPictureEx("c:\\os_show\\low_pwr.bmp", 0);
		wBoot_timer_delay(3000);
		return -1;
	}
	power_start = 0;
	//0: ��������ţֱ�ӿ���������ͨ���жϣ�����������������ֱ�ӿ���������power������ǰ����ϵͳ״̬�������ص������ͣ���������
	//1: ����״̬�£�������ţֱ�ӿ�����ͬʱҪ���ص����㹻��
	//2: ��������ţֱ�ӿ���������ͨ���жϣ�����������������ֱ�ӿ���������power������ǰ����ϵͳ״̬����Ҫ���ص���
	//3: ����״̬�£�������ţֱ�ӿ�������Ҫ���ص���
	wBoot_script_parser_fetch("target", "power_start", &power_start, 1);
	__inf("power_start=%x\n", (power_start<<8) | status);
	if(status == 3)					//�͵磬ͬʱ���ⲿ��Դ״̬��
	{
		if(!(power_start & 0x02))	//��Ҫ�жϵ�ǰ��ص�����Ҫ��power_start�ĵ�1bit��ֵΪ0
		{							//��������£�ֱ�ӹػ�
			__inf("battery low power with dc or ac, should charge longer\n");
			ShowPictureEx("c:\\os_show\\bempty.bmp", 0);
			wBoot_timer_delay(3000);

			return -1;
		}
		else
		{
			if(power_start == 3)	//����Ҫ�жϵ�ǰ��ص��������Ϊ3�������ϵͳ�����Ϊ0������к����ж�
			{
				return 0;
			}
		}
	}
	else							//status=2 ��ص����㹻�����
	{
		if(power_start & 0x01)		//�����0bit��ֵΪ1�������ϵͳ
		{
			return 0;
		}
	}								//��������£���������ж�

	status = -1;
	status = wBoot_power_check_startup();
	__inf("startup status = %d\n", status);
	if(status)
	{
	    return 0;
    }
	{
		
		__s32 bat_cal, this_bat_cal;
		__u32 bat_show_hd = NULL;
		int   i, j;
		int   bat_full_status = 0;
		int   key_status;

		//��ǰ����ȷ���ǻ�ţ�����������Ƿ񿪻�����ȷ������Ҫȷ�ϵ���Ƿ����
		//����ز����ڼ���������ش�����ػ�
//		power_int_reg();
//		usb_detect_enter();
		bat_show_hd = ShowBatteryCharge_init(0);

		wBoot_timer_delay(1500);
		dcin = 0;
		bat_exist = 0;
		wBoot_power_get_dcin_battery_exist(&dcin, &bat_exist);
		if(!bat_exist)
		{
			__inf("no battery exist\n");
			ShowBatteryCharge_exit(bat_show_hd);
//			power_int_rel();
//			usb_detect_exit();

			return 0;
		}
		wlibc_int_disable();
		this_bat_cal = wBoot_power_get_cal();
		wlibc_int_enable();
		__inf("base bat_cal = %d\n", this_bat_cal);
		if(this_bat_cal == 100)
		{
			ShowBatteryCharge_exit(bat_show_hd);
			bat_show_hd = NULL;
			show_battery_full(&bat_full_status);
			for(i =0;i<12;i++)
			{
				key_status = wBoot_power_get_key();
				if(key_status > 0)
				{
					if(key_status & 0x02)	//�̰�
					{
						i = 0;
					}
					else if(key_status & 0x01) //����
					{
						ShowBatteryCharge_exit(bat_show_hd);
		                 __inf("Battery full and Long Pressing, Go to system now...\n");
						return 0;
					}
				}
//				if(power_ops_int_status & 0x02)	//�̰�
//				{
//					power_ops_int_status &= ~0x02;
//					j = 0;
//					__inf("short key\n");
//				}
//				else if(power_ops_int_status & 0x01)	//����
//				{
//					ShowBatteryCharge_exit(bat_show_hd);
//					wlibc_int_disable();
//					power_int_rel();
//					usb_detect_exit();
//					power_ops_int_status &= ~0x01;
//					wlibc_int_enable();
//					power_int_reg();
//					__inf("long key\n");
//
//					return 0;
//				}
				wBoot_timer_delay(250);
			}
		} else {
			int one_delay;

			one_delay = 1000/(10 - (this_bat_cal/10));
			for(j=0;j<3;j++)
			{
				for(i=this_bat_cal;i<110;i+=10)
				{
					ShowBatteryCharge_rate(bat_show_hd, i);
					key_status = wBoot_power_get_key();
					if(key_status > 0)
					{
						if(key_status & 0x02)	//�̰�
						{
							j = 0;
						}
						else if(key_status & 0x01) //����
						{
							ShowBatteryCharge_exit(bat_show_hd);

		                     __inf("Battery not full and Long Pressing, Go to system now...\n");
							return 0;
						}
					}
					wBoot_timer_delay(one_delay);
//					if(power_ops_int_status & 0x02)	//�̰�
//					{
//						power_ops_int_status &= ~0x02;
//						j = 0;
//						__inf("short key\n");
//					}
//					else if(power_ops_int_status & 0x01)	//����
//					{
//						ShowBatteryCharge_exit(bat_show_hd);
//						wlibc_int_disable();
//						power_int_rel();
//						usb_detect_exit();
//						power_ops_int_status &= ~0x01;
//						wlibc_int_enable();
//						power_int_reg();
//						__inf("long key\n");
//
//						return 0;
//					}
				}
			}
			ShowBatteryCharge_rate(bat_show_hd, this_bat_cal);
			wBoot_timer_delay(1000);
		}
		wBoot_power_get_key();
		__inf("YEP inside: External power connected, Still Power off\n");
		return -1;
/******************************************************************
*
*	standby ����ֵ˵��
*
*	   -1: ����standbyʧ��
*		1: ��ͨ��������
*		2: ��Դ�����̰�����
*		3: ��Դ������������
*		4: �ⲿ��Դ�Ƴ�����
*		5: ��س�����
*		6: �ڻ���״̬���ⲿ��Դ���Ƴ�
*		7: �ڻ���״̬�³�����
*
******************************************************************/
		do
		{
			__inf("enter standby\n");
//			if(power_ops_int_status & 0x04)
//			{
//				status = 8;
//				power_ops_int_status &= ~0x04;
//			}
//			else
			{
//				wlibc_int_disable();
//				power_int_rel();
//				usb_detect_exit();
//				wlibc_int_enable();
				De_CloseLayer(board_res.layer_hd);
				status = wBoot_standby();
				__inf("exit standby by %d\n", status);

//				wlibc_int_disable();
				bat_cal = wBoot_power_get_cal();
//				wlibc_int_enable();
				__inf("current bat_cal = %d\n", bat_cal);
				if(bat_cal > this_bat_cal)
				{
					this_bat_cal = bat_cal;
				}
			}
			switch(status)
			{
				case 2:		//�̰�power�������»��� short press
				{
					//power_int_reg();
					De_OpenLayer(board_res.layer_hd);

					if(this_bat_cal == 100){
						if(bat_show_hd)
						{
							ShowBatteryCharge_exit(bat_show_hd);
							bat_show_hd = NULL;
						}
						show_battery_full(&bat_full_status);

						for(i =0;i<12;i++){

							key_status = wBoot_power_get_key();

							if(key_status > 0){
								if(key_status & 0x02)	//�̰�
								{
									j = 0;
								}
								else if(key_status & 0x01) //����
								{
									ShowBatteryCharge_exit(bat_show_hd);

									return 0;
								}
							}
//							if(power_ops_int_status & 0x02)	//�̰�
//							{
//								power_ops_int_status &= ~0x02;
//								j = 0;
//								__inf("short key\n");
//							}
//							else if(power_ops_int_status & 0x01)	//����
//							{
//								ShowBatteryCharge_exit(bat_show_hd);
//								wlibc_int_disable();
//								power_int_rel();
//								usb_detect_exit();
//								power_ops_int_status &= ~0x01;
//								wlibc_int_enable();
//								power_int_reg();
//								__inf("long key\n");
//
//								return 0;
//							}
							wBoot_timer_delay(250);
						}

					} else {
						int one_delay;

						one_delay = 1000/(10 - (this_bat_cal/10));
						for(j=0;j<3;j++)
						{
							for(i=this_bat_cal;i<110;i+=10)
							{
								ShowBatteryCharge_rate(bat_show_hd, i);
								key_status = wBoot_power_get_key();
								if(key_status > 0)
								{
									if(key_status & 0x02)	//�̰�
									{
										j = 0;
									}
									else if(key_status & 0x01) //����
									{
										ShowBatteryCharge_exit(bat_show_hd);

										return 0;
									}
								}
								wBoot_timer_delay(one_delay);
//								if(power_ops_int_status & 0x02)	//�̰�
//								{
//									power_ops_int_status &= ~0x02;
//									j = 0;
//									__inf("short key\n");
//								}
//								else if(power_ops_int_status & 0x01)	//����
//								{
//									ShowBatteryCharge_exit(bat_show_hd);
//									wlibc_int_disable();
//									power_int_rel();
//									usb_detect_exit();
//									power_ops_int_status &= ~0x01;
//									wlibc_int_enable();
//									power_int_reg();
//									__inf("long key\n");
//
//									return 0;
//								}
							}
						}
						ShowBatteryCharge_rate(bat_show_hd, this_bat_cal);
						wBoot_timer_delay(1000);
					}
				}/* end of case 2  short press */
				break;

				case 3:		//������Դ����֮�󣬹رյ��ͼ�꣬����ϵͳ
				{
					ShowBatteryCharge_exit(bat_show_hd);
//					power_int_reg();

					return 0;
				}

				case 4:		//���Ƴ��ⲿ��Դʱ��������ʾ��ǰ���ͼ���3���ػ�
				case 5:		//����س����ɵ�ʱ����Ҫ�ػ�
					De_OpenLayer(board_res.layer_hd);
					ShowBatteryCharge_rate(bat_show_hd, this_bat_cal);
				case 6:
				case 7:
				{
//					power_int_reg();
					if((status != 4) && (status != 5))
					{
						De_OpenLayer(board_res.layer_hd);
						ShowBatteryCharge_rate(bat_show_hd, this_bat_cal);
					}
					wBoot_timer_delay(500);
					if(bat_show_hd)
					{
						ShowBatteryCharge_degrade(bat_show_hd, 10);
						ShowBatteryCharge_exit(bat_show_hd);
					}
					else
					{
						shut_battery_full();
					}

					return -1;
				}
				case 8:		//standby�����м�⵽vbus����
				{
//					usb_detect_enter();
//					wBoot_timer_delay(600);
//					usb_detect_exit();
				}
				break;
				case 9:		//standby�����м�⵽vbus�Ƴ���ͬʱ������ͨdc
				{
//					power_set_usbpc();
				}
				break;

				default:
					break;
			}
		}
		while(1);
	}
}

static int show_battery_full(int *status)
{
	if(!*status)
	{
		*status = 1;
		pic_layer_para = ShowPictureEx("c:\\os_show\\bat10.bmp", 0);
	}
	else
	{
		De_OpenLayer(board_res.layer_hd);
	}

	return 0;
}

static int shut_battery_full(void)
{
	int  alpha_step, i;
	int  aplha, delay_time;
	display_layer_info_t *layer_para;

	if(!pic_layer_para)
    {
        return -1;
    }
    layer_para = (display_layer_info_t *)pic_layer_para;
	alpha_step = 5;
	delay_time = 50;
	aplha = layer_para->alpha_val;

	for(i=0xff;i>0;i -= alpha_step)
	{
		layer_para->alpha_en = 1;
		aplha -= alpha_step;
		if(aplha > 0)
		{
			De_SetLayerPara(board_res.layer_hd, layer_para);
			wBoot_timer_delay(delay_time);
			layer_para->alpha_val = aplha;
		}
		else
		{
			break;
		}
	}

	return 0;
}

