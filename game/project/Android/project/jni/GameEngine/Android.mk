LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
VPATH := /Users/kristofe/Documents/Projects/GameEngine/game/source/Engine/
LOCAL_MODULE := NativeGameEngine

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../libzip/ $(LOCAL_PATH)/../libpng/
LOCAL_STATIC_LIBRARIES := libzip libpng

#-Wno-psabi to remove warning about GCC 4.4 va_list warning
LOCAL_CFLAGS := -DANDROID_NDK -Wno-psabi

LOCAL_DEFAULT_CPP_EXTENSION := cpp 

MY_ENGINE_PATH := ../../../../../source/Engine/

LOCAL_SRC_FILES := \
    app-android.cpp \
    utils.cpp \
    $(MY_ENGINE_PATH)Perlin.cpp

LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz

include $(BUILD_SHARED_LIBRARY)
