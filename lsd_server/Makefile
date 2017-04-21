CXX=g++
CXXFLAGS=-g -std=c++11 -Wall
LDLIBS=-ljsoncpp -lmemcached -lmysqlclient -lpthread -levent
src=$(wildcard ./*.cpp)
obj=$(patsubst ./%.cpp, ./%.o, $(src))
target=lsd

$(target):$(obj)
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDLIBS)

%.o:$(src)
	$(CXX) -o $@ -c $<

clean:
	rm -f $(target) $(obj)
