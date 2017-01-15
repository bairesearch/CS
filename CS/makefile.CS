
CC=g++
#CFLAGS=-c -Wall
#CFLAGS=-c -W --no-warn
#CFLAGS=-c -W --no-warn -I/usr/X11R6/include/ -L/usr/X11R6/lib -lX11 -lXmu -lglut -lGL -lG
CFLAGS=-c -W --no-warn -I/usr/X11R6/include/
#LDFLAGS=-W --no-warn 
LDFLAGS=-W --no-warn -I/usr/X11R6/include/ -L/usr/X11R6/lib -lX11 -lXmu -lglut -lGL -lGLU
SOURCES= CSmain.cpp CSexecflow.cpp CSdataflow.cpp CSpreprocessorfilter.cpp CSreferenceClass.cpp CSoperations.cpp CSdraw.cpp RTscene.cpp  RToperations.cpp  RTparser.cpp  RTppm.cpp  RTraytracer.cpp RTreferenceManipulation.cpp RTpixelMaps.cpp RTviewinfo.cpp LDsprite.cpp LDopengl.cpp LDparser.cpp LDreferenceManipulation.cpp LDreferenceClass.cpp LDsvg.cpp SHAREDglobalDefs.cpp SHAREDvector.cpp SHAREDvars.cpp XMLParserClass.cpp XMLrulesClass.cpp 



OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=OpenCS.exe

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

