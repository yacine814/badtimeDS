TARGET = badtimeDS
BUILD  = build
SOURCES = source

include $(DEVKITPRO)/devkitARM/base_tools

CFILES = $(wildcard $(SOURCES)/*.c)
OFILES = $(CFILES:.c=.o)

CFLAGS = -g -O2 -Wall -DARM9
LDFLAGS = -specs=ds_arm9.specs

LIBS = -lnds9

all: $(TARGET).nds

$(TARGET).nds: $(TARGET).elf
	@ndstool -c $@ -7 arm7.bin -9 $<

$(TARGET).elf: $(OFILES)
	$(CC) $(OFILES) $(LDFLAGS) $(LIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OFILES) $(TARGET).elf $(TARGET).nds