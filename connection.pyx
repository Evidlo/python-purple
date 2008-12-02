#
#  Copyright (c) 2008 INdT - Instituto Nokia de Tecnologia
#
#  This file is part of python-purple.
#
#  python-purple is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  python-purple is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

cimport purple

cdef void signed_on_cb (connection.PurpleConnection *gc, glib.gpointer null):
    cdef account.PurpleAccount *acc = connection.c_purple_connection_get_account(gc)
    print "Account connected: %s %s" % (acc.username, acc.protocol_id)

cdef class Connection:
    """ Connection class """
    cdef connection.PurpleConnection *__conn

    def __init__(self):
        pass

    def connect(self):
        cdef int handle

        signals.c_purple_signal_connect(connection.c_purple_connections_get_handle(),
            "signed-on", &handle, <signals.PurpleCallback> signed_on_cb, NULL)