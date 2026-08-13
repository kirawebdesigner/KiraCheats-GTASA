LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CPP_EXTENSION := .cpp
LOCAL_MODULE := KiraCheats

LOCAL_SRC_FILES := \
    main.cpp \
    mod/logger.cpp

LOCAL_CFLAGS += -O2 -DNDEBUG -std=c++17
LOCAL_CXXFLAGS += -O2 -DNDEBUG -std=c++17

LOCAL_LDLIBS += -llog

include $(BUILD_SHARED_LIBRARY)
