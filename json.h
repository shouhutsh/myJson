#ifndef _JSON_H
#define _JSON_H

#define ATOM_TYPE   0
#define PEAR_TYPE   1
#define OBJECT_TYPE 2
#define ARRAY_TYPE  3

typedef union{
    int size;
    char * key;
} Key;

typedef union{
    char * value;
    struct JsonEntity ** items;
}Value;

struct JsonEntity {
    int type;
    Key desc;
    Value data;
};

#define ADD_END ((struct JsonEntity *) NULL)
struct JsonEntity * add_Children(struct JsonEntity *, struct JsonEntity *, ...);

void jsonPrint(struct JsonEntity *);
char * getString(struct JsonEntity *);

struct JsonEntity * new_Atom(const char *);
struct JsonEntity * new_Pear(const char *, const char *);
struct JsonEntity * new_Object();
struct JsonEntity * new_Array();
void delete_Json(struct JsonEntity *);

#endif