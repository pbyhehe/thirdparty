/*
 * Copyright (c) 2013 www.360buy.com
 * All rights reserved.
 *
 * @FILE: 		zookeeper_client.h
 * @AUTHOR: 	liyongqiang (yfliyongqiang@360buy.com)
 * @DATE: 		2013-01-24
 * @VERSION: 	1.0
 *
 * @DESCRIPTION:Header file of class ZookeeperClient
 */
#ifndef JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_ZOOKEEPER_CLIENT_H_
#define JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_ZOOKEEPER_CLIENT_H_

extern "C" {
#include <zookeeper.h>
}
#include <string>
#include <vector>
#include "boost/include/boost/shared_ptr.hpp"
#include "zkservice/include/lock.h"

#define MAX_VALUE_LEN 8192
#define MAX_PATH_LEN 8192

using std::string;
namespace Zookeeper {
namespace ptrlib = boost;

class WatcherAction;
class ZookeeperClient {
 public:
    ZookeeperClient();
    ~ZookeeperClient();

 public:
    void SetHost(const std::string host);
    void SetRecvTimeoutMs(int32_t recv_timeout);
    void SetWatcher(WatcherAction *watcherp);
    const string GetHost();
    const WatcherAction *GetWatcher();
    const zhandle_t *GetRawZkHandlePtr();
    int32_t Open();
    int32_t ReOpen();
    int32_t Close();
    int32_t GetChildren(const char *path, std::vector<std::string> *const children);
    int32_t GetNodeValue(const char *path, std::string *const value);
    int32_t NodeExists(const char *path, bool *const exists);
    int32_t NodeCreate(const char *path, const char *value, const struct ACL_vector *acl, int flags);
    int32_t NodeDelete(const char *path);

 private:
    ptrlib::shared_ptr<WatcherAction> watcherp_;
    std::string host_;
    int32_t recv_timeout_;
    zhandle_t *zh_;
    zk_util::Lock lock_;
};
}  // namespace Zookeeper
#endif  // JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_ZOOKEEPER_CLIENT_H_
