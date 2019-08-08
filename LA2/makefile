all: mpi-mvp

mpi-mvp: mpi-mvp.c
	mpicc mvp.c mpi-mvp.c -o mpi-mvp.x

run:
	mpiexec -np 2 --map-by ppr:1:node --hostfile $PBS_NODEFILE ./mpi-mvp.x

clean:
	rm *.x

clean_o:
	rm *.o*