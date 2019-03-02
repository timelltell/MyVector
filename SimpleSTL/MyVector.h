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
		typedef simple_alloc<value_type, Alloc> data_allocator; // ����ռ�������
		iterator start;
		iterator finish;
		iterator end_of_storage;
#pragma region �ڴ����͹���
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

#pragma region �ڴ��ͷ�������
		void deallocate() {
			if (start) {
				data_allocator::deallocate(start, end_of_storage - start);
			}
		}
#pragma endregion

#pragma region ����Ԫ��
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

#pragma region ��cbVector�Ķ�����
		iterator begin() { return start; } // ����cbVectorͷԪ�صĵ�ַ
		iterator end() { return finish; } // ����cbVectorβԪ�صĵ�ַ
		size_type size() { return size_type(int(end() - begin())); } // cbVector�ĳ��� = βԪ�ص�ַ - ͷԪ�ص�ַ
		size_type capacity()const { return size_type(end_of_storage - start); } // cbVector������ = ���������ַ - ͷԪ�ص�ַ
		bool empty()const { return begin() == end(); } //cbVector�Ƿ�Ϊ�գ�ͷԪ�ص�ַ �Ƿ���� βԪ�ص�ַ
		reference operator[](size_type n) { return *(begin() + n); } // ����ָ��λ�õ�Ԫ������
		reference front() { return *begin(); } // ���ͷԪ��λְ
		reference back() { return *(end() - 1); } // ���βԪ��λ��

#pragma endregion

#pragma region ���캯��
		cbVector() :start(0), finish(0), end_of_storage(0) {};
		cbVector(size_type n, const T& value) { fill_initialize(n, value); }
		cbVector(int n, const T& value) { fill_initialize(n, value); } // ͬ��
		cbVector(long n, const T& value) { fill_initialize(n, value); } // ͬ��
		explicit cbVector(size_type n) { fill_initialize(n, T()); } // ��ʼ��cbVector�ĳ���Ϊn
#pragma endregion 

#pragma region ��������
		~cbVector()
		{
			destroy(start, finish);
			deallocate();
		}
#pragma endregion

#pragma region ��cbVector��д����
		/**
		*  ����βԪ��
		*/
		void pop_back() {
			--finish;
			destroy(finish);
		}
		/**
		*  ��cbVectorĩβ����һ��Ԫ��
		*/
		void push_back(const T& x)
		{
			// �ж�cghVector�������Ƿ����ˣ����û������ֱ�������е��ڴ������Ϲ���Ԫ��
			if (finish != end_of_storage)
			{
				construct(finish, x);
				++finish;
			}
			else // ����������Ǿ�Ҫ���·����ڴ沢���¹��캯��
			{
				insert_aux(end(), x);
			}
		}

		/**
		*  ���[first, last)�����ڵ�Ԫ��
		*/

		iterator erase(iterator first, iterator last)
		{
			// ��last��finish���Ԫ�ؿ�����first��Ϊ��ʼ�����ڴ�ռ䣬����first + ( finish - last ) �ĵ�ַ
			iterator i = std::copy(last, finish, first);
			destroy(i, finish); // ����(first + ( finish - last ), finish]����ڴ�
			finish = finish - (last - first); // ��������finish
			return first;
		}

		/**
		*  ���ĳ��λ���ϵ�Ԫ��
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

