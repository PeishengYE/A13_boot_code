/*
*********************************************************************************************************
*											        eBase
*
*
*
*						        (c) Copyright 2006-2010, AW China
*											All	Rights Reserved
*
* File    	: 	egon_drv_common_inc.h
* Date	:	2010-09-25
* By      	: 	holigun
* Version 	: 	V1.00
*********************************************************************************************************
*/
#ifndef	_WBOOT_MBR_H_
#define	_WBOOT_MBR_H_

#define     DOWNLOAD_MAP_NAME   "dlinfo.fex"
/* MBR       */
#define     MBR_SIZE			1024
#define   	MBR_MAGIC			"softw311"
#define     MBR_START_ADDRESS	0x00000000
#define     MBR_MAX_PART_COUNT	15
#define     MBR_COPY_NUM        4    //mbr�ı�������
#define     MBR_RESERVED        (MBR_SIZE - 20 - (MBR_MAX_PART_COUNT * sizeof(PARTITION)))   //mbr�����Ŀռ�

//������Ϣ, 64byte
#pragma pack(push, 1)
typedef struct tag_PARTITION
{
	unsigned  int       addrhi;				//��ʼ��ַ, ������Ϊ��λ
	unsigned  int       addrlo;				//
	unsigned  int       lenhi;				//����
	unsigned  int       lenlo;				//
	unsigned  char      classname[12];		//���豸��
	unsigned  char      name[12];			//���豸��
	unsigned  int       user_type;          //�û�����
	unsigned  int       ro;                 //��д����
	unsigned  char      res[16];			//����
}PARTITION;
#pragma pack(pop)
//MBR��Ϣ
#pragma pack(push, 1)
typedef struct tag_MBR
{
	unsigned  int       crc32;				        // crc 1k - 4
	unsigned  int       version;			        // �汾��Ϣ�� 0x00000100
	unsigned  char 	    magic[8];			        //"softw311"
	unsigned  char 	    copy;				        //����
	unsigned  char 	    index;				        //�ڼ���MBR����
	unsigned  short     PartCount;			        //��������
	PARTITION           array[MBR_MAX_PART_COUNT];	//
	unsigned  char      res[MBR_RESERVED];
}MBR;
#pragma pack(pop)


typedef struct tag_one_part_info
{
	unsigned  char      classname[12];      //����д�����Ĵ��豸��
	unsigned  char      name[12];           //����д���������豸��
	unsigned  int       addrhi;             //����д�����ĸߵ�ַ��������λ
	unsigned  int       addrlo;             //����д�����ĵ͵�ַ��������λ
	unsigned  int       lenhi;				//����д�����ĳ��ȣ���32λ��������λ
	unsigned  int       lenlo;				//����д�����ĳ��ȣ���32λ��������λ
	unsigned  char      part_name[12];      //����д���������ƣ���MBR�еķ��� classname ��Ӧ
	unsigned  char      dl_filename[16];    //����д�������ļ����ƣ����ȹ̶�16�ֽ�
	unsigned  char      vf_filename[16];    //����д������У���ļ����ƣ����ȹ̶�16�ֽ�
	unsigned  int       encrypt;            //����д�����������Ƿ���м��� 0:����   1��������
}
dl_one_part_info;
//������д��Ϣ
typedef struct tag_download_info
{
	unsigned  int       crc32;				        		//crc
	unsigned  int       version;                            //�汾��  0x00000101
	unsigned  char 	    magic[8];			        		//"softw311"
	unsigned  int       download_count;             		//��Ҫ��д�ķ�������
	dl_one_part_info	one_part_info[MBR_MAX_PART_COUNT];	//��д��������Ϣ
}
download_info;


//��׼������Ϣ
#pragma pack(push, 1)
typedef struct tag_part_info_stand
{
	char 				indicator;			//��ʾ�÷����Ƿ��ǻ����
	char				start_head;			//������ʼ�Ĵ�ͷ
	short				start_sector:6;		//������ʼ������
	short				start_cylinder:10;	//������ʼ������
	char				part_type;			//��������			00H:û��ָ��  01H:DOS12	02H:xenix	04H:DOS16 05H:��չ����	06H:FAT16
											//					07H:NTFS	  0BH:FAT32
	char				end_head;			//���������Ĵ�ͷ
	short				end_sector:6;		//��������������
	short				end_cylinder:10;	//��������������
	//int					start_sectors;		//��ʼ������ǰ��˵��������Ӳ�̵ĸ��������������߼�����
	//int					total_sectors;		//�����е���������
	short				start_sectorl;
	short				start_sectorh;
	short				total_sectorsl;
	short				total_sectorsh;
}
part_info_stand;
#pragma pack(pop)
//��׼MBR
#pragma pack(push, 1)
typedef struct tag_mbr_stand
{
	char				mbr[0x89];			//��������¼
	char				err_info[0x135];	//������Ϣ
	part_info_stand		part_info[4];		//��������
	short				end_flag;			//�̶�ֵ 0x55aa
}
mbr_stand;
#pragma pack(pop)


//��̬���ݴ����ʽ
typedef struct tag_dynamic_data_form
{
	char  name[16];
	int   datasize;
	int   usedsize;
}
dynamic_data_form;

typedef struct tag_dynamic_boot_head
{
	char  magic[8];
	int   count;
	int   size;
}
dynamic_boot_head;

#endif	//_WBOOT_MBR_H_

/* end of _WBOOT_MBR_H_ */

