#include <stdio.h>
#include <dlfcn.h>
int main() {
	void *lib;
	int (*libFunc)(int, int);
	if (!(lib = dlopen("libfuncD.so", RTLD_LAZY))){
		fprintf(stderr,"dlopen() error: %s\n", dlerror());
		return 1;
	};
	libFunc = dlsym(lib, "summing");	
	printf("2 + 2 = %d\n", libFunc(2, 2));
	libFunc = dlsym(lib, "subtraction");	
	printf("5 - 3 = %d\n", libFunc(5, 3));
	return 0;
}