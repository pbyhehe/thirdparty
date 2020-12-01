/*
 * Copyright (c) 2013 www.360buy.com
 * All rights reserved.
 *
 * @FILE: 		zookeeper_client.cc
 * @AUTHOR: 	liyongqiang (yfliyongqiang@360buy.com)
 * @DATE: 		2013-01-24
 * @VERSION: 	1.0
 *
 * @DESCRIPTION:Implementation of class ZookeeperClient
 */

#include <string.h>
#include <cassert>
#include "zkservice/include/zookeeper_client.h"
#include "zkservice/include/watcher_action.h"

namespace Zookeeper {

ZookeeperClient::ZookeeperClient() : host_(""), recv_timeout_(10000), zh_(NULL) {}

ZookeeperClient::~ZookeeperClient() { Close(); }

void ZookeeperClient::SetHost(const std::string host) { host_ = host; }

void ZookeeperClient::SetRecvTimeoutMs(int32_t recv_timeout) { recv_timeout_ = recv_timeout * 3; }

void ZookeeperClient::SetWatcher(WatcherAction *watcherp) {
    watcherp_.reset(watcherp);
    assert(watcherp_ != NULL);
    // fprintf(stderr, "===========================%d\n", watcherp_.use_count());
}

const string ZookeeperClient::GetHost() { return host_; }

const WatcherAction *ZookeeperClient::GetWatcher() { return watcherp_.get(); }

const zhandle_t *ZookeeperClient::GetRawZkHandlePtr() { return zh_; }

int32_t ZookeeperClient::Open() {
    zk_util::MutexLock mlock(&lock_);
    if (zh_) {
        fprintf(stderr, "zookeeper_init failed,zh_ already exit\n");
        return -2;
    }
    zh_ = zookeeper_init(host_.c_str(), active_watcher, recv_timeout_, 0, watcherp_.get(), 0);

    if (zh_ == NULL) return -1;

    zoo_set_debug_level(ZOO_LOG_LEVEL_ERROR);
    return 0;
}

int32_t ZookeeperClient::Close() {
    zk_util::MutexLock mlock(&lock_);
    int32_t rc = zookeeper_close(zh_);
    if (rc == ZOK) {
        zh_ = NULL;
    }
    return rc;
}

int32_t ZookeeperClient::ReOpen() {
    zk_util::MutexLock mlock(&lock_);
    if (zh_ != NULL) {
        zookeeper_close(zh_);
    }

    zh_ = zookeeper_init(host_.c_str(), active_watcher, recv_timeout_, 0, watcherp_.get(), 0);

    if (zh_ == NULL) return -1;

    zoo_set_debug_level(ZOO_LOG_LEVEL_ERROR);
    return 0;
}

int32_t ZookeeperClient::GetChildren(const char *path, std::vector<std::string> *const children) {
    struct String_vector str_vec;
    struct Stat stat;
    int32_t rc;

    {
        zk_util::MutexLock mlock(&lock_);
        if (!zh_) {
            return -1;
        }
        rc = zoo_wget_children2(zh_, path, active_watcher, watcherp_.get(), &str_vec, &stat);
    }
    if (rc == ZOK && children) {
        for (int32_t i = 0; i < str_vec.count; ++i) {
            children->push_back(/*std::string(path)+"/"+*/ std::string(str_vec.data[i]));
        }
    }

    return rc;
}

int32_t ZookeeperClient::GetNodeValue(const char *path, std::string *const value) {
    struct Stat stat;
    int32_t rc;
    char val[MAX_VALUE_LEN] = {'\0'};
    int32_t val_len = MAX_VALUE_LEN;

    {
        zk_util::MutexLock mlock(&lock_);
        if (!zh_) {
            return -1;
        }
        rc = zoo_wget(zh_, path, active_watcher, watcherp_.get(), val, &val_len, &stat);
    }
    if (rc == ZOK && value) {
        *value = std::string(val);
    }

    return rc;
}

int32_t ZookeeperClient::NodeExists(const char *path, bool *const exists) {
    struct Stat stat;
    int32_t rc;

    {
        zk_util::MutexLock mlock(&lock_);
        if (!zh_) {
            return -1;
        }
        rc = zoo_wexists(zh_, path, active_watcher, watcherp_.get(), &stat);
    }
    if (rc == ZOK && exists) {
        *exists = true;
    } else if (exists) {
        *exists = false;
    }

    return rc;
}

int32_t ZookeeperClient::NodeCreate(const char *path, const char *value, const struct ACL_vector *acl, int flags) {
    int32_t rc;
    char val[MAX_VALUE_LEN] = {'\0'};
    int32_t val_len = MAX_VALUE_LEN;
    char pth[MAX_PATH_LEN] = {'\0'};
    int32_t pth_len = MAX_PATH_LEN - 1;

    if (strlen(value) >= MAX_VALUE_LEN) {
        fprintf(stderr, "Value_length too big\n");
        return ZBADARGUMENTS;
    }
    strncpy(val, value, strlen(value));
    val_len = strlen(value);

    {
        zk_util::MutexLock mlock(&lock_);
        if (!zh_) {
            return -1;
        }
        rc = zoo_create(zh_, path, val, val_len, acl, flags, pth, pth_len);
    }

    return rc;
}

int32_t ZookeeperClient::NodeDelete(const char *path) {
    int32_t rc;

    {
        zk_util::MutexLock mlock(&lock_);
        if (!zh_) {
            return -1;
        }
        rc = zoo_delete(zh_, path, -1);
    }

    return rc;
}
}  // namespace Zookeeper
