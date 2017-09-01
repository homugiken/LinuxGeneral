CC = gcc
TARGET = main
SOURCE = $(wildcard *.c)
OBJECT = $(SOURCE:.c=.o)

$(TARGET): $(OBJECT)
	$(CC) -v -o $@ $^

.PHONY: clean

clean:
	$(RM) *.o
	$(RM) $(TARGET)
