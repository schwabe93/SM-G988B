/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * Core file for Samsung EXYNOS Scaler driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/debug-snapshot.h>
#include "scaler.h"

static void show_crop(struct v4l2_rect *rect)
{
	pr_info("   - crop: L,T,W,H: %d, %d, %d, %d\n",
			rect->left, rect->top, rect->width, rect->height);
}

static void show_addr(struct sc_addr *addr)
{
	int i;

	for (i = 0; i < SC_MAX_PLANES; i++)
		pr_info("  * plane%d. addr: %#lx, size: %#x\n",
			i, (unsigned long)addr->ioaddr[i], addr->size[i]);
}

static void show_frame(char *name, struct sc_frame *frame)
{
	pr_info("  * %s) WxH : %d x %d\n", name, frame->width, frame->height);
	show_crop(&frame->crop);
	show_addr(&frame->addr);
	if (frame->sc_fmt)
		pr_info("  * Name : %s\n", frame->sc_fmt->name);

}

static void show_int_frame(struct sc_int_frame *i_frame)
{
	show_addr(&i_frame->src_addr);
	show_addr(&i_frame->dst_addr);
}

void sc_ctx_dump(struct sc_ctx *ctx)
{
	pr_info("> scaler context info <\n");
	show_frame("source", &ctx->s_frame);
	show_frame("dest", &ctx->d_frame);
	pr_info("  - h_ratio: %#x, v_ratio: %#x\n", ctx->h_ratio, ctx->v_ratio);
	pr_info("  - flip_rot_cfg: %#x\n", ctx->flip_rot_cfg);
	pr_info("  - flags: %#lx\n", ctx->flags);

	if (test_bit(CTX_INT_FRAME, &ctx->flags)) {
		show_frame("Internal", &ctx->i_frame->frame);
		show_int_frame(ctx->i_frame);
	}
}

void sc_print_dbg_snapshot(struct sc_ctx *ctx)
{
	struct sc_frame *s_frame, *d_frame;

	if (ctx->context_type == SC_CTX_EXT_TYPE) {
		dbg_snapshot_printk("MSCL : this job is EXT\n");
		return;
	}

	s_frame = &ctx->s_frame;
	d_frame = &ctx->d_frame;

	__dbg_snapshot_printk("MSCL : src([%s], [%dx%d], [%dx%d@%dx%d])-dst([%s], [%dx%d], [%dx%d@%dx%d])\n",
			s_frame->sc_fmt->name, s_frame->width, s_frame->height,
			s_frame->crop.width, s_frame->crop.height,
			s_frame->crop.left, s_frame->crop.top,
			d_frame->sc_fmt->name, d_frame->width, d_frame->height,
			d_frame->crop.width, d_frame->crop.height,
			d_frame->crop.left, d_frame->crop.top);
}
