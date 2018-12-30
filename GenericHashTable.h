/**Author: Saar Weitzman
 * Date: 15.12.18
 * Hash Table Implementation
 * **/

#ifndef EX1_GENERICHASHTABLE_H
#define EX1_GENERICHASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_TYPE 0
#define STR_TYPE 1

#define ERR (-1)    //return ERR in case of an error during a run of a function that returns integer

typedef struct Object {
    void* data;
    struct Object* next; //points to the next object in the linked list
}Object;

typedef struct Table {
    Object** arr;
    int original_size;
    int current_size;
    int d;
    int listLength;
    int dType;
}Table;


/**
* The function gets the original size and the type of the data in the table elements.
* it initializes the Table struct members.
* On success, the function returns a pointer to the new created Table, otherwise, it return NULL.
**/
Table* createTable(int size, int dType, int listLength);

/**
* The function release all the allocated members of struct Table.
**/
void freeTable(Table* table);

/**
* The function adds data to the hashtable (as described in the exe definition)
* On success, the function returns the array index of the added data, otherwise, it return -1.
**/
int add(Table* table, void* data);

/**
* The function removes the Object which its data equals to data, if there are more than one, it removes the first one.
* On success, the function returns the array index of the removed data, otherwise, it returns -1.
* -1 is also returned in the case where there is no such object.
**/
int removeObj(Table* table, void* data);

/**
* The function search for an object that its data is equal to data and returns a pointer to that object.
* If there is no such object or in a case of an error, NULL is returned.
**/
Object* search(Table* table, void* data);

/**
* The function print the table (the format is in the exe definition)
**/
void printTable(Table* table);

/**
* This function creates an object and return the pointer to it or NULL if there is some error.
**/
Object* createObject(void* data);

/**
* This function frees an object,  note the in the case of STR_TYPE, the data
* of the object should also be freed.
**/
void freeObject(Object* obj, int type);

/**
* check the equality of the data of two objects. The implementation is different depending the type of the data.
* the function returns 0 if they are equal or some other value if they are not equal.
**/
int isEqual(int type, void* data1, void* data2);

/**
* returns the hash value of an integer, which is key mod origSize
**/
int intHashFun(int* key, int origSize);

/**
* returns the hash value of an string, which is m mod origSize, where m is the sum of the ascii value of all the
* character in key.
**/
int strHashFun(char* key, int origSize);


/**Private functions**/

/**method to find the length of the list in a certain index of the hash table **/
int list_length(Object* object);

/**method that gets a pointer to object which need to be added to the end of the list of "index" **/
void add_object_to_list(Table* table, Object* ptr_new_object, int index);

/** method to check if there is still place in the list of the input index. It returns the index where the list is not full. If the list or lists
 * are full , it returns -1 **/
int check_for_place(Table* table, int index);

/** recursive method which gets to the last object in each list of the hash table indexes, and frees from the last object untill the first object
 * in the list **/
void freeTable_rec(Table* table, int j, Object* current_ptr);

#endif //EX1_GENERICHASHTABLE_H
