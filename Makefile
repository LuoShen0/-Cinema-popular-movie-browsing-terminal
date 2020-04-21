export image=main

all:
	$(MAKE) -C src


clean:
	-rm $(image)
	$(MAKE) -C src clean

.PHONY:clean
