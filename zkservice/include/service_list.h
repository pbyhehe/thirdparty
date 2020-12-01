/*
 * Copyright (c) 2018 xueyaohong@jd.com. All rights reserved.
 */
#ifndef JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_SERVICE_LIST_H_
#define JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_SERVICE_LIST_H_

#include <stdint.h>
#include <vector>
#include <string>
#include <memory>

namespace zk_util {
class RWLock;
}

namespace Zookeeper {
class ServiceDiscovery;

/*
 *@bref ServiceListManager class to manage service list fetched from zk server
 */
class ServiceListManager {
 public:
    typedef std::vector<std::string> Container;
    typedef std::unique_ptr<std::vector<std::string> > ContainerPtr;

    ServiceListManager();

    ~ServiceListManager();

 public:
    void GetServiceList(Container* service_list);
    std::string RoundRobin();

 private:
    // instance to update service list by friend ServiceDiscovery
    ContainerPtr SetServiceList(ContainerPtr service_list);

 private:
    ContainerPtr _service_list_ptr;
    zk_util::RWLock* _lock;
    int64_t _rr_idx;

    friend class Zookeeper::ServiceDiscovery;
};  // class ServiceListManager

}  // namespace Zookeeper
#endif  // JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_SERVICE_LIST_H_
