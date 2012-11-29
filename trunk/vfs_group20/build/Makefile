vfs_group20: freelist.o file_system.o file_operators.o directory_operators.o  hashtable.o bst_hash_chain.o bst_full_path.o nary.o validation.o string_operations.o vfsdriver.o
	gcc ../bin/freelist.o ../bin/file_system.o ../bin/file_operators.o ../bin/directory_operators.o  ../bin/hashtable.o ../bin/bst_hash_chain.o ../bin/bst_full_path.o ../bin/nary.o   ../bin/validation.o ../bin/string_operations.o ../bin/vfsdriver.o -o ../bin/vfs_group20

freelist.o:../src/freelist.c 
	gcc -c ../src/freelist.c -o ../bin/freelist.o

file_system.o:../src/file_system.c  
	gcc -c ../src/file_system.c -o ../bin/file_system.o


file_operators.o:../src/file_operators.c 
	gcc -c ../src/file_operators.c -o ../bin/file_operators.o

directory_operators.o:../src/directory_operators.c
	gcc -c ../src/directory_operators.c -o ../bin/directory_operators.o


hashtable.o: ../src/hashtable.c
	gcc -c ../src/hashtable.c -o ../bin/hashtable.o

bst_hash_chain.o: ../src/bst_hash_chain.c
	gcc -c ../src/bst_hash_chain.c -o ../bin/bst_hash_chain.o

bst_full_path.o: ../src/bst_full_path.c
	gcc -c ../src/bst_full_path.c -o ../bin/bst_full_path.o


nary.o: ../src/nary.c
	gcc -c ../src/nary.c -o ../bin/nary.o


validation.o: ../src/validation.c
	gcc -c ../src/validation.c -o ../bin/validation.o

string_operations.o: ../src/string_operations.c
	gcc -c ../src/string_operations.c -o ../bin/string_operations.o

#main.o:../src/main.c 
#	gcc -c ../src/main.c -o ../bin/main.o

vfsdriver.o:../test/vfsdriver.c 
	gcc -c ../test/vfsdriver.c -o ../bin/vfsdriver.o



