#KSRC ?= /lib/modules/$(shell uname -r)/build
KSRC ?= /home/thomas/workspace/debug_linux5.15.1_x86_0

TARGET ?= debug
obj-m := $(TARGET).o

all:
	make -C $(KSRC)/ M=$(CURDIR) modules
	cp $(TARGET).ko ../build

%:
	make -C $(KSRC)/ M=$(CURDIR) $@
