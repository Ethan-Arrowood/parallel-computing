all: ring-int ring-array ping-pong

ring-int: ring-int.c
	mpicc ring-int.c -o ring-int.x

ring-array: ring-array.c
	mpicc ring-array.c -o ring-array.x

ping-pong: ping-pong.c
	mpicc ping-pong.c -o ping-pong.x

run-int:
	mpiexec -np 2 --map-by ppr:1:node --hostfile $PBS_NODEFILE ./ring-int.x

run-array:
	mpiexec -np 2 --map-by ppr:1:node --hostfile $PBS_NODEFILE ./ring-array.x

run-ping-pong:
	mpiexec -np 2 --map-by ppr:1:node --hostfile $PBS_NODEFILE ./ping-pong.x

clean:
	rm *.x

clean_output:
	rm *.o*