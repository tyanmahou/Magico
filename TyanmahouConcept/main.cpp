#include<iostream>
#include<vector>

#include"Concept.hpp"


void main()
{
	int a = 1000;

	tc::concept_any < tc::Concept::Scalar > scalar = a;

}
