LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

CINDER_PATH = ../../../../../../../Cinder
HB_PATH = ../../../../hb

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../src
FILE_LIST := $(wildcard $(LOCAL_PATH)/../../src/*.cpp)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(CINDER_PATH)/blocks/Freetype/include

include $(LOCAL_PATH)/$(HB_PATH)/Android.mk

#LOCAL_CFLAGS += -DDEBUG
LOCAL_CFLAGS += -O3

LOCAL_LDLIBS := -landroid
LOCAL_STATIC_LIBRARIES := cinder boost_system boost_filesystem freeimage ft2 android_native_app_glue 

LOCAL_MODULE := VirtualFont
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,cinder)
$(call import-module,boost)
