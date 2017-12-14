# Magico
C++17 Concept Library
## About

"Magico" is a library for C ++ 17 only with headers that provides type constraints as meta functions like C++11's concept also Concepts Lite.    
It also provides many meta functions that constrain *iterators* , *containers* , etc by default.


## Example

### concept

```cpp

//Stack concept
MAGICO_CONCEPT(Stack)
{
	template<class X>
	using value_type = typename X::value_type;

	template<class X>
	auto require(X s, const X& cs, const value_type<X>& v)->decltype(
		valid_expr<void>((s.push(v), is_void)),
		valid_expr<void>((s.pop(), is_void)),
		valid_expr<value_type<X>>(cs.top()),
		valid_expr<bool>(cs.empty())
		);
};


int main()
{
	static_assert(Stack<std::vector<int>>::value == false);
	static_assert(Stack_v<std::stack<int>> == true);

	return 0;
}

```

`valid_expr <Return> (exp)` checks if the `exp` can convert to `Return` type.  
If the expression requires a void type, use `valid_expr <void> ((exp, is _ void))`.  

### Constraint on template parameters


`where<Return,Concepts...>`will be `Return` type if it satisfies all `Concepts` given as a meta function.    
```cpp
template<class T>
auto Func(T& a)->where<void,Stack<T>>
{}
```
`where_bool <Return, bool>` can test `where` with a bool value.  
```cpp
template<class T>
auto Func2(T& a)->where_bool<void, Stack_v<T>>
{}
```
`require <Concepts ...>` is  `std :: nullptr_t` only if it satisfies` Concepts`.

```cpp
template<class T, require<Stack<T>> = nullptr>
void Func3(T& a)
{}
```
`MAGICO_CONCEPT_ASSERT (Concepts ...)` is a macro that asserts if it doesn't satisfy `Concepts`.  
```cpp
template<class T>
struct Class
{
	MAGICO_CONCEPT_ASSERT(Stack<T>);
};

```

### concept_map

```cpp
template<class T>
struct concept_map<Stack<std::vector<T>>>:std::vector<T>
{
	using value_type = typename std::vector<T>::value_type;

	void push(const value_type& v)
	{
		this->emplace_back(v);
	}
	void pop()
	{
		this->pop_back(v);
	}
	value_type top()const
	{
		return this->back();
	}
	//vector has empty

	//Assign with "apply"
	static decltype(auto) apply(std::vector<T>& v)
	{
		return static_cast<concept_map&>(v);
	}
};

int main()
{

	static_assert(Stack<std::vector<int>>::value == false);

	//Using "as_mapped" will result in a mapped type
	static_assert(Stack<as_mapped<std::vector<int>>>::value == true);

	return 0;
}
```

### How to mapping

```cpp
//mapping
std::vector<int> v{ 1,2,3 };

auto&&[_v] = concept_mapping<Stack>(v);

_v.push(4);
_v.top();
```
or

```cpp
auto&&_v2 = concept_mapping<Stack<std::vector<int>>>(v);

```
#### example
```cpp
template<class T>
auto Func(T& _a)->where<void, Stack<as_mapped<T>>>
{
	auto[a] = concept_mapping<Stack>(_a);
	/*
	something...
	*/
}
```

#### If you don't approve an implicit concept_map

Use `MAGICO_CONCEPT_MAP_NONE_DEFAULT` for **global space**.
```cpp
//Animal
MAGICO_CONCEPT(Animal)
{
	template<class T>
	auto require(T t)->void;
};

MAGICO_CONCEPT_MAP_NONE_DEFAULT(Animal);

struct Cat {};
struct Dog {};
template<>
struct magico::concept_map<Animal<Cat>>
{
	static decltype(auto) apply(Cat& c)
	{
		return c;
	}
};

int main()
{
	static_assert(Animal<as_mapped<Cat>>::value==true);
	static_assert(Animal<as_mapped<Dog>>::value==false);
	return 0;
}

```
### Concept Base Overload
```cpp
template<class T>
auto _Func(priority<0>,T& a)
{
	std::cout << "Bidirectional" << std::endl;
}
template<class T>
auto _Func(priority<1>,T& a)->where<void, concepts::RandomAccessIterator<T>>
{
	std::cout << "RandomAccess" << std::endl;
}
template<class T>
auto Func(T a)->where<void, concepts::BidirectionalIterator<T>>
{
	_Func(priority_v<1>, a);
}

int main()
{
	std::vector<int> v;
	std::list<int> l;

	Func(v.begin());//RandomAccess
	Func(l.begin());//Bidirectional

	return 0;
}

```

## How to
Header Include Only in `Include` Directory.

## License
MIT

## Experimental

### concept_any
### axiom
