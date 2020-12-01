/*
 * Copyright (c) 2018 xueyaohong@jd.com. All rights reserved.
 */
#include "zkservice/include/service_list.h"
#include "zkservice/include/service_discovery.h"
#include "zkservice/include/lock.h"

namespace Zookeeper {

ServiceListManager::ServiceListManager()
    : _service_list_ptr(new Container()), _lock(new zk_util::RWLock()), _rr_idx(0) {}

ServiceListManager::~ServiceListManager() {
    if (nullptr != _lock) {
        delete _lock;
    }
}

void ServiceListManager::GetServiceList(Container* service_list) {
    if (service_list == nullptr) {
        return;
    }
    service_list->clear();
    zk_util::ReaderLock rlock(_lock);
    service_list->reserve(_service_list_ptr->size());
    for (Container::iterator it = _service_list_ptr->begin(); it != _service_list_ptr->end(); ++it) {
        service_list->emplace_back(*it);
    }
}

std::string ServiceListManager::RoundRobin() {
    int64_t rr_idx = __sync_fetch_and_add(&_rr_idx, 1);
    zk_util::ReaderLock rlock(_lock);
    if (_service_list_ptr->size() == 0) {
        return std::string();
    }
    int64_t idx = rr_idx % _service_list_ptr->size();
    return (*_service_list_ptr)[idx];
}

ServiceListManager::ContainerPtr ServiceListManager::SetServiceList(ContainerPtr service_list) {
    zk_util::WriterLock wlock(_lock);
    _service_list_ptr.swap(service_list);
    return service_list;
}

}  // namespace Zookeeper
