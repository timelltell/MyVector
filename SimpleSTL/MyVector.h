#pragma once


#ifndef _CB_VECTOR
#define _CB_VECTOR
#include "pch.h"
#include "VectorAllocator.h"

namespace CBVector {
	template<class T, class Alloc = cbAllocator<T>>
	class cbVector {
	public:
		typedef T				value_type;
		typedef value_type*		pointer;
		typedef value_type*		iterator;
		typedef value_type&		reference;
		typedef size_t			size_type;
		typedef ptrdiff_t		difference_type;
	protected:
		typedef simple_alloc<value_type, Alloc> data_allocator; // 定义空间配置器
		iterator start;
		iterator finish;
		iterator end_of_storage;
#pragma region 内存分配和构造
		void fill_initialize(size_type n, const T& value)
		{
			start = allocate_initialize(n, value);
			finish = start + n;
			end_of_storage = finish;
		}
		iterator allocate_initialize(size_type n, const T& value)
		{
			iterator result = data_allocator::allocate(n);
			iterator cur = result;
			for (; n > 0; --n,++cur) {
				construct(&*cur, value);
			}
			return result;
		}
#pragma endregion

#pragma region 内存释放与析构
		void deallocate() {
			if (start) {
				data_allocator::deallocate(start, end_of_storage - start);
			}
		}
#pragma endregion

#pragma region 插入元素
		void insert_aux(iterator position,const T& value) {
			if (finish != end_of_storage) {
				construct(finish, *(finish - 1));
				finish++;
				T x_value = value;
				std::copy_backward(position, finish - 2, finish - 1);
				*position = x_value;
			}
			else {
				ptrdiff_t old_size = size();
				const ptrdiff_t len = old_size != 0 ? 2 * old_size : 0;
				iterator new_start = data_allocator::allocate(len);
				iterator new_finish = new_start;
				try {
					new_finish=std::uninitialized_copy(start, position, new_start);
					construct(new_finish, value);
					++new_finish;
					new_finish = std::uninitialized_copy(position, finish, new_finish);
				}
				catch(std::exception ex){
					destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, len);
					throw;
				}
				destroy(start, finish);
				deallocate();
				start = new_start;
				finish = new_finish;
				end_of_storage = start + len;
			}
		}
#pragma endregion
	public:

#pragma region 对cbVector的读操作
		iterator begin() { return start; } // 返回cbVector头元素的地址
		iterator end() { return finish; } // 返回cbVector尾元素的地址
		size_type size() { return size_type(int(end() - begin())); } // cbVector的长度 = 尾元素地址 - 头元素地址
		size_type capacity()const { return size_type(end_of_storage - start); } // cbVector的容量 = 最大容量地址 - 头元素地址
		bool empty()const { return begin() == end(); } //cbVector是否为空：头元素地址 是否等于 尾元素地址
		reference operator[](size_type n) { return *(begin() + n); } // 返回指定位置的元素引用
		reference front() { return *begin(); } // 获得头元素位职
		reference back() { return *(end() - 1); } // 获得尾元素位置

#pragma endregion

#pragma region 构造函数
		cbVector() :start(0), finish(0), end_of_storage(0) {};
		cbVector(size_type n, const T& value) { fill_initialize(n, value); }
		cbVector(int n, const T& value) { fill_initialize(n, value); } // 同上
		cbVector(long n, const T& value) { fill_initialize(n, value); } // 同上
		explicit cbVector(size_type n) { fill_initialize(n, T()); } // 初始化cbVector的长度为n
#pragma endregion 

#pragma region 析构函数
		~cbVector()
		{
			destroy(start, finish);
			deallocate();
		}
#pragma endregion

#pragma region 对cbVector的写操作
		/**
		*  弹出尾元素
		*/
		void pop_back() {
			--finish;
			destroy(finish);
		}
		/**
		*  在cbVector末尾插入一个元素
		*/
		void push_back(const T& x)
		{
			// 判断cghVector的容量是否满了，如果没满我们直接在已有的内存区域上构造元素
			if (finish != end_of_storage)
			{
				construct(finish, x);
				++finish;
			}
			else // 如果满了我们就要重新分配内存并重新构造函数
			{
				insert_aux(end(), x);
			}
		}

		/**
		*  清除[first, last)区间内的元素
		*/

		iterator erase(iterator first, iterator last)
		{
			// 把last到finish这段元素拷贝以first作为起始处的内存空间，返回first + ( finish - last ) 的地址
			iterator i = std::copy(last, finish, first);
			destroy(i, finish); // 销毁(first + ( finish - last ), finish]这段内存
			finish = finish - (last - first); // 重新设置finish
			return first;
		}

		/**
		*  清除某个位置上的元素
		*/
		iterator erase(iterator position) {
			if (position + 1 != end()) {
				copy(position + 1, finish, position);
			}
			--finish;
			destroy(finish);
			return position;
		}
#pragma endregion
	};
}
#endif // !CB_VECTOR

