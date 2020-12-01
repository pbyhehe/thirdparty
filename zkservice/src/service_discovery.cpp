/*
 * Copyright (c) 2018 xueyaohong@jd.com. All rights reserved.
 */
#include "zkservice/include/lock.h"
#include "zkservice/include/service_discovery.h"
#include "zkservice/include/service_list.h"

namespace Zookeeper {

// class ServiceDiscoveryFactory
ServiceDiscoveryFactory::Storage ServiceDiscoveryFactory::_map_apps;
zk_util::Lock *ServiceDiscoveryFactory::_lock = new zk_util::Lock();

ServiceDiscoveryFactory::ServiceDiscoveryPtr ServiceDiscoveryFactory::Get(std::string host, std::string path,
                                                                          int timeout) {
    std::string app = host + path;
    zk_util::MutexLock lck(_lock);
    Storage::iterator it = _map_apps.find(app);
    if (it == _map_apps.end()) {
        ptrlib::shared_ptr<ServiceDiscovery> sd(new ServiceDiscovery(host, path, timeout));
        _map_apps[app] = sd;
    }
    return _map_apps[app];
}

// class ServiceDiscovery
ServiceDiscovery::ServiceDiscovery(std::string host, std::string path, int timeout) : _watch_path(path) {
    _service_list_mgr.reset(new ServiceListManager());
    owner_ = new ZookeeperClient();
    owner_->SetHost(host);
    owner_->SetRecvTimeoutMs(timeout);
    owner_->SetWatcher(this);
    int ret = owner_->Open();
}

ServiceDiscovery::~ServiceDiscovery() {
    if (owner_ != nullptr) {
        delete owner_;
    }
}

void ServiceDiscovery::SetOwner(ZookeeperClient *zk) {
    // local (ZookeeperClient)owner_ can not be changed
}

void ServiceDiscovery::OnConnected() {
    bool exists = false;
    owner_->NodeExists(_watch_path.c_str(), &exists);
    if (exists) UpdateServiceList();
    WatcherAction::OnConnected();
}

void ServiceDiscovery::OnNodeCreated(const char *path) {
    UpdateServiceList();
    WatcherAction::OnNodeCreated(path);
}

void ServiceDiscovery::OnNodeValueChanged(const char *path) { WatcherAction::OnNodeValueChanged(path); }

void ServiceDiscovery::OnNodeDeleted(const char *path) {
    bool exists = false;
    int ret = owner_->NodeExists(path, &exists);
    if (!exists) {
        UpdateServiceList(true);
    } else {
        UpdateServiceList();
    }
    WatcherAction::OnNodeDeleted(path);
}

void ServiceDiscovery::OnChildChanged(const char *path) {
    UpdateServiceList();
    WatcherAction::OnChildChanged(path);
}

int ServiceDiscovery::UpdateServiceList(bool clear_old) {
    ServiceListManager::ContainerPtr tmp(new ServiceListManager::Container());
    if (!clear_old) {
        // if connection lost, do NOT update the service list
        if (owner_ == nullptr) return -1;
        int ret = owner_->GetChildren(_watch_path.c_str(), tmp.get());
        if (ret != 0) return ret;
    }
    // can NOT use \tmp after this call
    if (nullptr != _service_list_mgr) {
        tmp = _service_list_mgr->SetServiceList(std::move(tmp));
    }
    return 0;
}

};  // namespace Zookeeper
