/*
 * Copyright (c) 2008 INdT - Instituto Nokia de Tecnologia
 *
 * This file is part of python-purple.
 *
 * python-purple is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * python-purple is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <purple.h>

#include <glib.h>

#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "c_purple.h"

#define PURPLE_GLIB_READ_COND  (G_IO_IN | G_IO_HUP | G_IO_ERR)
#define PURPLE_GLIB_WRITE_COND (G_IO_OUT | G_IO_HUP | G_IO_ERR | G_IO_NVAL)

typedef struct _PurpleGLibIOClosure {
	PurpleInputFunction function;
	guint result;
	gpointer data;
} PurpleGLibIOClosure;

static gboolean purple_glib_io_invoke(GIOChannel *source, GIOCondition condition,
		gpointer data)
{
	PurpleGLibIOClosure *closure = data;
	PurpleInputCondition purple_cond = 0;

	if (condition & PURPLE_GLIB_READ_COND)
		purple_cond |= PURPLE_INPUT_READ;
	if (condition & PURPLE_GLIB_WRITE_COND)
		purple_cond |= PURPLE_INPUT_WRITE;

	closure->function(closure->data, g_io_channel_unix_get_fd(source),
			purple_cond);

	return TRUE;
}

guint glib_input_add(gint fd, PurpleInputCondition condition, PurpleInputFunction function,
		gpointer data)
{
	PurpleGLibIOClosure *closure = g_new0(PurpleGLibIOClosure, 1);
	GIOChannel *channel;
	GIOCondition cond = 0;

	closure->function = function;
	closure->data = data;

	if (condition & PURPLE_INPUT_READ)
		cond |= PURPLE_GLIB_READ_COND;
	if (condition & PURPLE_INPUT_WRITE)
		cond |= PURPLE_GLIB_WRITE_COND;

	channel = g_io_channel_unix_new(fd);
	closure->result = g_io_add_watch_full(channel, 0, cond,
			purple_glib_io_invoke, closure, g_free);

	g_io_channel_unref(channel);
	return closure->result;
}

static void
signed_on(PurpleConnection *gc, gpointer null)
{
	PurpleAccount *account = purple_connection_get_account(gc);
	printf("Account connected: %s %s\n", account->username, account->protocol_id);
}

void connect_to_signals_for_demonstration_purposes_only(void)
{
	static int handle;

	purple_signal_connect(purple_connections_get_handle(), "signed-on", &handle,
				PURPLE_CALLBACK(signed_on), NULL);
}

void glib_main_loop() {
	GMainLoop *loop = g_main_loop_new(NULL, 0);
	g_main_loop_run(loop);
}
