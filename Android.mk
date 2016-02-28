LOCAL_PATH := $(call my-dir)

###########################
#
# SDL shared library
#
###########################

include $(CLEAR_VARS)

LOCAL_MODULE := THREE

define walk
  $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

#find all the file recursively under jni/
ALLFILES = $(call walk, $(LOCAL_PATH)/three)
FILE_LIST := $(filter %.cpp, $(ALLFILES))

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_CFLAGS += -DGL_GLEXT_PROTOTYPES -DTHREE_GLES
LOCAL_CPPFLAGS += -DGL_GLEXT_PROTOTYPES -std=c++11 -frtti
LOCAL_LDLIBS := -ldl -lGLESv1_CM -lGLESv2 -llog -landroid

include $(BUILD_SHARED_LIBRARY)
