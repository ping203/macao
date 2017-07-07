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

/*
 * shared objects for native management
 */
var __shared_object_holder__    = __shared_object_holder__ || {};
var __shared_object_id__        = __shared_object_id__ || 0;

function __register_shared_object(obj)
{
        if(obj.__soi__ == undefined) {
                __shared_object_id__                    += 1;
                obj.__soi__                             = __shared_object_id__;
                __shared_object_holder__[obj.__soi__]   = obj;
        }
}

function __remove_shared_object(obj)
{
        if(obj.__soi__ != undefined) {
                __shared_object_holder__[obj.__soi__]   = undefined;
                obj.__soi__                             = undefined;
        }
}

function __shared_object_get(id)
{
        return __shared_object_holder__[id];
}

function __shared_object_free(id)
{
        var obj = __shared_object_holder__[id];
        if(obj != null && obj != undefined && obj.free != null && obj.free != undefined) {
                obj.free();
        }
}
