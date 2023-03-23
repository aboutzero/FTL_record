#include <iostream>
#include <vector>

struct temp
{
    int a;
    int b;
    int c;
};

#define containof(address, type, field) \
    (type *)((long *)(address) - (long *)(&((type *)0)->field))

void func(void)
{
    std::cout << "this" << std::endl;
};


int main()
{
    temp A[3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    char *p = (char *)&A[2].a; // head address of A
    int z = 0;

    int *q = &z;

    *q = (int)(((temp *)p)->a);

    p = (char *)&A[2].b;

    q = (int *)&(((temp *)(&A[2].a))->b);

    // int *q = containof(p, int, A);

    // std::cout << containof(p, int, A) << std::endl;

    std::cin.get();
}
