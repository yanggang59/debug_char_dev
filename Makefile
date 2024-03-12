KSRC ?= /lib/modules/$(shell uname -r)/build
#KSRC ?= /home/ubuntu/workspace/debug_linux5.0_x86

obj-m := debug.o

all:
	make -C $(KSRC)/ M=$(CURDIR) modules

%:
	make -C $(KSRC)/ M=$(CURDIR) $@
