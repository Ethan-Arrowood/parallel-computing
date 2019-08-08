FC = gfortran
LD = gfortran
CC=gcc
CPP=g++
LDC=gcc
LDP=g++
LD_FLAGS = -lstdc++ -lopencv_core -lopencv_highgui -lopencv_imgproc
FLAGS= -I/usr/include/opencv
PROGC = test.cx
OBJSC = imageTestC.o imageToolsC.o

PROGF = test.x
OBJSF = imageToolsC.o imgBindingF.o imageTestF.o 
RM = /bin/rm

#all rule
all: $(PROGC) $(PROGF)

$(PROGC): $(OBJSC)
	$(LDP) $^ $(LD_FLAGS) -o $@
$(PROGF): $(OBJSF)
	$(LD) $^ $(LD_FLAGS) -o $@
%C.o: %.c
	$(CC) $(FLAGS) -c $^ -o $@
%C.o: %.cpp
	$(CPP) $(FLAGS) -c $^ -o $@
%F.o: %.f90
	$(FC) $(FLAGS) -c $^ -o $@


#clean rule
clean:
	$(RM) -rf *.o $(PROGF) $(PROGC) *.mod
