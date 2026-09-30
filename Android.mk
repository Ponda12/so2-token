LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := big_array
LOCAL_CFLAGS += -O3
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := token

file_list := $(wildcard $(LOCAL_PATH)/src/*.cpp) \
             $(wildcard $(LOCAL_PATH)/src/*/*.cpp) \    
             $(wildcard $(LOCAL_PATH)/src/*/*/*.cpp) \

LOCAL_SRC_FILES := $(file_list:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := \
    src \
LOCAL_STATIC_LIBRARIES := libcrypto big_array

LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv3 -lz


LOCAL_CFLAGS += -O2 -w -fvisibility=hidden -ffunction-sections -fdata-sections -DIMGUI_DISABLE_DEMO_WINDOWS -fdiagnostics-color=always

LOCAL_LDFLAGS += -Wl,--gc-sections -Wl,--exclude-libs,ALL

include $(BUILD_EXECUTABLE)