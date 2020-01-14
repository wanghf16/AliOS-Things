
EXTRA_POST_BUILD_TARGETS += mkimage

SOURCE_BIN = $(SOURCE_ROOT)out/$(CLEANED_BUILD_STRING)/binary/$(CLEANED_BUILD_STRING).bin
SECBOOT_IMG = $(SOURCE_ROOT)platform/mcu/$(HOST_MCU_FAMILY)/tools/secboot.img
TOOLS_DIR = $(SOURCE_ROOT)platform/mcu/$(HOST_MCU_FAMILY)/tools
OUT_DIR = $(SOURCE_ROOT)out/$(CLEANED_BUILD_STRING)/binary/


mkimage:	
	$(TOOLS_DIR)/wm_tool -b $(SOURCE_BIN) -sb $(SECBOOT_IMG) -df -o $(OUT_DIR)wm_w600

