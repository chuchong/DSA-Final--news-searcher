//���ֹ�ϣ
//ʹ��((innercode of ch1 * innercode of ch2) ) mod N ��ʵ��
//һ19968 ��40863
//innercode = (highbyte ) * 94 + (lowbyte)//�ַ�����
//N Ӧ��Ϊ����
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
//�ӿ���,֮��Ҳ������������ʽ��ʾһ���ֵ�
//-------����: ��������ʵ��������CharStringCollection,���Ҿ�ֱ�ӱ�����һ��
//HASH �ֵ�,���д洢�����д���
//hash ����ÿ���ڵ�ľ���ʵ��,�ڵ��Ͽ��Դ洢��ֹһ������,������д���ӿ�,�������Ըı�ڵ����ݽṹ���Ч��

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
	int mod_;//��ģ��,ͬʱҲ��hash���С
	Stack<HashCouple<T>> * map_;//ǰһ�������ñ���,map��һ������
	//ÿ�����ַ������Ӧ����
	int innercode(wchar_t ch) {
		int hi = ch >> 8;//highbyte;
		int low = ch & 0x00FF;//lowbyte
		int ans = hi * 94 + low;
		return ans;
	}
	//�����ַ�����hash ֵ,��ǰ��������ĳ˻�
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
		return (size - 2 + (inner1) * (inner2)) % mod_;//ǰ��˻�Ӧ��û����int ���� ����ɾ��
	}
public:
	HashString2Map(int mod) {
		map_ = new Stack<HashCouple<T>>[mod_];
	}
	//����ַ���
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
	//û����...
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
