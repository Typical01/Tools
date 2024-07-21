#pragma once

//计数器
#ifndef _COUNTS_H
#define _COUNTS_H

#include "Tools_Tool.h"


namespace Tools_Tool {

	/*
	* 计数类
	*/
	template<typename T>
	class TOOLS_TOOL_API Counts
	{
	public:
		//计数
		vector<T> Count;

		//元素位置即 索引位置
		Counts() {}

		/* 元素位置即 索引位置
		* 接受 指针/数组
		*/
		Counts(T* count)
		{
			for (int i = 0; i < ToSize(count); i++)
				Count[i] = count[i];
		};

	public:

		//确定大小
		int ToSize(T* count)
		{
			return count / sizeof(Counts);
		}
		//确定范围
		int DetermineScope(int countLocation)
		{
			if (countLocation > 0 && countLocation < Count.size())
			{
				return 1;
			}
			else if (countLocation < 1)
			{
				return -1;
			}

			return 0;
		}

		//添加
		void Add(T& tempObject)
		{
			Count.push_back(tempObject);
		}
		//删除: 根据位置(int)
		bool Delete(int countLocation)
		{
			//范围内
			if (this->DetermineScope(countLocation))
			{
				Count.pop_back();
			}

			return false;
		}
		//插入
		bool Insert(int countLocation, T Object)
		{
			//范围内
			if (this->DetermineScope(countLocation))
			{
				Count.insert(Count.cbegin() + countLocation, Object);
			}

			return false;
		}

	public:
		/*
		* 获取 Object: 根据位置(int)
		* 带范围检查
		*/
		T Get(int countLocation) const
		{
			//在范围内: 0 ~ Count.size()
			if (DetermineScope(countLocation))
			{
				return Count[Count++]; //后加加：先用值，后增值
			}
			//超出范围
			else if (DetermineScope(countLocation) == 0) //小于
			{
				return *Count.cbegin();
			}
			else //大于
			{
				return *--Count.cend;
			}
		}
		//获取 Object: 最后的
		T Get() const
		{
			return *--Count.cend;
		}
		//获取 vector<Object>
		vector<T> GetVector() const
		{
			return Count;
		}
		//获取 Count 的元素数量
		int GetSize() const
		{
			return Count.size();
		}

		/*
		* 设置 Object : 根据位置(int)
		* 带范围检查
		*/
		bool SetCount(int countLocation, T Object)
		{
			//在范围内: 0 ~ Count.size()
			if (DetermineScope(countLocation))
			{
				Count[--countLocation] = Object;
			}

			return false;
		}

	};
}
#endif