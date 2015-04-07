#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"

int main(void)
{
    char * str;
    struct JsonEntity * object = new_Object();

    add_Children(object,
        new_Pair("array",
            add_Children(new_Array(),
                new_Atom("hello"),
                new_Atom("world"),
                ADD_END)),
        new_Pair("key",
            new_Atom("value")),
        ADD_END);

    str = getJsonString(object);
    prettyPrint(str);

    delete_Json(object);
    free(str);
    return 0;
}
