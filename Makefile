# Linux build
LDFLAGS = '-Wl,-rpath,$$ORIGIN' -L./ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all: vshooter

vshooter: vshooter.o enemies.o libraylib.so
	$(CXX) vshooter.o enemies.o $(LDFLAGS) -o vshooter

vshooter.o: main.cpp
	$(CXX) -o vshooter.o -c $(CXXFLAGS) main.cpp

enemies.o: enemies.cpp
	$(CXX) -o enemies.o -c $(CXXFLAGS) enemies.cpp

# Windows build
LD-W64 = -lgdi32 -lwinmm -static-libgcc -static-libstdc++ raylib.dll
w64: vshooter.exe

vshooter.exe: vshooter-w64.o enemies-w64.o raylib.h raylib.dll 
	$(CXX) vshooter-w64.o enemies-w64.o $(LD-W64) -o vshooter.exe

vshooter-w64.o: main.cpp
	$(CXX) -o vshooter-w64.o -c $(CXXFLAGS) main.cpp

enemies-w64.o: enemies.cpp
	$(CXX) -o enemies-w64.o -c $(CXXFLAGS) enemies.cpp