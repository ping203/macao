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
#ifndef __MACAO_SOURCE_GAME_H__
#define __MACAO_SOURCE_GAME_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <macao/exec.h>
#include <macao/net.h>
#include <native_ui/view_controller.h>
#include <native_ui/action.h>
#include <native_ui/manager.h>
#include <native_ui/touch_handle.h>
#include <native_ui/parser.h>
#include <native_ui/preferences.h>
#include <native_ui/view.h>
#include <smartfox/data.h>
#include <smartfox/network_data.h>
#include <smartfox/smartfox.h>
#include <cherry/stdio.h>
#include <cherry/math/math.h>
#include <cherry/map.h>
#include <cherry/array.h>
#include <cherry/memory.h>
#include <cherry/string.h>

typedef void(*extension_response)(struct nexec *p, struct smart_extension_in_out *out);

#ifdef __cplusplus
}
#endif

#endif
