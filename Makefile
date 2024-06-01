CC = gcc
SRCS =  main.c  process.c  scheduling.c  queue.c
OBJS = $(SRCS:.c=.o)
TARGET = exefile

all: $(TARGET)

$(TARGET): $(OBJS)
	cc $(OBJS) -o $(TARGET)
	rm -rf $(OBJS)

%.o: %.c main.h
	cc -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(TARGET)

re: fclean all