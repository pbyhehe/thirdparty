/*
 * Copyright (c) 2018 xueyaohong@jd.com. All rights reserved.
 */
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "zkservice/include/service_discovery.h"
#include "zkservice/include/service_list.h"

/*
 * @bref use zookeeper service discovery
 * @param
 *  zk_host zookeeper server list
 *  service_path service registered path
 *  zk_timeout_ms zookeeper session timeout
 */
void ZkServiceDiscovery(string zk_host, string service_path, int zk_timeout_ms) {
    // step1: get the ServiceDiscovery object
    boost::shared_ptr<Zookeeper::ServiceDiscovery> service_discover =
        Zookeeper::ServiceDiscoveryFactory::Get(zk_host, service_path, zk_timeout_ms);
    // step2: get ServiceListManager object
    boost::shared_ptr<Zookeeper::ServiceListManager> service_list_mgr =
        service_discover->GetServiceListManager();
    // step3: using service list
    std::vector<std::string> service_list;
    while (1) {
        // get services list
        service_list_mgr->GetServiceList(&service_list);
        std::cout << "Service list now are: [";
        for (int i = 0; i < service_list.size(); ++i) {
            std::cout << service_list[i] << " ";
        }
        std::cout << "]" << std::endl;
        // get one service by round robin
        std::cout << "RoudRobin: " << service_list_mgr->RoundRobin() << std::endl;
        service_list.clear();
        std::cout << "size: " << service_list.size() << std::endl;
        sleep(2);
    }
}

int main() {
    // zk host list
    std::string zk_host = "172.20.167.111:2181,172.20.145.104:2181,172.18.179.98:2181";
    // zk session timeout
    int zk_timeout_ms = 2000;
    // path of services
    std::string service_home = "/xueyaohong/zk_client/example";

    ZkServiceDiscovery(zk_host, service_home, zk_timeout_ms);

    return 0;
}
