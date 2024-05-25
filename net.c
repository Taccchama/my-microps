#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "platform.h"

#include "util.h"
#include "net.h"

/*
 * NOTE: if you want to add/delete the entries after net_run(),
 *       you need to protect these lists with a lock.
 */
static struct net_device *devices;

struct net_device *net_device_alloc(void) {
    struct net_device *dev;

    dev = memory_alloc(sizeof(*dev));
    if (!dev) {
        errorf("memory_alloc() failure");
        return NULL;
    }

    return dev;
}

/*
 * NOTE: must not be call after net_run()
 */
int net_device_register(struct net_device *dev) {
    static unsigned int index = 0;

    dev->index = index++;
    snprintf(dev->name, sizeof(dev->name), "net%d", dev->index);
    dev->next = devices;    // ここで次のデバイスにdevicesを指定
    devices = dev;          // devicesオブジェクトを初期化
    infof("success, dev=%s, type=0x%04x", dev->name, dev->type);

    return 0;
}

static int net_device_open(struct net_device *dev) {
    infof("dev=%s", dev->name);
    // NET_DEVICE_IS_UP：目的のフラグとフラグ集合との論理積
    if (NET_DEVICE_IS_UP(dev)) {
        errorf("already opened, dev=%s", dev->name);
        return -1;
    }

    // フラグを立てる
    // 目的のフラグとの論理和
    dev->flags |= NET_DEVICE_FLAG_UP;

    return 0;
}

static int net_device_close(struct net_device *dev) {
    infof("dev=%s", dev->name);
    // NET_DEVICE_IS_UP：目的のフラグとフラグ集合との論理積
    if (!NET_DEVICE_IS_UP(dev)) {
        errorf("not opened, dev=%s", dev->name);
        return -1;
    }

    // フラグを降ろす
    // 目的のフラグの否定との論理積
    dev->flags &= ~NET_DEVICE_FLAG_UP;
    
    return 0;
}

int net_device_output(struct net_device *dev, uint16_t type, const uint8_t *data, size_t len, const void *dst) {
    debugf("dev=%s, type=0x%04x, len=%zu", dev->name, type, len);
    debugdump(data, len);

    if (!NET_DEVICE_IS_UP(dev)) {
        errorf("not opened, dev=%s", dev->name);
        return -1;
    }

    if (dev->mtu < len) {
        errorf("too long, dev=%s, mtu=%u, len=%zu", dev->name, dev->mtu, len);
        return -1;
    }

    return 0;
}

int
net_input(uint16_t type, const uint8_t *data, size_t len, struct net_device *dev)
{
}

int
net_input(uint16_t type, const uint8_t *data, size_t len, struct net_device *dev)
{
}

int net_init(void) {
    infof("initiallize...");
    if (platform_init() == -1) {
        errorf("platform_init() failure");
        return -1;
    }
    infof("success");
    return 0;
}

int net_run(void) {
    struct net_device *dev;

    infof("startup...");
    if (platform_run() == -1) {
        errorf("platform_run() failure");
        return -1;
    }

    // 連結リスト上のforループ走査
    // devがNULL出ない間、dev = dev->nextを参照してループ処理を実行する。
    /* whileループだとdev = devices, dev = dev->nextはループ処理の中で実行することになる。
     *
     * dev = devices
     * while (dev) {
     *   dev = dev->next
     * }
     */
    for (dev = devices; dev; dev = dev->next) {
        net_device_open(dev);
    }

    infof("success");
    return 0;
}

int net_shutdown(void) {
    struct net_device *dev;

    infof("shutting down...");
    if (platform_shutdown() == -1) {
        errorf("platform_init() failure");
        return -1;
    }

    // 連結リスト上のforループ走査
    for (dev = devices; dev; dev = dev->next) {
        net_device_close(dev);
    }

    infof("success");
    return 0;
}
