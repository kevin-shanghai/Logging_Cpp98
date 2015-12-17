#ifndef SINGLETON_H__
#define SINGLETON_H__
#include "Common/Logger/Log/include/base/Mutex.h"


template<class T>
class Singleton
{
public:
	static T* Instance()
	{
		if (m_pInstance == nullptr)  //double check
		{
			static MutexLock m_Mutex;
			MutexLockGuard lock(m_Mutex);
			if (m_pInstance == NULL)
			{
				m_pInstance = new T();
			}
		}
		return m_pInstance;
	}
private:
	Singleton();
	static T* m_pInstance;
};

template<class T>
T* Singleton<T>::m_pInstance = NULL;


#endif