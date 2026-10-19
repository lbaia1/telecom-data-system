CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

TARGET = telecom_analyzer

OBJS = main.o migration.o archive.o analytics.o linked_list.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c telecom.h
	$(CC) $(CFLAGS) -c main.c

migration.o: migration.c telecom.h
	$(CC) $(CFLAGS) -c migration.c

archive.o: archive.c telecom.h
	$(CC) $(CFLAGS) -c archive.c

analytics.o: analytics.c telecom.h
	$(CC) $(CFLAGS) -c analytics.c

linked_list.o: linked_list.c telecom.h
	$(CC) $(CFLAGS) -c linked_list.c

clean:
	rm -f $(OBJS) $(TARGET) archive.bin