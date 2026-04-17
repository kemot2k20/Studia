CC = gcc
CFLAGS = -Wall -Wextra -std=gnu18
TARGET = traceroute
SRCS = traceroute.c
OBJS = $(SRCS:.c=.o)
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

distclean:
	rm -f $(OBJS) $(TARGET)