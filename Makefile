PRJ_NAME    := pe
PRJ_BRANCH  := 1.0
PRJ_VERSION := $(PRJ_BRANCH).0
PRJ_TYPE    := lib

TEST_EXTRA_LINK_LIBS = -lrt
CPP_EXTRA_FLAGS = -Wno-deprecated-declarations

include ../makefile/Makefile
