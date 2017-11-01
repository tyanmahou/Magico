# Magico
C++17 Concept Library
## About

Magicoはメタ関数としてC++11時代のconcept風に型制約を提供するヘッダーのみのC++17向けライブラリです
また、デフォルトでイテーレーターやコンテナなどを制約する多くのメタ関数を提供します。


## Example

### concept

```cpp

///スタックコンセプト
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

	//operator = で割り当て
	decltype(auto) operator =(std::vector<T>& v)
	{
		return static_cast<concept_map&>(v);
	}
};

int main()
{

	static_assert(Stack<std::vector<int>>::value == false);

	//as_mappedを指定するとマッピング後で判定できる
	static_assert(Stack<as_mapped<std::vector<int>>>::value == true);

	return 0;
}
```

### マッピングの方法

```cpp
	//マッピング
	std::vector<int> v{ 1,2,3 };

	auto&&[_v] = concept_mapping<Stack>(v);

	_v.push(4);
	_v.top();
```
もしくは

```cpp
	auto&&_v2 = concept_mapping<Stack<std::vector<int>>>(v);

```
#### 暗黙のconcept_mapを認めない場合

`MAGICO_CONCEPT`の代わりに`MAGICO_CONCEPT_NONE_DEFAULT`を使用します
```cpp
///Animal
MAGICO_CONCEPT_NONE_DEFAULT(Animal)
{
	template<class T>
	auto require(T t)->void;
};

struct Cat {};
struct Dog {};
template<>
struct concept_map<Animal<Cat>>
{
	decltype(auto) operator =(Cat& c)
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

## How to
`Include`ディレクトリのヘッダーインクルードのみ

## License
MIT
