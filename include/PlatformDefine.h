#ifndef PLATFORM_DEFINE_H__
#define PLATFORM_DEFINE_H__

#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#define ON_WINDOWS
#pragma warning(disable: 4251)
#pragma warning(disable: 4996)
#pragma warning(disable: 4800)
#pragma warning(disable: 4244)

#elif defined __linux__
#define ON_LINUX
#include <pthread.h>

#else
#include <pthread.h>
#include <taskLibCommon.h>
#define ON_VXWORKS
#endif

#ifdef ON_WINDOWS
#define EXPORTLOG __declspec (dllexport)
#else
#define EXPORTLOG
#endif


#ifdef ON_WINDOWS
typedef CRITICAL_SECTION MutexType;
typedef CONDITION_VARIABLE  ConditionType;

#else
typedef pthread_mutex_t MutexType;
typedef pthread_cond_t ConditionType;
#endif



#endif
