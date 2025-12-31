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
extern unsigned int sysctl_net_latency_breakdown_validation;

/* Per-CPU variables for measurements. */
extern ktime_t latency_breakdown_irq_ts[];
extern ktime_t latency_breakdown_napi_ts[];

/* Clean runtime capture 
 * This functionality relies on irq time accounting to judge if interrupted.
 */
#if IS_ENABLED(CONFIG_IRQ_TIME_ACCOUNTING)

/*
 * STAGE_HIDDEN_APP: tx_xmit_finish to rx_sleep_enter
 * 		This stage doesn't show up in end-to-end latency, but it does contribute
 * 		significantly to the overall CPU utilization (runtime). It is also the
 * 		major part when the threads is interrupted (or handle bottom half).
 * STAGE_RX_DATA_COPY: rx_data_copy to rx_read_return
 * STAGE_APPLICATION: rx_read_return to tx_write_enter
 * STAGE_TX_DATA_COPY: tx_data_copy to tx_tcp
 * STAGE_TX_TCP_PROC: tx_tcp to tx_ip
 * STAGE_TX_IP_PROC: tx_ip to tx_queue_xmit
 * STAGE_TX_QUEUE: tx_queue_xmit to tx_xmit
 * STAGE_TX_XMIT: tx_xmit to tx_xmit_finish
 */
enum stage_invalid_bit {
	// STAGE_HIDDEN_APP_IRQ_BIT = 0,
    // STAGE_RX_DATA_COPY_IRQ_BIT,
    // STAGE_APPLICATION_IRQ_BIT,
    // STAGE_TX_DATA_COPY_IRQ_BIT,
    // STAGE_TX_TCP_PROC_IRQ_BIT,
    // STAGE_TX_IP_PROC_IRQ_BIT,
    // STAGE_TX_QUEUE_IRQ_BIT,
    // STAGE_TX_XMIT_IRQ_BIT,
	STAGE_HIDDEN_APP_CSW_BIT = 0,
	STAGE_RX_DATA_COPY_CSW_BIT,
	STAGE_APPLICATION_CSW_BIT,
	STAGE_TX_DATA_COPY_CSW_BIT,
	STAGE_TX_TCP_PROC_CSW_BIT,
	STAGE_TX_IP_PROC_CSW_BIT,
	STAGE_TX_QUEUE_CSW_BIT,
	STAGE_TX_XMIT_CSW_BIT,
    STAGE_INVALID_BIT_MAX
};

// #define STAGE_HIDDEN_APP_IRQ_INVALID 	(1ULL << STAGE_HIDDEN_APP_IRQ_BIT)
// #define STAGE_RX_DATA_COPY_IRQ_INVALID  (1ULL << STAGE_RX_DATA_COPY_IRQ_BIT)
// #define STAGE_APPLICATION_IRQ_INVALID   (1ULL << STAGE_APPLICATION_IRQ_BIT)
// #define STAGE_TX_DATA_COPY_IRQ_INVALID  (1ULL << STAGE_TX_DATA_COPY_IRQ_BIT)
// #define STAGE_TX_TCP_PROC_IRQ_INVALID   (1ULL << STAGE_TX_TCP_PROC_IRQ_BIT)
// #define STAGE_TX_IP_PROC_IRQ_INVALID    (1ULL << STAGE_TX_IP_PROC_IRQ_BIT)
// #define STAGE_TX_QUEUE_IRQ_INVALID      (1ULL << STAGE_TX_QUEUE_IRQ_BIT)
// #define STAGE_TX_XMIT_IRQ_INVALID       (1ULL << STAGE_TX_XMIT_IRQ_BIT)
#define STAGE_HIDDEN_APP_CSW_INVALID 	(1ULL << STAGE_HIDDEN_APP_CSW_BIT)
#define STAGE_RX_DATA_COPY_CSW_INVALID  (1ULL << STAGE_RX_DATA_COPY_CSW_BIT)
#define STAGE_APPLICATION_CSW_INVALID   (1ULL << STAGE_APPLICATION_CSW_BIT)
#define STAGE_TX_DATA_COPY_CSW_INVALID  (1ULL << STAGE_TX_DATA_COPY_CSW_BIT)
#define STAGE_TX_TCP_PROC_CSW_INVALID   (1ULL << STAGE_TX_TCP_PROC_CSW_BIT)
#define STAGE_TX_IP_PROC_CSW_INVALID    (1ULL << STAGE_TX_IP_PROC_CSW_BIT)
#define STAGE_TX_QUEUE_CSW_INVALID	  	(1ULL << STAGE_TX_QUEUE_CSW_BIT)
#define STAGE_TX_XMIT_CSW_INVALID     	(1ULL << STAGE_TX_XMIT_CSW_BIT)

#define LATENCY_STAGE_MARK_INVALID(mask, stage) \
    ((mask) |= (stage))

#endif

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
#if IS_ENABLED(CONFIG_IRQ_TIME_ACCOUNTING)
	ktime_t last_xmit_finish;
	u64 last_csw;
	u64 last_irqtime;
	u64 valid;
	// we should use 64-bit here, but u32 is enough?
	u32 hidden_app_irq_delta;
	u32 rx_data_copy_irq_delta;
	u32 application_irq_delta;
	u32 tx_data_copy_irq_delta;
	u32 tx_tcp_irq_delta;
	u32 tx_ip_irq_delta;
	u32 tx_queue_irq_delta;
	u32 tx_xmit_irq_delta;
#endif
};

/* Per-socket specific timestamps. */
struct sock_timestamps_t {
	ktime_t	read_enter;
	ktime_t	read_return;
	ktime_t	write_enter;
	ktime_t	sleep_enter;
	ktime_t	ready;
	ktime_t	wake_up;
#if IS_ENABLED(CONFIG_IRQ_TIME_ACCOUNTING)
	ktime_t last_xmit_finish;
	u64 last_csw;
	u64 last_irqtime;
	u64 valid;
	u32 hidden_app_irq_delta;
	u32 rx_data_copy_irq_delta;
	u32 application_irq_delta;
#endif
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
		"-- sched -- t1: %lld t2: %lld t3: %lld t4: %lld t5: %lld t6: %lld t7: %lld p: %u f: %u "
#endif
#if IS_ENABLED(CONFIG_IRQ_TIME_ACCOUNTING)
		"-- last_xmit_finish: %lld valid: %llu 1: %u 2: %u 3: %u 4: %u 5: %u 6: %u 7: %u 8: %u "
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
		sinfo->sched_rq_clock_start,
		sinfo->sched_rq_clock_end,
		sinfo->sched_rq_clock_task_start,
		sinfo->sched_rq_clock_task_end,
		sinfo->sched_enter,
		sinfo->sched_middle,
		sinfo->sched_exit,
		sinfo->sched_preempted,
		sinfo->sched_rq_clock_update_flags
#endif
#if IS_ENABLED(CONFIG_IRQ_TIME_ACCOUNTING)
		,
		tx_ts.last_xmit_finish,
		tx_ts.valid,
		tx_ts.hidden_app_irq_delta,
		tx_ts.rx_data_copy_irq_delta,
		tx_ts.application_irq_delta,
		tx_ts.tx_data_copy_irq_delta,
		tx_ts.tx_tcp_irq_delta,
		tx_ts.tx_ip_irq_delta,
		tx_ts.tx_queue_irq_delta,
		tx_ts.tx_xmit_irq_delta
#endif
	);
}

#endif	/* _LATENCY_H */
