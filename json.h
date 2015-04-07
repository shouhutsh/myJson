#ifndef _JSON_H
#define _JSON_H

#define ATOM_TYPE   0

#define PAIR_TYPE   1
#define OBJECT_TYPE 2
#define ARRAY_TYPE  3

#define JSON_TYPE_COUNT	4

typedef union{
    char * key;

    int size;
} Key;

typedef union{
    char * value;

    struct JsonEntity * entity;

    struct JsonEntity ** items;
}Value;

struct JsonEntity {
    int type;
    Key desc;
    Value data;
};

#define ADD_END ((struct JsonEntity *) NULL)

#define type(json) ((json)->type)
#define size(json) ((json)->desc.size)
#define key(json) ((json)->desc.key)
#define value(json) ((json)->data.value)
#define entity(json) ((json)->data.entity)
#define items(json) ((json)->data.items)
#define item(json, i) (items(json)[i])
#define inc(json) (++size(json))

struct JsonEntity * add_Child(struct JsonEntity *, struct JsonEntity *);
struct JsonEntity * add_Children(struct JsonEntity *, struct JsonEntity *, ...);

void jsonPrint(struct JsonEntity *);
void prettyPrint(const char *);
char * getJsonString(struct JsonEntity *);
char * dislodgeWhitespace(const char *);
struct JsonEntity ** getEntities(const char *);

struct JsonEntity * new_Atom(const char *);
struct JsonEntity * new_Pair(const char *, struct JsonEntity *);
struct JsonEntity * new_Object();
struct JsonEntity * new_Array();
void delete_Json(struct JsonEntity *);

#endif