TARGET = fs.img
WORK_DIR = fs_x280
FILES = init urclock bg.bgra lsbg.bgra urclockbg.bgra
FILES += $(notdir $(shell ls fs_qemu/e.*))
FILES += $(notdir $(shell ls fs_qemu/s.*))
FILES += $(notdir $(shell ls fs_qemu/i.*))
TARGET_DEV ?= /dev/sdb1
MOUNT_POINT ?= $(WORK_DIR)/mnt

$(WORK_DIR)/$(TARGET): $(addprefix $(WORK_DIR)/, $(FILES))
	cd $(WORK_DIR) && ../tools/create_fs.sh $(FILES)

$(WORK_DIR)/init:
	make -C adv_if deploy DEPLOY_DIR=../$(WORK_DIR)

$(WORK_DIR)/urclock:
	make -C urclock deploy DEPLOY_DIR=../$(WORK_DIR)

deploy: $(WORK_DIR)/$(TARGET)
	mkdir -p $(MOUNT_POINT)
	if ! mount | grep -q $(MOUNT_POINT); then \
		sudo mount $(TARGET_DEV) $(MOUNT_POINT); \
	fi
	if [ ! -f $(MOUNT_POINT)/kernel.bin ]; then \
		echo error: $(MOUNT_POINT)/kernel.bin is not found. >&2; \
		echo error: Is $(TARGET_DEV) correct? >&2; \
		false; \
	fi
	sudo cp $< $(MOUNT_POINT)

run: deploy
	if mount | grep -q $(MOUNT_POINT); then \
		sudo umount $(MOUNT_POINT); \
	fi
	sudo reboot

clean:
	make -C adv_if clean
	make -C urclock clean
	rm -rf *~ $(WORK_DIR)/*~ $(WORK_DIR)/init $(WORK_DIR)/urclock \
		$(WORK_DIR)/$(TARGET) $(WORK_DIR)/mnt

.PHONY: deploy run clean
