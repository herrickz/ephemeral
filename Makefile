

all:
	cmake -S . -B build
	cmake --build build -j$(sysctl -n hw.logicalcpu)

clean:
	rm -rf bin build

run: all
	./bin/2d_sidescroller
