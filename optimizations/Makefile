# optimizations/Makefile
CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = cover
SRCS = main.c vertex-cover.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

