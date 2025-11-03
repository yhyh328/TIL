CC=gcc
CFLAGS=-Wall -O2

all: sensor_sim client

sensor_sim: sensor_sim.c
	$(CC) $(CFLAGS) -o sensor_sim sensor_sim.c

client: client.c
	$(CC) $(CFLAGS) -o client client.c

clean:
	rm -f sensor_sim client
