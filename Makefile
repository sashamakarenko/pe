PRJ_NAME    := pe
PRJ_BRANCH  := 1.0
PRJ_VERSION := $(PRJ_BRANCH).0
PRJ_TYPE    := lib

TEST_EXTRA_LINK_LIBS = -lrt -lpthread
CPP_EXTRA_FLAGS = -Wno-deprecated-declarations

LIB_PRELOAD = build/lib/$(BUILD_MODE)/libPePreload-$(PRJ_BRANCH).so
TARGET_EXTRA_DEPENDENCY = $(LIB_PRELOAD)
TEST_LAUNCHER = LD_PRELOAD=$(LIB_PRELOAD) #LD_DEBUG=symbols,libs,calls

include ../makefile/Makefile

LINK_OPTIONS += -Wl,-init,initPe

$(LIB_PRELOAD): build/obj/$(BUILD_MODE)/preload/PePreload.o
	$(V)echo "  linking $@"
	$(V)mkdir -p $(@D)
	$(V)gcc -shared -g -Wl,-init,initPePreload -o $@ $< -ldl -lpthread

build/obj/$(BUILD_MODE)/preload/PePreload.o: src/preload/PePreload.c
	$(V)echo "  compiling $<"
	$(V)mkdir -p $(@D)
	$(V)gcc -fPIC -Wall -g -O0 -c $< -o $@ # -DPEPRELOAD_DEBUG
