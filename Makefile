src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)
CXXFLAGS = -O1 -g -lpthread -Wall
LDFLAGS = -lGL -lglut -lpng -lz -lm 

myprog: $(obj)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) myprog

