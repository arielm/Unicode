LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

CINDER_PATH = ../../../../../../../Cinder

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../src
FILE_LIST := $(wildcard $(LOCAL_PATH)/../../src/*.cpp)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(CINDER_PATH)/blocks/Freetype/include

HB_SRC = ../../../../hb
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(HB_SRC)

LOCAL_SRC_FILES += $(HB_SRC)/hb-blob.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-buffer.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-common.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-face.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-fallback-shape.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-font.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-ft.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-ot-layout.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-ot-map.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-ot-shape-complex-arabic.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-ot-shape-complex-default.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-ot-shape-complex-indic-table.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-ot-shape-complex-indic.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-ot-shape-complex-myanmar.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-ot-shape-complex-sea.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-ot-shape-complex-thai.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-ot-shape-fallback.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-ot-shape-normalize.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-ot-shape.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-ot-tag.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-set.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-shape-plan.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-shape.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-shaper.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-ucdn.cc
LOCAL_SRC_FILES += $(HB_SRC)/hb-unicode.cc
LOCAL_SRC_FILES += $(HB_SRC)/ucdn.c

LOCAL_CFLAGS := -DHB_NO_MT -DHAVE_OT -DHAVE_UCDN
#LOCAL_CFLAGS += -O3
LOCAL_CFLAGS += -DDEBUG

LOCAL_LDLIBS := -landroid
LOCAL_STATIC_LIBRARIES := cinder boost_system boost_filesystem freeimage ft2 android_native_app_glue 

LOCAL_MODULE := CinderHarfbuzz
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,cinder)
$(call import-module,boost)
