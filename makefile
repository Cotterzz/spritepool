ifeq ($(OS),Windows_NT)
    PLATFORM_OS = WINDOWS
else
    UNAMEOS = $(shell uname)
    ifeq ($(UNAMEOS),Linux)
        PLATFORM_OS = LINUX
    endif
endif
CC = gcc
CFLAGS = -std=c17 -Wall -v
ifeq ($(PLATFORM_OS),WINDOWS)
    LDFLAGS = -lraylib -lgdi32 -lwinmm
    EXE = $(basename $(notdir $(CURDIR))).exe
else
	LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	EXE = $(basename $(notdir $(CURDIR)))
endif


# Phony target to ensure it's always run
.PHONY: run clean

all: $(EXE) run

OBJS := $(patsubst %.c,%.o,$(wildcard $(CURDIR)/*.c))

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

run: $(EXE)
	./$(EXE)

clean:
	rm -f $(EXE) $(OBJS)
