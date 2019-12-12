TARGET = fs.img
WORK_DIR = fs_qemu
FILES = init urclock bg.bgra lsbg.bgra urclockbg.bgra dotoon_cpu
FILES += e.dotoon e.biosys e.biosys_full e.bio_mid e.auto_slide
FILES += $(notdir $(shell ls fs_qemu/s.* 2>/dev/null || true))
FILES += $(notdir $(shell ls fs_qemu/i.* 2>/dev/null || true))
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
	make -C adv_if deploy DEPLOY_DIR=../$(WORK_DIR) RUN_QEMU=true

$(WORK_DIR)/urclock:
	make -C urclock deploy DEPLOY_DIR=../$(WORK_DIR) RUN_QEMU=true

$(WORK_DIR)/dotoon_cpu:
	make -C dotoon_cpu deploy DEPLOY_DIR=../$(WORK_DIR) RUN_QEMU=true

$(WORK_DIR)/e.dotoon:
	make -C dotoon deploy DEPLOY_DIR=../$(WORK_DIR) RUN_QEMU=true

$(WORK_DIR)/e.biosys:
	make -C bio_sys RUN_QEMU=true clean
	make -C bio_sys RUN_QEMU=true
	cp bio_sys/init $@

$(WORK_DIR)/e.biosys_full:
	make -C bio_sys RUN_QEMU=true FULL_DEMO=true clean
	make -C bio_sys RUN_QEMU=true FULL_DEMO=true
	cp bio_sys/init $@

$(WORK_DIR)/e.bio_mid:
	make -C bio_middle RUN_QEMU=true DEPLOY_DIR=../$(WORK_DIR) deploy

$(WORK_DIR)/e.auto_slide:
	make -C auto_slideshow deploy DEPLOY_DIR=../$(WORK_DIR) RUN_QEMU=true

deploy: $(WORK_DIR)/$(TARGET)
	cp $< $(FS_DIR)

run: deploy
	qemu-system-x86_64 -m 4G -enable-kvm \
	-drive if=pflash,format=raw,readonly,file=$(HOME)/OVMF/OVMF_CODE.fd \
	-drive if=pflash,format=raw,file=$(HOME)/OVMF/OVMF_VARS.fd \
	-drive dir=$(FS_DIR),driver=vvfat,rw=on \
	$(QEMU_ADDITIONAL_ARGS)

clean:
	make -C adv_if clean
	make -C urclock clean
	make -C dotoon clean
	make -C dotoon_cpu clean
	make -C bio_sys clean
	make -C bio_middle clean
	make -C auto_slideshow clean
	rm -f *~ $(WORK_DIR)/*~ $(WORK_DIR)/init $(WORK_DIR)/urclock	\
		$(WORK_DIR)/dotoon_cpu $(WORK_DIR)/e.dotoon		\
		$(WORK_DIR)/e.biosys $(WORK_DIR)/e.biosys_full		\
		$(WORK_DIR)/e.bio_mid $(WORK_DIR)/e.auto_slide		\
		$(WORK_DIR)/$(TARGET)

.PHONY: deploy run clean
