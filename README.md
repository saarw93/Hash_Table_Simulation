Name of Student: Saar Weitzman

Generic Hash Table

-------------List of files------------

1. GenericHashTable.h - Header file contains the 2 structs (Table and Object) and all the functions we implemented in the c file
2. GenericHashTable.c - Code file written in c programming language, contains all the implementaions of the functions written in the header file.
3. makefile - File with the commands to compile the program and make the executeable file (the tester.o file).



-------------Description-------------

This program implements a data structure of an hash table, and the handling with collisions is done with linked lists.
The hash table support these three operations: 1. Adding an element 2. Removing an element 3. Searching for an element.
The hash table can be from the type of integers, or from the type of strings. Each type has little different implementation of the hash function.
In this program we limit the size of the linked lists that can be made. In case there is an object that the hash function sends to a full linked
list, the way this hash table deal with it is by duplicating the hash table size (i.e, expand the table).  
In case of expansion, all the existed objects of the table in entries 0-(n-1) will be mapped to the respective even entries in the new hash table.


--------Installation/Compilation of the program---------

open linux terminal, navigate to the folder containing "ex1" folder using the "cd" command (confirm it by using ls command)
incase you want to use the makefile, type make and the program will automaticily be compiled,
if you don't, type in the terminal:  gcc GenericHashTable.h GenericHashTable.c tester.c -o tester -Wvla -g -Wall.
very important: the name of the file that contains the main function to run this program needs to be called tester.c, for this instructions to work.


-----------Activation of the program--------------------

open linux terminal, navigate to tester executeable file location using "cd" command (confirm it using ls command) 
and type: ./tester . To run the program with valgrind, type: valgrind ./tester .


-------------Program data structures--------------------

struct Table: contains array of pointers to Objects (will be the hash table), original size of the table, current size of the table, d variable that says
the ratio between the current size of the table to its original size, list length which is the max length each list can be, and dType is the data type of the table.

struct Object: contains the data of the object and the pointer to the next object in the list 


----------program functions and their output---------------

Table* createTable(int size, int dType, int listLength);
output: pointer to the new created Table, otherwise, it returns NULL.

void freeTable(Table* table);
output: release all the allocated members of struct Table.

int add(Table* table, void* data);
output: the array index of the added data, otherwise, it return -1.

int removeObj(Table* table, void* data);
output: The array index of the removed data, otherwise, it returns -1. -1 is also returned in the case where there is no such object.

Object* search(Table* table, void* data);
output: pointer to the searched object. If there is no such object or in a case of an error, NULL is returned.

void printTable(Table* table);
output: prints the hash table in the format of: "[index]	object	-->	object	-->" etc.

Object* createObject(void* data);
output: pointer to the object that was created, or NULL in case of an error.

void freeObject(Object* obj, int type);
output: release the object "obj". In case of a "STR_TYPE" hash table, it release also the malloc of the string array itself.

int isEqual(int type, void* data1, void* data2);
output: 0 if data and data2 are equal or the ascii difference if they are not equal. If at least one of data1 and data2 in NULL, it returns -1.

int intHashFun(int* key, int origSize);
output: The hash value of an integer, which is the index where the key need to be putted in the hash table.

int strHashFun(char* key, int origSize);
output: hash value of an string, which is the index where the key need to be putted in the hash table.

int list_length(Object* object);
output: the length of the list which "object" is the head of it.

void add_object_to_list(Table* table, Object* ptr_new_object, int index);
output: The object will be added to the end of the list of "index".

int check_for_place(Table* table, int index);
output: the index where the list is not full. If the list or lists are full , returns -1

void freeTable_rec(Table* table, int j, Object* current_ptr);
output: gets to the last object in each list of the hash table indexes, and frees from the last object untill the first object in the list.
