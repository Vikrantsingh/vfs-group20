main.exe: freelist.o file_system.o file_operators.o main.o
	gcc ../bin/freelist.o ../bin/file_system.o ../bin/file_operators.o ../bin/main.o -o ../bin/main.exe

freelist.o:../src/freelist.c 
	gcc -c ../src/freelist.c -o ../bin/freelist.o

file_system.o:../src/file_system.c  
	gcc -c ../src/file_system.c -o ../bin/file_system.o


file_operators.o:../src/file_operators.c 
	gcc -c ../src/file_operators.c -o ../bin/file_operators.o

#file_operators.o:directory_operators.c directory_operators.h, file_operators.h,structure.h,freelist.h
#	gcc -c directory_operators.c

main.o:../src/main.c 
	gcc -c ../src/main.c -o ../bin/main.o




