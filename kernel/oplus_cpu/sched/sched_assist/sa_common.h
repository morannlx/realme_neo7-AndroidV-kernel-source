/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2020-2022 Oplus. All rights reserved.
 */


#ifndef _OPLUS_SA_COMMON_H_
#define _OPLUS_SA_COMMON_H_

#include <linux/sched.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/atomic.h>
#include <linux/hashtable.h>
#include <linux/cgroup-defs.h>
#if IS_ENABLED(CONFIG_SCHED_WALT)
#include <linux/sched/walt.h>
#endif
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/slab.h>

#include "sa_oemdata.h"
#include "sa_common_struct.h"

#define SA_DEBUG_ON 0

#if (SA_DEBUG_ON >= 1)
#define DEBUG_BUG_ON(x) BUG_ON(x)
#define DEBUG_WARN_ON(x) WARN_ON(x)
#else
#define DEBUG_BUG_ON(x)
#define DEBUG_WARN_ON(x)
#endif

#define ux_err(fmt, ...) \
		pr_err("[sched_assist][%s]"fmt, __func__, ##__VA_ARGS__)
#define ux_warn(fmt, ...) \
		pr_warn("[sched_assist][%s]"fmt, __func__, ##__VA_ARGS__)
#define ux_debug(fmt, ...) \
		pr_info("[sched_assist][%s]"fmt, __func__, ##__VA_ARGS__)

#define UX_MSG_LEN		64
#define UX_DEPTH_MAX		5

/* define for debug */
#define DEBUG_SYSTRACE (1 << 0)
#define DEBUG_FTRACE   (1 << 1)
#define DEBUG_KMSG     (1 << 2)	/* used for frameboost */
#define DEBUG_PIPELINE (1 << 3)
#define DEBUG_DYNAMIC_HZ (1 << 4)
#define DEBUG_DYNAMIC_PREEMPT (1 << 5)
#define DEBUG_AMU_INSTRUCTION (1 << 6)
#define DEBUG_VERBOSE  (1 << 10)	/* used for frameboost */
#define DEBUG_SET_SCHEDPROP (1 << 11) /* used for hmbird */

/* define for sched assist feature */
#define FEATURE_COMMON (1 << 0)
#define FEATURE_SPREAD (1 << 1)
/* only top tasks set ux-prefer-cluster UX_PRIORITY_PROTECT*/
#define FEATURE_LIMIT (1 << 2)
/* top tasks do not select little cores when the load is greater than the threshold*/
#define FEATURE_TOPUP (1 << 3)

#define UX_EXEC_SLICE (4000000U)

/* define for sched assist thread type, keep same as the define in java file */
#define SA_OPT_CLEAR				(0)
#define SA_TYPE_LIGHT				(1 << 0)
#define SA_TYPE_HEAVY				(1 << 1)
#define SA_TYPE_ANIMATOR			(1 << 2)
/* SA_TYPE_LISTPICK for camera */
#define SA_TYPE_LISTPICK			(1 << 3)
#define SA_OPT_SET					(1 << 7)
#define SA_OPT_RESET				(1 << 8)
#define SA_OPT_SET_PRIORITY			(1 << 9)

/* The following ux value only used in kernel */
#define SA_TYPE_SWIFT				(1 << 14)
/* clear ux type when dequeue */
#define SA_TYPE_ONCE				(1 << 15)
#define SA_TYPE_INHERIT				(1 << 16)
#define SA_TYPE_URGENT_MASK	(SA_TYPE_LIGHT|SA_TYPE_ANIMATOR|SA_TYPE_SWIFT)
#define SCHED_ASSIST_UX_MASK	(SA_TYPE_LIGHT|SA_TYPE_HEAVY|SA_TYPE_ANIMATOR|SA_TYPE_LISTPICK|SA_TYPE_SWIFT)

/* load balance operation is performed on the following ux types */
#define SCHED_ASSIST_LB_UX			(SA_TYPE_SWIFT | SA_TYPE_ANIMATOR | SA_TYPE_LIGHT | SA_TYPE_HEAVY)
#define POSSIBLE_UX_MASK			(SA_TYPE_SWIFT | \
									SA_TYPE_LIGHT | \
									SA_TYPE_HEAVY | \
									SA_TYPE_ANIMATOR | \
									SA_TYPE_LISTPICK | \
									SA_TYPE_ONCE | \
									SA_TYPE_INHERIT)

#define SCHED_ASSIST_UX_PRIORITY_MASK	(0xFF000000)
#define SCHED_ASSIST_UX_PRIORITY_SHIFT	24

/* Priority sorting of UX type tasks set by UAD, the value may change. record
#define UX_PRIORITY_MEDIUM		0x03000000
#define UX_PRIORITY_HIGH		0x06000000
#define UX_PRIORITY_SUPER		0x08000000
UX_PRIORITY_PROTECT: Lowest priority protected ux type
*/
#define UX_PRIORITY_PROTECT		0x06000000
#define UX_PRIORITY_TOP_APP		0x0A000000

#define UX_PRIORITY_AUDIO		0x0A000000
#if IS_ENABLED(CONFIG_OPLUS_FEATURE_PIPELINE)
#define UX_PRIORITY_PIPELINE_UI 0x06000000
#define UX_PRIORITY_PIPELINE    0x05000000
#endif

/* define for sched assist scene type, keep same as the define in java file */
#define SA_SCENE_OPT_CLEAR			(0)
#define SA_LAUNCH					(1 << 0)
#define SA_SLIDE					(1 << 1)
#define SA_CAMERA					(1 << 2)
#define SA_ANIM_START				(1 << 3) /* we care about both launcher and top app */
#define SA_ANIM						(1 << 4) /* we only care about launcher */
#define SA_INPUT					(1 << 5)
#define SA_LAUNCHER_SI				(1 << 6)
#define SA_SCENE_OPT_SET			(1 << 7)
#define SA_GPU_COMPOSITION  			(1 << 8)

#define ROOT_UID               0
#define SYSTEM_UID             1000
#define CAMERA_UID             1047
#define FIRST_APPLICATION_UID  10000
#define LAST_APPLICATION_UID   19999
#define PER_USER_RANGE         100000
#define SCHED_UX_STATE_DEBUG_MAGIC  123456789

#define CAMERA_PROVIDER_NAME "ider-service_64"
extern pid_t save_audio_tgid;
extern pid_t save_top_app_tgid;
extern unsigned int top_app_type;
extern int global_lowend_plat_opt;

#ifdef CONFIG_OPLUS_SCHED_HALT_MASK_PRT
/* This must be the same as the definition of pause_type in walt_halt.c */
enum oplus_pause_type {
	OPLUS_HALT,
	OPLUS_PARTIAL_HALT,

	OPLUS_MAX_PAUSE_TYPE
};
extern cpumask_t cur_cpus_halt_mask;
extern cpumask_t cur_cpus_phalt_mask;
DECLARE_PER_CPU(int[OPLUS_MAX_PAUSE_TYPE], oplus_cur_pause_client);
extern void sa_corectl_systrace_c(void);
#endif /* CONFIG_OPLUS_SCHED_HALT_MASK_PRT */

/* define for boost threshold unit */
#define BOOST_THRESHOLD_UNIT (51)
/* define for clear IM_FLAG
if im_flag is 70, it should clear im_flag_audio (70 - 64 = 6)
eg: gerrit patchset "30438485"
 */
#define IM_FLAG_CLEAR			64

enum UX_STATE_TYPE {
	UX_STATE_INVALID = 0,
	UX_STATE_NONE,
	UX_STATE_SCHED_ASSIST,
	UX_STATE_INHERIT,
	MAX_UX_STATE_TYPE,
};

enum INHERIT_UX_TYPE {
	INHERIT_UX_BINDER = 0,
	INHERIT_UX_RWSEM,
	INHERIT_UX_MUTEX,
	INHERIT_UX_FUTEX,
	INHERIT_UX_PIFUTEX,
	INHERIT_UX_MAX,
};

/*
 * WANNING:
 * new flag should be add before MAX_IM_FLAG_TYPE, never change
 * the value of those existed flag type.
 */
enum IM_FLAG_TYPE {
	IM_FLAG_NONE = 0,
	IM_FLAG_SURFACEFLINGER,
	IM_FLAG_HWC,			/* Discarded */
	IM_FLAG_RENDERENGINE,
	IM_FLAG_WEBVIEW,
	IM_FLAG_CAMERA_HAL,
	IM_FLAG_AUDIO,
	IM_FLAG_HWBINDER,
	IM_FLAG_LAUNCHER,
	IM_FLAG_LAUNCHER_NON_UX_RENDER,
	IM_FLAG_SS_LOCK_OWNER,
	IM_FLAG_FORBID_SET_CPU_AFFINITY = 11, /* forbid setting cpu affinity from app */
	IM_FLAG_SYSTEMSERVER_PID,
	IM_FLAG_MIDASD,
	IM_FLAG_AUDIO_CAMERA_HAL, /* audio mode disable camera hal ux */
	IM_FLAG_AFFINITY_THREAD,
	IM_FLAG_TPD_SET_CPU_AFFINITY = 16,
	IM_FLAG_COMPRESS_THREAD, /* compress thread skips locking protect */
	MAX_IM_FLAG_TYPE,
};

#define MAX_IM_FLAG_PRIO	MAX_IM_FLAG_TYPE

enum ots_state {
	OTS_STATE_SET_AFFINITY,
	OTS_STATE_MAX,
};

#ifdef CONFIG_OPLUS_FEATURE_TICK_GRAN
DECLARE_PER_CPU(u64, retired_instrs);
DECLARE_PER_CPU(u64, nvcsw);
DECLARE_PER_CPU(u64, nivcsw);
#endif

struct ux_sched_cluster {
	struct cpumask cpus;
	unsigned long capacity;
};

#define OPLUS_NR_CPUS (8)
#define OPLUS_MAX_CLS (5)
struct ux_sched_cputopo {
	int cls_nr;
	struct ux_sched_cluster sched_cls[OPLUS_NR_CPUS];

#if IS_ENABLED(CONFIG_OPLUS_FEATURE_LOADBALANCE)
	cpumask_t oplus_cpu_array[2*OPLUS_MAX_CLS][OPLUS_MAX_CLS];
#endif
};

struct oplus_rq {
	/* CONFIG_OPLUS_FEATURE_SCHED_ASSIST */
	struct rb_root_cached ux_list;
	/* a tree to track minimum exec vruntime */
	struct rb_root_cached exec_timeline;
	/* malloc this spinlock_t instead of built-in to shrank size of oplus_rq */
	spinlock_t *ux_list_lock;
	int nr_running;
	u64 min_vruntime;
	u64 load_weight;
#ifdef CONFIG_LOCKING_PROTECT
	struct list_head locking_thread_list;
	spinlock_t *locking_list_lock;
	int rq_locking_task;
#endif

#if IS_ENABLED(CONFIG_OPLUS_FEATURE_LOADBALANCE)
	/* for loadbalance */
	struct oplus_lb lb;
#endif
#ifdef CONFIG_OPLUS_FEATURE_TICK_GRAN
	struct hrtimer *resched_timer;
	int cpu;
#endif
};

extern int global_debug_enabled;
extern int global_sched_assist_enabled;
extern int global_sched_assist_scene;

struct rq;

#ifdef CONFIG_LOCKING_PROTECT
struct sched_assist_locking_ops {
	void (*replace_next_task_fair)(struct rq *rq,
		struct task_struct **p, struct sched_entity **se, bool *repick, bool simple);
	void (*check_preempt_tick)(struct task_struct *p,
			unsigned long *ideal_runtime, bool *skip_preempt,
			unsigned long delta_exec, struct cfs_rq *cfs_rq,
			struct sched_entity *curr, unsigned int granularity);
	void (*enqueue_entity)(struct rq *rq, struct task_struct *p);
	void (*dequeue_entity)(struct rq *rq, struct task_struct *p);
	void (*check_preempt_wakeup)(struct rq *rq, struct task_struct *p,
			bool *preempt, bool *nopreempt, bool *waker_hold_lock);
	void (*state_systrace_c)(unsigned int cpu, struct task_struct *p);
	void (*opt_ss_lock_contention)(struct task_struct *p, unsigned long old_im, int new_im);
	void (*locking_tick_hit)(struct task_struct *prev, struct task_struct *next);
};

extern struct sched_assist_locking_ops *locking_ops;


#define LOCKING_CALL_OP(op, args...) 						\
do {														\
	if (locking_ops && locking_ops->op) {					\
		locking_ops->op(args);								\
	}														\
} while (0)

#define LOCKING_CALL_OP_RET(op, args...) 						\
({																\
	__typeof__(locking_ops->op(args)) __ret = 0;				\
	if (locking_ops && locking_ops->op) 							\
			__ret = locking_ops->op(args);						\
	__ret; 														\
})

void register_sched_assist_locking_ops(struct sched_assist_locking_ops *ops);
#endif

/* attention: before insert .ko, task's list->prev/next is init with 0 */
static inline bool oplus_list_empty(struct list_head *list)
{
	return list_empty(list) || (list->prev == 0 && list->next == 0);
}

static inline bool oplus_rbtree_empty(struct rb_root_cached *list)
{
	return (rb_first_cached(list) == NULL);
}

/**
 * Check if there are ux tasks waiting to run on the specified cpu
 */
static inline bool orq_has_ux_tasks(struct oplus_rq *orq)
{
	return !oplus_rbtree_empty(&orq->ux_list);
}

/* attention: before insert .ko, task's node->__rb_parent_color is init with 0 */
static inline bool oplus_rbnode_empty(struct rb_node *node)
{
	return RB_EMPTY_NODE(node) || (node->__rb_parent_color == 0);
}

static inline struct oplus_task_struct *ux_list_first_entry(struct rb_root_cached *list)
{
	struct rb_node *leftmost = rb_first_cached(list);
	if (leftmost == NULL) {
		return NULL;
	}
	return rb_entry(leftmost, struct oplus_task_struct, ux_entry);
}

static inline struct oplus_task_struct *exec_timeline_first_entry(struct rb_root_cached *tree)
{
	struct rb_node *leftmost = rb_first_cached(tree);
	if (leftmost == NULL) {
		return NULL;
	}
	return rb_entry(leftmost, struct oplus_task_struct, exec_time_node);
}

typedef bool (*migrate_task_callback_t)(struct task_struct *tsk, int src_cpu, int dst_cpu);
extern migrate_task_callback_t fbg_migrate_task_callback;
typedef void (*android_rvh_schedule_handler_t)(struct task_struct *prev,
	struct task_struct *next, struct rq *rq);
extern android_rvh_schedule_handler_t fbg_android_rvh_schedule_callback;

extern struct kmem_cache *oplus_task_struct_cachep;

#define ots_to_ts(ots)	(ots->task)
#define OTS_IDX			0

static inline struct oplus_task_struct *get_oplus_task_struct(struct task_struct *t)
{
	struct oplus_task_struct *ots = NULL;

	/* not Skip idle thread */
	if (!t)
		return NULL;

	ots = (struct oplus_task_struct *) READ_ONCE(t->android_oem_data1[OTS_IDX]);
	if (IS_ERR_OR_NULL(ots))
		return NULL;

	return ots;
}

static inline unsigned long oplus_get_im_flag(struct task_struct *t)
{
	struct oplus_task_struct *ots = get_oplus_task_struct(t);

	if (IS_ERR_OR_NULL(ots))
		return IM_FLAG_NONE;

	return ots->im_flag;
}

static inline bool is_optimized_audio_thread(struct task_struct *t)
{
	unsigned long im_flag;

	im_flag = oplus_get_im_flag(t);
	if (test_bit(IM_FLAG_AUDIO, &im_flag))
		return true;

	return false;
}

static inline void oplus_set_im_flag(struct task_struct *t, int im_flag)
{
	struct oplus_task_struct *ots = NULL;
	ots = get_oplus_task_struct(t);

	if (IS_ERR_OR_NULL(ots))
		return;

	set_bit(im_flag, &ots->im_flag);
}

static inline int oplus_get_ux_state(struct task_struct *t)
{
	struct oplus_task_struct *ots = get_oplus_task_struct(t);

	if (IS_ERR_OR_NULL(ots))
		return 0;

	return ots->ux_state;
}

void oplus_set_ux_state_lock(struct task_struct *t, int ux_state, int inherit_type, bool need_lock_rq);

static inline s64 oplus_get_inherit_ux(struct task_struct *t)
{
	struct oplus_task_struct *ots = get_oplus_task_struct(t);

	if (IS_ERR_OR_NULL(ots))
		return 0;

	return atomic64_read(&ots->inherit_ux);
}

static inline void oplus_set_inherit_ux(struct task_struct *t, s64 inherit_ux)
{
	struct oplus_task_struct *ots = get_oplus_task_struct(t);

	if (IS_ERR_OR_NULL(ots))
		return;

	atomic64_set(&ots->inherit_ux, inherit_ux);
}

static inline int oplus_get_ux_depth(struct task_struct *t)
{
	struct oplus_task_struct *ots = get_oplus_task_struct(t);

	if (IS_ERR_OR_NULL(ots))
		return 0;

	return ots->ux_depth;
}

static inline void oplus_set_ux_depth(struct task_struct *t, int ux_depth)
{
	struct oplus_task_struct *ots = get_oplus_task_struct(t);

	if (IS_ERR_OR_NULL(ots))
		return;

	ots->ux_depth = ux_depth;
}

static inline u64 oplus_get_enqueue_time(struct task_struct *t)
{
	struct oplus_task_struct *ots = get_oplus_task_struct(t);

	if (IS_ERR_OR_NULL(ots))
		return 0;

	return ots->enqueue_time;
}

static inline void oplus_set_enqueue_time(struct task_struct *t, u64 enqueue_time)
{
	struct oplus_task_struct *ots = get_oplus_task_struct(t);

	if (IS_ERR_OR_NULL(ots))
		return;

	ots->enqueue_time = enqueue_time;

#if IS_ENABLED(CONFIG_OPLUS_FEATURE_LOADBALANCE)
	/*
	 * Record the number of task context switches
	 * and scheduling delays when enqueueing a task.
	 */
	ots->snap_pcount = t->sched_info.pcount;
	ots->snap_run_delay = t->sched_info.run_delay;
#endif
}

static inline void oplus_set_inherit_ux_start(struct task_struct *t, u64 start_time)
{
	struct oplus_task_struct *ots = get_oplus_task_struct(t);

	if (IS_ERR_OR_NULL(ots))
		return;

	ots->inherit_ux_start = t->se.sum_exec_runtime;
}

static inline void init_task_ux_info(struct task_struct *t)
{
	struct oplus_task_struct *ots = get_oplus_task_struct(t);
#ifdef CONFIG_HMBIRD_SCHED_GKI
	struct oplus_task_struct *curr_ots = get_oplus_task_struct(current);
#endif
	if (IS_ERR_OR_NULL(ots))
		return;

	RB_CLEAR_NODE(&ots->ux_entry);
	RB_CLEAR_NODE(&ots->exec_time_node);
	ots->ux_state = 0;
	atomic64_set(&ots->inherit_ux, 0);
	ots->ux_depth = 0;
	ots->enqueue_time = 0;
	ots->inherit_ux_start = 0;
	ots->ux_priority = -1;
	ots->ux_nice = -1;
	ots->vruntime = 0;
	ots->preset_vruntime = 0;
	ots->cfs_delta = -1;
#if IS_ENABLED(CONFIG_OPLUS_FEATURE_ABNORMAL_FLAG)
	ots->abnormal_flag = 0;
#endif
#ifdef CONFIG_OPLUS_FEATURE_SCHED_SPREAD
	ots->lb_state = 0;
	ots->ld_flag = 0;
#endif
	cpumask_clear(&ots->cpus_requested);
	ots->exec_calc_runtime = 0;
	ots->is_update_runtime = 0;
	ots->target_process = -1;
	ots->wake_tid = 0;
	ots->running_start_time = 0;
	ots->update_running_start_time = false;
	ots->last_wake_ts = 0;
/*#if IS_ENABLED(CONFIG_OPLUS_LOCKING_STRATEGY)*/
	memset(&ots->lkinfo, 0, sizeof(struct locking_info));
	INIT_LIST_HEAD(&ots->lkinfo.node);
/*#endif*/
	ots->block_start_time = 0;
#ifdef CONFIG_LOCKING_PROTECT
	INIT_LIST_HEAD(&ots->locking_entry);
	ots->locking_start_time = 0;
	ots->locking_depth = 0;
#endif

#if IS_ENABLED(CONFIG_OPLUS_FEATURE_LOADBALANCE)
	/* for loadbalance */
	ots->snap_pcount = 0;
	ots->snap_run_delay = 0;
	plist_node_init(&ots->rtb, MAX_IM_FLAG_PRIO);
#endif

#if IS_ENABLED(CONFIG_OPLUS_FEATURE_PIPELINE)
	atomic_set(&ots->pipeline_cpu, -1);
#endif

#ifdef CONFIG_OPLUS_CAMERA_UX
	if (CAMERA_UID == task_uid(t).val) {
		if (!strncmp(t->comm, CAMERA_PROVIDER_NAME, 15)) {
			ots->ux_state = SA_TYPE_HEAVY;
		}
	}
#endif

#if IS_ENABLED(CONFIG_ARM64_AMU_EXTN) && IS_ENABLED(CONFIG_OPLUS_FEATURE_CPU_JANKINFO)
	ots->amu_cycle = 0;
	ots->amu_instruct = 0;
#endif
#ifdef CONFIG_HMBIRD_SCHED_GKI
	ots->scx.dsq		= NULL;
	INIT_LIST_HEAD(&ots->scx.dsq_node.fifo);
	RB_CLEAR_NODE(&ots->scx.dsq_node.priq);
	ots->scx.flags		= 0;
	ots->scx.dsq_flags	= 0;
	ots->scx.sticky_cpu	= -1;
	ots->scx.runnable_at	= INITIAL_JIFFIES;
	ots->scx.slice		= SCX_SLICE_DFL;
	ots->scx.sched_prop = 0;
	ots->scx.ext_flags	= 0;
	ots->scx.prio_backup = 0;
	ots->scx.gdsq_idx = DEFAULT_CGROUP_DL_IDX;
	memset(&ots->scx.sts, 0, sizeof(struct scx_task_stats));
	if (!IS_ERR_OR_NULL(curr_ots)) {
		if ((curr_ots->scx.ext_flags & EXT_FLAG_RT_CHANGED)
					&& !t->sched_reset_on_fork) {
			ots->scx.ext_flags |= EXT_FLAG_RT_CHANGED;
			ots->scx.prio_backup = curr_ots->scx.prio_backup;
		}
		if (curr_ots->scx.ext_flags & EXT_FLAG_CFS_CHANGED)
			ots->scx.ext_flags |= EXT_FLAG_CFS_CHANGED;
	}
#endif
};

static inline bool test_sched_assist_ux_type(struct task_struct *task, unsigned int sa_ux_type)
{
	return oplus_get_ux_state(task) & sa_ux_type;
}

/* only top tasks set ux-prefer-cluster when limit_ux_task enabled(FEATURE_LIMIT).
 * This function is used to filter the ux type protected when the feature is turned on.
 */
static inline bool test_filter_protect_task(struct task_struct *task)
{
	int task_priority = (oplus_get_ux_state(task) & SCHED_ASSIST_UX_PRIORITY_MASK);

	return (task_priority >= UX_PRIORITY_PROTECT);
}

static inline bool is_heavy_ux_task(struct task_struct *t)
{
	struct oplus_task_struct *ots = get_oplus_task_struct(t);

	if (IS_ERR_OR_NULL(ots))
		return false;

	return ots->ux_state & SA_TYPE_HEAVY;
}

static inline bool is_anim_ux_task(struct task_struct *t)
{
	struct oplus_task_struct *ots = get_oplus_task_struct(t);

	if (IS_ERR_OR_NULL(ots))
		return false;

	return ots->ux_state & SA_TYPE_ANIMATOR;
}

static inline bool sched_assist_scene(unsigned int scene)
{
	if (unlikely(!global_sched_assist_enabled))
		return false;

	return global_sched_assist_scene & scene;
}

static inline unsigned long oplus_task_util(struct task_struct *p)
{
#if IS_ENABLED(CONFIG_SCHED_WALT)
	struct walt_task_struct *wts = (struct walt_task_struct *) p->android_vendor_data1;

	return wts->demand_scaled;
#else
	return READ_ONCE(p->se.avg.util_avg);
#endif
}

#if IS_ENABLED(CONFIG_SCHED_WALT)
static inline u32 task_wts_sum(struct task_struct *tsk)
{
	struct walt_task_struct *wts = (struct walt_task_struct *) tsk->android_vendor_data1;

	return wts->sum;
}
#endif

bool is_min_cluster(int cpu);
bool is_max_cluster(int cpu);
bool is_mid_cluster(int cpu);
bool is_top(struct task_struct *p);
bool task_is_runnable(struct task_struct *task);
int get_ux_state(struct task_struct *task);

#if IS_ENABLED(CONFIG_OPLUS_FEATURE_LOADBALANCE)
int ux_mask_to_prio(int ux_mask);
int ux_prio_to_mask(int prio);
#endif

noinline int tracing_mark_write(const char *buf);
void hwbinder_systrace_c(unsigned int cpu, int flag);
void sched_assist_init_oplus_rq(void);
void queue_ux_thread(struct rq *rq, struct task_struct *p, int enqueue);

void inherit_ux_inc(struct task_struct *task, int type);
void inherit_ux_sub(struct task_struct *task, int type, int value);
void set_inherit_ux(struct task_struct *task, int type, int depth, int inherit_val);
void reset_inherit_ux(struct task_struct *inherit_task, struct task_struct *ux_task, int reset_type);
void unset_inherit_ux(struct task_struct *task, int type);
void unset_inherit_ux_value(struct task_struct *task, int type, int value);
void inc_inherit_ux_refs(struct task_struct *task, int type);
void clear_all_inherit_type(struct task_struct *p);
int get_max_inherit_gran(struct task_struct *p);

bool is_heavy_load_top_task(struct task_struct *p);
bool test_task_is_fair(struct task_struct *task);
bool test_task_is_rt(struct task_struct *task);

bool prio_higher(int a, int b);
bool test_task_ux(struct task_struct *task);
bool is_limit_task_ux_enabled(void);
bool is_top_ux_task_up_enabled(void);
bool test_task_ux_depth(int ux_depth);
bool test_inherit_ux(struct task_struct *task, int type);
bool test_set_inherit_ux(struct task_struct *task);
bool test_task_identify_ux(struct task_struct *task, int id_type_ux);
bool test_list_pick_ux(struct task_struct *task);
int get_ux_state_type(struct task_struct *task);
void sched_assist_target_comm(struct task_struct *task, const char *comm);
unsigned int ux_task_exec_limit(struct task_struct *p);

void update_ux_sched_cputopo(void);
bool is_task_util_over(struct task_struct *tsk, int threshold);
bool oplus_task_misfit(struct task_struct *tsk, int cpu);
ssize_t oplus_show_cpus(const struct cpumask *mask, char *buf);
void adjust_rt_lowest_mask(struct task_struct *p, struct cpumask *local_cpu_mask, int ret, bool force_adjust);
bool sa_skip_rt_sync(struct rq *rq, struct task_struct *p, bool *sync);
bool sa_rt_skip_ux_cpu(int cpu);
int is_vip_mvp(struct task_struct *p);

/* s64 account_ux_runtime(struct rq *rq, struct task_struct *curr); */
void opt_ss_lock_contention(struct task_struct *p, unsigned long old_im, int new_im);

/* register vender hook in kernel/sched/topology.c */
void android_vh_build_sched_domains_handler(void *unused, bool has_asym);

/* register vender hook in kernel/sched/rt.c */
void android_rvh_select_task_rq_rt_handler(void *unused, struct task_struct *p, int prev_cpu, int sd_flag, int wake_flags, int *new_cpu);
void android_rvh_find_lowest_rq_handler(void *unused, struct task_struct *p, struct cpumask *local_cpu_mask, int ret, int *best_cpu);

/* register vender hook in kernel/sched/core.c */
void android_rvh_sched_fork_handler(void *unused, struct task_struct *p);
void android_rvh_after_enqueue_task_handler(void *unused, struct rq *rq, struct task_struct *p, int flags);
void android_rvh_dequeue_task_handler(void *unused, struct rq *rq, struct task_struct *p, int flags);
void android_rvh_schedule_handler(void *unused, unsigned int sched_mode, struct task_struct *prev, struct task_struct *next, struct rq *rq);
void android_vh_scheduler_tick_handler(void *unused, struct rq *rq);

#ifdef CONFIG_OPLUS_FEATURE_TICK_GRAN
void android_vh_account_process_tick_gran_handler(void *unused, int user_tick, int *ticks);
void sa_sched_switch_handler(void *unused, bool preempt, struct task_struct *prev, struct task_struct *next, unsigned int prev_state);
#endif

void set_im_flag_with_bit(int im_flag, struct task_struct *task);

/* register vendor hook in kernel/cgroup/cgroup-v1.c */
void android_vh_cgroup_set_task_handler(void *unused, int ret, struct task_struct *task);
/* register vendor hook in kernel/signal.c  */
void android_vh_exit_signal_handler(void *unused, struct task_struct *p);
void android_rvh_set_cpus_allowed_by_task_handler(void *unused, const struct cpumask *cpu_valid_mask, const struct cpumask *new_mask,
												struct task_struct *p, unsigned int *dest_cpu);
#if IS_ENABLED(CONFIG_OPLUS_FEATURE_BAN_APP_SET_AFFINITY)
void android_vh_sched_setaffinity_early_handler(void *unused, struct task_struct *task, const struct cpumask *new_mask, bool *skip);
#endif

#ifdef CONFIG_BLOCKIO_UX_OPT
int sa_blockio_init(void);
void sa_blockio_exit(void);
#endif
extern struct notifier_block process_exit_notifier_block;


#endif /* _OPLUS_SA_COMMON_H_ */
