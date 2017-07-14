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
#ifndef __MACAO_NET_H__
#define __MACAO_NET_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <macao/types.h>

/*
 * sfs client
 */
struct smartfox_client *sfs_client_shared();

void net_get_user_info();

/*
 * game state
 */
u8 game_get_state();
void game_set_state(u8 state);

struct sarray *slot_info_shared();
void slot_info_update(struct sarray *s);

/*
 * user data
 */
struct sobj *user_info_shared();

#ifdef __cplusplus
}
#endif

#endif
