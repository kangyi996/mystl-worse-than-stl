#ifndef MYSTL_ALLOCATOR_H
#define MYSTL_ALLOCATOR_H   //宏定义
//  #pragma once    同#ifndef ... #define ... #endif  作用相同
#include<new>   //申请内存
#include<cstddef>   //size_t,ptrdiff_t
#include <utility>  //确保临时变量传给构造函数时还是临时变量，更快！
namespace mystl
{
    template<typename T>    //声明接下来的T可以被任何数据类型代替
    class allocator
    {
        public:
            typedef T value_type;
            typedef T* pointer;
            typedef const T* const_pointer;
            typedef T& reference;
            typedef const T& const_reference;
            typedef size_t size_type;
            typedef ptrdiff_t difference_type;
            //申请空间
            static pointer allocate(size_type n)
            {
                if(n==0)return nullptr;
                return static_cast<pointer>(::operator new(n*sizeof(value_type)));
                //static_cast<type_name>效果等同强制类型转换，但
                //有安全检查、可搜索性（相较于强制类型转换很显眼）
            }
            //删除空间
            static void deallocate(pointer p,size_type n)
            {
                if(p==nullptr)return;
                ::operator delete(p);
            }
            //构造对象
            template<typename... Args>
            //...代表任意数量、任意类型的参数
            static void construct(pointer p,Args&&... args)
            {
                //...在args前面表示将所有参数打包为一个参数包args
                //&&表示完美转发，保持参数的值类别（左值或右值）
                ::new((void*)p) value_type(std::forward<Args>(args)...);
                //先略过，，
            }
            //大爆炸
            static void destroy(pointer p)
            {
                if(p==nullptr)return;
                p->~value_type();
            }
        
    };

}
#endif