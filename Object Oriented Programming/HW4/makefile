all: pdadd.exe pdlist.exe pdremove.exe pdshow.exe
pdadd.exe: pdadd.o
	g++ -o pdadd.exe pdadd.o
pdadd.o: diary.o
	g++ -c pdadd.cpp
pdlist.exe: pdlist.o
	g++ -o pdlist.exe pdlist.o
pdlist.o: diary.o
	g++ -c pdlist.cpp
pdremove.exe: pdremove.o
	g++ -o pdremove.exe pdremove.o
pdremove.o: diary.o
	g++ -c pdremove.cpp
pdshow.exe: pdshow.o
	g++ -o pdshow.exe pdshow.o
pdshow.o: diary.o
	g++ -c pdshow.cpp
diary.o: diary.h
	g++ -c diary.cpp