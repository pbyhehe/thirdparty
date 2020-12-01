/*
 * Copyright (c) 2018 xueyaohong@jd.com. All rights reserved.
 */
#include <string>
#include <vector>
#include <iostream>
#include "zkservice/include/zookeeper_client.h"
#include "zkservice/include/register_action.h"
#include "zkservice/include/service_register.h"

Zookeeper::ZookeeperClient* kZookeeperClientPtr = NULL;
Zookeeper::RegServiceWatcherAction* kWatcher = NULL;

// Register ONE service to zk
int ZkRegisterService(const std::string& zk_host, const std::string& service_path, const std::string& service_name,
                      const std::string& service_value, const unsigned int zk_timeout_ms = 2000) {
    // new Zookeeper Client
    kZookeeperClientPtr = new (std::nothrow) Zookeeper::ZookeeperClient();
    if (NULL == kZookeeperClientPtr) {
        std::cerr << "Failed to create ZookeeperClient" << std::endl;
        return -1;
    }
    kZookeeperClientPtr->SetHost(zk_host);
    kZookeeperClientPtr->SetRecvTimeoutMs(zk_timeout_ms);

    // new ServiceRegister, owned by RegServiceWatcherAction
    Zookeeper::ServiceRegister* service_register =
        new (std::nothrow) Zookeeper::ServiceRegister(service_path, service_name, service_value);
    if (NULL == service_register) {
        std::cerr << "Failed to create ServiceRegister" << std::endl;
        return -2;
    }
    service_register->SetZookeeperClient(kZookeeperClientPtr);

    // new RegServiceWatcherAction, owned by user
    kWatcher = new (std::nothrow) Zookeeper::RegServiceWatcherAction(service_register);
    if (NULL == kWatcher) {
        std::cerr << "Failed to create RegServiceWatcherAction" << std::endl;
        return -3;
    }
    kWatcher->SetOwner(kZookeeperClientPtr);

    // start to use the register watcher
    kZookeeperClientPtr->SetWatcher(kWatcher);
    int ret = kZookeeperClientPtr->Open();
    if (0 != ret) {
        std::cerr << "Failed to open ZookeeperClient, ret=" << ret << std::endl;
    }
    return 0;
}

int main() {
    // config zookeeper
    std::string zk_host = "172.20.167.111:2181,172.20.145.104:2181,172.18.179.98:2181";
    int zk_timeout_ms = 2000;

    // config service1
    std::string service_path = "/xueyaohong/zk_client/example";
    std::string service_port = "12345";  // guarantee the legality of this value here
    std::string hostname = Zookeeper::ServiceRegister::FetchHost();
    if (hostname.empty()) {
        std::cerr << "Fetch hostname failed!" << std::endl;
    }
    std::string service_name = hostname + ":" + service_port;
    std::string service_value = "";
    // register service
    int ret = ZkRegisterService(zk_host, service_path, service_name, service_value, zk_timeout_ms);

    // working
    std::cout << "serving..." << std::endl;
    sleep(3);

    // clean
    if (NULL != kZookeeperClientPtr) {
        kZookeeperClientPtr->Close();
    }
    if (NULL != kWatcher) {
        delete kWatcher;
    }
    std::cout << "service closed." << std::endl;
    return 0;
}
