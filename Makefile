# Linux build
LDFLAGS = '-Wl,-rpath,$$ORIGIN' -L./ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all: vshooter

vshooter: vshooter.o enemies.o globalVar.o raylib.h libraylib.so
	$(CXX) -o vshooter vshooter.o enemies.o globalVar.o $(LDFLAGS)

vshooter.o: main.cpp raylib.h enemies.hpp globalVariables.hpp
	$(CXX) -o vshooter.o -c $(CXXFLAGS) main.cpp

enemies.o: enemies.cpp raylib.h globalVariables.hpp
	$(CXX) -o enemies.o -c $(CXXFLAGS) enemies.cpp

globalVar.o: raylib.h
	$(CXX) -o globalVar.o -c $(CXXFLAGS) globalVariables.cpp

# Windows build
LD-W64 = -lgdi32 -lwinmm -static-libgcc -static-libstdc++ raylib.dll
w64: vshooter.exe

vshooter.exe: vshooter-w64.o enemies-w64.o globalVar-w64.o raylib.h raylib.dll
	$(CXX) -o vshooter.exe vshooter-w64.o enemies-w64.o globalVar-w64.o $(LD-W64)

vshooter-w64.o: main.cpp raylib.h enemies.hpp globalVariables.hpp
	$(CXX) -o vshooter-w64.o -c $(CXXFLAGS) main.cpp

enemies-w64.o: enemies.cpp raylib.h globalVariables.hpp
	$(CXX) -o enemies-w64.o -c $(CXXFLAGS) enemies.cpp

globalVar-w64.o: raylib.h
	$(CXX) -o globalVar-w64.o -c $(CXXFLAGS) globalVariables.cpp