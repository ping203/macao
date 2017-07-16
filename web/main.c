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
#include <GL/glew.h>
#include <GL/glfw.h>
#include <pthread.h>
#include <cherry/lock.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <cherry/stdio.h>
#include <cherry/list.h>
#include <cherry/array.h>
#include <cherry/map.h>
#include <cherry/string.h>
#include <cherry/memory.h>
#include <cherry/platform.h>
#include <cherry/math/math.h>
#include <cherry/graphic/types.h>
#include <native_ui/view.h>
#include <native_ui/parser.h>
#include <native_ui/manager.h>
#include <native_ui/view_controller.h>
#include <smartfox/data.h>
#include <macao/exec.h>
#include <ftw.h>
#include <macao/game.h>

struct string *__root_directory__ = NULL;
static struct nview *root = NULL;

struct touch_event {
        struct list_head head;
        struct event e;
};

static struct game *game = NULL;
static struct list_head touch_list;
static spin_lock lock;
static int resized = 0;


EMSCRIPTEN_KEEPALIVE
void macao_resize(int width, int height)
{
        if(root) {
                nview_set_size(root, (union vec2){(float)width, (float)height});
                nview_set_position(root, (union vec2){root->size.width / 2, root->size.height/2});
                nview_update_layout(root);
                resized = 1;

                video_width = width;
                video_height = height;
                glViewport(0, 0, video_width , video_height);
                if(game) game_resize(game, video_width, video_height);
        }
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
        // struct string *cache = string_alloc_chars(qskey(__root_directory__));
        // string_cat(cache, qlkey("/"));
        // char *ver = file_get_version_directory();
        // string_cat(cache, ver, strlen(ver));
        // nftw(cache->ptr, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
        // string_free(cache);

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
                var root = document.getElementById("root");
                root.appendChild(obj);

                var cv = document.getElementById("canvas");
                var w = root.offsetWidth;
                var h = root.offsetHeight;
                var sf = window.devicePixelRatio || 1;
                cv.width = w * sf;
                cv.height = h * sf;
                cv.style.width = w + "px";
                cv.style.height = h + "px";
                _macao_resize(w * sf, h * sf);
        }, root->ptr);
}

static void push_mouse_event(int x, int y, int state)
{
        struct touch_event *te  = smalloc(sizeof(struct touch_event), sfree);
        te->e.type              = EVENT_MOUSE;
        te->e.mouse_x           = x;
        te->e.mouse_y           = y;
        te->e.mouse_state       = state;

        spin_lock_lock(&lock);
        list_add_tail(&te->head, &touch_list);
        spin_lock_unlock(&lock);
}

static int mouse_down(int eventType, const EmscriptenMouseEvent *e, void *userData)
{
        push_mouse_event(e->targetX, e->targetY, MOUSE_DOWN);
        return EMSCRIPTEN_RESULT_SUCCESS;
}

static int mouse_up(int eventType, const EmscriptenMouseEvent *e, void *userData)
{
        push_mouse_event(e->targetX, e->targetY, MOUSE_UP);
        return EMSCRIPTEN_RESULT_SUCCESS;
}

static int mouse_move(int eventType, const EmscriptenMouseEvent *e, void *userData)
{
        push_mouse_event(e->targetX, e->targetY, MOUSE_MOVE);
        return EMSCRIPTEN_RESULT_SUCCESS;
}

static int mouse_cancel(int eventType, const EmscriptenMouseEvent *e, void *userData)
{
        push_mouse_event(e->targetX, e->targetY, MOUSE_CANCEL);
        return EMSCRIPTEN_RESULT_SUCCESS;
}

static void push_touch_event(int x, int y, int state)
{
        struct touch_event *te  = smalloc(sizeof(struct touch_event), sfree);
        te->e.type              = EVENT_TOUCH;
        te->e.touch_x           = x;
        te->e.touch_y           = y;
        te->e.touch_state       = state;

        spin_lock_lock(&lock);
        list_add_tail(&te->head, &touch_list);
        spin_lock_unlock(&lock);
}

static int touch_down(int eventType, const EmscriptenTouchEvent *e, void *userData)
{
        push_touch_event(e->touches[0].targetX, e->touches[0].targetY, TOUCH_DOWN);
        return EMSCRIPTEN_RESULT_SUCCESS;
}

static int touch_up(int eventType, const EmscriptenTouchEvent *e, void *userData)
{
        push_touch_event(e->touches[0].targetX, e->touches[0].targetY, TOUCH_UP);
        return EMSCRIPTEN_RESULT_SUCCESS;
}

static int touch_move(int eventType, const EmscriptenTouchEvent *e, void *userData)
{
        push_touch_event(e->touches[0].targetX, e->touches[0].targetY, TOUCH_MOVE);
        return EMSCRIPTEN_RESULT_SUCCESS;
}

static int touch_cancel(int eventType, const EmscriptenTouchEvent *e, void *userData)
{
        push_touch_event(e->touches[0].targetX, e->touches[0].targetY, TOUCH_CANCEL);
        return EMSCRIPTEN_RESULT_SUCCESS;
}


int init_gl()
{
        int height = 640;
        int width = height * 16 / 9;

        if (glfwInit() != GL_TRUE) {
                printf("glfwInit() failed\n");
                return GL_FALSE;
        }
        // glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
        if (glfwOpenWindow(width, height, 8, 8, 8, 8, 16, 8, GLFW_WINDOW) != GL_TRUE) {
                printf("glfwOpenWindow() failed\n");
                return GL_FALSE;
        }

        video_width = width;
        video_height = height;
        glViewport(0, 0, video_width, video_height);

        INIT_LIST_HEAD(&touch_list);
        spin_lock_init(&lock, 0);

        emscripten_set_mousedown_callback("#canvas", NULL, 1, mouse_down);
        emscripten_set_mouseup_callback("#canvas", NULL, 1, mouse_up);
        emscripten_set_mousemove_callback("#canvas", NULL, 1, mouse_move);
        emscripten_set_mouseout_callback("#canvas", NULL, 1, mouse_cancel);
        emscripten_set_mouseleave_callback("#canvas", NULL, 1, mouse_cancel);

        emscripten_set_touchstart_callback("#canvas", NULL, 1, touch_down);
        emscripten_set_touchend_callback("#canvas", NULL, 1, touch_up);
        emscripten_set_touchmove_callback("#canvas", NULL, 1, touch_move);
        emscripten_set_touchcancel_callback("#canvas", NULL, 1, touch_cancel);
        return GL_TRUE;
}

void do_frame()
{
        if(root) nmanager_update(nmanager_shared(), 1.0f / 60);
        else return;
        /*
         * process mouse event
         */
        struct list_head *head = NULL;
        struct touch_event *te;

        if(!resized) return;

        if(!game) {
                game = game_alloc();
        }

get_touch:
        spin_lock_lock(&lock);
        if(!list_singular(&touch_list)) {
                head = touch_list.next;
                list_del(head);
        }
        spin_lock_unlock(&lock);

        if(head) {
                te = (struct touch_event *)
                        ((char *)head - offsetof(struct touch_event, head));
                game_read_event(game, &te->e);
                sfree(te);
                head = NULL;
                goto get_touch;
        }
        /*
         * update game
         */
        game_update(game);
        game_render(game);

        glfwSwapBuffers();
}

void shutdown_gl()
{
        if(game) {
                game_free(game);
                /* destroy cache and free memory pages allocated */
                cache_free();
                dim_memory();
                game = NULL;
        }
        glfwTerminate();
}

int main(int argc, char **argv)
{
        __root_directory__ = string_alloc_chars(qlkey("/macao"));
        file_set_version_directory(qlkey("v1"));

        if (init_gl() == GL_TRUE) {
                EM_ASM(
                        FS.mkdir('/macao');
                        FS.mount(IDBFS, {}, '/macao');


                        FS.syncfs(true, function (err) {
                                _macao_start();
                        });
                );
                emscripten_set_main_loop(do_frame, 0, 1);
        }
        shutdown_gl();

        return 0;
}
