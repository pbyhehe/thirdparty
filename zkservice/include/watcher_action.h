/*
 * Copyright (c) 2013 www.360buy.com
 * All rights reserved.
 *
 * @FILE: 		watcher_action.h
 * @AUTHOR: 	liyongqiang (yfliyongqiang@360buy.com)
 * @DATE: 		2013-01-24
 * @VERSION: 	1.0
 *
 * @DESCRIPTION:Header file of class WatcherAction
 *				ServiceWatcherAction RegServiceWatcherAction
 */
#ifndef JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_WATCHER_ACTION_H_
#define JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_WATCHER_ACTION_H_

extern "C" {
#include <zookeeper.h>
}
#include <stdio.h>
#include <string>
#include "zkservice/include/zookeeper_client.h"

namespace Zookeeper {

/**
* @class WatcherAction Base class of the watcheraction
*/
class WatcherAction {
 protected:
    ZookeeperClient *owner_;

 public:
    WatcherAction() { owner_ = NULL; }
    virtual ~WatcherAction() {}

 public:
    void SetOwner(ZookeeperClient *zk) { owner_ = zk; }

    virtual void OnSessionExpired() {
        owner_->ReOpen();
        fprintf(stderr, "OnSessionExpired\n");
    }
    virtual void OnAuthFailed() { fprintf(stderr, "OnAuthFailed\n"); }
    virtual void OnConnecting() { fprintf(stderr, "OnConnecting\n"); }
    virtual void OnAssociating() { fprintf(stderr, "OnAssociating\n"); }
    virtual void OnConnected() { fprintf(stderr, "OnConnected\n"); }
    virtual void OnDisconnected() { fprintf(stderr, "OnDisconnected\n"); }
    virtual void OnNodeCreated(const char *path) { fprintf(stderr, "OnNodeCreated:%s\n", path); }
    virtual void OnNodeValueChanged(const char *path) { fprintf(stderr, "OnNodeValueChanged:%s\n", path); }
    virtual void OnNodeDeleted(const char *path) { fprintf(stderr, "OnNodeDeleted:%s\n", path); }
    virtual void OnChildChanged(const char *path) { fprintf(stderr, "OnChildChanged:%s\n", path); }
    virtual void OnNotWatching(const char *path) { fprintf(stderr, "OnNotWatching:%s\n", path); }

    friend void active_watcher(zhandle_t *zh, int type, int state, const char *path, void *ctx);
};

inline void active_watcher(zhandle_t *zh, int type, int state, const char *path, void *ctx) {
    if (zh == NULL || ctx == NULL) {
        return;
    }

    WatcherAction *actionp = reinterpret_cast<WatcherAction *>(ctx);

    if (type == ZOO_CREATED_EVENT) {
        actionp->OnNodeCreated(path);
    } else if (type == ZOO_DELETED_EVENT) {
        actionp->OnNodeDeleted(path);
    } else if (type == ZOO_CHANGED_EVENT) {
        actionp->OnNodeValueChanged(path);
    } else if (type == ZOO_CHILD_EVENT) {
        actionp->OnChildChanged(path);
    } else if (type == ZOO_SESSION_EVENT) {
        if (state == ZOO_EXPIRED_SESSION_STATE) {
            actionp->OnSessionExpired();
        } else if (state == ZOO_AUTH_FAILED_STATE) {
            actionp->OnAuthFailed();
        } else if (state == ZOO_CONNECTING_STATE) {
            actionp->OnConnecting();
        } else if (state == ZOO_ASSOCIATING_STATE) {
            actionp->OnAssociating();
        } else if (state == ZOO_CONNECTED_STATE) {
            actionp->OnConnected();
        }
    } else if (type == ZOO_NOTWATCHING_EVENT) {
        actionp->OnNotWatching(path);
    } else {
        fprintf(stderr, "[ERROR] Invalid action type:%d\n", type);
    }
}
}  // namespace Zookeeper
#endif  // JD_SEARCH_COMMON_3RDPARTY_3RDPARTY_CENTOS7_ZKSERVICE_INCLUDE_WATCHER_ACTION_H_
