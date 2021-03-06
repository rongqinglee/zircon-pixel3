# Copyright 2018 The Fuchsia Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_TYPE := driver

MODULE_SRCS += \
    $(LOCAL_DIR)/fx3.c \

MODULE_STATIC_LIBS := system/ulib/ddk system/ulib/fidl system/dev/lib/usb

MODULE_LIBS := system/ulib/driver system/ulib/c system/ulib/zircon

MODULE_FIDL_LIBS := \
    system/fidl/fuchsia-mem \
    system/fidl/fuchsia-hardware-usb-fwloader \

MODULE_BANJO_LIBS := \
    system/banjo/ddk-protocol-usb \
    system/banjo/ddk-protocol-usb-composite \
    system/banjo/ddk-protocol-usb-request \

ifeq ($(call TOBOOL,$(INTERNAL_ACCESS)),true)
MODULE_FIRMWARE := \
    fx3-flash/cyfxflashprog.img \
    usb-testing/fx3/fx3.img
endif

include make/module.mk
