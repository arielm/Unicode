LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

CINDER_PATH = ../../../../../../../Cinder

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../src
FILE_LIST := $(wildcard $(LOCAL_PATH)/../../src/*.cpp)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

ICU_SRC = ../../../../icu/common
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(ICU_SRC)

LOCAL_SRC_FILES += $(ICU_SRC)/cmemory.c
LOCAL_SRC_FILES += $(ICU_SRC)/cstring.c
LOCAL_SRC_FILES += $(ICU_SRC)/cwchar.c
LOCAL_SRC_FILES += $(ICU_SRC)/icudataver.c
LOCAL_SRC_FILES += $(ICU_SRC)/icuplug.c
LOCAL_SRC_FILES += $(ICU_SRC)/locmap.c
LOCAL_SRC_FILES += $(ICU_SRC)/propsvec.c
LOCAL_SRC_FILES += $(ICU_SRC)/uarrsort.c
LOCAL_SRC_FILES += $(ICU_SRC)/ubidi.c
LOCAL_SRC_FILES += $(ICU_SRC)/ubidi_props.c
LOCAL_SRC_FILES += $(ICU_SRC)/ubidiln.c
LOCAL_SRC_FILES += $(ICU_SRC)/ubidiwrt.c
LOCAL_SRC_FILES += $(ICU_SRC)/ucat.c
LOCAL_SRC_FILES += $(ICU_SRC)/uchar.c
LOCAL_SRC_FILES += $(ICU_SRC)/ucln_cmn.c
LOCAL_SRC_FILES += $(ICU_SRC)/ucmndata.c
LOCAL_SRC_FILES += $(ICU_SRC)/ucnv.c
LOCAL_SRC_FILES += $(ICU_SRC)/ucnv_cb.c
LOCAL_SRC_FILES += $(ICU_SRC)/ucnv_cnv.c
LOCAL_SRC_FILES += $(ICU_SRC)/ucnv_ct.c
LOCAL_SRC_FILES += $(ICU_SRC)/ucnv_err.c
LOCAL_SRC_FILES += $(ICU_SRC)/ucnv_lmb.c
LOCAL_SRC_FILES += $(ICU_SRC)/ucnv_set.c
LOCAL_SRC_FILES += $(ICU_SRC)/ucnv_u16.c
LOCAL_SRC_FILES += $(ICU_SRC)/ucnv_u32.c
LOCAL_SRC_FILES += $(ICU_SRC)/ucnv_u7.c
LOCAL_SRC_FILES += $(ICU_SRC)/ucnv_u8.c
LOCAL_SRC_FILES += $(ICU_SRC)/ucnvdisp.c
LOCAL_SRC_FILES += $(ICU_SRC)/ucnvhz.c
LOCAL_SRC_FILES += $(ICU_SRC)/ucnvisci.c
LOCAL_SRC_FILES += $(ICU_SRC)/ucnvlat1.c
LOCAL_SRC_FILES += $(ICU_SRC)/ucnvmbcs.c
LOCAL_SRC_FILES += $(ICU_SRC)/ucnvscsu.c
LOCAL_SRC_FILES += $(ICU_SRC)/udatamem.c
LOCAL_SRC_FILES += $(ICU_SRC)/udataswp.c
LOCAL_SRC_FILES += $(ICU_SRC)/uenum.c
LOCAL_SRC_FILES += $(ICU_SRC)/uhash.c
LOCAL_SRC_FILES += $(ICU_SRC)/uinvchar.c
LOCAL_SRC_FILES += $(ICU_SRC)/ulist.c
LOCAL_SRC_FILES += $(ICU_SRC)/uloc_tag.c
LOCAL_SRC_FILES += $(ICU_SRC)/umapfile.c
LOCAL_SRC_FILES += $(ICU_SRC)/umath.c
LOCAL_SRC_FILES += $(ICU_SRC)/unorm_it.c
LOCAL_SRC_FILES += $(ICU_SRC)/ures_cnv.c
LOCAL_SRC_FILES += $(ICU_SRC)/uresdata.c
LOCAL_SRC_FILES += $(ICU_SRC)/usc_impl.c
LOCAL_SRC_FILES += $(ICU_SRC)/uscript.c
LOCAL_SRC_FILES += $(ICU_SRC)/ustr_cnv.c
LOCAL_SRC_FILES += $(ICU_SRC)/ustrfmt.c
LOCAL_SRC_FILES += $(ICU_SRC)/utf_impl.c
LOCAL_SRC_FILES += $(ICU_SRC)/utrace.c
LOCAL_SRC_FILES += $(ICU_SRC)/utypes.c
LOCAL_SRC_FILES += $(ICU_SRC)/wintz.c
LOCAL_SRC_FILES += $(ICU_SRC)/appendable.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/bmpset.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/brkeng.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/brkiter.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/bytestream.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/bytestrie.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/bytestriebuilder.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/bytestrieiterator.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/caniter.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/chariter.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/charstr.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/dictbe.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/dictionarydata.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/dtintrv.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/errorcode.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/filterednormalizer2.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/listformatter.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/locavailable.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/locbased.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/locdispnames.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/locid.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/loclikely.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/locresdata.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/locutil.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/messagepattern.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/normalizer2.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/normalizer2impl.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/normlzr.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/parsepos.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/patternprops.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/propname.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/punycode.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/putil.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/rbbi.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/rbbidata.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/rbbinode.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/rbbirb.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/rbbiscan.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/rbbisetb.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/rbbistbl.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/rbbitblb.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/resbund.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/resbund_cnv.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ruleiter.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/schriter.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/serv.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/servlk.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/servlkf.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/servls.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/servnotf.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/servrbf.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/servslkf.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/stringpiece.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/stringtriebuilder.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ubrk.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ucase.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ucasemap.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ucasemap_titlecase_brkiter.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ucharstrie.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ucharstriebuilder.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ucharstrieiterator.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/uchriter.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ucnv2022.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ucnv_bld.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ucnv_ext.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ucnv_io.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ucnvbocu.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ucnvsel.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ucol_swp.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/udata.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/uhash_us.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/uidna.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/uinit.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/uiter.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/uloc.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/umutex.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/unames.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/unifilt.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/unifunct.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/uniset.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/uniset_closure.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/uniset_props.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/unisetspan.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/unistr.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/unistr_case.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/unistr_case_locale.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/unistr_cnv.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/unistr_props.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/unistr_titlecase_brkiter.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/unorm.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/unormcmp.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/uobject.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/uprops.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/uresbund.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/uscript_props.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/uset.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/uset_props.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/usetiter.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ushape.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/usprep.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ustack.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ustr_titlecase_brkiter.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ustr_wcs.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ustrcase.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ustrcase_locale.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ustrenum.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ustring.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/ustrtrns.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/utext.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/util.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/util_props.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/utrie.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/utrie2.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/utrie2_builder.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/uts46.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/uvector.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/uvectr32.cpp
LOCAL_SRC_FILES += $(ICU_SRC)/uvectr64.cpp

# This is the empty compiled-in icu data structure
# that we need to satisfy the linker.
LOCAL_SRC_FILES += $(ICU_SRC)/../stubdata/stubdata.c

# We make the ICU data directory relative to $ANDROID_ROOT on Android, so both
# device and sim builds can use the same codepath, and it's hard to break one
# without noticing because the other still works.
LOCAL_CFLAGS += '-DICU_DATA_DIR_PREFIX_ENV_VAR="ANDROID_ROOT"'
LOCAL_CFLAGS += '-DICU_DATA_DIR="/usr/icu"'

# bionic doesn't have <langinfo.h>.
LOCAL_CFLAGS += -DU_HAVE_NL_LANGINFO_CODESET=0

LOCAL_CFLAGS += -D_REENTRANT
LOCAL_CFLAGS += -DU_COMMON_IMPLEMENTATION

LOCAL_CFLAGS += -fvisibility=hidden

LOCAL_CFLAGS += -O3
#LOCAL_CFLAGS += -DDEBUG

LOCAL_LDLIBS := -landroid
LOCAL_STATIC_LIBRARIES := cinder boost_system boost_filesystem freeimage ft2 android_native_app_glue 

LOCAL_MODULE := BIDI
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,cinder)
$(call import-module,boost)
