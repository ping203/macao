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
#version 100

#ifdef GL_ES
        #define input   attribute
        #define output  varying
#else
        #define input   in
        #define output  out
#endif

#include <../common/light.slib>
#include <../common/matrix.slib>
#include <../common/skin.slib>
#include <../common/camera.slib>
#include <../common/transform.slib>

input vec3      position;
input vec3      normal;
input vec4      color;
input vec3      texcoord;

uniform camera_uniform          camera;
uniform transform_uniform       transform;
uniform object_light_uniform    object_light;

/*
 * pixel_color used to adjust fragment color
 */
output vec4     pixel_color;
output vec2     pixel_texcoord;

void main()
{
        skin_result kr          = skin_calculate(position, normal);
        gl_Position             = camera.project * camera.view * transform.transform * kr.skin_pos;

        /*
         * pass pixel parameters
         */
        vec3 pixel_normal       = mat3(matrix4_transpose(matrix4_inverse(transform.transform))) * kr.skin_norm.xyz;
        vec3 norm               = normalize(pixel_normal);

        vec3 result             = calculate_light(transform.transform, view.position, position, norm, bright);

        pixel_color             = color * vec4(result, 1.0);
        pixel_texcoord          = texcoord;
}
