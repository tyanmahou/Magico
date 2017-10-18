#include<iostream>
#include"Concept.hpp"


//�p�^�[��1 �\���̂ƃ����o�֐��@�ϐ�����������̂��悢

struct _Addable
{
	template<class T , class U>
	auto requires(T&& a, U&& b)->decltype(
		a + b ,
		a - b ,
		a * b 
		);
};



//�p�^�[��2 �G�C���A�X�e���v���[�g�@declval���g���\�����ǐ��_�E���H�@�s���͌���

template<class T, class U>
using _Addable2 = decltype(
	tc::val<T> + tc::val<U>,
	tc::val<T> - tc::val<U>, 
	tc::val<T> * tc::val<U>
	);



























//����
template<class T, class U = T>
using Addable2 = tc::alias_to_concept<_Addable2, T, U>;


struct A
{
	A operator +(const A& o)
	{
		return o;
	}
	A operator -(const A& o)
	{
		return o;
	}
};
int main()
{
	Addable2<int>::value;
	return 0;

}
