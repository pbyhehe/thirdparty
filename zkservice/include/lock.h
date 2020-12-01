/*
 * Copyright (c) 2013 www.360buy.com
 * All rights reserved.
 *
 * @FILE: 		lock.h
 * @AUTHOR: 	liyongqiang (yfliyongqiang@360buy.com)
 * @DATE: 		2013-01-24
 * @VERSION: 	1.0
 *
 * @DESCRIPTION:Header file for class RWLock ReaderLock
 * 				WriterLock Lock MutexLock.
 *				All of those should be used as scoped locks.
 */
#ifndef JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_LOCK_H_
#define JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_LOCK_H_
#include <pthread.h>

namespace zk_util {
class RWLock;
class Lock;
class ReaderLock;
class WriterLock;
class MutexLock;

class RWLock {
 public:
    RWLock() { pthread_rwlock_init(&rwlock_, NULL); }
    ~RWLock() { pthread_rwlock_destroy(&rwlock_); }
    friend class ReaderLock;
    friend class WriterLock;

 private:
    pthread_rwlock_t rwlock_;
};

class Lock {
 public:
    Lock() { pthread_mutex_init(&lock_, NULL); }
    ~Lock() { pthread_mutex_destroy(&lock_); }
    friend class MutexLock;

 private:
    pthread_mutex_t lock_;
};

class ReaderLock {
 public:
    explicit ReaderLock(RWLock *lockp) : lockp_(lockp) { pthread_rwlock_rdlock(&lockp_->rwlock_); }
    ~ReaderLock() { pthread_rwlock_unlock(&lockp_->rwlock_); }

 private:
    RWLock *lockp_;
};

class WriterLock {
 public:
    explicit WriterLock(RWLock *lockp) : lockp_(lockp) { pthread_rwlock_wrlock(&lockp_->rwlock_); }
    ~WriterLock() { pthread_rwlock_unlock(&lockp_->rwlock_); }

 private:
    RWLock *lockp_;
};

class MutexLock {
 public:
    explicit MutexLock(Lock *lockp) : lockp_(lockp) { pthread_mutex_lock(&lockp_->lock_); }
    ~MutexLock() { pthread_mutex_unlock(&lockp_->lock_); }

 private:
    Lock *lockp_;
};
}  // namespace zk_util
#endif  // JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_LOCK_H_
