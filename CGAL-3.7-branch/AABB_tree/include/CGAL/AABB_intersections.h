// Copyright (c) 2008-2009  INRIA Sophia-Antipolis (France), ETH Zurich (Switzerland).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
// 
//
// Author(s)     : Camille Wormser, Pierre Alliez, Stephane Tayeb

#include <CGAL/intersections.h>

#include <CGAL/internal/AABB_Intersections_3/Bbox_3_Plane_3_do_intersect.h>
#include <CGAL/internal/AABB_Intersections_3/Bbox_3_Sphere_3_do_intersect.h>
#include <CGAL/internal/AABB_Intersections_3/Bbox_3_Triangle_3_do_intersect.h>

#include <CGAL/internal/AABB_Intersections_3/Triangle_3_Plane_3_intersection.h>

#include <CGAL/internal/AABB_tree/nearest_point_segment_3.h>
#include <CGAL/internal/AABB_tree/nearest_point_triangle_3.h>
