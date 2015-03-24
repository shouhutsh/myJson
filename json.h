#ifndef _JSON_H
#define _JSON_H

#define ATOM_TYPE	0
#define OBJECT_TYPE 1
#define ARRAY_TYPE	2

#define JSON_TYPE_COUNT	3

struct JsonEntity {
    int	state;
    /*
      desc
      如果json类型为atom则描述为字符串
      如果类型为object或array则代表长度
     */
    void *	desc;
    void *	data;
};

char * toString(struct JsonEntity *);
void jsonPrint(struct JsonEntity *);

char * atomToString(struct JsonEntity *);
char * objectToString(struct JsonEntity *);
char * arrayToString(struct JsonEntity *);
char * (*JSON_toString[JSON_TYPE_COUNT])(struct JsonEntity *) = {atomToString, objectToString, arrayToString};

struct JsonEntity * new_JsonEntity(int, void *, void *);
void delete_JsonEntity(struct JsonEntity *);

#define size(json)  (*((int *)json->desc))

#endif