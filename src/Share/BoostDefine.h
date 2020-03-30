#pragma once

#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

//��д��
typedef boost::shared_mutex							BoostRWMutex;
typedef boost::shared_lock<boost::shared_mutex>		BoostReadLock;
typedef boost::unique_lock<boost::shared_mutex>		BoostWriteLock;

//�򵥵Ļ�����
typedef boost::recursive_mutex						BoostRecursiveMutex;
typedef boost::recursive_mutex::scoped_lock			BoostScopedLock;

typedef boost::mutex								BoostUniqueMutex;

typedef boost::unique_lock<boost::mutex>			BoostUniqueLock;				

//�߳�
typedef boost::thread								BoostThread;
typedef boost::shared_ptr<boost::thread>			BoostThreadPtr;

typedef boost::condition_variable_any				BoostCondition;
