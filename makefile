CC = gcc
CFLAGS = -std=c17 -Wall -v
LDFLAGS = -lraylib -lgdi32 -lwinmm
EXE = $(basename $(notdir $(CURDIR)))

# Phony target to ensure it's always run
.PHONY: run clean

all: $(EXE).exe run

OBJS := $(patsubst %.c,%.o,$(wildcard $(CURDIR)/*.c))

$(EXE).exe: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

run: $(EXE).exe
	./$(EXE).exe

clean:
	rm -f $(EXE).exe $(OBJS)