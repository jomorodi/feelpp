###  TEMPLATE.txt.tpl; coding: utf-8 ---
#  Author(s): Christophe Prud'homme <christophe.prudhomme@feelpp.org>
#       Date: 2013-02-18
#
#  Copyright (C) 2013-2016 Feel++ Consortium
#
# Distributed under the GPL(GNU Public License):
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
#
set_directory_properties(PROPERTIES EXCLUDE_FROM_ALL TRUE)

feelpp_add_application( stokes SRCS stokes.cpp LABELS doc )
feelpp_add_application( stokes_curl SRCS stokes_curl.cpp LABELS doc GEO curvedtube.geo straighttube.geo CFG stokes_curl.cfg)
feelpp_add_application( stokes_lm SRCS stokes.cpp LABELS doc DEFS FEELPP_USE_LM )
feelpp_add_application( stokes_periodic SRCS stokes_periodic.cpp LABELS doc NO_MPI_TEST )
feelpp_add_application( stokes_dirichletlm SRCS stokes_dirichletlm.cpp LABELS doc  )
feelpp_add_application( stokes_pressurelm SRCS stokes_pressurelm.cpp LABELS doc  )
