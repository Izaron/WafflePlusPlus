#include <string>

// @jsonable
struct Test0 {
};

struct EmptyStruct {
};

struct NestedNestedStruct {
    std::string val1;
};

struct NestedStruct {
    int val1;
    NestedNestedStruct val2;
    float val3;
};

// @jsonable
struct Test1 {
    int val1;
    long int val2;
    long long int val3;
    float val4;
    double val5;
    EmptyStruct val6;
    NestedStruct val7;
    std::string val8;
    std::string_view val9;
    void* val10;
    std::string* val11;

    void foo();
    int bar();
    float baz();
};
