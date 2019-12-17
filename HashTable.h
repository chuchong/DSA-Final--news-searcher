//二字哈希
//使用((innercode of ch1 * innercode of ch2) ) mod N 来实现
//一19968 龟40863
//innercode = (highbyte ) * 94 + (lowbyte)//字符内码
//N 应该为质数
#pragma once
#include <string>
#include <fstream>
#include <ostream>
#include <locale>
#include <codecvt>
#include <assert.h>
#include <sstream>
#include "CharString.h"
#include "Stack.h"
//接口类,之后也可以用其他方式表示一个字典
//-------以上: 单链表来实现上述的CharStringCollection,查找就直接遍历看一看
//HASH 字典,其中存储了所有词语
//hash 表中每个节点的具体实现,节点上可以存储不止一个单词,这里先写个接口,后续可以改变节点数据结构提高效率

template<typename T>
struct HashCouple {
	T value;
	CharString str;

	HashCouple() {}
	HashCouple(const CharString & s,const T & t):value(t), str(s) {}
};

template<typename T>
class HashString2Map
{
	int mod_;//被模数,同时也是hash表大小
	Stack<HashCouple<T>> * map_;//前一个是引用变量,map是一个数组
	//每个宽字符定义对应内码
	int innercode(wchar_t ch) {
		int hi = ch >> 8;//highbyte;
		int low = ch & 0x00FF;//lowbyte
		int ans = hi * 94 + low;
		return ans;
	}
	//计算字符串的hash 值,用前两个内码的乘积
	int hash(const CharString & str) {
		int size = str.size();
		wchar_t c1 = str.charAt(0);
		// bug!!
		wchar_t c2;
		if (str.size() > 2)
			c2 = str.charAt(1);
		else
			c2 = L' ';
		int inner1 = innercode(c1);
		int inner2 = innercode(c2);
		return (size - 2 + (inner1) * (inner2)) % mod_;//前面乘积应该没超过int 上限 不用删除
	}
public:
	HashString2Map(int mod) {
		map_ = new Stack<HashCouple<T>>[mod_];
	}
	//添加字符串
	void Add(const CharString & str, const T & t) {
		int i = hash(str);

		auto iter = map_[i].head;
		while (iter != nullptr && iter->t_.str != str) {
			iter = iter->next_;
		}
		if (iter == nullptr) {
			HashCouple<T> cou(str, t);
			map_[i].push(cou);
		}
	}
	//没人用...
	void Remove(const CharString &str) {
	}

	bool find(const CharString &str) {
		int i = hash(str);

		auto iter = map_[i].head;
		while (iter != nullptr) {
			if (str == iter->t_.str)
				return 1;
			iter = iter->next_;
		}
		
		return 0;
	}

	T * getT(const CharString & str) {
		int i = hash(str);

		auto iter = map_[i].head;
		while (iter != nullptr) {
			if (str == iter->t_.str)
				return &iter->t_.value;
			iter = iter->next_;
		}

		return nullptr;
	}

	T & operator[] (const CharString & str) {
		int i = hash(str);

		auto iter = map_[i].head;
		while (iter != nullptr) {
			if (str == iter->t_.str)
				return iter->t_.value;
			iter = iter->next_;
		}

		T t = T();
		HashCouple<T> cou(str, t);
		map_[i].push(cou);
		return *this->getT(str);
	}

	~HashString2Map() {
		delete[] map_;
	}

	HashString2Map() {
		mod_ = 499979;
		map_ = new Stack<HashCouple<T>>[mod_];
	}
};
