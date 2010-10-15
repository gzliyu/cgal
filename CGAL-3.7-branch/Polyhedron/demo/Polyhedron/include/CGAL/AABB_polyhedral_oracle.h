// Copyright (c) 2008  INRIA Sophia-Antipolis (France).
// Copyright (c) 2009  GeometryFactory (France).
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
// Author(s) : Pierre Alliez, Laurent Rineau, Stephane Tayeb

#ifndef CGAL_AABB_POLYHEDRAL_ORACLE_H
#define CGAL_AABB_POLYHEDRAL_ORACLE_H

#include <utility>
#include <CGAL/iterator.h>

#include <CGAL/point_generators_3.h>
#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_polyhedron_triangle_primitive.h>

#include <boost/shared_ptr.hpp>

namespace CGAL {

  template <class Polyhedron, class Kernel, class Dummy_kernel>
  class AABB_polyhedral_oracle : public Polyhedron
  {
  public:
    typedef typename Kernel::FT FT;
    typedef typename Kernel::Ray_3 Ray_3;
    typedef typename Kernel::Line_3 Line_3;
    typedef typename Kernel::Point_3 Point_3;
    typedef typename Kernel::Segment_3 Segment_3;

    typedef AABB_polyhedral_oracle<Polyhedron,Kernel,Dummy_kernel> Self;
    typedef Self Surface_mesher_traits_3;
    typedef Point_3 Intersection_point;
    typedef Self Surface_3;

    // AABB tree
    typedef AABB_const_polyhedron_triangle_primitive<Kernel, Polyhedron>
                                                                AABB_primitive;
    typedef class AABB_traits<Kernel,AABB_primitive> AABB_traits;
    typedef AABB_tree<AABB_traits> Tree;
    typedef typename AABB_traits::Bounding_box Bounding_box;
    
    typedef boost::shared_ptr<Tree> Tree_shared_ptr;
    Tree_shared_ptr m_pTree;

  public:
    Tree* tree() const { return m_pTree.get(); }

  public:
    // Surface constructor
    AABB_polyhedral_oracle(const Polyhedron& poly)
    {
      m_pTree = Tree_shared_ptr(new Tree(poly.facets_begin(),
                                         poly.facets_end()));
    }

    AABB_polyhedral_oracle(const AABB_polyhedral_oracle& oracle)
    {
      m_pTree = oracle.m_pTree;
    }

    class Intersect_3;
    friend class Intersect_3;

    class Intersect_3 {
      typedef boost::optional<typename Tree::Object_and_primitive_id> 
        AABB_intersection;
      
      const Self& self;
    public:
      Intersect_3(const Self& self) : self(self)
      {
      }

      Object operator()(const Surface_3& surface, const Segment_3& segment) const
      {
        AABB_intersection intersection = surface.tree()->any_intersection(segment);
      
        if ( intersection )
          return intersection->first;
        else
          return Object();
      }

      Object operator()(const Surface_3& surface, const Ray_3& ray) const
      {
        AABB_intersection intersection = surface.tree()->any_intersection(ray);
        
        if ( intersection )
          return intersection->first;
        else
          return Object();
      }

      Object operator()(const Surface_3& surface, const Line_3& line) const
      {
        AABB_intersection intersection = surface.tree()->any_intersection(line);
        
        if ( intersection )
          return intersection->first;
        else
          return Object();
      }
    };

    Intersect_3 intersect_3_object() const
    {
      return Intersect_3(*this);
    }

    class Construct_initial_points;

    friend class Construct_initial_points;

    class Construct_initial_points
    {
      const Self& self;
    public:
      Construct_initial_points(const Self& self) : self(self)
      {
      }

      template <typename OutputIteratorPoints>
      OutputIteratorPoints operator() (const Surface_3& surface,
	OutputIteratorPoints out,
	int n) const
      {
	// std::cout << "AABB_polyhedral_oracle: empty initial point set" << std::endl;
	return out;
      }
    };

    Construct_initial_points construct_initial_points_object() const
    {
      return Construct_initial_points(*this);
    }

    template <class P>
    bool is_in_volume(const Surface_3& surface, const P& p)
    {
      const Bounding_box bbox = surface.tree()->root_bbox();
      if(p.x() < bbox.xmin() || p.x() > bbox.xmax())
        return false;
      if(p.y() < bbox.ymin() || p.y() > bbox.ymax())
        return false;
      if(p.z() < bbox.zmin() || p.z() > bbox.zmax())
        return false;

      const double diameter = max_bbox_length(bbox) * 2;

      typename CGAL::Random_points_on_sphere_3<Point_3> random_point(FT(1));
      typename Kernel::Construct_vector_3 vector =
        Kernel().construct_vector_3_object();
      typename Kernel::Construct_segment_3 segment =
        Kernel().construct_segment_3_object();
      typename Kernel::Construct_translated_point_3 translate =
        Kernel().construct_translated_point_3_object();
      typename Kernel::Construct_scaled_vector_3 scale =
        Kernel().construct_scaled_vector_3_object();

      const Segment_3 seg =
        segment(p, translate(p,
                             scale(vector(ORIGIN,
                                          *random_point),
                                   diameter)));
      return (surface.tree()->number_of_intersections(seg) % 2) == 1;
    }

  private:
    double max_bbox_length(const Bounding_box& bbox) const
    {
      return (std::max)(bbox.xmax()-bbox.xmin(),
                       (std::max)(bbox.ymax()-bbox.ymin(),
                                  bbox.zmax()-bbox.zmin()));
    }

  }; // end class AABB_polyhedral_oracle

} // end namespace CGAL

#endif // CGAL_AABB_POLYHEDRAL_ORACLE_H
