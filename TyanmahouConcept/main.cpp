#include<iostream>
#include<vector>
#include<list>
#include<unordered_map>
#include<memory>
#include<chrono>

#include<Concept.hpp>


int main()
{
	tc::concept::RandomAccessContainer<std::list<int>>::value;
	tc::concept::AllocatorAwareContainer<std::list<int>>::value;
	return 0;
}
