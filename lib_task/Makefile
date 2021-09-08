all: static dynamic
funcS.o:
	gcc -c func.c -o funcS.o
funcD.o:
	gcc -c -fPIC func.c -o funcD.o
static: funcS.o
	ar rc libfuncS.a funcS.o
	ranlib libfuncS.a
dynamic: funcD.o
	gcc -shared -o libfuncD.so funcD.o
test: static dynamic
	gcc test.c -o testStatic -L. -l funcS
	gcc test.c -o testDynamic -L. -l funcD -Wl,-rpath,.
	gcc testD2.c -o testDynamic2 -ldl -Wl,-rpath,.
clean:
	rm -rf funcS.o funcD.o libfuncS.a libfuncD.so testStatic testDynamic testDynamic2