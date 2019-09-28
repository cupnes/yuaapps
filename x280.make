TARGET = fs.img
WORK_DIR = fs_x280
FILES = init e.test bg.bgra lsbg.bgra
TARGET_DEV ?= /dev/sdb1
MOUNT_POINT ?= $(WORK_DIR)/mnt

$(WORK_DIR)/$(TARGET): $(addprefix $(WORK_DIR)/, $(FILES))
	cd $(WORK_DIR) && ../tools/create_fs.sh $(FILES)

$(WORK_DIR)/init:
	make -C adv_if deploy DEPLOY_DIR=../$(WORK_DIR)

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
	rm -f *~ $(WORK_DIR)/*~ $(WORK_DIR)/init $(WORK_DIR)/$(TARGET)

.PHONY: deploy run clean
