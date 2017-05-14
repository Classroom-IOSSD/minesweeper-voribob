CC = gcc
OUTPUT_OPTION=-MMD -MP -o $@
LDLIBS=-lm

SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=%.o)
DEP = $(SRCS:%.c=%.d)
CFLAGS = -g -Wall
TARGET = main

all: $(TARGET)

-include $(DEP)
$(TARGET): $(OBJS) 
	$(CC) $(OUTPUT_OPTION) $(CFLAGS) $^


tar:
	tar -czvf $(TARGET).gz $(SRCS) conio.h Makefile

.PHONY: clean
clean:
	$(RM) $(OBJS) $(DEP) $(TARGET)


