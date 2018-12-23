CXX=g++
CXXFLAGS=-O2 -Wall -Wextra
LDFLAGS=-s

TARGET=stl2pmd
OBJS=stl2pmd.o stlData.o pmdData.o

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)
