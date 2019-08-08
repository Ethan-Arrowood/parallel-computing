export TAU_MAKEFILE=/opt/sw/tau/x86_64/lib/Makefile.tau-papi-mpi-pdt

CC = mpicc
TCC = tau_cc.sh
CPP = mpic++
TCPP = tau_cxx.sh
LDC = mpicc
TLDC = tau_cc.sh
LD_FLAGS = -lm -fopenmp -lstdc++ -lopencv_core -lopencv_highgui -lopencv_imgproc
FLAGS = -fopenmp -Wall
CPPFLAGS= -I/usr/include/opencv $(FLAGS)
PROG = mandelbrot.cx
RM= /bin/rm


#all rule

OBJS=$(PROG:%.cx=%.o)
TOOLS=imageTools.o

all: $(PROG)

$(PROG): $(OBJS) $(TOOLS)
	$(TLDC)  $^ $(LD_FLAGS) -o $@

%.o: %.c
	$(TCC) $(FLAGS) -c $^ -o $@
%.o: %.cpp
	$(TCPP) $(CPPFLAGS) -c $^ -o $@

#clean rule
clean:
	$(RM) -rf *.o *.x *.cx *.mod
