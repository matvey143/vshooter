LDFLAGS = '-Wl,-rpath,$$ORIGIN' -L./ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all: vshooter

vshooter: vshooter.o raylib.h libraylib.so
	$(CXX) -o vshooter vshooter.o $(LDFLAGS)

vshooter.o: main.cpp raylib.h
	$(CXX) -o vshooter.o -c $(CXXFLAGS) main.cpp
