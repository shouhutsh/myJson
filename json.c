#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "json.h"

#define JSON_TYPE_COUNT	4

char * atomToString(struct JsonEntity *);
char * pearToString(struct JsonEntity *);
char * objectToString(struct JsonEntity *);
char * arrayToString(struct JsonEntity *);

#define type(json) ((json)->type)
#define desc(json) ((json)->desc)
#define data(json) ((json)->data)
#define inc(json) (desc(json).size++)
#define sizeOf(json) (desc(json).size)

char * (*JSON_toString[JSON_TYPE_COUNT])(struct JsonEntity *) = {atomToString, pearToString, objectToString, arrayToString};

char * getString(struct JsonEntity * json){
    return JSON_toString[type(json)](json);
}

void jsonPrint(struct JsonEntity * json){
    char * str = JSON_toString[type(json)](json);
    printf("%s", str);
    free(str);
}

char * atomToString(struct JsonEntity * json){
    if(NULL == json || ATOM_TYPE != type(json) || NULL == data(json).value){
        exit(0);
    }

    int len = strlen(data(json).value);
    char * str = (char *) calloc(len+1, sizeof(char));

    sprintf(str, "%s", data(json).value);
    return str;
}

char * pearToString(struct JsonEntity * json){
    if(NULL == json || PEAR_TYPE != type(json) || NULL == desc(json).key || NULL == data(json).value){
        exit(0);
    }

    int len = strlen(desc(json).key) + strlen(data(json).value);
    char * str = (char *) calloc(len+4, sizeof(char));

    sprintf(str, "\"%s\":%s", desc(json).key, data(json).value);
    return str;
}

char * OA_ToString(struct JsonEntity * json, char left, char right){
    int size = sizeOf(json);
    char *str = (char *)malloc(3);
    if(0 == size){
        str[0] = left, str[1] = right;
    }else{
        int i, len = 1;
        for(i = 0; i < size; i++){
            char * data = getString(data(json).items[i]);
            int l = strlen(data);
            str = realloc(str, len + l + 2);
            sprintf(str+len, "%s,", data);
            len += l + 1;
            free(data);
        }
        str[0] = left, str[len-1] = right;
    }
    return str;
}

char * objectToString(struct JsonEntity * json){
    if(NULL == json || OBJECT_TYPE != type(json)){
        exit(0);
    }

    char * str = OA_ToString(json, '{', '}');

    return str;
}

char * arrayToString(struct JsonEntity * json){
    if(NULL == json || ARRAY_TYPE != type(json)){
        exit(0);
    }

    char * str = OA_ToString(json, '[', ']');

    return str;
}

struct JsonEntity * add_Children(struct JsonEntity * root, struct JsonEntity * child, ...){
    if(NULL == root || NULL == child || ATOM_TYPE == type(root)){
        exit(0);
    }

    va_list ap;
    va_start(ap, child);

    while(ADD_END != child){
        inc(root);
        data(root).items = realloc(data(root).items, sizeof(void **) * sizeOf(root));
        data(root).items[sizeOf(root)-1] = child;

        child = va_arg(ap, struct JsonEntity *);
    }
    va_end(ap);
    return root;
}

struct JsonEntity * new_Atom(const char * value){
    struct JsonEntity * json = (struct JsonEntity *) calloc(1, sizeof(struct JsonEntity));
    type(json) = ATOM_TYPE;
    data(json).value = strcpy(malloc(strlen(value)+1), value);
    return json;
}

struct JsonEntity * new_Pear(const char * key, const char * value){
    struct JsonEntity * json = (struct JsonEntity *) calloc(1, sizeof(struct JsonEntity));
    type(json) = PEAR_TYPE;
    desc(json).key = strcpy(malloc(strlen(key)+1), key);
    data(json).value = strcpy(malloc(strlen(value)+1), value);
    return json;
}

struct JsonEntity * new_Object(){
    struct JsonEntity * json = (struct JsonEntity *) calloc(1, sizeof(struct JsonEntity));
    type(json) = OBJECT_TYPE;
    return json;
}

struct JsonEntity * new_Array(){
    struct JsonEntity * json = (struct JsonEntity *) calloc(1, sizeof(struct JsonEntity));
    type(json) = ARRAY_TYPE;
    return json;
}

void delete_Json(struct JsonEntity * json){
    if(ATOM_TYPE == type(json) || PEAR_TYPE == type(json)){
        free(desc(json).key);
        free(data(json).value);
        free(json);
    }else{
        int i;
        for(i = 0; i < sizeOf(json); i++){
            delete_Json(data(json).items[i]);
        }
        free(data(json).items);
        free(json);
    }
}
