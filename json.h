#ifndef _JSON_H
#define _JSON_H

#define ATOM_TYPE   0

#define PAIR_TYPE   1
#define OBJECT_TYPE 2
#define ARRAY_TYPE  3
#define JSON_TYPE_COUNT	4

#define SIN_QUO '\''
#define DOU_QUO '\"'
#define NONE 0

struct JsonEntity {
    int type;

    union{
        struct Atom{
            int sym;
            char *value;
        }atom;

        struct Pair{
            char *key;
            struct JsonEntity *entity;
        }pair;

        struct Array{
            int size;
            struct JsonEntity **items;
        }array;

        struct Object{
            int size;
            struct JsonEntity **items;
        }object;
    }json;
};

#define CHILD_END ((struct JsonEntity *) NULL)

#define type(jsonEntity) ((jsonEntity)->type)
#define sym(jsonEntity) ((jsonEntity)->json.atom.sym)
#define value(jsonEntity) ((jsonEntity)->json.atom.value)
#define key(jsonEntity) ((jsonEntity)->json.pair.key)
#define entity(jsonEntity) ((jsonEntity)->json.pair.entity)
#define size(jsonEntity) ((jsonEntity)->json.object.size)
#define items(jsonEntity) ((jsonEntity)->json.object.items)
#define item(jsonEntity, i) (items(jsonEntity)[i])
#define inc(jsonEntity) (++size(jsonEntity))

struct JsonEntity * add_Child(struct JsonEntity *, struct JsonEntity *);
struct JsonEntity * add_Children(struct JsonEntity *, struct JsonEntity *, ...);

void jsonPrint(struct JsonEntity *);
void prettyPrint(const char *);
char * getJsonString(struct JsonEntity *);
char * dislodgeWhitespace(const char *);
struct JsonEntity ** getEntities(const char *);

struct JsonEntity * new_Atom(const char *, int);
struct JsonEntity * new_Pair(const char *, struct JsonEntity *);
struct JsonEntity * new_Object();
struct JsonEntity * new_Array();
void delete_Json(struct JsonEntity *);

#endif