// SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Latency	Headers for measuring the latency overheads of various layers
 * 		in the kernel newtwork IO stack.
 *
 *		Definitions for the latency module.
 *
 * Version:	@(#)latency.h	1.0.0	06/10/21
 *
 * Authors:	Shubham Chaudhary, <shubhamchaudhary1996@gmail.com>
 */
#ifndef _LATENCY_H
#define _LATENCY_H

#include <linux/ktime.h>
#include <linux/string.h>
#include <netian/sysray_sched.h>
#include <uapi/linux/if.h>

/* Standard defines */
#define LATENCY_BREAKDOWN_LOG_MIN 10
#define LATENCY_BREAKDOWN_LOG_MAX 10000
#define LATENCY_BREAKDOWN_LOG_DEFAULT 997

/* Sysctl variables to control latency measurements. */
extern unsigned int sysctl_net_latency_breakdown_on;
extern unsigned int sysctl_net_latency_breakdown_log;
extern unsigned int sysctl_net_latency_breakdown_log_max;
extern unsigned int sysctl_net_latency_breakdown_log_min;
extern unsigned int sysctl_net_latency_breakdown_nrfs;
extern unsigned int sysctl_net_latency_rx_sched_lat_only;
extern unsigned int sysctl_net_latency_req_size;

/* Per-CPU variables for measurements. */
extern ktime_t latency_breakdown_irq_ts[];
extern ktime_t latency_breakdown_napi_ts[];

/* Receive data path timestamps for a single skb. */
struct rx_timestamps_t {
	/* We're including NIC Rx hardware timestamp
	 * but there is currently no way to synchronize
	 * it with the ktime_get_real clock so this
	 * is unused for now in experiments.
	 */
	ktime_t	hw;
	ktime_t	alloc;
	ktime_t	irq;
	ktime_t	napi;
	ktime_t	gro;
	ktime_t	ip;
	ktime_t	tcp;
	ktime_t	ready;
	ktime_t	wake_up;
	ktime_t	data_copy;
	ktime_t	sleep_enter;
	ktime_t	read_enter;
	ktime_t	read_return;
};

/* Send data path timestamps for a single skb. */
struct tx_timestamps_t {
	ktime_t	write_enter;
	ktime_t	alloc;
	ktime_t	data_copy;
	ktime_t	tcp;
	ktime_t	ip;
	ktime_t	queue_xmit;
	ktime_t xmit;
	ktime_t	xmit_finish;
};

/* Per-socket specific timestamps. */
struct sock_timestamps_t {
	ktime_t	read_enter;
	ktime_t	read_return;
	ktime_t	write_enter;
	ktime_t	sleep_enter;
	ktime_t	ready;
	ktime_t	wake_up;
};

/* Prints the log of the latency breakdown for a given skb. */
static inline void latency_breakdown_print_log(unsigned int sport, unsigned int dport, struct rx_timestamps_t rx_ts, struct tx_timestamps_t tx_ts) {
#ifdef CONFIG_SYSRAY_SCHED_INSTR
	struct sysray_sched_info *sinfo;
	sinfo = this_cpu_ptr(&sysray_sched_percpu);
#endif
	trace_printk(
		"[latency-breakdown] source port: %u destination port: %u "
		"-- rx -- hw: %lld alloc: %lld irq: %lld napi: %lld gro: %lld ip: %lld tcp: %lld read: %lld sleep: %lld ready: %lld wakeup: %lld data copy: %lld return: %lld "
		"-- tx -- alloc: %lld write: %lld data copy: %lld tcp: %lld ip: %lld queue: %lld xmit: %lld finish: %lld "
#ifdef CONFIG_SYSRAY_SCHED_INSTR
		"-- sched -- t1: %llu t2: %llu t3: %llu t4: %llu d: %u "
#endif
		"\n",
		sport,
		dport,
		rx_ts.hw,
		rx_ts.alloc,
		rx_ts.irq,
		rx_ts.napi,
		rx_ts.gro,
		rx_ts.ip,
		rx_ts.tcp,
		rx_ts.read_enter,
		rx_ts.sleep_enter,
		rx_ts.ready,
		rx_ts.wake_up,
		rx_ts.data_copy,
		rx_ts.read_return,
		tx_ts.alloc,
		tx_ts.write_enter,
		tx_ts.data_copy,
		tx_ts.tcp,
		tx_ts.ip,
		tx_ts.queue_xmit,
		tx_ts.xmit,
		tx_ts.xmit_finish
#ifdef CONFIG_SYSRAY_SCHED_INSTR
		,
		sinfo->sched_rq_clock_raw,
		sinfo->sched_rq_clock,
		sinfo->sched_exit_rq_clock,
		sinfo->sched_path_duration,
		sinfo->dirty
#endif
	);
}

#endif	/* _LATENCY_H */
