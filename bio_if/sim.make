TARGET = sim
CFLAGS = -Wall -Wextra -Iinclude -fPIE
SRCS = main.c bio_if.c

$(TARGET): $(SRCS)
	gcc $(CFLAGS) -o $@ $+

clean:
	rm -f *~ *.o $(TARGET) include/*~

.PHONY: clean
