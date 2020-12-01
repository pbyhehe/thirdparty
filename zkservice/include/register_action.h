/*
 * Copyright (c) 2013 www.360buy.com
 * All rights reserved.
 *
 * @FILE:         watcher_action.h
 * @AUTHOR:     liyongqiang (yfliyongqiang@360buy.com)
 * @DATE:         2013-01-24
 * @VERSION:     1.0
 *
 * @DESCRIPTION:Header file of class WatcherAction
 *                ServiceWatcherAction RegServiceWatcherAction
 */
#ifndef JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_REGISTER_ACTION_H_
#define JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_REGISTER_ACTION_H_

#include <map>
#include <vector>
#include <string>
#include "boost/include/boost/shared_ptr.hpp"
#include "zkservice/include/watcher_action.h"

namespace Zookeeper {
namespace ptrlib = boost;

class ServiceRegister;

/**
* @class RegServiceWatcherAction Class for the watcher of
*    the zookeeper,used by the regist_service's zookeeper client.
*/
class RegServiceWatcherAction : public WatcherAction {
 public:
    explicit RegServiceWatcherAction(ServiceRegister *server, WatcherAction *watcher_action = NULL);

    explicit RegServiceWatcherAction(const std::vector<ServiceRegister *> &server,
                                     WatcherAction *watcher_action = NULL);

    virtual ~RegServiceWatcherAction();

    virtual void OnConnected();
    virtual void OnNodeCreated(const char *path);
    virtual void OnNodeValueChanged(const char *path);
    virtual void OnNodeDeleted(const char *path);
    virtual void OnChildChanged(const char *path);

 private:
    ptrlib::shared_ptr<WatcherAction> m_watcher_action;
    std::map<std::string, ptrlib::shared_ptr<ServiceRegister> > m_server;
    bool _is_running;
};

}  // namespace Zookeeper
#endif  // JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_REGISTER_ACTION_H_
