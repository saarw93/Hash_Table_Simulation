//
// Created by student on 12/3/18.
//

#include "GenericHashTable.h"

int main() {
//    int num =7;
//
//    printf("%d",intHashFun(&num,4));
//
//    printf("\n\n");
//    char* str= "cd";
//    printf("%d",strHashFun(str, 4));
//
//    printf("\n\n");

     /*Table* table = createTable(5,INT_TYPE, 3);

     int num1 = 1, num2 = 6, num3 = 11, num4 = 16,num5 = 21, num6 = 26, num7 = 1, num8 = 4;
     int res1 = isEqual(0,&num1,&num2);
     printf("%d",res1);

     printf("\n\n");
     char str_space[] = "\n#######################################\n";

     add(table,&num1);
     //printTable(table);
     //printf("%s", str_space);
     add(table,&num2);
     //printTable(table);
     //printf("%s", str_space);
     add(table,&num3);
     printTable(table);
     printf("%s", str_space);
     add(table,&num4);
     add(table,&num5);
     add(table, &num6);
     printTable(table);
     printf("%s", str_space);
     add(table, &num7);
     add(table, &num8);
     printTable(table);*/


    int bla[] = {1, 1, 1, 1};

    Table *table = createTable(4, 0, 2);
    add(table, &bla[0]);
    add(table,&bla[1]);
    add(table,&bla[2]);
    add(table,&bla[3]);
    printTable(table);
    char str[]= {"################"};
    printf("%s\n", str);
    freeTable(table);
}