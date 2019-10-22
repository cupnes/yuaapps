TARGET = fs.img
WORK_DIR = bio_fs_qemu
FILES = init
FS_DIR ?= ../fs
ifdef NO_GRAPHIC
	QEMU_ADDITIONAL_ARGS += --nographic
endif
ifdef SMP
	QEMU_ADDITIONAL_ARGS += -smp ${SMP}
endif

$(WORK_DIR)/$(TARGET): $(addprefix $(WORK_DIR)/, $(FILES))
	cd $(WORK_DIR) && ../tools/create_fs.sh $(FILES)

$(WORK_DIR)/init:
	make -C bio_sys deploy DEPLOY_DIR=../$(WORK_DIR) RUN_QEMU=true

$(WORK_DIR):
	mkdir $@

deploy: $(WORK_DIR)/$(TARGET)
	cp $< $(FS_DIR)

run: deploy
	qemu-system-x86_64 -m 4G -enable-kvm \
	-drive if=pflash,format=raw,readonly,file=$(HOME)/OVMF/OVMF_CODE.fd \
	-drive if=pflash,format=raw,file=$(HOME)/OVMF/OVMF_VARS.fd \
	-drive dir=$(FS_DIR),driver=vvfat,rw=on \
	$(QEMU_ADDITIONAL_ARGS)

clean:
	make -C bio_sys clean
	rm -f *~ $(WORK_DIR)/*~ $(WORK_DIR)/init $(WORK_DIR)/$(TARGET)

.PHONY: deploy run clean
