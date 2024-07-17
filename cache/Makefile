# cache/Makefile
CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = cache
SRCS = main.c cache_simulator.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)
