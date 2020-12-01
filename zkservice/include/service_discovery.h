/*
 * Copyright (c) 2018 xueyaohong@jd.com. All rights reserved.
 */
#ifndef JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_SERVICE_DISCOVERY_H_
#define JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_SERVICE_DISCOVERY_H_

#include <string>
#include <vector>
#include "boost/include/boost/unordered_map.hpp"
#include "zkservice/include/watcher_action.h"

namespace zk_util {
class Lock;
class MutexLock;
}

namespace Zookeeper {
class ServiceListManager;
class ServiceDiscovery;

/*
 * @bref ServiceDiscoveryFactory Class to generate and manage ServiceDiscovery objects
 */
class ServiceDiscoveryFactory {
 public:
    typedef ptrlib::shared_ptr<ServiceDiscovery> ServiceDiscoveryPtr;

    ~ServiceDiscoveryFactory() {
        if (_lock) delete _lock;
    }

    /*
     * @bref Get the object of ServiceDiscovery by app identifier(pair of zk host and app register path)
     *
     * @param IN host, the zk host list
     *
     * @param IN path, app register path
     *
     * @param IN timeout, zk session timeout
     **/
    static ServiceDiscoveryPtr Get(std::string host, std::string path, int timeout = 10000);

 private:
    typedef ptrlib::unordered_map<std::string, ServiceDiscoveryPtr> Storage;
    static Storage _map_apps;
    static zk_util::Lock *_lock;
};  // class ServiceDiscoveryFactory

/**
 * @class ServiceDiscovery Class derived from watcher of the zookeeper.
 *    the service-list stored in _service_list_mgr which will be update
 *    once the service list at the path of zk changes
 */
class ServiceDiscovery : private WatcherAction {
 public:
    ServiceDiscovery(std::string host, std::string path, int timeout);
    ~ServiceDiscovery();

    /*
     * @bref SetServiceListManager
     */
    void SetServiceListManager(ptrlib::shared_ptr<ServiceListManager> mgr) {
        _service_list_mgr = mgr;
        UpdateServiceList();
    }

    /*
     * @bref GetServiceListManager
     */
    ptrlib::shared_ptr<ServiceListManager> GetServiceListManager() { return _service_list_mgr; }

    void SetTimeout(int timeout);

 private:
    void SetOwner(ZookeeperClient *zk);
    virtual void OnConnected();
    virtual void OnNodeCreated(const char *path);
    virtual void OnNodeValueChanged(const char *path);
    virtual void OnNodeDeleted(const char *path);
    virtual void OnChildChanged(const char *path);
    int UpdateServiceList(bool clear_old = false);

 private:
    std::string _watch_path;
    ptrlib::shared_ptr<ServiceListManager> _service_list_mgr;

    friend class ServiceDiscoveryFacotory;
};  // class ServiceDiscovery

};  // namespace Zookeeper

#endif  // JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_SERVICE_DISCOVERY_H_
