/**Author: Saar Weitzman
 * I.D: 204175137
 * Date: 13.12.18
 * **/


#include "GenericHashTable.h"


/***method to find the length of the list in a certain index of the hash table*/
int list_length(Object* object)
{
    if (object == NULL)
        return 0;

    int length = 1;
    while (object->next != NULL)
    {
        length++;
        object = object->next;
    }
    return length;
}


/**method that gets a pointer to object which need to be added to the end of the list of "index" **/
void add_object_to_list(Table* table, Object* ptr_new_object, int index)
{
    Object* object_ptr = table->arr[index]->next;
    if(object_ptr == NULL)  //there are no objects in the list of "index"
    {
        table->arr[index]->next = ptr_new_object;
        return;
    }

    while (object_ptr->next != NULL)
    {
        object_ptr = object_ptr->next;
    }
    object_ptr->next = ptr_new_object;
    return;
}


/**method to check if there is still place in the list of the input index. It returns the index where the list is not full. If the list or lists
 * are full , it returns -1**/
int check_for_place(Table* table, int index)
{
    Object* object_ptr = NULL;
    int i;
    for (i = index; i < index+table->d; i++)
    {
        object_ptr = table->arr[i]->next; //pointer to move on the list
        int listLength = list_length(object_ptr);
        if (listLength < table->listLength)
            return i;
    }
    return -1;  //all the lists of the index place in the table are full
}


/**
* returns the hash value of an integer, which is key mod origSize
*/
int intHashFun(int* key, int origSize)
{
    return (*key)%origSize;
}


/**
* returns the hash value of an string, which is m mod origSize, where m is the sum of the ascii value of all the
* character in key.
*/
int strHashFun(char* key, int origSize)
{
    char* ptr = key;
    int sum = 0;
    while ((*ptr) != '\0')
    {
        sum+= (int)*ptr;
        ptr++;
    }
    return sum%origSize;
}


/**
* The function gets the original size and the type of the data in the table elements.
* it initializes the Table struct members.
* On success, the function returns a pointer to the new created Table, otherwise, it return NULL.
*/
Table* createTable(int size, int dType, int listLength)
{
    if (listLength <= 0)
    {
        perror("Error: The input of the list length was negative\n");
        return NULL;
    }
    if (size <= 0)
    {
        perror("Error: The input of the size of the table was negative\n");
        return NULL;
    }


    Table* table_ptr = (Table*)malloc(sizeof(Table));
    if(table_ptr == NULL)
        return NULL;

    table_ptr->original_size = size;
    table_ptr->current_size = size;
    table_ptr->d = 1;
    table_ptr->listLength = listLength;
    table_ptr->dType = dType;
    table_ptr->arr = (Object**)malloc(sizeof(Object*)*size);
    if(table_ptr->arr == NULL)
        return NULL;

    int i;
    for (i = 0; i < table_ptr->original_size; i++)
    {
        table_ptr->arr[i] = createObject(NULL);
    }
    return table_ptr;
}


/**
* The function adds data to the hashtable (as described in the exe definition)
* On success, the function returns the array index of the added data, otherwise, it return -1.
*/
/**1.implement the hash function and gets from it the index that the new object is need to go to
 * 2.check if the list in the index of the hash table is not full before adding it to the list. If full-> duplicate the table
 *  */
int add(Table* table, void* data)
{
    if (table == NULL)
    {
        perror("Error: The input pointer of the table equals to NULL\n");
        return -1;
    }

    int index = 0;
    Object* ptr_new_object;

    if (table->dType == STR_TYPE)
    {
        char* str_data = (char*)malloc(sizeof(data)+1 ); //strlen(data)+1
        if (str_data == NULL)
            return -1;
        strcpy(str_data, (char*)data);

        ptr_new_object = createObject(str_data);

        index = table->d * strHashFun(str_data, table->original_size);
    }
    else  // (table->dType == INT_TYPE)
    {
        index = table->d * intHashFun((int *) data, table->original_size);
        ptr_new_object = createObject(data);
    }

    if(table->arr[index]->next == NULL)  //there are no objects in the list of "index"
    {
        table->arr[index]->next = ptr_new_object;
        return index;
    }

    /*if we got here, there are objects in the list of "index". Need to check if the list is full or not*/

    index = check_for_place(table, index);
    if ( index != -1)  //the new object can be inserted to the list of 'index' (the list is not full)
    {
        add_object_to_list(table, ptr_new_object, index);
        return index;
    }

    else   //the new object cannot be inserted to the list because it's full. need to duplicate the table
    {
        table->arr = (Object**)realloc(table->arr, sizeof(Object*)*table->current_size*2); //duplicate the size of the hash table
        if (table->arr == NULL)
            return -1;

        int i;
        for (i = table->current_size; i < (table->current_size*2); i++)
            table->arr[i] = createObject(NULL);

        /*need to move all the pointers of the object which where on the hash table before the realloc to their new location after the realloc*/
        int new_index = 0;


        for (i = table->current_size-1; i > 0 ; i--)
        {
//                if (i%table->d == 0)
//                {
//                    new_index = i * 2;  //the new index for the objects from the old array to be putted in the new array
//                    int j;
//                    for (j = i; j < i + table->d; j++)
//                    {
//                        table->arr[new_index]->next = table->arr[j]->next; //make the new index point to the objects in the list (if there are objects)
//                        new_index++;
//                        if (new_index != j) //for the case that i=0 so also new_index = 0
//                            table->arr[j]->next = NULL;
//                    }
//                }
            new_index = i * 2;
            table->arr[new_index]->next = table->arr[i]->next; //make the new index point to the objects in the list (if there are objects)
            if (new_index != i) //for the case that i=0 so also new_index = 0
                table->arr[i]->next = NULL;
        }
        table->d *= 2;  //update the d attribute
        if (table->dType == STR_TYPE)
            index = table->d * strHashFun((char*)data, table->original_size);
        else   //  (table->>dType == INT_TYPE)
            index = table->d * intHashFun((int*)data, table->original_size);
        index = check_for_place(table, index);   //get the real index of the list where the new object will be
        add_object_to_list(table, ptr_new_object, index);   //should return the index that points to the list
        table->current_size *= 2;   //update the size attribute
        return index;    //j here is the index of the list in the table, where the new object was inserted
    }
}


/**
* check the equality of the data of two objects. The implementation is different depending the type of the data.
* the function returns 0 if they are equal or some other value if they are not equal.
*/
int isEqual(int type, void* data1, void* data2)
{
    if (data1 == NULL && data2 == NULL)
        return 0;
    if ((data1 == NULL && data2 != NULL) || (data2 == NULL && data1 != NULL))
        return -1;

    if (type == INT_TYPE)
    {
        if((*(int*)data1) == (*(int*)data2))
            return 0;
        else
            return (*(int*)data1)-(*(int*)data2); //return the difference between the 2 object
    }
    else    //it's STR_TYPE
        return strcmp((char*)data1,(char*)data2);
}


/**
* The function search for an object that its data is equal to data and returns a pointer to that object.
* If there is no such object or in a case of an error, NULL is returned.
*/
/**
 * */
Object* search(Table* table, void* data)
{
    if (table == NULL)
    {
        perror("Error: The input pointer equals to NULL\n");
        return NULL;
    }

    int index = 0;
    if (table->dType == STR_TYPE)
        index = table->d * strHashFun((char*)data, table->original_size);
    else
        index = table->d * intHashFun((int*)data, table->original_size);

    int i;
    for (i = 0; i < table->d; i++)
    {
        Object* temp = table->arr[index+i];

        while (temp != NULL)
        {
            if (isEqual(table->dType, data, temp->data) == 0)
                return temp;
            else
                temp = temp->next;
        }
    }
    return NULL; // we did not found the data
}


/**
* This function creates an object and return the pointer to it or NULL if there is some error.
*/
Object* createObject(void* data)
{
    Object* object = (Object*)malloc(sizeof(Object));
    if (object == NULL)
        return NULL;
    object->data = data;
    object->next = NULL;

    return object;
}


/**
* The function removes the Object which its data equals to data, if there are more than one, it removes the first one.
* On success, the function returns the array index of the removed data, otherwise, it returns -1.
* -1 is also returned in the case where there is no such object.
*/
int removeObj(Table* table, void* data)
{
    if (table == NULL)
    {
        perror("Error: The input of the list length was negative\n");
        return -1;
    }

    int index = 0;
    if (table->dType == STR_TYPE)
        index = table->d * strHashFun((char*)data, table->original_size);
    else
        index = table->d * intHashFun((int*)data, table->original_size);

    int i;
    for (i = 0; i < table->d; i++)
    {
        Object* temp = table->arr[index+i];
        Object* prev_temp = table->arr[index+i];

        while (temp != NULL)
        {
            if (isEqual(table->dType, data, temp->data) == 0)
            {
                prev_temp->next = temp->next;
                freeObject(temp, table->dType);

                return (index+i);
            }

            else
            {
                prev_temp = temp;
                temp = temp->next;
            }
        }
    }
    return -1; // we did not found the data that needs to be removed
}


/**
* The function print the table (the format is in the exe definition)
*/
void printTable(Table* table)  //TODO prints only int for now
{
    if (table == NULL)
    {
        perror("Error: The input pointer of the table equals to NULL\n");
        return;
    }


    int i;
    for (i = 0; i < table->current_size ; i++)
    {
        Object* temp = table->arr[i]->next; //create a temp object to print with
        printf("[%d]\t", i);
        while (temp != NULL)
        {
            if (table->dType == INT_TYPE)
                printf("%d\t-->\t", *(int*)(temp->data));
            else
                printf("%s\t-->\t", (char*)temp->data);
            temp = temp->next;
        }
        printf("\n");
    }
}


/**
* This function frees an object,  note the in the case of STR_TYPE, the data
* of the object should also be freed.
*/
void freeObject(Object* obj, int type)
{
    if (obj == NULL)
    {
        perror("The input pointer of object equals to NULL\n");
        return;
    }

    if(type == STR_TYPE)
        free(obj->data);
    free(obj);
}


/**
* The function release all the allocated members of struct Table.
*/
void freeTable(Table* table)
{
    if (table == NULL)
    {
        perror("Error: The pointer to the table equals to NULL\n");
        return;
    }

    int i;
    for (i = table->current_size-1; i >= 0  ; i--)   //runs on all the indexes and free the objects in their lists
    {
        freeTable_rec(table, 0, table->arr[i], table->arr[i]);
    }

    free(table->arr);
    free(table);
}


/**recursive method which gets to the last object in each list of the hash table indexes, and frees from the last object untill the first object
 * in the list**/
void freeTable_rec(Table* table, int j, Object* current_ptr, Object* prev_ptr)
{
    if (j < table->listLength && current_ptr->next != NULL)  //in case we still did not reach the last object in the list
        freeTable_rec(table, j+1, current_ptr->next,current_ptr);

    freeObject(current_ptr,table->dType);  //current_ptr is the last object in the list if we got here

    /*if (current_ptr->next == NULL) //in case we are reached to the last object in the list
    {
        prev_ptr->next = NULL;
        freeObject(current_ptr,table->dType);

    }*/
}
