/*
 * Copyright (c) 2013 www.360buy.com
 * All rights reserved.
 *
 * @FILE: 		server_cluster.h
 * @AUTHOR: 	liyongqiang (yfliyongqiang@360buy.com)
 * @DATE: 		2013-01-24
 * @VERSION: 	1.0
 *
 * @DESCRIPTION:Header file and implementation of class
 *				ServiceRegister
 *				This is used to regist service to zookeeper
 *				when a rpc service starts,so the users can
 *				be notified that the server is online.
 */
#ifndef JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_SERVICE_REGISTER_H_
#define JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_SERVICE_REGISTER_H_

#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string>
#include <vector>
#include "zkservice/include/zookeeper_client.h"
// #include "zkservice/include/defines.h"
#include "zkservice/include/string_algo.h"

namespace Zookeeper {
using std::string;

class ServiceRegister {
 public:
    /**
    * Constructor
    *
    * It's not preferred to use this.Try use another one instead.
    *
    * @param zclientp Pointer of a zookeeper client
    * @param service_path String of the service path registed on
    *	zookeeper server.
    */
    ServiceRegister(const string &service_path, const string &service_name, const string &service_value);
    ~ServiceRegister();

    /**
    * Function join a service into the cluster.
    * @param service_port Port of the service.
    */
    int32_t Join();

    void SetZookeeperClient(ZookeeperClient *zclientp) { zclientp_ = zclientp; }

    const string GetServerNode() const { return service_path_; }

    void Quit() {
        fprintf(stderr, "[DEBUG]: quit\n");
        int32_t rc = zclientp_->NodeDelete(service_path_.c_str());
        fprintf(stderr, "[DEBUG]: rc=%d\n", rc);
    }

    static string FetchHost();

 private:
    ZookeeperClient *zclientp_;
    string service_path_;
    string service_value_;
    std::vector<string> zk_nodes_;
};  // class ServiceRegister

}  // namespace Zookeeper
#endif  // JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_SERVICE_REGISTER_H_
