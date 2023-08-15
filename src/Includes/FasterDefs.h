#pragma once
#include <string.h>
#include "WTSMarcos.h"
#include "../FasterLibs/tsl/robin_map.h"
#include "../FasterLibs/tsl/robin_set.h"


/*
 *	By Wesley @ 2023.08.15
 *	���ź���robin_map����std::string�����������ʱ�򣨾�������13106�����ݣ���ͬ���Ի����ܾ�����ֵ��ͬ��
 *	�����bad allocate���쳣
 *	�Ҳ²���std::string�޷���string�����Զ��Ż�
 *	�������������ʱ�򣬾ͻ�ռ�÷ǳ�����ڴ棬�����л����ڴ��Сʱ���ͻ�����쳣
 *	������ΰ�LongKey��LongKey��ע�͵����ĳ�std::string
 */

NS_WTP_BEGIN

struct string_hash
{
	//BKDRHash�㷨
	size_t operator()(const std::string& key) const
	{
		size_t seed = 131; // 31 131 1313 13131 131313 etc..
		size_t hash = 0;

		char* str = (char*)key.c_str();
		while (*str)
		{
			hash = hash * seed + (*str++);
		}

		return (hash & 0x7FFFFFFF);
	}
};

template<class Key, class T>
class fastest_hashmap : public tsl::robin_map<Key, T>
{
public:
	typedef tsl::robin_map<Key, T>	Container;
	fastest_hashmap():Container(){}
};

template<class T>
class fastest_hashmap<std::string, T> : public tsl::robin_map<std::string, T, string_hash>
{
public:
	typedef tsl::robin_map<std::string, T, string_hash>	Container;
	fastest_hashmap() :Container() {}
};

template<class Key>
class fastest_hashset : public tsl::robin_set<Key>
{
public:
	typedef tsl::robin_set<Key>	Container;
	fastest_hashset() :Container() {}
};

template<>
class fastest_hashset<std::string> : public tsl::robin_set<std::string, string_hash>
{
public:
	typedef tsl::robin_set<std::string, string_hash>	Container;
	fastest_hashset() :Container() {}
};

class StringKey : public std::string
{
public:
	StringKey(const char* s):std::string(s){}
};

typedef fastest_hashset<std::string> CodeSet;

NS_WTP_END;
