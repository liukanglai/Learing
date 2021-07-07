clean:
	rm -rf summa
summa:clean
	mpicc -g -Wall -O3 -fopenmp -std=c11 main.c -o summa -lm
run:summa
	mpirun -np 16 ./summa 16 256 256 256


