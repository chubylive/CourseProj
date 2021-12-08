src = $(wildcard *.cpp *.c)
obj = $(src:.cpp=.o)
CXXFLAGS = -O1 -g -pthread -Wall
LDFLAGS =  -lm -lpthread

myprog: $(obj)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) myprog

