CC	= gcc
CFLAGS	= -Wall -O2

all: consumer producer tester caching

caching.o: caching.c
	$(CC) -c caching.c

caching: caching.o
	$(CC) $(CFLAGS) -o caching caching.o

consumer.o: consumer.c
	$(CC) -c consumer.c

consumer: consumer.o
	$(CC) $(CFLAGS) -o consumer consumer.o -pthread

producer.o: producer.c
	$(CC) -c producer.c

producer: producer.o
	$(CC) $(CFLAGS) -o producer producer.o -pthread


tester.o: tester.c
	$(CC) -c tester.c

tester: tester.o
	$(CC) $(CFLAGS) -o tester tester.o -pthread

clean:
	rm -f consumer producer tester caching *.o

