# Target library
lib := libuthread.a

srcs = $(wildcard ./*.c)
objs = $(srcs:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror
AR = ar
ARFLAGS = cr

all: $(lib)
	rm -f $(objs)

$(lib): $(objs)
	$(AR) $(ARFLAGS) $@ $^

$(objs): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(objs) $(lib)
