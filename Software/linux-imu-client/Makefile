
all: imu-client imu-test

clean:
	rm -f imu-client *.o *~ \#* *.lst

imu-client: imu-client.o sio_cmd.o
	$(CC) -o imu-client imu-client.o sio_cmd.o

imu-test: imu-test.o sio_cmd.o
	$(CC) -o imu-test imu-test.o sio_cmd.o
