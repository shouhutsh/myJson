#include <stdio.h>
#include <stdlib.h>
#include "json.h"

int main(int args, char ** argv)
{
    int i;
    char * str = "{\n \"array\" : [\n hello, world \n],\n \"key\" : value \n}";
    char * temp = dislodgeWhitespace(str);
    struct JsonEntity ** entities = getEntities(temp);

    for(i = 0; ADD_END != entities[i]; ++i){
        jsonPrint(entities[i]);
        delete_Json(entities[i]);
    }

    free(entities);
    free(temp);
	return 0;
}
