#ifndef MYSTL_VECTOR_H
#define MYSTL_VECTOR_H
#include"mystl_allocator.h"
#include<cstddef>
namespace mystl
{
    template<typename T,typename alloc=mystl::allocator<T>>
    //定义一个模板类，元素类型T，分配器类型alloc，默认使用mystl::allocator<T>
    class vector
    {
        public:
            typedef T value_type;
            typedef T* iterator;
            typedef const T* const_iterator;
            typedef T& reference;
            typedef const T& const_reference;
            typedef size_t size_type;
            typedef ptrdiff_t difference_type;
        private:
            iterator start;
            iterator finish;
            iterator end_of_storage;
            alloc data_allocator;

            void fill_initialize(size_type n,const_reference value)
            {
                start=data_allocator.allocate(n);
                finish=start;
                try
                {
                    for(size_type i=0;i<n;++i)
                    {
                        alloc::construct(finish++,value);
                    }
                }
                catch(...)
                {
                    clear_and_release();
                    throw;
                }
                end_of_storage=finish;
            }

            void clear_and_release()
            {
                if(start!=nullptr)
                {
                    while(start!=finish)
                    {
                        alloc::destroy(--finish);
                    }
                    alloc::deallocate(start,end_of_storage-start);
                    start=finish=end_of_storage=nullptr;
                }
            }
            void range_initialize(const_iterator first,const_iterator last)
            {
                size_type n=last-first;
                start=data_allocator.allocate(n);
                finish=start;
                try
                {
                    for(;first!=last;++first)
                    {
                        alloc::construct(finish++,*first);
                    }
                }
                catch(...)
                {
                    clear_and_release();
                    throw;
                }
                end_of_storage=finish;
            }
        public:
            vector():start(nullptr),finish(nullptr),end_of_storage(nullptr){}
            //explicit关键字用于构造函数，防止隐式类型转换，用就是了。
            explicit vector(size_type n,const_reference value=T())
            {
                fill_initialize(n,value);
            }
            vector(const vector& other)
            {
                range_initialize(other.begin(),other.end());
            }
            ~vector()
            {
                clear_and_release();
            }
            //基础接口
            iterator begin(){return start;}
            const_iterator begin()const{return start;}
            iterator end(){return finish;}
            const_iterator end()const{return finish;}
            size_type size()const{return static_cast<size_type>(finish-start);}
            //与begin()、end()的区别。。。
            size_type capacity()const{return static_cast<size_type>(end_of_storage-start);}
            bool empty()const{return start==finish;}
            reference operator[](size_type n){return *(start+n);}
            const_reference operator[](size_type n)const{return *(start+n);}
            
            void swap(vector& other)
            {
                iterator tmp=start;
                start=other.start;
                other.start=tmp;

                tmp=finish;
                finish=other.finish;
                other.finish=tmp;

                tmp=end_of_storage;
                end_of_storage=other.end_of_storage;
                other.end_of_storage=tmp;
            }
            vector& operator=(vector other)
            {
                swap(other);
                return *this;
            }
    };
}


#endif