#include<iostream>
#include"mystl/mystl_allocator.h"
struct spy
{
    spy(){std::cout<<"default constructor"<<std::endl;}
    ~spy(){std::cout<<"destructor"<<std::endl;}
};
int main()
{
    mystl::allocator<spy> alloc;
    auto p=alloc.allocate(3);  //申请空间
    for(int i=0;i<2;i++)
        alloc.construct(p+i);   //构造对象
    for(int i=0;i<2;i++)
        alloc.destroy(p+i);     //大爆炸
    alloc.deallocate(p,3);   //删除空间
    return 0;
}