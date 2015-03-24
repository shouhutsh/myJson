#ifndef _JSON_H
#define _JSON_H

#define ATOM_TYPE	0
#define OBJECT_TYPE 1
#define ARRAY_TYPE	2

#define JSON_TYPE_COUNT	3

struct JsonEntity {
    int type;
    void * desc;
    void * data;
};

char * atomToString(struct JsonEntity *);
char * objectToString(struct JsonEntity *);
char * arrayToString(struct JsonEntity *);

struct JsonEntity * add_children(struct JsonEntity *, struct JsonEntity *);
struct JsonEntity * new_JsonEntity(int, void *, void *);
void delete_JsonEntity(struct JsonEntity *);

int inc(void *);
int sizeOf(struct JsonEntity *);
void jsonPrint(struct JsonEntity *);
char * toString(struct JsonEntity *);

#endif