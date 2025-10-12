

.PHONY: run build all compile



all: compile

compile: 
	cmake --build build

run:
	./build/app/app

build:
	cmake -B build -S .