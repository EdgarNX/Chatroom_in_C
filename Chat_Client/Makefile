COMP=gcc
TARGET=client
CFLAGS= -g3 -fsanitize=address -pthread -Wall -lpthread
MLINK= -o
FILES= ./src/client.c

client: $(FILES)
	$(COMP) $(MLINK) $(TARGET) $(CFLAGS) $(FILES)
