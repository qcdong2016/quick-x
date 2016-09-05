APP_STL := gnustl_static
APP_CPPFLAGS := -frtti -std=c++11 -Wno-error=format-security -Wno-literal-suffix -Wno-deprecated-declarations -fsigned-char -Os $(CPPFLAGS)
APP_DEBUG := $(strip $(NDK_DEBUG))
ifeq ($(APP_DEBUG),1)
  APP_CPPFLAGS += -DCOCOS2D_DEBUG=1
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DNDEBUG
  APP_OPTIM := release
endif
APP_ABI := armeabi


CC_CURL_ENABLED := 1
APP_CPPFLAGS += -DCC_CURL_ENABLED=1

CC_JPEG_ENABLED := 1
APP_CPPFLAGS += -DCC_JPEG_ENABLED=1

CC_WEBP_ENABLED := 1
APP_CPPFLAGS += -DCC_WEBP_ENABLED=1

CC_SQLITE_ENABLED := 1
APP_CPPFLAGS += -DCC_SQLITE_ENABLED=1
APP_CFLAGS += -DCC_SQLITE_ENABLED=1
