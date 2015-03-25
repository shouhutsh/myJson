#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"

int main(void)
{
    struct JsonEntity * object = new_Object();

    add_Children(object,
        add_Children(new_Array(),
            new_Atom("hello"),
            new_Atom("world"),
                     ADD_END),
        new_Pear("key", "value"),
                 ADD_END);

    jsonPrint(object);

    delete_Json(object);
    return 0;
}
