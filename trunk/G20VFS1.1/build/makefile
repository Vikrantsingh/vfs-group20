main.exe: freelist.o file_system.o file_operators.o directory_operators.o ui.o hashtable.o nary.o validation.o string_operations.o main.o
	gcc bin/freelist.o bin/file_system.o bin/file_operators.o bin/directory_operators.o bin/ui.o  bin/hashtable.o bin/nary.o   bin/validation.o bin/string_operations.o bin/main.o -o bin/main.exe

freelist.o:src/freelist.c 
	gcc -c src/freelist.c -o bin/freelist.o

file_system.o:src/file_system.c  
	gcc -c src/file_system.c -o bin/file_system.o


file_operators.o:src/file_operators.c 
	gcc -c src/file_operators.c -o bin/file_operators.o

directory_operators.o:src/directory_operators.c
	gcc -c src/directory_operators.c -o bin/directory_operators.o

ui.o: src/ui.c
	gcc -c src/ui.c -o bin/ui.o


hashtable.o: src/hashtable.c
	gcc -c src/hashtable.c -o bin/hashtable.o

nary.o: src/nary.c
	gcc -c src/nary.c -o bin/nary.o


validation.o: src/validation.c
	gcc -c src/validation.c -o bin/validation.o

string_operations.o: src/string_operations.c
	gcc -c src/string_operations.c -o bin/string_operations.o

main.o:src/main.c 
	gcc -c src/main.c -o bin/main.o




