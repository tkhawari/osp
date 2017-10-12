obj-m += s3422192Device.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load: 
	insmod s3422192Device.ko

unload: 
	rmmod s3422192Device.ko

