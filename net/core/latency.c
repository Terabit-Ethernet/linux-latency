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

unsigned int sysctl_net_latency_breakdown_nrfs __read_mostly = 0;
EXPORT_SYMBOL(sysctl_net_latency_breakdown_nrfs);

unsigned int sysctl_net_latency_rx_sched_lat_only __read_mostly = 0;
EXPORT_SYMBOL(sysctl_net_latency_rx_sched_lat_only);

unsigned int sysctl_net_latency_req_size __read_mostly = 64;
EXPORT_SYMBOL(sysctl_net_latency_req_size);

unsigned int sysctl_net_latency_breakdown_validation __read_mostly = 0;
EXPORT_SYMBOL(sysctl_net_latency_breakdown_validation);

unsigned int sysctl_net_latency_dumb_schedule_enable __read_mostly = 0;
EXPORT_SYMBOL(sysctl_net_latency_dumb_schedule_enable);

unsigned int sysctl_net_latency_dumb_schedule_weight __read_mostly = 0;
EXPORT_SYMBOL(sysctl_net_latency_dumb_schedule_weight);

unsigned int sysctl_net_latency_dumb_schedule_disable_clamp __read_mostly = 0;
EXPORT_SYMBOL(sysctl_net_latency_dumb_schedule_disable_clamp);

unsigned int sysctl_net_latency_perstage_rdpmc_on __read_mostly = 0;
EXPORT_SYMBOL(sysctl_net_latency_perstage_rdpmc_on);

ktime_t latency_breakdown_irq_ts[CONFIG_NR_CPUS];
EXPORT_SYMBOL(latency_breakdown_irq_ts);

ktime_t latency_breakdown_napi_ts[CONFIG_NR_CPUS];
EXPORT_SYMBOL(latency_breakdown_napi_ts);

#if IS_ENABLED(CONFIG_IRQ_TIME_ACCOUNTING)
DEFINE_PER_CPU(u64, latency_last_irqtime);
EXPORT_PER_CPU_SYMBOL(latency_last_irqtime);

DEFINE_PER_CPU(struct irq_pmu_counter, irq_pmu_counter_cpu);
EXPORT_PER_CPU_SYMBOL(irq_pmu_counter_cpu);
#endif