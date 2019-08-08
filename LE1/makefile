CC= mpicc
LDC= mpicc
LDC_FLAGS= -lm -fopenmp
FLAGS = -fopenmp
HEATC = heatFTCS.cx
INTC=integral.cx
RM= /bin/rm

HEATOBJSC = heatFTCSC.o
INTOBJSC=integralC.o

#all rule

all: $(HEATC) $(INTC) $(INTCP)

$(HEATC): $(HEATOBJSC)
	$(LDC) $(HEATOBJSC) $(LDC_FLAGS) -o $(HEATC)
$(INTC): $(INTOBJSC)
	$(LDC) $(INTOBJSC) $(LDC_FLAGS) -o $(INTC)

%C.o: %.c
	$(CC) $(FLAGS) -c $^ -o $@

#clean rule
clean:
	$(RM) -rf *.o $(HEATF) $(HEATC) $(INTF) $(INTC) *.x *.mod

