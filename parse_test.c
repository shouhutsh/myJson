#include <stdio.h>
#include <stdlib.h>
#include "json.h"

int main(int args, char ** argv)
{
    int i;
    char * str = "{\"array\":[hello,world],\"key\":value}";
    struct JsonEntity ** entities = getEntities(str);

    for(i = 0; ADD_END != entities[i]; ++i){
        jsonPrint(entities[i]);
        delete_Json(entities[i]);
    }

    free(entities);
	return 0;
}
