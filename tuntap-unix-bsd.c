/*
 * Copyright (c) 2012 Tristan Le Guern <leguern AT medu DOT se>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <net/if.h>

#include <string.h>

#include "tuntap.h"

int
tuntap_sys_set_ipv6(struct device *dev, struct sockaddr_in6 *s, uint32_t bits) {
	(void)dev;
	(void)s;
	(void)bits;
	tuntap_log(TUNTAP_LOG_INFO, "IPv6 is not implemented on your system");
	return -1;
}

int
tuntap_sys_set_ifname(struct device *dev, const char *ifname, size_t len) {
	(void)dev;
	(void)ifname;
	(void)len;
	tuntap_log(TUNTAP_LOG_ERR,
	    "Your system does not support tuntap_set_ifname()");
	return -1;
}

int
tuntap_sys_set_descr(struct device *dev, const char *descr, size_t len) {
#if !defined Darwin && !defined NetBSD
	struct ifreq ifr;
	struct ifreq_buffer ifrbuf;

	(void)memset(&ifr, '\0', sizeof ifr);
	(void)strlcpy(ifr.ifr_name, dev->if_name, sizeof dev->if_name);

	ifrbuf.buffer = (void *)descr;
	ifrbuf.length = len;
	ifr.ifr_buffer = ifrbuf;

	if (ioctl(dev->ctrl_sock, SIOCSIFDESCR, &ifr) == -1) {
		tuntap_log(TUNTAP_LOG_ERR,
		    "Can't set the interface description");
		return -1;
	}
	return 0;
#else
	return -1;
#endif
}
