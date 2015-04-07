#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "json.h"

#define LEFT_BIG '{'
#define RIGHT_BIG '}'
#define LEFT_MID '['
#define RIGHT_MID ']'
#define END_CHAR '\0'
#define COMMA ','
#define SPACE ' '
#define TAB_SIZE 4

char * atomToString(struct JsonEntity *);
char * pairToString(struct JsonEntity *);
char * objectToString(struct JsonEntity *);
char * arrayToString(struct JsonEntity *);

char * (*TO_STRING[JSON_TYPE_COUNT])(struct JsonEntity *) = {atomToString, pairToString, objectToString, arrayToString};

char * getJsonString(struct JsonEntity * json){
    return TO_STRING[type(json)](json);
}

void jsonPrint(struct JsonEntity * json){
    char * str = TO_STRING[type(json)](json);
    printf("%s", str);
    free(str);
}

char * atomToString(struct JsonEntity * json){
    if(NULL == json || ATOM_TYPE != type(json) || NULL == value(json)){
        exit(0);
    }

    int len = strlen(value(json));
    char * str = (char *) calloc(len+3, sizeof(char));

    if(NONE == sym(json)){
        sprintf(str, "%s", value(json));
    }else{
        sprintf(str, "%c%s%c", sym(json), value(json), sym(json));
    }
    return str;
}

char * pairToString(struct JsonEntity * json){
    if(NULL == json || PAIR_TYPE != type(json) || NULL == key(json) || NULL == value(json)){
        exit(0);
    }

    char * value = getJsonString(entity(json));
    int len = strlen(key(json)) + (NULL == value ? 0 : strlen(value));
    char * str = (char *) calloc(len+4, sizeof(char));

    sprintf(str, "\"%s\":%s", key(json), value);
    return str;
}

char * OA_ToString(const struct JsonEntity * json, char left, char right){
    int size = size(json);
    char *str = (char *)malloc(3);
    if(0 == size){
        str[0] = left, str[1] = right;
    }else{
        int i, len = 1;
        for(i = 0; i < size; i++){
            char * data = getJsonString(item(json, i));
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

    return OA_ToString(json, LEFT_BIG, RIGHT_BIG);
}

char * arrayToString(struct JsonEntity * json){
    if(NULL == json || ARRAY_TYPE != type(json)){
        exit(0);
    }

    return OA_ToString(json, LEFT_MID, RIGHT_MID);
}

struct JsonEntity * add_Child(struct JsonEntity * root, struct JsonEntity * child){
    if(NULL == root || NULL == child || ATOM_TYPE == type(root)){
        exit(0);
    }

    inc(root);
    items(root) = realloc(items(root), sizeof(void **) * size(root));
    item(root, size(root)-1) = child;
    return root;
}

struct JsonEntity * add_Children(struct JsonEntity * root, struct JsonEntity * child, ...){
    if(NULL == root || NULL == child || ATOM_TYPE == type(root)){
        exit(0);
    }

    va_list ap;
    va_start(ap, child);

    while(CHILD_END != child){
        inc(root);
        items(root) = realloc(items(root), sizeof(void **) * size(root));
        item(root, size(root)-1) = child;

        child = va_arg(ap, struct JsonEntity *);
    }
    va_end(ap);
    return root;
}

struct JsonEntity * new_Atom(const char * value, int sym){
    struct JsonEntity * json = (struct JsonEntity *) calloc(1, sizeof(struct JsonEntity));
    type(json) = ATOM_TYPE;
    sym(json) = sym;
    value(json) = strcpy(malloc(strlen(value)+1), value);
    return json;
}

struct JsonEntity * new_Pair(const char * key, struct JsonEntity * entity){
    struct JsonEntity * json = (struct JsonEntity *) calloc(1, sizeof(struct JsonEntity));
    type(json) = PAIR_TYPE;
    key(json) = strcpy(malloc(strlen(key)+1), key);
    entity(json) = entity;
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
    if(ATOM_TYPE == type(json) || PAIR_TYPE == type(json)){
        free(key(json));
        free(value(json));
        free(json);
    }else{
        int i;
        for(i = 0; i < size(json); i++){
            delete_Json(item(json, i));
        }
        free(items(json));
        free(json);
    }
}

/*
  上边是encode，下边是parse
 */

void prettyPrint(const char * str){
    if(NULL == str){
        return;
    }

    int c, l = 0, i = 0;
    while(str[i]){
        c = str[i++];
        l = l < 0 ? 0 : l;
        switch(c){
            case LEFT_BIG:
                ++l;
                printf("{\n%*c", l * TAB_SIZE, SPACE);
                break;
            case LEFT_MID:
                ++l;
                printf("[\n%*c", l * TAB_SIZE, SPACE);
                break;
            case RIGHT_BIG:
                --l;
                printf("\n%*c}\n", l * TAB_SIZE, SPACE);
                break;
            case RIGHT_MID:
                --l;
                printf("\n%*c]\n", l * TAB_SIZE, SPACE);
                break;
            case COMMA:
                printf(",\n%*c", l * TAB_SIZE, SPACE);
                break;
            default:
                printf("%c", c);
        }
    }
}

struct JsonEntity * stringToAtom(const char *, int *);
struct JsonEntity * stringToPair(const char *, int *);
struct JsonEntity * stringToObject(const char *, int *);
struct JsonEntity * stringToArray(const char *, int *);

struct JsonEntity * (*TO_ENTITY[JSON_TYPE_COUNT])(const char *, int *) = {stringToAtom, stringToPair, stringToObject, stringToArray};

int contain(char c, const char * str){
    if(NULL == str){
        return -1;
    }
    int i;
    for(i = 0; i < strlen(str); ++i){
        if(c == str[i]){
            return 0;
        }
    }
    return 1;
}

char * getStringEndWith(const char * src, int * offset, char end){
    if(NULL == src){
        return NULL;
    }

    char c, * s = NULL;
    int len = 0, off = *offset;

    while((c = src[(*offset)++])){
        if(end == c){
            s = strncpy(calloc(len+1, sizeof(char)), src+off, len);
            break;
        }
        ++len;
    }
    return s;
}

struct JsonEntity ** getEntities(const char * src){
    if(NULL == src){
        return NULL;
    }

    int offset = 0, count = 0;
    struct JsonEntity ** entities = (struct JsonEntity **) malloc(sizeof(void **) * 2);
    while(src[offset]){
        if(LEFT_BIG == src[offset]){
            entities[count] = stringToObject(src, &offset);
        }else if(LEFT_MID == src[offset]){
            entities[count] = stringToArray(src, &offset);
        }else{
            printf("Parse Error: %s\n", src+offset);
            exit(0);
        }
        ++count;
        entities = (struct JsonEntity **) realloc(entities, (count+1)*sizeof(void **));
    }
    entities[count+1] = CHILD_END;
    return entities;
}

struct JsonEntity * stringToAtom(const char * src, int * offset){
    if(NULL == src || END_CHAR == src[*offset]){
        return NULL;
    }

    int off = *offset;
    char * s = NULL;
    char c = src[(*offset)++], sym = 0;
    struct JsonEntity * json = NULL;

    if(0 == contain(c, "\'\"")){
        sym = c;
        s = getStringEndWith(src, offset, c);
    }else{
        int len = 1;
        while(0 != contain(src[*offset], ",]}")){
            ++len; ++(*offset);
        }
        s = strncpy(calloc(len+1, sizeof(char)), src+off, len);
    }
    if(COMMA == src[*offset]){
        ++(*offset);
    }
    json = new_Atom(s, sym);
    free(s);
    return json;
}

struct JsonEntity * stringToPair(const char * src, int * offset){
    if(NULL == src || END_CHAR == src[*offset]){
        return NULL;
    }

    char * key = NULL;
    char c = src[(*offset)++];
    struct JsonEntity * json = NULL, * data = NULL;

    key = getStringEndWith(src, offset, c);

    ++(*offset);

    if(LEFT_BIG == src[*offset]){
        data = stringToObject(src, offset);
    }else if(LEFT_MID == src[*offset]){
        data = stringToArray(src, offset);
    }else if(0 == contain(src[*offset], "\'\"")){
        data = stringToAtom(src, offset);
    }else if(COMMA == src[*offset]){
        data = NULL;
    }else{
        data = stringToAtom(src, offset);
    }
    if(COMMA == src[*offset]){
        ++(*offset);
    }
    json = new_Pair(key, data);
    free(key);
    return json;
}

struct JsonEntity * stringToObject(const char * src, int * offset){
    if(NULL == src || END_CHAR == src[*offset]){
        return NULL;
    }

    char c = ++(*offset);
    struct JsonEntity * json = new_Object();
    while((c = src[*offset])){
        add_Child(json,
            stringToPair(src, offset));
        if(RIGHT_BIG == src[*offset]){
            ++(*offset); break;
        }
    }
    if(COMMA == src[*offset]){
        ++(*offset);
    }
    return json;
}

struct JsonEntity * stringToArray(const char * src, int * offset){
    if(NULL == src || END_CHAR == src[*offset]){
        return NULL;
    }

    char c = ++(*offset);
    struct JsonEntity * json = new_Array();
    while((c = src[*offset])){
        if(LEFT_BIG == c){
            add_Child(json,
                stringToObject(src, offset));
        }else if(LEFT_MID == c){
            add_Child(json,
                stringToArray(src, offset));
        }else{
            add_Child(json,
                stringToAtom(src, offset));
        }
        if(RIGHT_MID == src[*offset]){
            ++(*offset); break;
        }
    }
    if(COMMA == src[*offset]){
        ++(*offset);
    }
    return json;
}

char * dislodgeWhitespace(const char * str){
    if(NULL == str){
        return NULL;
    }
    int s = 0, r = 0;;
    char * res = (char *)calloc(strlen(str)+1, sizeof(char));

    while(str[s]){
        if(0 == contain(str[s], " \f\n\r\t\v")){
            ++s;
        }else{
            res[r++] = str[s++];
            if(DOU_QUO == str[s-1]){
                char * sub = getStringEndWith(str, &s, DOU_QUO);
                sprintf(res+r, "%s\"", sub);
                r += strlen(sub) + 1;
                free(sub);
            }
        }
    }

    return res;
}