NAME = trans
CXX = g++
CFLAGS = -std=c++0x
LIBS = -lopencv_core -lopencv_highgui

SRCS = transformer.cc

all:
	$(CXX) -o $(NAME) $(CFLAGS) $(SRCS) $(LIBS)
clean:
	rm $(NAME) *.bin
