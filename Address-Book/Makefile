var:=$(patsubst %.c, %.o, $(wildcard *.c))
calc.exe:$(var)
	gcc -o a.out $(var)
clean:
	rm *.exe *.o