
#include "GenericHashTable.h"


#define INT_TYPE 0
#define STR_TYPE 1


/**
* The function gets the original size and the type of the data in the table elements.
* it initializes the Table struct members.
* On success, the function returns a pointer to the new created Table, otherwise, it return NULL.
*/
Table* createTable(int size, int dType, int listLength)
{

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
* The function print the table (the format is in the exe definition)
*/
void printTable(Table* table)  //TODO prints only int for now
{
    if (table == NULL)
        return;

    int i;
    for (i = 0; i < table->current_size ; i++)
        {
            Object* temp = table->arr[i]->next; //create a temp object to print with
            printf("[%d]", i);
            while (temp != NULL)
            {
                printf("\t%d\t-->\t", *(int*)(temp->data));
                    temp = temp->next;
            }
            printf("\n");
        }
}

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
    return 0;  //all the lists of the index place in the table are full
}



/**
* The function adds data to the hashtable (as described in the exe definition)
* On success, the function returns the array index of the added data, otherwise, it return -1.
*/
/**1.implement the hash function and gets from it the index that the new object is need to go to
 * 2.check if the list in the index of the hash table is not full before adding it to the list. If full-> duplicate the table
 * 3. */
int add(Table* table, void* data)
{
    if (table == NULL)
        return -1; //the pointer to table equals to NULL

    //TODO check if the type of data is int or string. if the type of the table is int and data is string return error
    int index = 0;
    Object* ptr_new_object = createObject(data);

    if (table->dType == INT_TYPE)
    {
        index = table->d * intHashFun((int*)data, table->original_size);


        if(table->arr[index]->next == NULL)  //there are no objects in the list of "index"
        {
            table->arr[index]->next = ptr_new_object;
            return index;
        }

        /*if we got here, there are object in the list of "index". Need to check if the list is full or not*/


        /**need to do a loop here**/

        index = check_for_place(table, index);
        if ( index != 0)  //the new object can be inserted to the list of 'index' (the list is not full)
        {
            add_object_to_list(table, ptr_new_object, index);
            //while (object_ptr->next != NULL)
            //{
            //    object_ptr = object_ptr->next;
            //}
            //object_ptr->next = ptr_new_object;
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


            //need to move all the pointers of the object which where on the hash table before the realloc to their new location after the realloc
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
            index = table->d * intHashFun((int*)data, table->original_size);
//            int j;
//            for (j = index; j < index+table->d; j++)
//            {
//                object_ptr = table->arr[j]->next; //pointer to move on the list
//                listLength = list_length(object_ptr);
//                if (listLength < table->listLength)
//                {
            index =  check_for_place(table, index); //get the real index of the list where the new object will be
                    add_object_to_list(table, ptr_new_object, index); //should return the index that points to the list
                    table->current_size *= 2;  //update the size attribute
                    return index; //j here is the index of the list in the table, where the new object was inserted
               // }
            //}



            //update here the current size
        }

    }




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
* check the equality of the data of two objects. The implementation is different depending the type of the data.
* the function returns 0 if they are equal or some other value if they are not equal.
*/
int isEqual(int type, void* data1, void* data2)
{
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
* This function frees an object,  note the in the case of STR_TYPE, the data
* of the object should also be freed.
*/
void freeObject(Object* obj, int type)
{
    if(type == STR_TYPE)
        free(obj->data);
    free(obj);
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
* The function search for an object that its data is equal to data and returns a pointer to that object.
* If there is no such object or in a case of an error, NULL is returned.
*/
/**
 * */
Object* search(Table* table, void* data)
{
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
* The function removes the Object which its data equals to data, if there are more than one, it removes the first one.
* On success, the function returns the array index of the removed data, otherwise, it returns -1.
* -1 is also returned in the case where there is no such object.
*/
int removeObj(Table* table, void* data)
{
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
* The function release all the allocated members of struct Table.
*/
void freeTable(Table* table)
{

   freeTable_1(table, table->current_size-1, 0, table->arr[table->current_size-1], table->arr[table->current_size-1], table->listLength);

}

void freeTable_1(Table* table, int i, int j, Object* currnet_ptr, Object* prev_ptr, int listLength)
{
    if (i == 0)  //in case all the lists are empty
    {
        int k;
        for (int k = 0; k < table->current_size; k++)
        {
            freeObject(table->arr[k], table->dType);
        }

        free(table->arr);
        free(table);
        return;
    }


    if (j < listLength)  //in case the list still have objects
    {
        if (currnet_ptr->next != NULL)
            freeTable_1(table, i, j+1, currnet_ptr->next,currnet_ptr, table->listLength);
        else  //in case the list is empty, but were are not the end of the array
            freeTable_1(table, i-1, 0, table->arr[i-1], table->arr[i-1], table->listLength);
    }

    if (j == 0)
        freeTable_1(table, i-1, 0, table->arr[i-1], table->arr[i-1], table->listLength);

    if (j != 0)  //in case we are in the end of the list
    {
        freeObject(currnet_ptr,table->dType);
        prev_ptr->next = NULL;
        printTable(table);
    printf("\n\n");
        return;
    }

}


//TODO need to do the freeTable function and string