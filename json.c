#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"

char * (*JSON_toString[JSON_TYPE_COUNT])(struct JsonEntity *) =
{atomToString, objectToString, arrayToString};

char * toString(struct JsonEntity * json){
    return JSON_toString[json->type](json);
}

void jsonPrint(struct JsonEntity * json){
    char * str = JSON_toString[json->type](json);
    printf("%s", str);
    free(str);
}

int inc(void * data){
    return (*((int *) data))++;
}

int sizeOf(struct JsonEntity * json){
    if(NULL == json){
        exit(0);
    }

    if(NULL == json->desc){
        return 0;
    }
    return *((int *) json->desc);
}

char * atomToString(struct JsonEntity * json){
    if(NULL == json || ATOM_TYPE != json->type || NULL == json->data){
        exit(0);
    }

    int len = 0, l;
    char * str = (char *) calloc(1, sizeof(char));

    if(NULL != json->desc){
        l = 3 + strlen(json->desc);
        str = realloc(str, len + l + 1);
        sprintf(str+len, "\"%s\":", (char *)json->desc);
        len += l;
    }

    l = strlen(json->data);
    str = realloc(str, len + l + 1);
    sprintf(str+len, "%s", (char *)json->data);

    return str;
}

char * objectToString(struct JsonEntity * json){
    if(NULL == json || OBJECT_TYPE != json->type){
        exit(0);
    }

    int i, len = 2;
    int size = sizeOf(json);
    char * str = (char *) calloc(len + 1, sizeof(char));
    struct JsonEntity ** items = json->data;

    str[0] = '{';
    for(i = 0; i < size; i++){
        char * data = toString(items[i]);
        int l = strlen(data);
        str = realloc(str, len + l + 2);
        sprintf(str+len-1, "%s,", data);
        len += l;
    }
    str[len-1] = '}';
    return str;
}

char * arrayToString(struct JsonEntity * json){
    if(NULL == json || ARRAY_TYPE != json->type){
        exit(0);
    }

    int i, len = 2;
    int size = sizeOf(json);
    char * str = (char *) calloc(len + 1, sizeof(char));
    struct JsonEntity ** items = json->data;

    str[0] = '[';
    for(i = 0; i < size; i++){
        char * data = toString(items[i]);
        int l = strlen(data);
        str = realloc(str, len + l + 2);
        sprintf(str+len-1, "%s,", data);
        len += l;
    }
    str[len-1] = ']';
    return str;
}

struct JsonEntity * add_children(struct JsonEntity * root, struct JsonEntity * child){
    if(ATOM_TYPE == root->type){
        exit(0);
    }

    if(NULL == root->desc){
        root->desc = calloc(1, sizeof(int));
        root->data = calloc(1, sizeof(void **));
    }

    inc(root->desc);
    root->data = realloc(root->data, sizeof(void **) * sizeOf(root));

    ((struct JsonEntity **)root->data)[sizeOf(root)-1] = child;
    return root;
}

struct JsonEntity * new_JsonEntity(int type, void * desc, void * data){
    struct JsonEntity * json = (struct JsonEntity *) calloc(1, sizeof(struct JsonEntity));
    json->type = type;
    json->desc = desc;
    json->data = data;
    return json;
}

void delete_JsonEntity(struct JsonEntity * json){
    if(ATOM_TYPE == json->type){
        free(json->data);
    }else{
        int i, size = sizeOf(json);
        struct JsonEntity ** items = json->data;

        for(i = 0; i < size; i++){
            delete_JsonEntity(items[i]);
        }
    }
    free(json->desc);
    free(json);
}
