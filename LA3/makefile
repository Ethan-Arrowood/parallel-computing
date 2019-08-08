CC = tau_cc.sh
FLAGS = -std=c11

all: LA3 LA3P

LA3:
	tau_cc.sh -std=c11 LA3.c -o LA3.x -lm

LA3P:
	tau_cc.sh -std=c11 LA3P.c -o LA3P.x -lm

%.o: %.c
	$(CC) $(FLAGS) -c $<

clean:
	rm -rf *.o $(PROG) *.mod

