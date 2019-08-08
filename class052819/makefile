all: ex

ex: ex.c
	mpicc ex.c -o ex.x

run:
	mpiexec -np 2 --map-by ppr:1:node --hostfile $PBS_NODEFILE ./ex.x

clean:
	rm *.x