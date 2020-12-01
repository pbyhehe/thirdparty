/*
 * Copyright (c) 2018 xueyaohong@jd.com. All rights reserved.
 */
#include <vector>
#include "zkservice/include/service_register.h"

namespace Zookeeper {

ServiceRegister::ServiceRegister(const string &service_path, const string &service_name, const string &service_value) {
    assert(service_path.empty() == false && service_name.empty() == false);
    // string real_service_path(DEFAULT_NAMESPACE);
    string real_service_path;
    if (service_path.c_str()[0] == '/') {
        real_service_path += service_path;
    } else {
        real_service_path += "/";
        real_service_path += service_path;
    }
    if (real_service_path.c_str()[real_service_path.size() - 1] != '/') {
        real_service_path += "/";
    }
    // real_service_path += "rpc/";
    real_service_path += service_name;

    service_path_ = real_service_path;
    StringAlgo::tokenize(service_path_, "/", zk_nodes_);
    assert(zk_nodes_.size() > 0);
    fprintf(stderr, "service_path: %s\n", service_path_.c_str());
    // for (int i = 0; i < zk_nodes_.size(); ++i) fprintf(stderr, "%s\n", zk_nodes_[i].c_str());
    service_value_ = service_value;
}

ServiceRegister::~ServiceRegister() { Quit(); }

int32_t ServiceRegister::Join() {
    assert(zclientp_ != NULL);
    int32_t rc;
    bool exists;
    std::vector<string> &strs = zk_nodes_;

    string curr_path = "";
    for (size_t i = 0; i < strs.size(); ++i) {
        curr_path += "/";
        curr_path += strs[i];

        // fprintf(stderr, "join ++++++++++++++++++++:%s\n", curr_path.c_str());
        rc = zclientp_->NodeExists(curr_path.c_str(), &exists);
        if (rc == ZOK && exists == true) {
            fprintf(stderr, "node %s exist\n", curr_path.c_str());
            if (i == strs.size() - 1) {
                string node_value;
                rc = zclientp_->GetNodeValue(curr_path.c_str(), &node_value);
                if (rc == ZOK && service_value_ != node_value)
                    rc = zclientp_->NodeCreate(curr_path.c_str(), service_value_.c_str(), &ZOO_OPEN_ACL_UNSAFE,
                                               ZOO_EPHEMERAL);
                fprintf(stderr, "create node %s %d\n", curr_path.c_str(), rc != ZOK ? -1 : 0);
                return rc != ZOK ? -1 : 0;
            } else {
                continue;
            }
        }

        if (i == strs.size() - 1) {
            rc = zclientp_->NodeCreate(curr_path.c_str(), service_value_.c_str(), &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL);
            fprintf(stderr, "create node %s %d\n", curr_path.c_str(), rc != ZOK ? -1 : 0);
        } else {
            rc = zclientp_->NodeCreate(curr_path.c_str(), "jd_service", &ZOO_OPEN_ACL_UNSAFE, 0);
            fprintf(stderr, "create node %s %d\n", curr_path.c_str(), rc != ZOK ? -1 : 0);
        }

        if (rc != ZOK) {
            return -1;
        }
    }
    return 0;
}

string ServiceRegister::FetchHost() {
    char hostname[256];
    char **pptr;
    char str[32];
    string host;

    gethostname(hostname, sizeof(hostname));
    struct hostent *hptr = gethostbyname(hostname);
    if (hptr) {
        switch (hptr->h_addrtype) {
            case AF_INET:
                pptr = hptr->h_addr_list;
                for (; *pptr != NULL; pptr++) {
                    memset(str, 0, sizeof(str));
                    inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str));
                    if (strncmp(str, "127.", 4) != 0) {
                        host = str;
                        break;
                    }
                }
                break;
            default:
                break;
        }
    }
    if (host.empty()) {
        struct ifreq iface;
        strcpy(iface.ifr_ifrn.ifrn_name, "eth0");  // NOLINT
        int fd = socket(AF_INET, SOCK_DGRAM, 0);
        assert(fd != -1);
        if (ioctl(fd, SIOCGIFADDR, &iface)) return "";
        close(fd);
        char ip[32];
        inet_ntop(AF_INET, &((struct sockaddr_in *)(&iface.ifr_ifru.ifru_addr))->sin_addr, ip, sizeof(ip));  // NOLINT
        host = ip;
    }
    return host;
}

}  // namespace Zookeeper
