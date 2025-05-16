################################################################################
# \file common.mk
# \version 1.0
#
# \brief
# Settings shared across all projects.
#
################################################################################
# \copyright
# Copyright 2023-2025, Cypress Semiconductor Corporation (an Infineon company)
# SPDX-License-Identifier: Apache-2.0
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
################################################################################

MTB_TYPE=PROJECT

# Target board/hardware (BSP).
# To change the target, it is recommended to use the Library manager
# ('make library-manager' from command line), which will also update 
# Eclipse IDE launch configurations.
TARGET=APP_KIT_PSE84_EVAL_EPC2

# Name of toolchain to use. Options include:
#
# GCC_ARM 	-- GCC provided with ModusToolbox software
# ARM     	-- ARM Compiler (must be installed separately)
# IAR     	-- IAR Compiler (must be installed separately)
# LLVM_ARM	-- LLVM Embedded Toolchain (must be installed separately)
#
# See also: CY_COMPILER_PATH below
TOOLCHAIN=GCC_ARM

# Default build configuration. Options include:
#
# Debug -- build with minimal optimizations, focus on debugging.
# Release -- build with full optimizations
# Custom -- build with custom configuration, set the optimization flag in CFLAGS
# 
# If CONFIG is manually edited, ensure to update or regenerate 
# launch configurations for your IDE.
CONFIG=Debug

############################# Display module ###################################
# Option to choose the display module to realize the graphics application.
# Select one of them as per the required use-case.
# WF101JTYAHMNB0_DISP	- 10.1 inch 1024 * 600 pixel TFT DSI LCD and it's touch
#                         driver.
# WS7P0DSI_RPI_DISP     - Waveshare 7 inch Raspberry-Pi DSI LCD (C) 1024 * 600 pixel
#                         display and it's touch driver.
# Ex:
#   CONFIG_DISPLAY = WF101JTYAHMNB0_DISP
#   or
#   CONFIG_DISPLAY = WS7P0DSI_RPI_DISP


################################################################################
# Advanced Configuration
################################################################################

# Enable optional code that is ordinarily disabled by default.
#
# Available components depend on the specific targeted hardware and firmware
# in use. In general, if you have
#
#    COMPONENTS=foo bar
#
# ... then code in directories named COMPONENT_foo and COMPONENT_bar will be
# added to the build
#
COMPONENTS+=GFXSS

# Remap the System SRAM (SOCMEM) to accomodate graphics frame buffers in it
ifeq ($(filter GFXSS,$(COMPONENTS)),GFXSS)
SOCMEMSRAM_CM55NS_APP_SIZE=0x40000
SOCMEMSRAM_GPUBUF_SIZE=0x00300000
SOCMEMSRAM_SHARED_SIZE=0x800
SOCMEMSRAM_CM55NS_DATA_SIZE=0x00100000
SOCMEMSRAM_CM33NS_APP_SIZE=0x40000
SOCMEMSRAM_CM33NS_DATA_SIZE=0x7F800

DEFINES+=SOCMEMSRAM_CM55NS_APP_SIZE=$(SOCMEMSRAM_CM55NS_APP_SIZE)
DEFINES+=SOCMEMSRAM_GPUBUF_SIZE=$(SOCMEMSRAM_GPUBUF_SIZE)
DEFINES+=SOCMEMSRAM_SHARED_SIZE=$(SOCMEMSRAM_SHARED_SIZE)
DEFINES+=SOCMEMSRAM_CM55NS_DATA_SIZE=$(SOCMEMSRAM_CM55NS_DATA_SIZE)
DEFINES+=SOCMEMSRAM_CM33NS_APP_SIZE=$(SOCMEMSRAM_CM33NS_APP_SIZE)
DEFINES+=SOCMEMSRAM_CM33NS_DATA_SIZE=$(SOCMEMSRAM_CM33NS_DATA_SIZE)

# Allocating GPU buffer from socmem

ifeq ($(TOOLCHAIN),IAR)
LDFLAGS+=--config_def APP_SOCMEMSRAM_CM55NS_APP_SIZE=$(SOCMEMSRAM_CM55NS_APP_SIZE)
LDFLAGS+=--config_def APP_SOCMEMSRAM_GPUBUF_SIZE=$(SOCMEMSRAM_GPUBUF_SIZE)
LDFLAGS+=--config_def APP_SOCMEMSRAM_SHARED_SIZE=$(SOCMEMSRAM_SHARED_SIZE)
LDFLAGS+=--config_def APP_SOCMEMSRAM_CM55NS_DATA_SIZE=$(SOCMEMSRAM_CM55NS_DATA_SIZE)
LDFLAGS+=--config_def APP_SOCMEMSRAM_CM33NS_APP_SIZE=$(SOCMEMSRAM_CM33NS_APP_SIZE)
LDFLAGS+=--config_def APP_SOCMEMSRAM_CM33NS_DATA_SIZE=$(SOCMEMSRAM_CM33NS_DATA_SIZE)
else ifeq ($(TOOLCHAIN),ARM)
LDFLAGS+=--predefine="-DAPP_SOCMEMSRAM_CM55NS_APP_SIZE=$(SOCMEMSRAM_CM55NS_APP_SIZE)"
LDFLAGS+=--predefine="-DAPP_SOCMEMSRAM_GPUBUF_SIZE=$(SOCMEMSRAM_GPUBUF_SIZE)"
LDFLAGS+=--predefine="-DAPP_SOCMEMSRAM_SHARED_SIZE=$(SOCMEMSRAM_SHARED_SIZE)"
LDFLAGS+=--predefine="-DAPP_SOCMEMSRAM_CM55NS_DATA_SIZE=$(SOCMEMSRAM_CM55NS_DATA_SIZE)"
LDFLAGS+=--predefine="-DAPP_SOCMEMSRAM_CM33NS_APP_SIZE=$(SOCMEMSRAM_CM33NS_APP_SIZE)"
LDFLAGS+=--predefine="-DAPP_SOCMEMSRAM_CM33NS_DATA_SIZE=$(SOCMEMSRAM_CM33NS_DATA_SIZE)"
endif
endif


include ../common_app.mk
