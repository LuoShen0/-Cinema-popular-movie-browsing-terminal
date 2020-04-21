src = $(wildcard *.c)

obj = $(src:%.c=%.o)

com_flag = -I ../include 
com_lib=-L ../lib -l jpeg

CC=arm-linux-gcc

$(image):$(obj)
	$(CC) $^ -o $@ $(com_lib)
	mv $(image) ../

$(obj):%.o:%.c
	$(CC) $^ -o $@ -c $(com_flag)

clean:
	rm $(obj)

.PHONY:clean
