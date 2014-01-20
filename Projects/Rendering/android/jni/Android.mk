LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

CINDER_PATH = ../../../../../../../Cinder

LOCAL_SRC_FILES := main.cpp

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../src
FILE_LIST := $(wildcard $(LOCAL_PATH)/../../src/*.cpp)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(CINDER_PATH)/blocks/Freetype/include

include $(LOCAL_PATH)/$(CINDER_PATH)/blocks/new-chronotext-toolkit/android/Android.mk

LOCAL_CFLAGS += -DCHR_COMPLEX
#LOCAL_CFLAGS += -DDEBUG
LOCAL_CFLAGS += -ffast-math -O3

LOCAL_LDLIBS := -llog -landroid
LOCAL_STATIC_LIBRARIES := cinder boost_system boost_filesystem boost_thread freeimage ft2 android_native_app_glue
LOCAL_STATIC_LIBRARIES += icu-common hb

LOCAL_MODULE := Rendering
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,cinder)
$(call import-module,boost)
$(call import-module,../../blocks/hb-icu/android)
