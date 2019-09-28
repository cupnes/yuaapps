TARGET = urclock
CFLAGS = -Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-common -Iinclude -fPIE
LDFLAGS = -Map app.map -s -x -T apps.ld -pie
OBJS = main.o lib.o
DEPLOY_DIR ?= ../fs
ifdef RUN_QEMU
	CFLAGS += -DRUN_QEMU
endif

$(TARGET): $(OBJS)
	ld $(LDFLAGS) -o $@ $+

.o: %.c
	gcc $(CFLAGS) -c -o $@ $<
%.o: %.s
	gcc $(CFLAGS) -c -o $@ $<

deploy: $(TARGET)
	cp $(TARGET) $(DEPLOY_DIR)

clean:
	rm -f *~ *.o *.map $(TARGET) include/*~

.PHONY: clean
