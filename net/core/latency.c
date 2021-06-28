// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Latency	Headers for measuring the latency overheads of various layers
 * 		in the kernel newtwork IO stack.
 *
 *		Definitions for the latency module.
 *
 * Authors:	Shubham Chaudhary, <shubhamchaudhary1996@gmail.com>
 */

#include <linux/ktime.h>
#include <linux/export.h>
#include <linux/percpu.h>
#include <net/latency.h>
#include <linux/printk.h>
#include <linux/skbuff.h>
#include <uapi/linux/if.h>

unsigned int sysctl_net_latency_breakdown_on __read_mostly = 0;
EXPORT_SYMBOL(sysctl_net_latency_breakdown_on);

unsigned int sysctl_net_latency_breakdown_log __read_mostly = LATENCY_BREAKDOWN_LOG_DEFAULT;
EXPORT_SYMBOL(sysctl_net_latency_breakdown_log);

unsigned int sysctl_net_latency_breakdown_log_max __read_mostly = LATENCY_BREAKDOWN_LOG_MAX;
EXPORT_SYMBOL(sysctl_net_latency_breakdown_log_max);

unsigned int sysctl_net_latency_breakdown_log_min __read_mostly = LATENCY_BREAKDOWN_LOG_MIN;
EXPORT_SYMBOL(sysctl_net_latency_breakdown_log_min);

DEFINE_PER_CPU(ktime_t, latency_breakdown_irq_ts);
EXPORT_PER_CPU_SYMBOL(latency_breakdown_irq_ts);

DEFINE_PER_CPU(ktime_t, latency_breakdown_napi_ts);
EXPORT_PER_CPU_SYMBOL(latency_breakdown_napi_ts);
