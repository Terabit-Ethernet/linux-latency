// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Latency	Headers for measuring the latency overheads of various layers
 * 		in the kernel newtwork IO stack.
 *
 *		Definitions for the latency module.
 *
 * Authors:	Shubham Chaudhary, <shubhamchaudhary1996@gmail.com>
 */

#include <linux/export.h>
#include <net/latency.h>

unsigned int sysctl_net_latency_breakdown_on __read_mostly = 0;
EXPORT_SYMBOL(sysctl_net_latency_breakdown_on);

unsigned int sysctl_net_latency_breakdown_log __read_mostly = LATENCY_BREAKDOWN_LOG_DEFAULT;
EXPORT_SYMBOL(sysctl_net_latency_breakdown_log);

unsigned int sysctl_net_latency_breakdown_log_max __read_mostly = LATENCY_BREAKDOWN_LOG_MAX;
EXPORT_SYMBOL(sysctl_net_latency_breakdown_log_max);

unsigned int sysctl_net_latency_breakdown_log_min __read_mostly = LATENCY_BREAKDOWN_LOG_MIN;
EXPORT_SYMBOL(sysctl_net_latency_breakdown_log_min);

ktime_t latency_breakdown_irq_ts[CONFIG_NR_CPUS];
EXPORT_SYMBOL(latency_breakdown_irq_ts);

ktime_t latency_breakdown_napi_ts[CONFIG_NR_CPUS];
EXPORT_SYMBOL(latency_breakdown_napi_ts);
