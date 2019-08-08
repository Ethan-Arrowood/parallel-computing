all: dotprod dotprodv

dotprod: dotprod.c
	mpicc dotprod.c -o dotprod.x

dotprodv: dotprodv.c
	mpicc dotprodv.c -o dotprodv.x

clean:
	rm *.x

clean_o:
	rm *.o*