#! /bin/bash

#===============================================================
#   Copyright (C) 2012 AllWinner Technology. All Rights Reserved.
#   
#   file name£ºhelo.c
#   author£ºMartin Zheng <zhengjiewen@allwinnertech.com>
#   date£º2012Äê11ÔÂ12ÈÕ
#   brief£ºan auto build scripte to build boot1 
#
#   version£º1.0.0
#================================================================
set -e 

CUR_DIR=$PWD
BOOT1_ROOT=$CUR_DIR/boot1
TEMP_WORKSPACE=$CUR_DIR/workspace
BOOT1_CORE_OUT_DIR=$CUR_DIR/../tools/pack/chips/sun4i/eGon/storage_media
BOOT1_BOOTFS_OUT_DIR=$CUR_DIR/../tools/pack/chips/sun4i/wboot/bootfs

if [ ! -e $BOOT1_CORE_OUT_DIR ];then
BOOT1_CORE_OUT_DIR=$CUR_DIR/../tools/pack/chips/sun5i/eGon/storage_media
BOOT1_BOOTFS_OUT_DIR=$CUR_DIR/../tools/pack/chips/sun5i/wboot/bootfs
fi

cd $BOOT1_ROOT/misc

#make eGon2_libc.lib
make

cd $BOOT1_ROOT/lib_src/ui

#make eGon2_ui.lib
make

cd $BOOT1_ROOT/driver/drv_de

#make display driver
make

cd $BOOT1_ROOT/core

#make boot1_nand.bin
make -f make_nand

#make boot1_sdmmc.bin
make -f make_sdmmc

cd $BOOT1_ROOT/apps/Boot_Android

#make boot.axf
make

cd $BOOT1_ROOT/apps/Card_Android

#make sprite.axf
make

echo "copy the boot1_nand.bin "
cp -rf $TEMP_WORKSPACE/egon/boot1_nand.bin $BOOT1_CORE_OUT_DIR/nand/boot1.bin

echo "copy the boot1_sdmmc.bin "
cp -rf $TEMP_WORKSPACE/egon/boot1_sdmmc.bin $BOOT1_CORE_OUT_DIR/sdcard/boot1.bin

echo "copy the boot1 apps and drivers to the bootfs"
cp -rf $TEMP_WORKSPACE/wboot/bootfs/* $BOOT1_BOOTFS_OUT_DIR

echo -e '\033[0;32;1m'
echo "############################################"
echo "#             build boot done!             #"
echo "############################################"
echo -e '\033[0m'
