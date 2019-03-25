randmain: randmain.c randcpuid.c
	$(CC) $(CFLAGS) -c randcpuid.c -o randcpuid.o
	$(CC) $(CFLAGS) -c randmain.c -o randmain.o
	$(CC) $(CFLAGS) randmain.o randcpuid.o -ldl -Wl,-rpath=$(PWD) -o randmain

randlibhw.so: randlibhw.c
	$(CC) $(CFLAGS) -fPIC randlibhw.c -shared -o randlibhw.so

randlibsw.so: randlibsw.c
	$(CC) $(CFLAGS) -fPIC randlibsw.c -shared -o randlibsw.so
