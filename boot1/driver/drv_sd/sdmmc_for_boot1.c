/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON mod demo sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : sdmmc_for_boot1.c
*
* By      : Neil
*
* Version : V1.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
*/
#include  "sdmmc_for_boot1.h"
#include  "sdcard_op.h"
#include  "osal/sdmmc_osal_boot1.h"

#define    SD_BLOCK_MAXLENGTH_ONE_TIME          (2048)

smc_idma_des 		   *pdes = NULL;
static   __u32  bootcard_offset = 0;
static   __s32  gpio_no, line_count;
/*
************************************************************************************************************
*
*                                             SDMMC_PhyInit
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
__s32 SDMMC_PhyInit(__u32 card_no, __u32 speed_mode, __u32 line_sel)
{
    __s32 ret;
    __u32 sysclock, sdcard_clock;

	line_count = line_sel;
	gpio_no  = (card_no >> 16) & 0xffff;
	card_no &= 0xffff;
    //�򿪿���AHBʱ��
    SDMMC_OpenAHBClock(card_no);
    //��ȡϵͳʱ��Ƶ��
    sysclock = SDMMC_GetCmuClk();
    //���ÿ�ʱ�ӣ����䵽corepll��
    sdcard_clock = SDMMC_OpenClock(50, sysclock, card_no);
    if(!sdcard_clock)
    {
        sdmmc_printf("unable set card clock\n");

        return -1;
    }
    //����GPIO
    SDMMC_SetPinOn(gpio_no, line_count);
    //���ÿ��ĳ�ʼ������
    ret = SDMMC_Init(card_no, sdcard_clock, speed_mode, line_sel);
    if(ret == -1)
    {
        sdmmc_printf("fail to init sdmmc card\n");

        return -1;
    }
    if(!pdes)
    {
    	pdes = (smc_idma_des *)wBoot_malloc(1024 * 1024);
	}

    return ret;
}
/*
************************************************************************************************************
*
*                                             SDMMC_LogicalInit
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
__s32 SDMMC_LogicalInit(__u32 card_no, __s32 card_offset, __u32 speed_mode, __u32 line_sel)
{
	bootcard_offset = card_offset;

    return SDMMC_PhyInit(card_no, speed_mode, line_sel);
}
/*
************************************************************************************************************
*
*                                             SDMMC_PhyExit
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
__s32 SDMMC_PhyExit(__u32 card_no)
{
	wBoot_free(pdes);
	pdes = NULL;
    //�رտ�
    SDMMC_Exit(card_no);
    //�ر�GPIO
    SDMMC_SetPinOff(gpio_no, line_count);
    //�رտ���ģ��ʱ��
    SDMMC_CloseClock(card_no);
    //�رտ���AHBʱ��
    SDMMC_CloseAHBClock(card_no);

    return 0;
}
/*
************************************************************************************************************
*
*                                             SDMMC_LogicalExit
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
__s32 SDMMC_LogicalExit(__u32 card_no)
{
	bootcard_offset = 0;

    return SDMMC_PhyExit(card_no);
}
/*
************************************************************************************************************
*
*                                             SDMMC_PhyRead
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
__s32 SDMMC_PhyRead(__u32 start_sector, __u32 nsector, void *buf, __u32 card_no)
{
    return SDMMC_Read(start_sector, nsector, buf, card_no);
}
/*
************************************************************************************************************
*
*                                             SDMMC_PhyRead
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
__s32 SDMMC_LogicalRead(__u32 start_sector, __u32 nsector, void *buf, __u32 card_no)
{
	__s32 ret;
	void  *tmp_buf;

	tmp_buf = buf;
	start_sector += bootcard_offset;
	while(nsector >= SD_BLOCK_MAXLENGTH_ONE_TIME)
	{
		nsector -= SD_BLOCK_MAXLENGTH_ONE_TIME;
		ret = SDMMC_PhyRead(start_sector, SD_BLOCK_MAXLENGTH_ONE_TIME, tmp_buf, card_no);
		start_sector += SD_BLOCK_MAXLENGTH_ONE_TIME;
		tmp_buf = (__s8 *)tmp_buf + (SD_BLOCK_MAXLENGTH_ONE_TIME<<9);
		if(ret)
		{
			return -1;
		}
	}

	if(nsector)
	{
		return  SDMMC_PhyRead(start_sector, nsector, tmp_buf, card_no);
	}

    return 0;
}
/*
************************************************************************************************************
*
*                                             SDMMC_PhyWrite
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
__s32 SDMMC_PhyWrite(__u32 start_sector, __u32 nsector, void *buf, __u32 card_no)
{
    return SDMMC_Write(start_sector, nsector, buf, card_no);
}
/*
************************************************************************************************************
*
*                                             SDMMC_LogicalWrite
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
__s32 SDMMC_LogicalWrite(__u32 start_sector, __u32 nsector, void *buf, __u32 card_no)
{
	__s32 ret;
	void  *tmp_buf;

	tmp_buf = buf;
	start_sector += bootcard_offset;
	while(nsector >= SD_BLOCK_MAXLENGTH_ONE_TIME)
	{
		nsector -= SD_BLOCK_MAXLENGTH_ONE_TIME;
		ret = SDMMC_PhyWrite(start_sector, SD_BLOCK_MAXLENGTH_ONE_TIME, buf, card_no);
		start_sector += SD_BLOCK_MAXLENGTH_ONE_TIME;
		tmp_buf = (__s8 *)tmp_buf + (SD_BLOCK_MAXLENGTH_ONE_TIME<<9);
		if(ret)
		{
			return -1;
		}
	}

	if(nsector)
	{
		return  SDMMC_PhyWrite(start_sector, nsector, tmp_buf, card_no);
	}

    return 0;
}


__s32 SDMMC_PhyDiskSize(void)
{
	return SDMMC_GetDiskSize();
}


__s32 SDMMC_LogicalDiskSize(void)
{
	return SDMMC_GetDiskSize() - bootcard_offset;
}
