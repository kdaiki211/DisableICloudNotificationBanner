CC := gcc
CFLAGS :=

PRJ_NAME := DisableICloudNotificationBanner
OUT_BIN  := $(PWD)/$(PRJ_NAME).exe
SRC := src/main.c
ALL_OUTPUTS := $(OUT_BIN)

.PHONY: clean

$(OUT_BIN) : $(SRC)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(ALL_OUTPUTS)
