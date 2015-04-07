# myJson #

过去一年一直再用Java写东西，这个东西只是为了练手用。

为了方便，我没有做非常完善的异常检测，例如alloc函数我都假定能够正常运行
我也没有使用缓存，因为我主要是为了练手用，当然这也是以后有时间的话再进行优化的地方。


## 实现思路 ##
我将一个`Json`实体分为四种类型：
Atom、Pair、Array、Object。

### Atom ###
Atom是最基本的实体类型，含有两个属性：
```c
        struct Atom{
            int sym;
            char *value;
        }atom;
```
我不对Atom中存放的数据做类型区分，不管是int、double或者其他，都做字符串保存，`value`既是指向此字符串的指针。而`sym`则是存放环绕在这个字符串两边的符号。
我没有对`sym`的取指做检查，因此理论上此符号可以是任意的，我假定使用者知道自己在做什么，尽管如此我也提供了三种默认值：
```c
SIN_QUO '
DOU_QUO "
NONE    0
```
当`sym`为NONE时，我将该符号忽略。
示例：123、'123'、"123"、"hello world!\n"

### Pair ###
Pair是以 ":" 分割的键值对形式的实体类型。
```c
        struct Pair{
            char *key;
            struct JsonEntity *entity;
        }pair;
```
`key`为键的字符串，entity是值，我不允许值是字符串形式，而是再次指向一个Atom实体的形式完成。这样主要是为了统一操作，正因如此，Array和Object类型才能方便的完成。
注意`entity`的类型不能是Pair。
示例："key1":"value1"、 "key2":{}、 "key3":[]

### Array ###
Array是用 "[]" 包围的实体类型。
```c
        struct Array{
            int size;
            struct JsonEntity **items;
        }array;
```
`size`是该实体中包含的其他实体个数，`items`是指针数组，指向其他实体。
注意`items`包含的类型不能是Pair。
示例：["ATOM", {}, []]

### Object ###
Object是用 "{}" 包围的实体类型。
结构同Array类型一样。
注意`items`包含的类型只能是Pair。
示例：{"key1":"value1", "key2":{}, "key3":[]}

### 一些有用的东西 ###

#### 宏 ####
```c
type(jsonEntity)
sym(jsonEntity)
value(jsonEntity)
key(jsonEntity)
entity(jsonEntity)
size(jsonEntity)
items(jsonEntity)
item(jsonEntity, i)
inc(jsonEntity)
```
基本上只是简化实体属性的获取方式，另外对于不同的实体类型做了一些区分。

#### 函数 ####
```c
struct JsonEntity * new_Atom(const char *, int);
struct JsonEntity * new_Pair(const char *, struct JsonEntity *);
struct JsonEntity * new_Object();
struct JsonEntity * new_Array();
void delete_Json(struct JsonEntity *);
```
创建和释放实体函数。

```c
struct JsonEntity * add_Child(struct JsonEntity *, struct JsonEntity *);
struct JsonEntity * add_Children(struct JsonEntity *, struct JsonEntity *, ...);
```
对Array和Object类型，用于向里边添加实体。
`add_Child`是单个添加。
`add_Children`是多个添加，但是需要在末尾添加终止标识`CHILD_END`。

```c
void jsonPrint(struct JsonEntity *);
void prettyPrint(const char *);
char * getJsonString(struct JsonEntity *);
char * dislodgeWhitespace(const char *);
struct JsonEntity ** getEntities(const char *);
```
这些函数就是该项目的主体，需要注意内存的释放。


用法示例请参考 parse_test.c 和 encode_test.c
