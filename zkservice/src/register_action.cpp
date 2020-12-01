/*
 * Copyright (c) 2018 xueyaohong@jd.com. All rights reserved.
 */
extern "C" {
#include <zookeeper.h>
}
#include "zkservice/include/register_action.h"
#include "zkservice/include/lock.h"
#include "zkservice/include/watcher_action.h"
#include "zkservice/include/zookeeper_client.h"
#include "zkservice/include/service_register.h"

namespace Zookeeper {

RegServiceWatcherAction::RegServiceWatcherAction(ServiceRegister *service_register, WatcherAction *watcher_action) {
    std::string server_node = service_register->GetServerNode();
    if (m_server.end() == m_server.find(server_node)) {
        m_server.insert(make_pair(server_node, ptrlib::shared_ptr<ServiceRegister>(service_register)));
    }
    assert(m_server.size() > 0);
    m_watcher_action.reset(watcher_action);
    _is_running = true;
}

RegServiceWatcherAction::RegServiceWatcherAction(const std::vector<ServiceRegister *> &server,
                                                 WatcherAction *watcher_action) {
    std::map<std::string, ptrlib::shared_ptr<ServiceRegister> >::iterator itor;
    for (size_t i = 0; i < server.size(); ++i) {
        std::string server_node = server[i]->GetServerNode();
        // fprintf(stderr, "%s\n", server_node.c_str());
        itor = m_server.find(server_node);
        if (itor == m_server.end()) {
            m_server.insert(make_pair(server_node, ptrlib::shared_ptr<ServiceRegister>(server[i])));
        }
    }
    assert(m_server.size() > 0);
    m_watcher_action.reset(watcher_action);
    _is_running = true;
}

RegServiceWatcherAction::~RegServiceWatcherAction() { _is_running = false; }

void RegServiceWatcherAction::OnConnected() {
    fprintf(stderr, "OnConnected\n");
    std::map<std::string, ptrlib::shared_ptr<ServiceRegister> >::iterator itor = m_server.begin();
    while (itor != m_server.end()) {
        itor->second->SetZookeeperClient(owner_);
        itor->second->Join();
        ++itor;
    }
    m_watcher_action != NULL ? m_watcher_action->OnConnected() : WatcherAction::OnConnected();
}

void RegServiceWatcherAction::OnNodeCreated(const char *path) {
    fprintf(stderr, "OnNodeCreated\n");
    std::map<std::string, ptrlib::shared_ptr<ServiceRegister> >::iterator itor = m_server.find(std::string(path));
    if (itor == m_server.end()) {
        return m_watcher_action != NULL ? m_watcher_action->OnNodeCreated(path) : WatcherAction::OnNodeCreated(path);
    } else {
        bool exists;
        owner_->NodeExists(path, &exists);
        return WatcherAction::OnNodeCreated(path);
    }
}

void RegServiceWatcherAction::OnNodeValueChanged(const char *path) {
    fprintf(stderr, "OnNodeValueChanged\n");
    std::map<std::string, ptrlib::shared_ptr<ServiceRegister> >::iterator itor = m_server.find(std::string(path));
    if (itor == m_server.end()) {
        return m_watcher_action != NULL ? m_watcher_action->OnNodeValueChanged(path)
                                        : WatcherAction::OnNodeValueChanged(path);
    } else {
        itor->second->Join();
        return WatcherAction::OnNodeValueChanged(path);
    }
}

void RegServiceWatcherAction::OnNodeDeleted(const char *path) {
    fprintf(stderr, "OnNodeDeleted\n");
    std::map<std::string, ptrlib::shared_ptr<ServiceRegister> >::iterator itor = m_server.find(std::string(path));
    if (itor == m_server.end()) {
        return m_watcher_action != NULL ? m_watcher_action->OnNodeDeleted(path) : WatcherAction::OnNodeDeleted(path);
    } else if (_is_running) {
        itor->second->Join();
        return WatcherAction::OnNodeDeleted(path);
    }
}

void RegServiceWatcherAction::OnChildChanged(const char *path) {
    fprintf(stderr, "OnChildChanged\n");
    std::map<std::string, ptrlib::shared_ptr<ServiceRegister> >::iterator itor = m_server.find(std::string(path));
    if (itor == m_server.end())
        return m_watcher_action != NULL ? m_watcher_action->OnChildChanged(path) : WatcherAction::OnChildChanged(path);
    else
        return WatcherAction::OnChildChanged(path);
}

}  // namespace Zookeeper
