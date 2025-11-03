CC=gcc
CFLAGS=-Wall -O2

sysmon: main.o sysinfo.o
	$(CC) $(CFLAGS) -o sysmon main.o sysinfo.o
clean:
	rm -rf *.o sysmon