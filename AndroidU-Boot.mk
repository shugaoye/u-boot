# Copyright (C) 2015 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#******************************************************************************
#
# AndroidU-Boot.mk - U-Boot makefile file of virtual device armemu
#
# Copyright (c) 2015 Roger Ye.  All rights reserved.
#
# This is part of the build for virtual device armemu.
# Android makefile to build U-Boot as a part of Android Build.
#
#******************************************************************************

TARGET_OUT_INTERMEDIATES ?= $(PRODUCT_OUT)/obj
U-BOOT_OUT := $(TARGET_OUT_INTERMEDIATES)/U-BOOT_OBJ
U-BOOT_CONFIG := $(U-BOOT_OUT)/include/config.mk
TARGET_PREBUILT_INT_U-BOOT := $(U-BOOT_OUT)/u-boot.bin
U-BOOT_IMG=$(U-BOOT_OUT)/u-boot.bin

TARGET_PREBUILT_U-BOOT := $(TARGET_PREBUILT_INT_U-BOOT)

$(U-BOOT_OUT):
	mkdir -p $(U-BOOT_OUT)

$(U-BOOT_CONFIG): $(U-BOOT_OUT)
	export BUILD_DIR=../$(U-BOOT_OUT); cd $(TARGET_U_BOOT_SOURCE); $(MAKE) $(TARGET_U_BOOT_CONFIG) arch=ARM CROSS_COMPILE=arm-none-eabi-

$(TARGET_PREBUILT_INT_U-BOOT): $(U-BOOT_OUT) $(U-BOOT_CONFIG)
	export BUILD_DIR=../$(U-BOOT_OUT); cd $(TARGET_U_BOOT_SOURCE); $(MAKE) all arch=ARM CROSS_COMPILE=arm-none-eabi-
#	mkimage -A arm -C none -O linux -T kernel -d $(PRODUCT_OUT)/obj/KERNEL_OBJ/arch/arm/boot/zImage -a 0x00010000 -e 0x00010000 $(PRODUCT_OUT)/system/zImage.uimg
#	mkimage -A arm -C none -O linux -T ramdisk -d $(PRODUCT_OUT)/ramdisk.img -a 0x00800000 -e 0x00800000 $(PRODUCT_OUT)/system/rootfs.uimg

