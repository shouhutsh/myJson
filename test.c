#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"

int main(void)
{
    char * hello = strcpy(malloc(sizeof(6)), "hello");
    char * world = strcpy(malloc(sizeof(6)), "world");
    char * array1= strcpy(malloc(sizeof(7)), "array1");
    char * array2= strcpy(malloc(sizeof(7)), "array2");
    struct JsonEntity * atom = new_JsonEntity(ATOM_TYPE, hello, world);
    struct JsonEntity * json = new_JsonEntity(OBJECT_TYPE, NULL, NULL);

    add_children(json, atom);
    add_children(json,
        add_children(
            add_children(
                new_JsonEntity(ARRAY_TYPE, NULL, NULL),
                new_JsonEntity(ATOM_TYPE, NULL, array1)),
                new_JsonEntity(ATOM_TYPE, NULL, array2)));

    jsonPrint(json);

    delete_JsonEntity(json);
    return 0;
}
