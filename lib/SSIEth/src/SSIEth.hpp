#pragma once

#include "lwip/api.h"
#include "lwip/netif.h"
#include "Task.hpp"
#include "MAC.h"
#include "PHY.h"
#include "CountingSemaphore.hpp"

class MacAddr
{
public:
    u8_t mac_addr[6];
};

class SSIEth : public Task<500>
{

public:
    SSIEth(uint8_t priority, MacAddr macAddr) : Task(priority, "Ethernet"), macAddr(macAddr){};

private:
    void activity();

    static void lwip_setup(void *arg) { ((SSIEth *)arg)->lwip_setup(); }
    void lwip_setup();

    static err_t netif_init(struct netif *netif);

    MAC mac = MAC(GMAC);
    PHY phy = PHY(mac, 0);

    MacAddr macAddr;

    CountingSemaphore rx_sem = CountingSemaphore(CONF_GMAC_RXDESCR_NUM, 0);    

    struct netif lwip_netif;

    static void rx_frame_cb(void *);
    static struct pbuf *low_level_input(struct netif *netif);
    static void ethernetif_mac_input(struct netif *netif);
    static err_t mac_low_level_output(struct netif *netif, struct pbuf *p);
};