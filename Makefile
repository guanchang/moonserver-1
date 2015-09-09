src = $(wildcard *.cpp)
dir = $(notdir $(src))
obj = $(patsubst %.cpp,%.o,$(dir))
deps = $(patsubst %.cpp,%.d,$(dir))
LIBS = -ldl -lpthread -lboost_system -ggdb -rdynamic -lpython2.5
CFLAGS = -I/usr/include/python2.5
STATIC_LIB = /usr/local/lib/liblog4cplus.a

all: $(obj) $(deps)
	g++ -o moon_server $(obj) $(STATIC_LIB) $(CFLAGS) $(LIBS) 
	cp moon_server ./sample/ -f

$(deps): %.d: %.cpp
	rm -f $@
	g++ -MM $< -ggdb >> $@ 

sinclude $(deps)

$(obj): %.o: %.cpp
	g++ $< -c -o $@ $(CFLAGS) -g

clean:
	-rm -rf *.d
	-rm -rf *.o
	-rm moon_server
