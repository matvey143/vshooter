# Linux build
LDFLAGS = '-Wl,-rpath,$$ORIGIN' -L./ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all: vshooter

vshooter: vshooter.o enemies.o raylib.h libraylib.so
	$(CXX) -o vshooter vshooter.o enemies.o $(LDFLAGS)

vshooter.o: main.cpp raylib.h enemies.hpp
	$(CXX) -o vshooter.o -c $(CXXFLAGS) main.cpp

enemies.o: enemies.cpp raylib.h
	$(CXX) -o enemies.o -c $(CXXFLAGS) enemies.cpp

# Windows build
LD-W64 = -lgdi32 -lwinmm -static-libgcc -static-libstdc++ raylib.dll
w64: vshooter.exe

vshooter.exe: vshooter-w64.o enemies-w64.o raylib.h raylib.dll
	$(CXX) -o vshooter.exe vshooter-w64.o enemies-w64.o $(LD-W64)

vshooter-w64.o: main.cpp raylib.h enemies.hpp
	$(CXX) -o vshooter-w64.o -c $(CXXFLAGS) main.cpp

enemies-w64.o: enemies.cpp raylib.h
	$(CXX) -o enemies-w64.o -c $(CXXFLAGS) enemies.cpp