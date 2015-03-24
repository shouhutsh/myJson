#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"

char * toString(struct JsonEntity * json){
    return JSON_toString[json->state](json);
}

void jsonPrint(struct JsonEntity * json){
    char * str = JSON_toString[json->state](json);
    printf("%s", str);
    free(str);
}

char * atomToString(struct JsonEntity * json){
    if(ATOM_TYPE != json->state){
        exit(0);
    }

    int len = 0;
    char * str = NULL, * desc = NULL, * data = NULL;

    if(NULL != json->desc){
        int l = 3 + strlen(json->desc);
        desc = (char *) malloc(l + 1);
        sprintf(desc, "\"%s\":", (char *)json->desc);
        len += l;
    }else{
        desc = (char *) calloc(1, sizeof(char));
    }
    if(NULL != json->data){
        int l = strlen(json->data);
        data = (char *) malloc(l + 1);
        sprintf(data, "%s", (char *)json->data);
        len += l;
    }else{
        data = (char *) calloc(1, sizeof(char));
    }

    str = (char *) malloc(sizeof(len+1));

    sprintf(str, "%s%s", desc, data);

    free(desc);
    free(data);
    return str;
}

char * objectToString(struct JsonEntity * json){
    if(OBJECT_TYPE != json->state){
        exit(0);
    }

    int size = size(json);
    char * str = NULL, * data = NULL;

    while(size-- >= 0){
        data = toString(json->data);
        str = (char *) malloc(sizeof(3 + strlen(data)));
        sprintf(str, "{%s}", data);
    }
    return str;
}

// TODO
char * arrayToString(struct JsonEntity * json){
    if(ARRAY_TYPE != json->state){
        exit(0);
    }

    return NULL;
}

struct JsonEntity * new_JsonEntity(int type, void * desc, void * data){
    struct JsonEntity * json = (struct JsonEntity *) malloc(sizeof(struct JsonEntity));
    memset(json, 0, sizeof(*json));
    json->state = type;
    json->desc = desc;
    json->data = data;
    return json;
}

void delete_JsonEntity(struct JsonEntity * json){
    free(json->desc);
    if(ATOM_TYPE == json->state){
        free(json->data);
    }else{
        delete_JsonEntity(json->data);
    }
    free(json);
}

int main(void)
{
    char * hello = strcpy(malloc(sizeof(6)), "hello");
    char * world = strcpy(malloc(sizeof(6)), "world");
    int * len = (int *) malloc(sizeof(int));
    *len = 1;
    struct JsonEntity * atom = new_JsonEntity(ATOM_TYPE, hello, world);
    struct JsonEntity * json = new_JsonEntity(OBJECT_TYPE, len, atom);

    jsonPrint(json);
    
    delete_JsonEntity(json);
    return 0;
}
