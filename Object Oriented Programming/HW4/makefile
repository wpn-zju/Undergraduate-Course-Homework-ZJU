all: diary.o pdadd pdremove pdshow pdlist
pdadd:
	g++ -o pdadd pdadd.cpp diary.o
pdremove:
	g++ -o pdremove pdremove.cpp diary.o
pdshow:
	g++ -o pdshow pdshow.cpp diary.o
pdlist:
	g++ -o pdlist pdlist.cpp diary.o
diary.o:
	g++ -c diary.cpp
