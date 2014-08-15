NAME = 2bin 
CXX = g++
CFLAGS = -std=c++0x
LIBS = -lopencv_core -lopencv_highgui -lopencv_imgproc

SRCS = binner.cc

all:
	$(CXX) -o $(NAME) $(CFLAGS) $(SRCS) $(LIBS)
clean:
	rm $(NAME) *.bin
