.PHONY: all clean run

all:
	cmake -S . -B build -G "MinGW Makefiles"
	cmake --build build

clean:
	cmake --build build --target clean

run: all
	./build/stage4.exe