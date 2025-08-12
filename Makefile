CC = gcc
CFLAGS = -ansi -Wall -pedantic -g

SRC_DIR = src
HDR_DIR = headers

GLOBAL_DEP = globals.h
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=%.o)

TARGET = assembler

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	
%.o: $(SRC_DIR)/%.c $(HDR_DIR)/*.h $(GLOBALS_DEPS)
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	rm -f $(TARGET) *.o *.am *.ob *.ent *.ext
