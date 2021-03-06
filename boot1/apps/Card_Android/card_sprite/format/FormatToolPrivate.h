/*
****************************************************************************************************
*											PC FS Format Tool
*										Just for FAT file System
*
*						        (c) Copyright 2009-2010, Sunny China
*											All	Rights Reserved
*
* File   	: FormatToolPrivate.h
* Version	: V1.0
* By     	: Sunny
* Date		: 2009-11-6 14:58:20
****************************************************************************************************
*/

#ifndef		__FORMAT_TOOL_PRIVATE_H__
#define 	__FORMAT_TOOL_PRIVATE_H__

#include    "card_sprite_i.h"
/* debug control */
#define		FORMAT_TOOL_DEBUG

#ifdef		FORMAT_TOOL_DEBUG
#define 	format_log_info			__inf
#define		Msg						__wrn
#define		Err						__wrn
#else
#define 	format_log_info
#define		Msg
#define		Err
#endif		/* FORMAT_TOOL_DEBUG */

#define 	FSYS_FAT_SEC_SIZE 		512
#define 	FSYS_FAT_SEC_BITS 		9

/* BIOS parameter block (FAT12/FAT16) */
typedef struct __FSYS_FAT_BPB
{
    __u32               TotSec32;         /* RSVD + FAT + ROOT + FATA (>=64k) */
    __u32               FATSz32;          /* number of FAT sectors          */
    __u32               RootClus;         /* root dir clus for FAT32        */
    __u16               BytesPerSec;      /* _512_,1024,2048,4096           */
    __u16               RootEntCnt;       /* number of root dir entries     */
    __u16               TotSec16;         /* RSVD + FAT + ROOT + FATA (<64k) */
    __u16               FATSz16;          /* number of FAT sectors          */
    __u16               ExtFlags;         /* mirroring info                 */
    __u16               FSInfo;           /* position of FSInfo structure   */
    __u16               Signature;        /* 0xAA55 Signature               */
    __u16               RsvdSecCnt;       /* 1 for FAT12 & FAT16            */
    unsigned char       SecPerClus;       /* sec in allocation unit         */
    unsigned char       NumFATs;          /* 2                              */
} __fsys_fat_BPB_t;

/*
**********************************************************************************************************************
*             Local data types
**********************************************************************************************************************
*/

typedef struct
{
    __u32   SecNum;
    __u32   Num;
} _FS_FAT_ROOTENTCNT ;


typedef struct
{
    __u32 SecNum;
    __u16 Num;
} _FS_FAT_SECPERCLUST;


/*
**********************************************************************************************************************
*             Local Variables
**********************************************************************************************************************
*/

/* table for getting number of root entries for a given media size */

static const _FS_FAT_ROOTENTCNT _FS_auto_rootcnt[] =
{
    {         0x100,     0x40 },
    {         0x200,     0x80 },
    {      0x0b40UL,     0xe0 },
    {  0x0001f3c9UL,    0x100 },
    {  0xffffffffUL,    0x200 }
};

/* table for calculating cluster size */
static const _FS_FAT_SECPERCLUST _FS_auto_secperclust[] =
{
    /* medias up to 512MB are formatted with FAT16 */
    {     0x0b40UL, 0x0001 },
    {      32680UL, 0x0002 },
    {     262144UL, 0x0004 },
    {     524288UL, 0x0008 },
    {    1048576UL, 0x0010 },
    /* medias bigger than 512MB are formatted with FAT32 */
    {   16777216UL, 0x0008 },
    {   33554432UL, 0x0010 },
    {   67108864UL, 0x0020 },
    { 0xffffffffUL, 0x0040 }
};

#endif		/* __FORMAT_TOOL_PRIVATE_H__ */
