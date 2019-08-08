all: integrate mpi-integrate

integrate: integrate.c
	gcc integrate.c -o integrate.x

mpi-integrate: mpi-integrate.c
	mpicc mpi-integrate.c -o mpi-integrate.x

run-integrate:
	./integrate.x

run-mpi-integrate:
	mpiexec -np 4 --map-by ppr:1:node --hostfile $PBS_NODEFILE ./mpi-integrate.x

clean: 
	rm *.x
