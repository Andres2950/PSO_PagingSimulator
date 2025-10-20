

.PHONY: run build all compile



all: compile

run: compile
	./build/app/app

compile: 
	cmake --build build

build:
	cmake -B build -S .