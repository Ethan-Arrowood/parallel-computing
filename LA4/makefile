CC=mpicc
CPP=mpic++
LDC=mpicc
LDP=mpic++
LD_FLAGS = -lstdc++ -lopencv_core -lopencv_highgui -lopencv_imgproc
FLAGS= -I/usr/include/opencv
PROGC = la4.cx
OBJSC = la4.o imageToolsC.o

RM = /bin/rm

#all rule
all: $(PROGC) $(PROGF)

$(PROGC): $(OBJSC)
	$(LDP) $^ $(LD_FLAGS) -o $@

%C.o: %.c
	$(CC) $(FLAGS) -c $^ -o $@
%C.o: %.cpp
	$(CPP) $(FLAGS) -c $^ -o $@

#clean rule
clean:
	$(RM) -rf *.o $(PROGC) *.mod
