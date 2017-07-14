/*
 * Copyright (C) 2017 Manh Tran
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#ifndef __MACAO_EXEC_H__
#define __MACAO_EXEC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <macao/types.h>

struct nexec *mc_nexec_alloc(char *name, size_t len);

struct nexec_ppr {
        struct nexec   *exec_p;
        struct nexec   *exec_d;
        struct nview   *view_p;
        char           *file;
};

struct nexec *nexec_parse(struct nexec_ppr param);

struct nexec *mc_root_exec_alloc();
struct nexec *mcwc_exec_alloc();

struct nexec *mc_trans_cube_exec_alloc();

struct nexec *mc_slot_exec_alloc();

#ifdef __cplusplus
}
#endif

#endif
