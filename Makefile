all: simulation1 simulation2

simulation1: produceAndConsume.o 
		gcc -Wall -lpthread produceAndConsume.o -o simulation1

simulation2: philosopherDining.o
		gcc -Wall -lpthread philosopherDining.o -o simulation2 

produceAndConsume.o:
		gcc -c -lpthread -pedantic produceAndConsume.c 

philosopherDining.o: 
		gcc -c -lpthread -pedantic philosopherDining.c

clean:
		rm produceAndConsume.o philosopherDining.o simulation1 simulation2