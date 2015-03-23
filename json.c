#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"

void jsonPrint(struct JsonEntity * json){
    char * str = json->toString(json);
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

    int len = 0;
    char * str = NULL, * desc = NULL, * data = NULL;

    if(NULL != json->desc){
        int l = 3 + strlen(json->desc);
        desc = (char *) malloc(sizeof(l + 1));
        sprintf(desc, "\"%s\":", (char *)json->desc);
        len += l;
    }else{
        desc = (char *) calloc(1, sizeof(char));
    }
    if(NULL != json->data){
        struct JsonEntity * val = json->data;
        data = val->toString(val);
        len += strlen(data);
    }else{
        data = (char *) calloc(1, sizeof(char));
    }

    str = (char *) malloc(sizeof(len+3));

    sprintf(str, "{%s%s}", desc, data);

    free(desc);
    free(data);
    return str;
}

// TODO
char * arrayToString(struct JsonEntity * json){
    if(ARRAY_TYPE != json->state){
        exit(0);
    }

    return NULL;
}

struct JsonEntity * new_JsonEntity(int type){
    struct JsonEntity * json = (struct JsonEntity *) malloc(sizeof(struct JsonEntity));
    memset(json, 0, sizeof(*json));

    json->state = type;
    json->toString = JSON_toString[type];

    return json;
}




int main(void)
{
    struct JsonEntity * json = new_JsonEntity(OBJECT_TYPE);
    json->desc = "hello";
    struct JsonEntity * val = new_JsonEntity(ATOM_TYPE);
    val->data = "atom-world!";
    json->data = val;

    jsonPrint(json);
    
    free(val);
    free(json);
    return 0;
}
