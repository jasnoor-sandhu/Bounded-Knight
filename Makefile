build:
	g++ main.cpp frame.cpp `pkg-config --cflags sdl3` `pkg-config --libs sdl3` -o framebuffer

debug:
	g++ -g main.cpp frame.cpp `pkg-config --cflags --libs sdl3` -o framebuffer
C:
	gcc main.c `pkg-config --cflags sdl3` `pkg-config --libs sdl3` -o framebuffer

run:
	./framebuffer
