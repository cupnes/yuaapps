TARGET = e.auto_slide
CFLAGS = -Wall -Wextra -Iinclude -fPIE
ifeq ($(RUN_LOCAL),y)
CFLAGS += -DRUN_LOCAL -g
else
CFLAGS += -nostdinc -nostdlib -fno-builtin -fno-common
endif
LDFLAGS = -Map app.map -pie -s -x -T apps.ld
OBJS = main.o lib.o
DEPLOY_DIR ?= ../fs

$(TARGET): $(OBJS)
ifeq ($(RUN_LOCAL),y)
	gcc -g -o $@ $+
else
	ld $(LDFLAGS) -o $@ $+
endif

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<
%.o: %.s
	gcc $(CFLAGS) -c -o $@ $<

deploy: $(TARGET)
	cp $(TARGET) $(DEPLOY_DIR)

clean:
	rm -f *~ *.o *.map $(TARGET) include/*~

.PHONY: clean
