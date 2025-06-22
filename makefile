all:
	main
	start

main:
	g++ main.cpp tests/unity.c -std=c++17 -Wall -o main

start:
	./main.exe
