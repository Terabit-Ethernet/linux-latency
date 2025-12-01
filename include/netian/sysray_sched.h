/* include/netian/sysray_sched.h */
#ifndef _NETIAN_SYSRAY_SCHED_H
#define _NETIAN_SYSRAY_SCHED_H

#include <linux/percpu.h>
#include <linux/types.h>
#include <linux/cache.h>

struct sysray_sched_info {
	u64 sched_rq_clock_raw;
	u64 sched_rq_clock;
	u64 sched_rq_clock_unupdated;
	u64 sched_exit_rq_clock;
    u64 sched_path_duration;
	u32 ivcsw;
	u32 dirty;
	u32 sched_rq_clock_update_flags;
} ____cacheline_aligned;

DECLARE_PER_CPU(struct sysray_sched_info, sysray_sched_percpu);

#endif /* _NETIAN_SYSRAY_SCHED_H */