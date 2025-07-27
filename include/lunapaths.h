/*
 * Copyright (c) 2025 Sebastian Lau <sebastianlau995@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#ifndef LUNASVG_PATHS_H
#define LUNASVG_PATHS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"


typedef struct ColorRGBA {
    double r;
    double g;
    double b;
    double a;
} rgba_t;

struct ColorRGBA* lunasvg__init_color_rgba( const double r, const double g, const double b, const double a );
struct ColorRGBA* lunasvg__init_color_rgba_default();
unsigned short lunasvg_toHexColor( const double color );
unsigned int lunasvg_toHex( const struct ColorRGBA* color );
struct ColorRGBA* lunasvg_toRGBA( const unsigned int color_hex );

typedef struct Point {
    double x;
    double y;
} px_t;

struct ColorRGBA* lunasvg_getColorRGBA( const struct Point* px, const char* imageData, float dpi );
struct ColorRGBA* lunasvg_getColor_Position( const double x, const double y, const char* imageData, float dpi );

struct Point* lunasvg__init_point( const double x, const double y );
struct Point* lunasvg__init_point_default();


typedef struct PointColored {
    struct Point* pos;
    struct ColorRGBA* color;
} point_t;

struct PointColored* lunasvg__init_point_colored( const double x, const double y,  const double r, const double g, const double b, const double a );
struct PointColored* lunasvg__init_point_colored_default();
struct PointColored* lunasvg__init_point_colored_only_color(  const double r, const double g, const double b, const double a );
struct PointColored* lunasvg__init_point_colored_only_point( const double x, const double y );
void lunasvg_getColor( struct PointColored* px, const char* imageData, float dpi );
unsigned int lunasvg_getColor_from_px( struct Point* pos, const char* imageData, float dpi );

typedef struct PathClosure {
    struct PointColored* px;
    
    struct PathClosure* next;
    struct PathClosure* prev;
} path_closure_t;

struct PathClosure* lunasvg__init_path_closure( const double x, const double y, struct PathClosure* nxt, struct PathClosure* pre );
struct PathClosure* lunasvg__init_path_closure_from_point( struct PointColored* px, struct PathClosure* nxt, struct PathClosure* pre );

void lunasvg_assure_color( struct PathClosure* path_px, const char* imageData, float dpi, unsigned int max_points );
struct PointColored* lunasvg_getPointNext( struct PathClosure* path_px, const double direction_x, const char* imageData, float dpi );
void lunasvg_getPoint2( struct PathClosure* path_px, const double direction_x, const char* imageData, float dpi );
void lunasvg_getPoint3( struct PathClosure* path_px, const double direction_x, const char* imageData, float dpi );
const struct PointColored** lunasvg__as_array_points( const struct PathClosure* path_closure );

typedef struct Path {
    struct PathClosure* shape;

    struct PathClosure* path;
    bool closed;
} path_t;

struct PointColored* lunasvg_getNext( const struct PointColored** points );
struct PointColored* lunasvg_getPrex( const struct PointColored** points );

typedef struct LinearPath {
    const struct Point* point;
    struct LinearPath* next;
    struct LinearPath* prev;
} linear_path_t;

struct LinearPath* lunasvg__init_linear_path( const struct Point* px, struct LinearPath* next, struct LinearPath* prev );
struct LinearPath* lunasvg__init_linear_path_default( const struct Point* px );

typedef struct Parabole {
    const struct Point* start;
    struct Point* end;
    struct LinearPath* linear;
    struct Point* ceil;
    struct Point* floor;
    struct LinearPath* shapes;
} parabole_t;

struct Parabole* lunasvg__init_parabole( const struct Point* px );
struct Parabole* lunasvg__init_parabole_from_linearPath( const linear_path_t* linearPath );
void lunasvg_init_parabole( struct Parabole* para );
linear_path_t* lunasvg_parabole_to_linear_path( const struct Parabole* para );

linear_path_t* lunasvg_path_getLinear( const struct Point* px, const unsigned int max_additional_path_points );
parabole_t* lunasvg_path_getParabole( const struct Point* p1, const struct Point* p2, const unsigned int max_additional_path_points );
parabole_t* lunasvg_path_correctToParabole_linearPath( const linear_path_t* linearPath, const unsigned int max_additional_path_points );
parabole_t* lunasvg_path_correctToParabole( const struct Point* p1, const struct Point* p2, const struct Point* p3, const unsigned int max_additional_path_points );
void lunasvg_path_getMoreShape( struct PathClosure* path, const unsigned int max_additional_path_points );

#ifdef __cplusplus
}
#endif

#endif // LUNASVG_PATHS_H