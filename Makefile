produceConsume: produceAndConsume.o
		gcc -o produceConsume produceAndConsume.o -lpthread

produceAndConsume.o:
		gcc -c -lpthread -Wall -pedantic produceAndConsume.c

clean:
		rm produceAndConsume.o produceConsume