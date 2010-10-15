// Copyright (c) 2005  Stanford University (USA).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
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
// Author(s)     : Daniel Russel <drussel@alumni.princeton.edu>

#ifndef CGAL_KINETIC_INEXACT_SIMULATION_1_H
#define CGAL_KINETIC_INEXACT_SIMULATION_1_H

#include <CGAL/Kinetic/Simulation_traits.h>

namespace CGAL { namespace Kinetic {

struct Inexact_simulation_traits_1: public Suggested_inexact_simulation_traits_base {
  typedef Suggested_inexact_simulation_traits_base P;
  typedef Active_objects_vector<P::Kinetic_kernel::Point_1> Active_points_1_table;
  Active_points_1_table* active_points_1_table_handle() {
    return ap_.get();
  }
  const Active_points_1_table* active_points_1_table_handle() const {
    return ap_.get();
  }

  typedef Cartesian_instantaneous_kernel<Active_points_1_table, P::Static_kernel> Instantaneous_kernel;
  Instantaneous_kernel instantaneous_kernel_object() const {
    return Instantaneous_kernel(ap_, P::static_kernel_object());
  }
  Inexact_simulation_traits_1(const P::Time &lb = 0,
			    const P::Time &ub = std::numeric_limits<P::Time>::infinity()): P(lb,ub), 
											   ap_(new Active_points_1_table()){}
 

protected:
  Active_points_1_table::Handle ap_;
};
} } //namespace CGAL::Kinetic
#endif
