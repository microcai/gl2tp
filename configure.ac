
# gl2tp - PACKAGE_DESCRIBTION

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

m4_define([package_name], [gl2tp])

AC_INIT([package_name], [0.0.1], [BUG REPORT URL], [package_name])
AM_INIT_AUTOMAKE([-Wall])
# AC_GNU_SOURCE

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CC
AC_HEADER_STDC


AC_PREFIX_DEFAULT(/usr)

# define GETTEXT_* variables
GETTEXT_PACKAGE="$PACKAGE_NAME"
AC_SUBST(GETTEXT_PACKAGE)
AC_SUBST(prefix)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [Define to the read-only architecture-independent data directory.])
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION(0.17.0)

# OUTPUT files
AC_CONFIG_FILES(po/Makefile.in
src/Makefile
icons/Makefile
doc/Makefile
Makefile
)

AC_OUTPUT