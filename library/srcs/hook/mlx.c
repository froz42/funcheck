/* Funcheck - A tool for checking functions calls return protections
 * Copyright (C) 2023  Theo Matis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * This file contain specials hooks for the mlx library by 42
 * 
 * This is really hacky but since X11 stay frozen if his alloc is hooked
 * we need to do this
*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "hook.h"


/*
 * When XSync is called we close the program immediately
 * So the user don't have to click repeatedly on every check
*/
int EXPORT XSync(Display *a, Bool b)
{
	(void)a;
	(void)b;
	exit(0);
	return 0; // never reached
}


/*
 * The bellows function are hooked to ignore the alloc hooks
 * because X11 stay frozen if his alloc is hooked
*/

Display * EXPORT XOpenDisplay(const char *a)
{
	disable_alloc_hooks();
	typeof(&XOpenDisplay) original_function = get_original_function();
	Display *result = original_function(a);
	enable_alloc_hooks();
	return result;
}

int EXPORT XShmQueryVersion(Display *a, int *b, int *c, int *d)
{
	disable_alloc_hooks();
	typeof(&XShmQueryVersion) original_function = get_original_function();
	int result = original_function(a, b, c, d);
	enable_alloc_hooks();
	return result;
}

KeySym EXPORT XkbKeycodeToKeysym(Display *a, KeyCode b, int c, int d)
{
	disable_alloc_hooks();
	typeof(&XkbKeycodeToKeysym) original_function = get_original_function();
	KeySym result = original_function(a, b, c, d);
	enable_alloc_hooks();
	return result;
}

GC EXPORT XCreateGC(Display *a, Drawable b, unsigned long c, XGCValues *d)
{
	disable_alloc_hooks();
	typeof(&XCreateGC) original_function = get_original_function();
	GC result = original_function(a, b, c, d);
	enable_alloc_hooks();
	return result;
}

Atom EXPORT XInternAtom(Display *a, const char *b, Bool c)
{
	disable_alloc_hooks();
	typeof(&XInternAtom) original_function = get_original_function();
	Atom result = original_function(a, b, c);
	enable_alloc_hooks();
	return result;
}

int EXPORT XShmPixmapFormat(Display *a)
{
	disable_alloc_hooks();
	typeof(&XShmPixmapFormat) original_function = get_original_function();
	int result = original_function(a);
	enable_alloc_hooks();
	return result;
}

int EXPORT XGetWMNormalHints(Display *a, Window b, XSizeHints *c, long *d)
{
	disable_alloc_hooks();
	typeof(&XGetWMNormalHints) original_function = get_original_function();
	int result = original_function(a, b, c, d);
	enable_alloc_hooks();
	return result;
}