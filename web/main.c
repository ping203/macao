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
#include <emscripten.h>
#include <emscripten/html5.h>
#include <cherry/stdio.h>
#include <cherry/list.h>
#include <cherry/map.h>
#include <cherry/string.h>
#include <cherry/math/math.h>
#include <native_ui/view.h>
#include <native_ui/parser.h>
#include <native_ui/manager.h>
#include <native_ui/view_controller.h>
#include <smartfox/data.h>
#include <macao/exec.h>
#include <ftw.h>

struct string *__root_directory__ = NULL;
static struct nview *root = NULL;

EMSCRIPTEN_KEEPALIVE
void macao_resize(int width, int height)
{
        nview_set_size(root, (union vec2){(float)width, (float)height});
        nview_set_position(root, (union vec2){root->size.width / 2, root->size.height/2});
        nview_update_layout(root);
}

static void do_frame()
{
        if(root) nmanager_update(nmanager_shared(), 1.0f / 60);
}

int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    remove(fpath);

    return 0;
}


EMSCRIPTEN_KEEPALIVE
static void macao_start()
{
        /*
         * clear cache
         */
        struct string *cache = string_alloc_chars(qskey(__root_directory__));
        string_cat(cache, qlkey("/"));
        char *ver = file_get_version_directory();
        string_cat(cache, ver, strlen(ver));
        nftw(cache->ptr, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
        string_free(cache);

        nexec_set_fnf(mc_nexec_alloc);
        root = nview_alloc();
        nview_set_layout_type(root, NATIVE_UI_LAYOUT_RELATIVE);
        nview_set_user_interaction_enabled(root, 1);

        struct nparser *parser = nparser_alloc();
        nparser_parse_file(parser, "res/layout/root.xml", NULL);

        struct nview *view = (struct nview *)
                ((char *)parser->view.next - offsetof(struct nview, parser));

        nview_add_child(root, view);

        EM_ASM_({
                var obj =  __shared_object_get($0);
                var rt = document.getElementById("root");
                rt.appendChild(obj);
                _macao_resize(rt.offsetWidth, rt.offsetHeight);
        }, root->ptr);
}

int main(int argc, char **argv)
{
        __root_directory__ = string_alloc_chars(qlkey("/macao"));
        file_set_version_directory(qlkey("v1"));
        EM_ASM(
                FS.mkdir('/macao');
                FS.mount(IDBFS, {}, '/macao');

                FS.syncfs(true, function (err) {
                        assert(!err);
                        _macao_start();
                });
        );
        emscripten_set_main_loop(do_frame, 60, 1);

        return 0;
}
