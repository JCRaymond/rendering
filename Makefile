
LIBPNG_CFLAGS := $(libpng-config --cflags)

render: src/render.cpp include/*
	g++ -march=znver2 -std=c++20 -O3 -Wno-narrowing -fopenmp ./src/render.cpp -lpng -o render `libpng-config --cflags` -I./include

mandelbrot: src/mandelbrot.cpp include/*
	g++ -march=znver2 -std=c++20 -O3 -Wno-narrowing -fopenmp ./src/mandelbrot.cpp -lpng -o mandelbrot `libpng-config --cflags` -I./include

mandelbrot_zoom: src/mandelbrot_zoom.cpp include/*
	g++ -march=znver2 -std=c++20 -O3 -Wno-narrowing -fopenmp ./src/mandelbrot_zoom.cpp -lpng -o mandelbrot_zoom `libpng-config --cflags` -I./include
