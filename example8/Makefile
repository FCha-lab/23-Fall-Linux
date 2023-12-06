obj-m = hello.o

KDIR := /home/ghksdlqhdl/workspace/c/example8/WSL2-Linux-Kernel-linux-msft-wsl-5.15.133.1
PWD := $(shell pwd)

default:
	make -C $(KDIR) M=$(PWD) modules
	
clean:
	rm -rf *.kr *.mod.* .*.cmd *.o *.ko
