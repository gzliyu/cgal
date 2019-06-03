// 2D intersection tests.

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Homogeneous.h>

#include <CGAL/Intersection_traits_2.h>
#include <CGAL/Intersections_2/Segment_2_Segment_2.h>
#include <CGAL/Intersections_2/Line_2_Line_2.h>
#include <CGAL/Intersections_2/Ray_2_Ray_2.h>
#include <CGAL/Intersections_2/Triangle_2_Triangle_2.h>
#include <CGAL/Intersections_2/Iso_rectangle_2_Line_2.h>
#include <CGAL/Intersections_2/Iso_rectangle_2_Ray_2.h>

#include <CGAL/Intersections_2/Bbox_2_Circle_2.h>
#include <CGAL/Intersections_2/Bbox_2_Point_2.h>
#include <CGAL/Intersections_2/Circle_2_Iso_rectangle_2.h>
#include <CGAL/Intersections_2/Circle_2_Point_2.h>
#include <CGAL/Intersections_2/Point_2_Point_2.h>

#include <vector>
#include <iostream>
#include <cassert>

const double epsilon = 0.001;

struct randomint {
  randomint() ;
  int	get() const { return sequence[cur]; }
  int next() { cur = (cur+1)%11; return get();}
private:
  int sequence[11];
  int cur;
};

inline randomint::randomint()
{
  cur = 0;
  sequence[0] = 19;
  sequence[1] = 5;
  sequence[2] = 17;
  sequence[3] = 13;
  sequence[4] = 29;
  sequence[5] = 2;
  sequence[6] = 23;
  sequence[7] = 31;
  sequence[8] = 3;
  sequence[9] = 37;
  sequence[10] = 11;
}

randomint ri;

inline double to_nt(int d)
{
    return double(d);
}

template < typename K >
struct Test {

  typedef typename K::Point_2               P;
  typedef typename K::Line_2                L;
  typedef typename K::Segment_2             S;
  typedef typename K::Ray_2                 R;
  typedef typename K::Triangle_2            T;
  typedef typename K::Iso_rectangle_2       Rec;
  typedef typename K::Circle_2              C;
  typedef std::vector<P>              Pol;

  template < typename Type >
  bool approx_equal_nt(const Type &t1, const Type &t2)
  {
	if (t1 == t2)
		return true;
	if (CGAL::abs(t1 - t2) / (CGAL::max)(CGAL::abs(t1), CGAL::abs(t2)) < epsilon)
		return true;
	std::cout << " Approximate comparison failed between : " << t1 << "  and  " << t2 << "\n";
	return false;
  }

  template < typename Type >
  bool approx_equal(const Type&t1, const Type&t2)
  {
	return t1 == t2;
	// we need approx equal to check approx kernels, but maybe we should only test with exact kernels
	// (approx kernels were useful before, when the text output was checked by diff ?)
	// idea : test containment with intervals ?  or use some "epsilon double"?
	// I need to convert the text output to exact rationals in the source...
	// Well, for now the current scheme works.
  }

  bool approx_equal(const P & p, const P & q)
  {
	return approx_equal_nt(p.x(), q.x()) && approx_equal_nt(p.y(), q.y());
  }

  bool approx_equal(const Pol & p, const Pol & q)
  {
	if (p.size() != q.size())
		return false;
	for(typename Pol::const_iterator itp = p.begin(), itq = q.begin(); itp != p.end(); ++itp, ++itq)
		if (!approx_equal(*itp, *itq))
			return false;
	return true;
  }

  template < typename O1, typename O2>
  void check_no_intersection(const O1& o1, const O2& o2)
  {
	assert(!CGAL::do_intersect(o1, o2));
	assert(!CGAL::do_intersect(o2, o1));
        assert(!CGAL::intersection(o2, o1));
    
	//check with the functors
	typename CGAL::Kernel_traits<O1>::Kernel::Do_intersect_2 do_2;
	typename CGAL::Kernel_traits<O1>::Kernel::Intersect_2 i_2;
	assert(!do_2(o1, o2));
        assert(!i_2(o1, o2));
	assert(!do_2(o2, o1));
        assert(!i_2(o2, o1));
  }

  template < typename Res, typename O1, typename O2 >
  void check_intersection(const O1& o1, const O2& o2)
  {
	Res tmp;
	assert(CGAL::do_intersect(o1, o2));
	assert(CGAL::assign(tmp, CGAL::intersection(o1, o2)));
	assert(CGAL::do_intersect(o2, o1));
	assert(CGAL::assign(tmp, CGAL::intersection(o2, o1)));
  }

  template < typename Res, typename O1, typename O2 >
  void check_intersection(const O1& o1, const O2& o2, const Res& result, bool do_opposite=true)
  {
	Res tmp;
	assert(CGAL::do_intersect(o1, o2));
	assert(CGAL::assign(tmp, CGAL::intersection(o1, o2)));
	assert(approx_equal(tmp, result));
	if (do_opposite) {
	  assert(CGAL::do_intersect(o2, o1));
	  assert(CGAL::assign(tmp, CGAL::intersection(o2, o1)));
	  assert(approx_equal(tmp, result));
	}
  }


  P p(int x, int y)
  {
    int w = ri.next();
    return P(to_nt(x*w), to_nt(y*w), to_nt(w));
  }

  void B_P()
  {
    CGAL::Bbox_2 bb(0,0,10,10);
    P p(1,0), bl(0,0), tr(10,10);
    C c(bl,1);
    Rec r(bl,tr);
    check_intersection(bb,p,p,true);
    check_intersection(c,p,p,true);
    assert(do_intersect(r,c));
  }
  
  void L_L()
  {
    std::cout << "Line - Line\n";
    check_intersection     (L(p(0, 0), p(10, 0)), L(p(1,7), p(1,-2)), P(1,0));
    check_intersection     (L(p(0,-1), p(10, 0)), L(p(2,1), p(8,-6)), P(3.42105,-0.657895));
    check_intersection<L>  (L(p(0, 0), p(10, 0)), L(p(1,0), p(8, 0)));
    
check_no_intersection  (L(p(0, 0), p(10,10)), L(p(8,7), p(1, 0)));
    check_intersection<L>  (L(p(0, 0), p(10, 0)), L(p(8,0), p(1, 0)));
  }

  void S_S()
  {
    std::cout << "Segment - Segment\n";
    check_no_intersection  (S(p(29,  16), p( 28,   9)), S(p( 30,  12), p( 29,   6)));
    check_intersection     (S(p(68, 106), p(192, 106)), S(p(150, 106), p(255, 106)), S(P(150, 106), P(192, 106)));
    check_intersection     (S(p( 1,  10), p(  1,   2)), S(p(  1,   7), p(  1,   3)), S(P(  1,   3), P(  1,   7)));
    check_no_intersection  (S(p( 0,   0), p(103567,9826)), S(p(10000,3782), p(76250,83736)));
    check_intersection     (S(p( 0,  -1), p( 10,   0)), S(p(  2,   1), p(  8,  -6)), P(3.42105, -0.657895));
    check_intersection     (S(p( 0,   0), p( 10,   0)), S(p(  1,   0), p(  8,   0)), S(P(  1,   0), P(  8,   0)));
    check_intersection     (S(p( 0,   0), p( 10,   0)), S(p(  1,   6), p(  1,  -3)), P(1, 0));
  }

  void R_R()
  {
    std::cout << "Ray - Ray\n";
    check_intersection     (R(p( 2,  -1), p(  2,   1)), R(p(  1,   3), p(  2,   3)), P(2, 3));
    check_intersection     (R(p( 0,  -1), p( 10,   0)), R(p(  2,   1), p(  8,  -6)), P(3.42105, -0.657895));
    check_intersection     (R(p( 0,   0), p( 10,   0)), R(p(  1,   6), p(  1,  -3)), P(1, 0));
  }

  void S_R()
  {
    std::cout << "Segment - Ray\n";
    check_no_intersection  (S(p( 2,  -1), p(  2,   1)), R(p(  1,   3), p(  2,   3)));
    check_intersection     (S(p( 0,  -1), p( 10,   0)), R(p(  2,   1), p(  8,  -6)), P(3.42105, -0.657895));
    check_intersection     (S(p( 0,   0), p( 10,   0)), R(p(  1,   6), p(  1,  -3)), P(1, 0));
  }

  void L_R()
  {
    std::cout << "Line - Ray\n";
    check_intersection     (L(p( 2,  -1), p(  2,   1)), R(p(  1,   3), p(  2,   3)), P(2, 3));
    check_intersection     (L(p( 0,  -1), p( 10,   0)), R(p(  2,   1), p(  8,  -6)), P(3.42105, -0.657895));
    check_intersection     (L(p( 0,   0), p( 10,   0)), R(p(  1,   6), p(  1,  -3)), P(1, 0));
  }

  void S_L()
  {
    std::cout << "Segment - Line\n";
    check_no_intersection  (S(p( 2,  -1), p(  2,   1)), L(p(  1,   3), p(  2,   3)));
    check_intersection     (S(p( 0,  -1), p( 10,   0)), L(p(  2,   1), p(  8,  -6)), P(3.42105, -0.657895));
    check_intersection     (S(p( 0,   0), p( 10,   0)), L(p(  1,   6), p(  1,  -3)), P(1, 0));
  }

  void T_T()
  {
    std::cout << "Triangle - Triangle\n";
    check_intersection<S>  (T(p( -10,  0), p( 10,   0), p(0, -3)), T(p(  -8,   0), p( 12,   0), p(1, 5)));
    check_intersection<S>  (T(p( -10,  0), p( 10,   0), p(0, -3)), T(p(  -8,   0), p(  8,   0), p(1, 5)));
    check_intersection<S>  (T(p( -10,  0), p( 10,   0), p(0, -3)), T(p( -12,   0), p( 12,   0), p(1, 5)));
    check_intersection     (T(p( -10,  0), p( 10,   0), p(0,  3)), T(p( -12,   3), p( 12,   3), p(1, 5)), P(0, 3));
    Pol pol0;
    pol0.push_back(P(-6, -4));
    pol0.push_back(P( -5.11111, -0.222222 ));
    pol0.push_back(P( 0, 10 ));
    pol0.push_back(P( 8, 4 ));
    check_intersection     (T(p(   0, 10), p(-10, -10), p( 20, -5)), T(p(   2,  30), p( -6,  -4), p(15, 8)), pol0, false);
    check_intersection<T>  (T(p( -12,  1), p(  5,   3), p( -7, -15)), T(p(  29,  -2), p(  0, -13), p(1, 21)));
    Pol pol1;
    pol1.push_back(P( 8,  4));
    pol1.push_back(P( 0, 10 ));
    pol1.push_back(P( -5.11111, -0.222222 ));
    pol1.push_back(P(-6, -4));
    check_intersection     (T(p( -10,-10), p(  0,  10), p( 20, -5)), T(p(   2,  30), p( -6,  -4), p(15, 8)), pol1, false);
    Pol pol2;
    pol2.push_back(P( 10.2222, 2.33333 ));
    pol2.push_back(P( 1.96923, 8.52308 ));
    pol2.push_back(P( -0.680851, 8.6383 ));
    pol2.push_back(P( -5.94872, -1.89744 ));
    pol2.push_back(P( -3.96178, -8.99363 ));
    pol2.push_back(P( 3.5, -7.75 ));
    check_intersection     (T(p( -10,-10), p(  0,  10), p( 20, -5)), T(p(   -9,   9), p( 14,   8), p(-2, -16)), pol2, false);
    check_no_intersection  (T(p( -10,-10), p(  0,  10), p( 20, -5)), T(p(   90, -10), p(100,  10), p(120, -5)));
  }

  void L_T()
  {
    std::cout << "Line - Triangle\n";
    check_intersection<S>  (L(p( -1,  -1), p(  0,  -1)), T(p(  -10,   0), p(  10,   0), p(0, -4)));
    check_intersection<S>  (L(p( -1,  -1), p(  0,  -1)), T(p(  -10,   0), p(  15,   2), p(0, -4)));
    check_intersection<S>  (L(p(-10,   0), p( 10,   0)), T(p(   -8,   0), p(   8,   0), p(1,  5)));
    check_intersection<S>  (L(p(-10,   0), p( 10,   0)), T(p(  -12,   0), p(  12,   0), p(1,  5)));
    check_no_intersection  (L(p(-10,   0), p( 10,   0)), T(p(  -12,   3), p(  12,   3), p(1,  5)));
    check_intersection<S>  (L(p(  0,  10), p(-10, -10)), T(p(    2,  30), p(  -6,  -4), p(15, 8)));
    check_intersection<S>  (L(p(-12,   1), p(  5,   3)), T(p(   29,  -2), p(   0, -13), p( 1, 21)));
    check_intersection<S>  (L(p(-10, -10), p(  0,  10)), T(p(    2,  30), p(  -6,  -4), p(15,  8)));
    check_intersection<S>  (L(p(-10, -10), p(  0,  10)), T(p(   -9,   9), p(  14,   8), p(-2,-16)));
  }

  void R_T()
  {
    std::cout << "Ray - Triangle\n";
    check_intersection<S>  (R(p( -1,  -1), p(  0,  -1)), T(p(  -10,   0), p(  10,   0), p(0, -4)));
    check_intersection<S>  (R(p( -1,  -1), p(  0,  -1)), T(p(  -10,   0), p(  15,   2), p(0, -4)));
    check_intersection<S>  (R(p(-10,   0), p( 10,   0)), T(p(   -8,   0), p(   8,   0), p(1,  5)));
    check_intersection<S>  (R(p(-10,   0), p( 10,   0)), T(p(  -12,   0), p(  12,   0), p(1,  5)));
    check_no_intersection  (R(p(-10,   0), p( 10,   0)), T(p(  -12,   3), p(  12,   3), p(1,  5)));
    check_intersection<S>  (R(p(  0,  10), p(-10, -10)), T(p(    2,  30), p(  -6,  -4), p(15, 8)));
    check_intersection<S>  (R(p(-12,   1), p(  5,   3)), T(p(   29,  -2), p(   0, -13), p( 1, 21)));
    check_intersection<S>  (R(p(-10, -10), p(  0,  10)), T(p(    2,  30), p(  -6,  -4), p(15,  8)));
    check_intersection<S>  (R(p(-10, -10), p(  0,  10)), T(p(   -9,   9), p(  14,   8), p(-2,-16)));
  }

  void S_T()
  {
    std::cout << "Segment - Triangle\n";
    check_intersection<S>  (S(p( -1,  -1), p(  0,  -1)), T(p(  -10,   0), p(  10,   0), p(0, -4)));
    check_no_intersection  (S(p(-10, -10), p(  0,  10)), T(p(   90, -10), p( 100,  10), p(120, -5)));
    check_intersection<S>  (S(p( -1,  -1), p(  0,  -1)), T(p(  -10,   0), p(  15,   2), p(0, -4)));
    check_intersection<S>  (S(p(-10,   0), p( 10,   0)), T(p(   -8,   0), p(   8,   0), p(1,  5)));
    check_intersection<S>  (S(p(-10,   0), p( 10,   0)), T(p(  -12,   0), p(  12,   0), p(1,  5)));
    check_no_intersection  (S(p(-10,   0), p( 10,   0)), T(p(  -12,   3), p(  12,   3), p(1,  5)));
    check_intersection<S>  (S(p(  0,  10), p(-10, -10)), T(p(    2,  30), p(  -6,  -4), p(15, 8)));
    check_intersection<S>  (S(p(-12,   1), p(  5,   3)), T(p(   29,  -2), p(   0, -13), p( 1, 21)));
    check_intersection<S>  (S(p(-10, -10), p(  0,  10)), T(p(    2,  30), p(  -6,  -4), p(15,  8)));
    check_intersection<S>  (S(p(-10, -10), p(  0,  10)), T(p(   -9,   9), p(  14,   8), p(-2,-16)));
  }

  void P_P()
  {
    std::cout << "Point - Point\n";
    check_no_intersection<P>  (p(  8, 4), p(-4,  8));
    check_intersection<P>     (p(  8, 4), p( 8,  4));
  }

  void P_T()
  {
    std::cout << "Point - Triangle\n";
    check_intersection<P>  (p(  8,   4), T(p(    4,   0), p(  12,   4), p(-4,  8)));
    check_intersection<P>  (p(  8,   5), T(p(    4,   0), p(  12,   4), p(-4,  8)));
    check_no_intersection  (p(  8,   6), T(p(    4,   0), p(  12,   4), p(-4,  8)));
  }

  void L_Rec()
  {
    std::cout << "Line - Iso_rectangle\n";
    check_intersection<S>  (L(p( 18,   6), p(0, 0)), Rec(p(  2,   0), p( 6,  3)));
  }

  void R_Rec()
  {
    std::cout << "Ray - Iso_rectangle\n";
    check_intersection<S>  (R(p( 18,   6), p(0, 0)), Rec(p(  2,   0), p( 6,  3)));
  }

  void S_Rec()
  {
    std::cout << "Segment - Iso_rectangle\n";
    check_intersection<S>  (S(p( 18,   6), p(0, 0)), Rec(p(  2,   0), p( 6,  3)));
  }

  void Rec_Rec()
  {
    std::cout << "Iso_rectangle - Iso_rectangle\n";
    check_intersection     (Rec(p( 10,  12), p(30, 40)), Rec(p(  25,   40), p( 26,  103)), Rec(P(25, 40), P(26, 40)));
  }

  void T_Rec()
  {
    std::cout << "Triangle Iso_rectangle\n";
    check_no_intersection  (Rec(p( 10,  12), p(30, 40)), T(p(    4,   0), p(  12,   4), p(-4,  8)));
    check_intersection<T>(Rec(p( 0,  0), p(1, 1)), T(p(    -1,   0), p(  -1,   2), p(2,  2)));
    check_intersection<T>(Rec(p( 0,  0), p(1, 1)), T(p(    -1,   0), p(2,  2), p(  -1,   2)));
    check_intersection<Pol>(Rec(p( 0,  0), p(1, 1)), T(p(    -1,   -2), p(  -1,   2), p(5,  2)));
    check_intersection<T>(Rec(p( 0,  0), p(2, 2)), T(p(    0,   0), p(  1,   0), p(0,  1)));
    check_intersection<T>(Rec(p( 0,  0), p(3, 3)), T(p(    1,   1), p(  2,   1), p(1,  2)));
    check_intersection<P>(Rec(p( 0,  0), p(1, 1)), T(p(  -1,   0), p(    0,  0), p(0,  -1)));
    check_intersection<P>(Rec(p( 0,  0), p(1, 1)), T(p(    0,  0), p(  -1,   0), p(0,  -1)));
    check_intersection<Pol>(Rec(p( 100,  100), p(200, 200)), T(p(150, 50), p(250, 170), p(50, 170)));
  }
  
  void run()
  {
    std::cout << "2D Intersection tests\n";
    B_P();
    L_L();
    S_S();
    R_R();
    S_R();
    L_R();
    S_L();
    T_T();
    L_T();
    R_T();
    S_T();
    P_T();
    P_P();
    L_Rec();
    R_Rec();
    S_Rec();
    Rec_Rec();
    T_Rec();
  }

};

template<class A, class B>
void call_intersection(const A& a, const B& b) {
  typename CGAL::cpp11::result_of<CGAL::Simple_cartesian<double>::Intersect_2(A, B)>::type x = CGAL::intersection(a, b);
  typename CGAL::cpp11::result_of<CGAL::Simple_cartesian<double>::Intersect_2(A, B)>::type y = CGAL::intersection(b, a);
  typename CGAL::cpp11::result_of<CGAL::Simple_cartesian<double>::Intersect_2(B, A)>::type z = CGAL::intersection(b, a);
  CGAL_USE(x);
  CGAL_USE(y);
  CGAL_USE(z);
}

template<class A, class B>
void call_do_intersect(const A& a, const B& b) {
  CGAL::do_intersect(a, b);
  CGAL::do_intersect(b, a);
}

int main()
{
	Test< CGAL::Simple_cartesian<double>   >().run();
	Test< CGAL::Homogeneous<double> >().run();
        typedef typename CGAL::Simple_cartesian<double> K;
        typedef typename K::Point_2               P;
        typedef typename K::Line_2                L;
        typedef typename K::Segment_2             S;
        typedef typename K::Ray_2                 R;
        typedef typename K::Triangle_2            T;
        typedef typename K::Iso_rectangle_2       Rec;
        typedef typename K::Circle_2              C;

        try{
          call_intersection(P(),  P());
          call_intersection(P(),  L());
          call_intersection(P(),  S());
          call_intersection(P(),  R());
          call_intersection(P(),  T());
          call_intersection(P(),  Rec());

          call_intersection(L(), L());
          call_intersection(L(), S());
          call_intersection(L(), R());
          call_intersection(L(), T());
          call_intersection(L(), Rec());

          call_intersection(S(), S());
          call_intersection(S(), R());
          call_intersection(S(), T());
          call_intersection(S(), Rec());

          call_intersection(R(), R());
          call_intersection(R(), T());
          call_intersection(R(), Rec());

          call_intersection(T(), T());
          call_intersection(T(), Rec());

          call_intersection(Rec(), Rec());

          call_do_intersect(P(),  P());
          call_do_intersect(P(),  L());
          call_do_intersect(P(),  S());
          call_do_intersect(P(),  R());
          call_do_intersect(P(),  T());
          call_do_intersect(P(),  Rec());
          call_do_intersect(P(),  C());

          call_do_intersect(L(), L());
          call_do_intersect(L(), S());
          call_do_intersect(L(), R());
          call_do_intersect(L(), T());
          call_do_intersect(L(), Rec());
          call_do_intersect(L(),  C());

          call_do_intersect(S(), S());
          call_do_intersect(S(), R());
          call_do_intersect(S(), T());
          call_do_intersect(S(), Rec());
//          call_do_intersect(S(),  C());

          call_do_intersect(R(), R());
          call_do_intersect(R(), T());
          call_do_intersect(R(), Rec());
//          call_do_intersect(R(),  C());

          call_do_intersect(T(), T());
          call_do_intersect(T(), Rec());
//          call_do_intersect(T(),  C());

          call_do_intersect(Rec(), Rec());
          call_do_intersect(Rec(),  C());

          call_do_intersect(C(),  C());
        }
        catch(...){}
	// TODO : test more kernels.
}
