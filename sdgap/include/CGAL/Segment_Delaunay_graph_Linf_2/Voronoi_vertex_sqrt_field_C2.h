
#ifndef CGAL_SEGMENT_DELAUNAY_GRAPH_LINF_2_VORONOI_VERTEX_SQRT_FIELD_C2_H
#define CGAL_SEGMENT_DELAUNAY_GRAPH_LINF_2_VORONOI_VERTEX_SQRT_FIELD_C2_H



#include <CGAL/Segment_Delaunay_graph_Linf_2/Basic_predicates_C2.h>
#include <CGAL/Segment_Delaunay_graph_Linf_2/Are_same_points_C2.h>
#include <CGAL/Segment_Delaunay_graph_Linf_2/Are_same_segments_C2.h>
#include <CGAL/Segment_Delaunay_graph_Linf_2/Are_same_segments_C2.h>
#include <CGAL/Side_of_oriented_square_2.h>
#include <CGAL/Segment_Delaunay_graph_Linf_2/Bisector_Linf.h>

namespace CGAL {

namespace SegmentDelaunayGraphLinf_2 {


template<class K>
class Voronoi_vertex_sqrt_field_C2
  : public Basic_predicates_C2<K>
{
public:
  typedef Basic_predicates_C2<K> Base;

  typedef enum {PPP = 0, PPS, PSS, SSS} vertex_t;
  struct PPP_Type {};
  struct PPS_Type {};
  struct PSS_Type {};
  struct SSS_Type {};

  typedef typename Base::Point_2             Point_2;
  typedef typename Base::Segment_2           Segment_2;
  typedef typename Base::Line_2              Line_2;
  typedef typename Base::Site_2              Site_2;
  typedef typename Base::FT                  FT;
  typedef typename Base::RT                  RT;

  typedef typename Base::Homogeneous_point_2 Homogeneous_point_2;

  typedef typename Base::Orientation         Orientation;
  typedef typename Base::Comparison_result   Comparison_result;
  typedef typename Base::Oriented_side       Oriented_side;
  typedef typename Base::Sign                Sign;
  typedef typename Base::Compute_scalar_product_2 Compute_scalar_product_2;

  typedef typename Base::Polychainline_2     Polychainline_2;

private:
  typedef Are_same_points_C2<K>          Are_same_points_2;
  typedef Are_same_segments_C2<K>        Are_same_segments_2;
  typedef Side_of_oriented_square_C2<K>  Side_of_oriented_square_2_Type;
  typedef Bisector_Linf<K>               Bisector_Linf_Type;

  typedef typename K::Intersections_tag ITag;

  Are_same_points_2               same_points;
  Are_same_segments_2             same_segments;
  Side_of_oriented_square_2_Type  side_of_oriented_square;
  Bisector_Linf_Type              bisector_linf;


private:
  //--------------------------------------------------------------------------

  void
  compute_ppp(const Site_2& sp, const Site_2& sq, const Site_2& sr)
  {
    CGAL_precondition( sp.is_point() && sq.is_point() &&
		       sr.is_point() );

    Point_2 p = sp.point(), q = sq.point(), r = sr.point();

    FT x_min, x_max, y_min, y_max;
    FT x_center, y_center;
    FT half(0.5);
    FT two(2);

    bool is_set_x_center(false);
    bool is_set_y_center(false);
    bool is_set_x_max(false);
    bool is_set_y_max(false);
    bool is_set_x_min(false);
    bool is_set_y_min(false);

    Comparison_result cmpxqp = CGAL::compare(q.x(), p.x());

    if (cmpxqp == SMALLER) { // q.x() < p.x()
      x_min = q.x();
      x_max = p.x();
    } else if (cmpxqp == LARGER) { // q.x() > p.x()
      x_min = p.x();
      x_max = q.x();
    } else { // q.x() = p.x()
      x_min = p.x();
      x_max = p.x();
      y_center = half * (p.y() + q.y());
      is_set_y_center = true;

      CGAL_SDG_DEBUG(std::cout << "debug set y_center=" << 
        y_center << std::endl;);
      
      Comparison_result cmpxrothers = CGAL::compare(r.x(), p.x());
      if (cmpxrothers == SMALLER) {
        CGAL_SDG_DEBUG(std::cout << "debug r is left of p, q" << std::endl;);
        Comparison_result cmpyrp = CGAL::compare(r.y(), p.y());
        Comparison_result cmpyrq = CGAL::compare(r.y(), q.y());
        if (((cmpyrp == LARGER)  and (cmpyrq == LARGER)) or
            ((cmpyrp == SMALLER) and (cmpyrq == SMALLER))
           ) {
          // do fix
          if (cmpyrp == LARGER) {
            y_min = two*y_center - r.y();
            is_set_y_min = true;
            CGAL_SDG_DEBUG(std::cout << "debug set y_min=" << 
              y_min << std::endl;);
          } else {
            y_max = two*y_center - r.y();
            is_set_y_max = true;
            CGAL_SDG_DEBUG(std::cout << "debug set y_max=" << 
              y_max << std::endl;);
          }
        } 
      } else if (cmpxrothers == LARGER) {
        CGAL_SDG_DEBUG(std::cout << "debug r is right of p, q" << std::endl;);
        Comparison_result cmpyrp = CGAL::compare(r.y(), p.y());
        Comparison_result cmpyrq = CGAL::compare(r.y(), q.y());
        if (((cmpyrp == LARGER)  and (cmpyrq == LARGER)) or
            ((cmpyrp == SMALLER) and (cmpyrq == SMALLER))
           ) {
          // do fix
          if (cmpyrp == LARGER) {
            y_min = two*y_center - r.y();
            is_set_y_min = true;
            CGAL_SDG_DEBUG(std::cout << "debug set y_min=" << 
              y_min << std::endl;);
          } else {
            y_max = two*y_center - r.y();
            is_set_y_max = true;
            CGAL_SDG_DEBUG(std::cout << "debug set y_max=" << 
              y_max << std::endl;);
          }
        } 
      } else {
        // not possible
      }
    }

    Comparison_result cmpyqp = CGAL::compare(q.y(), p.y());

    if (cmpyqp == SMALLER) { // q.y() < p.y()
      if (not is_set_y_min) {
        y_min = q.y();
      }
      if (not is_set_y_max) {
        y_max = p.y();
      }
    } else if (cmpyqp == LARGER) { // q.y() > p.y()
      if (not is_set_y_min) {
        y_min = p.y();
      }
      if (not is_set_y_max) {
        y_max = q.y();
      }
    } else { //  q.y() = p.y()
      if (not is_set_y_min) {
        y_min = p.y();
      }
      if (not is_set_y_max) {
        y_max = p.y();
      }
      x_center = half * (p.x() + q.x());
      is_set_x_center = true;

      Comparison_result cmpyrothers = CGAL::compare(r.y(), p.y());
      if (cmpyrothers == SMALLER) {
        Comparison_result cmpxrp = CGAL::compare(r.x(), p.x());
        Comparison_result cmpxrq = CGAL::compare(r.x(), q.x());
        if (((cmpxrp == LARGER)  and (cmpxrq == LARGER)) or
            ((cmpxrp == SMALLER) and (cmpxrq == SMALLER))
           ) {
          // do fix
          if (cmpxrp == LARGER) {
            x_min = two*x_center - r.x();
            is_set_x_min = true;
          } else {
            x_max = two*x_center - r.x();
            is_set_x_max = true;
          }
        } 
      } else if (cmpyrothers == LARGER) {
        Comparison_result cmpxrp = CGAL::compare(r.x(), p.x());
        Comparison_result cmpxrq = CGAL::compare(r.x(), q.x());
        if (((cmpxrp == LARGER)  and (cmpxrq == LARGER)) or
            ((cmpxrp == SMALLER) and (cmpxrq == SMALLER))
           ) {
          // do fix
          if (cmpxrp == LARGER) {
            x_min = two*x_center - r.x();
            is_set_x_min = true;
          } else {
            x_max = two*x_center - r.x();
            is_set_x_max = true;
          }
        } 
      } else {
        // not possible
      }

    }

    Comparison_result cmpxrmin = CGAL::compare(r.x(), x_min);
    Comparison_result cmpxrmax = CGAL::compare(r.x(), x_max);
    if (cmpxrmin == SMALLER) { 
	// here r.x() < x_min <= x_max
        if (not is_set_x_min) {
          x_min = r.x();
        }
    } else if (cmpxrmin == LARGER) {
      // here r.x() > x_min
      if (cmpxrmax == LARGER) {
        // here x_min <= x_max < r.x()
        if (not is_set_x_max) {
          x_max = r.x();
        }
      } else if (cmpxrmax == SMALLER) {
        // x_min < r.x() < x_max
        // do nothing
      } else { // r.x() = x_max
        // r.x() = p.x() or r.x() = q.x()
        if (CGAL::compare(r.x(), p.x()) == EQUAL) {
          y_center = half * (p.y() + r.y());
          //Comparison_result cmpyqp = CGAL::compare(q.y(),p.y());
          Comparison_result cmpyqr = CGAL::compare(q.y(),r.y());
          if ((cmpyqp == LARGER) and (cmpyqr == LARGER)) {
            y_min = two*y_center - q.y();
            is_set_y_min = true;
          }
          if ((cmpyqp == SMALLER) and (cmpyqr == SMALLER)) {
            y_max = two*y_center - q.y();
            is_set_y_max = true;
          }
        } else {
          y_center = half * (q.y() + r.y());
          Comparison_result cmpypq = CGAL::compare(p.y(),q.y());
          Comparison_result cmpypr = CGAL::compare(p.y(),r.y());
          if ((cmpypq == LARGER) and (cmpypr == LARGER)) {
            y_min = two*y_center - p.y();
            is_set_y_min = true;
          }
          if ((cmpypq == SMALLER) and (cmpypr == SMALLER)) {
            y_max = two*y_center - p.y();
            is_set_y_max = true;
          }
        }
        is_set_y_center = true;
      }
    } else {
      // here r.x() = x_min
      // r.x() = p.x() or r.x() = q.x()
      if (CGAL::compare(r.x(), p.x()) == EQUAL) {
        CGAL_SDG_DEBUG(std::cout << "debug r.x = p.x" << std::endl;);
        // r.x() = p.x()
        y_center = half * (p.y() + r.y());
        //Comparison_result cmpyqp = CGAL::compare(q.y(),p.y());
        Comparison_result cmpyqr = CGAL::compare(q.y(),r.y());
        if ((cmpyqp == LARGER) and (cmpyqr == LARGER)) {
          CGAL_SDG_DEBUG(std::cout << "debug q is above p, r" << std::endl;);
          y_min = two*y_center - q.y();
          is_set_y_min = true;
        }
        if ((cmpyqp == SMALLER) and (cmpyqr == SMALLER)) {
          CGAL_SDG_DEBUG(std::cout << "debug q is below p, r" << std::endl;);
          y_max = two*y_center - q.y();
          is_set_y_max = true;
        }
      } else { 
        // r.x() = q.x()
        CGAL_SDG_DEBUG(std::cout << "debug r.x = q.x" << std::endl;);
        y_center = half * (q.y() + r.y());
        Comparison_result cmpypq = CGAL::compare(p.y(),q.y());
        Comparison_result cmpypr = CGAL::compare(p.y(),r.y());
        if ((cmpypq == LARGER) and (cmpypr == LARGER)) {
          CGAL_SDG_DEBUG(std::cout << "debug p is above q, r" << std::endl;);
          y_min = two*y_center - p.y();
          is_set_y_min = true;
        }
        if ((cmpypq == SMALLER) and (cmpypr == SMALLER)) {
          CGAL_SDG_DEBUG(std::cout << "debug p is below q, r" << std::endl;);
          y_max = two*y_center - p.y();
          is_set_y_max = true;
        }
      }
      is_set_y_center = true;
    }

    Comparison_result cmpyrmin = CGAL::compare(r.y(), y_min);
    Comparison_result cmpyrmax = CGAL::compare(r.y(), y_max);
    if (cmpyrmin == SMALLER) { 
      // here r.y() < y_min <= y_max
      if (not is_set_y_min) { 
        y_min = r.y();
      }
    } else if (cmpyrmin == LARGER) {
      // here r.y() > y_min
      if (cmpyrmax == LARGER) {
        // here y_min <= y_max < r.y()
        if (not is_set_y_max) { 
          y_max = r.y();
        }
      } else if (cmpyrmax == SMALLER) {
        // y_min < r.y() < y_max
        // do nothing
      } else { // r.y() = y_max
        // r.y() = p.y() or r.y() = q.y()
        if (CGAL::compare(r.y(), p.y()) == EQUAL) {
          x_center = half * (p.x() + r.x());
          //Comparison_result cmpxqp = CGAL::compare(q.x(),p.x());
          Comparison_result cmpxqr = CGAL::compare(q.x(),r.x());
          if ((cmpxqp == LARGER) and (cmpxqr == LARGER)) {
            x_min = two*x_center - q.x();
            is_set_x_min = true;
          }
          if ((cmpxqp == SMALLER) and (cmpxqr == SMALLER)) {
            x_max = two*x_center - q.x();
            is_set_x_max = true;
          }
        } else {
          x_center = half * (q.x() + r.x());
          Comparison_result cmpxpq = CGAL::compare(p.x(),q.x());
          Comparison_result cmpxpr = CGAL::compare(p.x(),r.x());
          if ((cmpxpq == LARGER) and (cmpxpr == LARGER)) {
            x_min = two*x_center - p.x();
            is_set_x_min = true;
          }
          if ((cmpxpq == SMALLER) and (cmpxpr == SMALLER)) {
            x_max = two*x_center - p.x();
            is_set_x_max = true;
          }
        }
        is_set_x_center = true;
      }
    } else {
      // here r.y() = y_min
      // r.y() = p.y() or r.y() = q.y()
      if (CGAL::compare(r.y(), p.y()) == EQUAL) {
        x_center = half * (p.x() + r.x());
        //Comparison_result cmpxqp = CGAL::compare(q.x(),p.x());
        Comparison_result cmpxqr = CGAL::compare(q.x(),r.x());
        if ((cmpxqp == LARGER) and (cmpxqr == LARGER)) {
          x_min = two*x_center - q.x();
          is_set_x_min = true;
        }
        if ((cmpxqp == SMALLER) and (cmpxqr == SMALLER)) {
          x_max = two*x_center - q.x();
          is_set_x_max = true;
        }
      } else {
        x_center = half * (q.x() + r.x());
        Comparison_result cmpxpq = CGAL::compare(p.x(),q.x());
        Comparison_result cmpxpr = CGAL::compare(p.x(),r.x());
        if ((cmpxpq == LARGER) and (cmpxpr == LARGER)) {
          x_min = two*x_center - p.x();
          is_set_x_min = true;
        }
        if ((cmpxpq == SMALLER) and (cmpxpr == SMALLER)) {
          x_max = two*x_center - p.x();
          is_set_x_max = true;
        }
      }
      is_set_x_center = true;
    }

    Comparison_result cmpsides = 
	CGAL::compare(x_max - x_min, y_max - y_min);

    // if bounding box is non-square and points are not 
    // on corners of it, then grow it to become square
    switch(cmpsides) {
      case SMALLER:
        CGAL_SDG_DEBUG(std::cout << "rectangle has to be made fatter" << std::endl;);
        // make rectangle fatter
        if (is_set_x_center) {
          CGAL_SDG_DEBUG(std::cout << "x_center already set" << std::endl;);
          // grow in both sides
          break;
        }
        // grow only if any point is inside vertical sides
	if (((CGAL::compare(p.x(), x_min) == EQUAL)   and
	     (CGAL::compare(p.y(), y_max) == SMALLER) and
	     (CGAL::compare(p.y(), y_min) == LARGER)     ) or 
	    ((CGAL::compare(q.x(), x_min) == EQUAL)   and
	     (CGAL::compare(q.y(), y_max) == SMALLER) and
	     (CGAL::compare(q.y(), y_min) == LARGER)     ) or 
	    ((CGAL::compare(r.x(), x_min) == EQUAL)   and
	     (CGAL::compare(r.y(), y_max) == SMALLER) and
	     (CGAL::compare(r.y(), y_min) == LARGER)     )   )
        { // grow rectangle to the right
          CGAL_SDG_DEBUG(std::cout << "debug vsqrnew grow right" << std::endl;);
          x_max = x_min + y_max - y_min;
        } else 
        { // grow rectangle to the left
          CGAL_SDG_DEBUG(std::cout << "debug vsqrnew grow left" << std::endl;);
          x_min = x_max - y_max + y_min;
        }
        break;
      case LARGER:
        CGAL_SDG_DEBUG(std::cout << "rectangle has to be made taller" << std::endl;);
        // make rectangle taller
        if (is_set_y_center) {
          // grow in both sides
          CGAL_SDG_DEBUG(std::cout << "y_center already set" << std::endl;);
          break;
        }
        // grow only if any point is inside horizontal sides
	if (((CGAL::compare(p.y(), y_min) == EQUAL)   and
	     (CGAL::compare(p.x(), x_max) == SMALLER) and
	     (CGAL::compare(p.x(), x_min) == LARGER)     ) or 
	    ((CGAL::compare(q.y(), y_min) == EQUAL)   and
	     (CGAL::compare(q.x(), x_max) == SMALLER) and
	     (CGAL::compare(q.x(), x_min) == LARGER)     ) or 
	    ((CGAL::compare(r.y(), y_min) == EQUAL)   and
	     (CGAL::compare(r.x(), x_max) == SMALLER) and
	     (CGAL::compare(r.x(), x_min) == LARGER)     )   )
        { // grow rectangle upwards
          CGAL_SDG_DEBUG(std::cout << "debug vsqrnew grow upwards" << std::endl;);
          y_max = y_min + x_max - x_min;
        } else 
        { // grow rectangle downwards
          CGAL_SDG_DEBUG(std::cout << "debug vsqrnew grow downwards" << std::endl;);
          y_min = y_max - x_max + x_min;
        }
        break;
      case EQUAL:
        // do nothing
        break;
    }

    ux = x_min + x_max;
    uy = y_min + y_max;
    uz = FT(2) ;
  }

  //--------------------------------------------------------------------------

  void
  compute_pss(const Site_2& p, const Site_2& q, const Site_2& r)
  {
#ifdef CGAL_PROFILE
    // In case CGAL profile is called then output the sites in case of
    // a filter failure
    if ( Algebraic_structure_traits<FT>::Is_exact::value ) {
      std::ofstream ofs("vv-failure-log.cin", std::ios_base::app);
      ofs.precision(16);
      ofs << p << std::endl;
      ofs << q << std::endl;
      ofs << r << std::endl;
      ofs << "=======" << std::endl;
      ofs.close();
    }
#endif

    CGAL_precondition( p.is_point() && q.is_segment() &&
		       r.is_segment() );

    bool pq =
      same_points(p, q.source_site()) || same_points(p, q.target_site());
    bool pr =
      same_points(p, r.source_site()) || same_points(p, r.target_site());

    Point_2 pp = p.point();

    if ( pq && pr ) {
      ux = pp.x();
      uy = pp.y();
      uz = FT(1);
      return;
    }
  //sandeep: We will find the vertex by comparisons and simple arithmatic
  
  Point_2 qsrc = q.source_site().point();
  Point_2 rsrc = r.source_site().point();
  
  Segment_2 qseg = q.segment();
  Segment_2 rseg = r.segment();
  //case1:when q and r are both horizontal
  if (qseg.is_horizontal() and rseg.is_horizontal()) {
    //if (CGAL::compare(q.source_site().y(),r.source_site().y()) == LARGER)
    ux = FT(2) * pp.x() - qsrc.y() + rsrc.y();
    uy = qsrc.y() + rsrc.y();
    uz = FT(2);
    return;
  }
  //case2:when q and r are both vertical
  if (qseg.is_vertical() and rseg.is_vertical()) {
    ux = qsrc.x() + rsrc.x();
    uy = FT(2) * pp.y() + qsrc.x() - rsrc.x();
    uz = FT(2);
    return;
  }
  //case3:when q is horizontal and r is vertical
  if (qseg.is_horizontal() and rseg.is_vertical()) {
    if (CGAL::compare( abs(pp.x() - rsrc.x()),
                      abs(pp.y() - qsrc.y()) )
        == SMALLER ) {
      //diff x is smaller than diff y
      ux = FT(2) * rsrc.x() + qsrc.y() - pp.y();
      uy = qsrc.y() + pp.y();
      uz = FT(2);
    } else {//diff x is larger or equal to diff y
      ux = pp.x() + rsrc.x();
      uy = FT(2) * qsrc.y() + rsrc.x() - pp.x();
      uz = FT(2);
    }
    return;
  }
  //case4:when q is vertical and r is horizontal
  if (CGAL::compare( abs(pp.x() - qsrc.x()),
                    abs(pp.y() - rsrc.y()) )
      == SMALLER ) {
    //diff x is smaller than diff y
    ux = FT(2) * qsrc.x() - rsrc.y() + pp.y();
    uy = rsrc.y() + pp.y();
    uz = FT(2);
  } else {//diff x is larger or equal to diff y
    ux = pp.x() + qsrc.x();
    uy = FT(2) * rsrc.y() - qsrc.x() + pp.x();
    uz = FT(2);
  }
  
  return;


  }


  //--------------------------------------------------------------------------


  void
  compute_pps(const Site_2& p, const Site_2& q, const Site_2& r)
  {
  CGAL_precondition( p.is_point() && q.is_point() &&
                    r.is_segment() );
  
  CGAL_SDG_DEBUG(std::cout << "debug: compute_pps entering p=" << p
                 << " q=" << q << " r=" << r << std::endl;);
  
  v_type = PPS;
  
  bool p_endp_r = is_endpoint_of(p, r);
  bool q_endp_r = is_endpoint_of(q, r);
  //incase p or q is end point of r no square is possible in ccw sense
  CGAL_assertion(not (p_endp_r or q_endp_r));
  
  bool samexpq = (scmpx(p, q) == EQUAL);
  bool sameypq = (scmpy(p, q) == EQUAL);
  
  bool samecoordpq = samexpq or sameypq ;
  
  Point_2 pp = p.point(), qp = q.point();
  Segment_2 rs = r.segment();
  //Sandeep: We divide the computation in two cases
  //case 1: r is horizontal
  //Case 2: r is vertical
  if (rs.is_horizontal()) {
    if( CGAL::compare( abs(pp.x() - qp.x()),
                       abs(pp.y() - rs.source().y()) )
                       == LARGER &&
       CGAL::compare( abs(pp.x() - qp.x()),
                      abs(qp.y() - rs.source().y()) )
                      == LARGER ) {
      // This is the case when diffx is greater than diffy
      
      ux = pp.x() + qp.x();
      uy = FT(2) * rs.source().y() + pp.x() - qp.x();
      uz = FT(2);
    } else {
      // diffy is greater
      if (sameypq) {
        ux = pp.x() + qp.x();
        uy = pp.y() + rs.source().y();
        uz = FT(2);
      } else if ( CGAL::compare( abs(pp.y() - rs.source().y()),
                                 abs(qp.y() - rs.source().y()) )
                                 == LARGER) {
        // p is more distant than q from r
        if (samexpq) {
          uy = pp.y() + qp.y();
          uz = FT(2);
          ux = (CGAL::compare(pp.y(),rs.source().y()) == LARGER) ?
          (FT(2) * pp.x() + (pp.y() + qp.y() - FT(2) * rs.source().y())) :
          (FT(2) * pp.x() - (FT(2) * rs.source().y() - pp.y() - qp.y()));
        } else {
          ux = FT(2) * qp.x() + pp.y() - rs.source().y();
          uy = pp.y() + rs.source().y();
          uz = FT(2);
        }
      } else {
        // q is more distant than p from r
        if (samexpq) {
          uy = pp.y() + qp.y();
          uz = FT(2);
          ux = (CGAL::compare(pp.y(),rs.source().y()) == LARGER) ?
          (FT(2) * pp.x() - (pp.y() + qp.y() - FT(2) * rs.source().y())) :
          (FT(2) * pp.x() + (FT(2) * rs.source().y() - pp.y() - qp.y()));
        } else {
          ux = FT(2) * pp.x() - qp.y() + rs.source().y();
          uy = qp.y() + rs.source().y();
          uz = FT(2);
        }
      }
    }// end of diffy greater
  } else {//rs is vertical
    if( CGAL::compare( abs(pp.y() - qp.y()),
                       abs(pp.x() - rs.source().x()) )
                       == LARGER &&
       CGAL::compare( abs(pp.y() - qp.y()),
                      abs(qp.x() - rs.source().x()) )
                      == LARGER ) {
      // This is the case when diffy is greater than diffx
      
      ux = FT(2) * rs.source().x() + pp.y() - qp.y();
      uy = pp.y() + qp.y();
      uz = FT(2);
    } else {
      // diffx is greater
      if (samexpq) {
        ux = pp.x() + rs.source().x();
        uy = pp.y() + qp.y();
        uz = FT(2);
      } else if ( CGAL::compare( abs(pp.x() - rs.source().x()),
                                 abs(qp.x() - rs.source().x()) )
                                 == LARGER) {
        
        // p is more distant than q from r
        if (sameypq) {
          ux = pp.y() + qp.y();
          uz = FT(2);
          uy = (CGAL::compare(pp.x(),rs.source().x()) == LARGER) ?
          (FT(2) * pp.y() - (pp.x() + qp.x() - FT(2) * rs.source().x())) :
          (FT(2) * pp.y() + (FT(2) * rs.source().x() - pp.x() - qp.x()));
        } else {
          ux = pp.x() + rs.source().x();
          uy = FT(2) * qp.y() + pp.x() - rs.source().x();
          uz = FT(2);
        }
      } else {
        // q is more distant than p from r
        if (sameypq) {
          ux = pp.y() + qp.y();
          uz = FT(2);
          uy = (CGAL::compare(pp.x(),rs.source().x()) == LARGER) ?
          (FT(2) * pp.y() + (pp.x() + qp.x() - FT(2) * rs.source().x())) :
          (FT(2) * pp.y() - (FT(2) * rs.source().x() - pp.x() - qp.x()));
        } else {
          ux = qp.x() + rs.source().x();
          uy = FT(2) * pp.y() - qp.x() + rs.source().x();
          uz = FT(2);
        }
      }
    }// end of diffx greater
  }//end of vertical rs
  return;
  }


  //--------------------------------------------------------------------------
  bool check_if_exact(const Site_2& , unsigned int ,
		      const Tag_false&) const
  {
    return true;
  }

  bool check_if_exact(const Site_2& s, unsigned int i,
		      const Tag_true&) const
  {
    return s.is_input(i);
  }

  // determines of the segment s is on the positive halfspace as
  // defined by the supporting line of the segment supp; the line l
  // is supposed to be the supporting line of the segment supp and we
  // pass it so that we do not have to recompute it
  bool
  is_on_positive_halfspace(const Site_2& supp,
			   const Site_2& s, const Line_2& l) const
  {
    CGAL_precondition( supp.is_segment() && s.is_segment() );

    if ( same_segments(supp.supporting_site(),
		       s.supporting_site()) ) {
      return false;
    }

    if ( same_points(supp.source_site(), s.source_site()) ||
	 same_points(supp.target_site(), s.source_site()) ) {
      return oriented_side_of_line(l, s.target()) == ON_POSITIVE_SIDE;
    }

    if ( same_points(supp.source_site(), s.target_site()) ||
	 same_points(supp.target_site(), s.target_site()) ) {
      return oriented_side_of_line(l, s.source()) == ON_POSITIVE_SIDE;
    }

    ITag itag;

    if ( !check_if_exact(s, 0, itag) &&
	 same_segments(supp.supporting_site(),
		       s.crossing_site(0)) ) {
      return oriented_side_of_line(l, s.target()) == ON_POSITIVE_SIDE;
    }

    if ( !check_if_exact(s, 1, itag) &&
	 same_segments(supp.supporting_site(),
		       s.crossing_site(1)) ) {
      return oriented_side_of_line(l, s.source()) == ON_POSITIVE_SIDE;
    }

    return Base::is_on_positive_halfspace(l, s.segment());
  }

  //--------------------------------------------------------------------------

  void
  compute_sss(const Site_2& p, const Site_2& q, const Site_2& r)
  {
  CGAL_precondition( p.is_segment() && q.is_segment() &&
                    r.is_segment() );
  
  int numh=0, numv=0;
  Segment_2 ps = p.segment(), qs = q.segment(), rs = r.segment();
  
  (ps.is_horizontal()) ? numh++ : numv++;
  (qs.is_horizontal()) ? numh++ : numv++;
  (rs.is_horizontal()) ? numh++ : numv++;
  
  CGAL_assertion(numh == 2 or numv == 2);
  
  if (numv == 2) { // two vertical and one horizontal segment
    if (ps.is_horizontal()) {
      ux = qs.source().x() + rs.source().x();
      uy = FT(2) * ps.source().y() + qs.source().x() - rs.source().x();
      uz = FT(2);
    }
    if (qs.is_horizontal()) {
      ux = ps.source().x() + rs.source().x();
      uy = FT(2) * qs.source().y() + rs.source().x() - ps.source().x();
      uz = FT(2);
    }
    if (rs.is_horizontal()) {
      ux = qs.source().x() + ps.source().x();
      uy = FT(2) * rs.source().y() + ps.source().x() - qs.source().x();
      uz = FT(2);
    }
    
  } else { // two horizontal and one vertical segment
    if (ps.is_vertical()) {
      uy = qs.source().y() + rs.source().y();
      ux = FT(2) * ps.source().x() + rs.source().y() - qs.source().y();
      uz = FT(2);
    }
    if (qs.is_vertical()) {
      uy = ps.source().y() + rs.source().y();
      ux = FT(2) * qs.source().x() + ps.source().y() - rs.source().y();
      uz = FT(2);
    }
    if (rs.is_vertical()) {
      uy = qs.source().y() + ps.source().y();
      ux = FT(2) * rs.source().x() + qs.source().y() - ps.source().y();
      uz = FT(2);
    }
    
  }//end of two horizontal and one vertical segment case
  

  


    

    return;

  }

  //--------------------------------------------------------------------------

  void
  compute_vertex(const Site_2& s1, const Site_2& s2, const Site_2& s3)
  {
    int npts = 0;
    if ( s1.is_point() ) ++npts;
    if ( s2.is_point() ) ++npts;
    if ( s3.is_point() ) ++npts;

    switch ( npts ) {
    case 0:
      v_type = SSS;
      break;
    case 1:
      v_type = PSS;
      break;
    case 2:
      v_type = PPS;
      break;
    default:
      v_type = PPP;
    }


    if ( v_type == PPP ) {
      compute_ppp(s1, s2, s3);
    } else if ( v_type == SSS ) {
      compute_sss(s1, s2, s3);
    } else if ( v_type == PPS ) {
      if ( s1.is_segment() ) {
	compute_pps(s2, s3, s1);
	pps_idx = 1;
      } else if ( s2.is_segment() ) {
	compute_pps(s3, s1, s2);
	pps_idx = 2;
      } else {
	compute_pps(s1, s2, s3);
	pps_idx = 0;
      }
    } else {
      if ( s1.is_point() ) {
	compute_pss(s1, s2, s3);
      } else if ( s2.is_point() ) {
	compute_pss(s2, s3, s1);
      } else {
	compute_pss(s3, s1, s2);
      }
    }
  }

  //--------------------------------------------------------------------------

  bool is_endpoint_of(const Site_2& p, const Site_2& s) const
  {
    CGAL_precondition( p.is_point() && s.is_segment() );
   return ( same_points(p, s.source_site()) ||
	    same_points(p, s.target_site()) );
  }
  

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  //                              the incircle test
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  //  the incircle test when the fourth site is a point
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------

  Sign
  check_easy_degeneracies(const Site_2& t, PPS_Type,
			  bool& use_result) const
  {
    CGAL_precondition( t.is_point() );

    use_result = false;
    if (  ( p_.is_point() && same_points(p_, t) ) ||
	  ( q_.is_point() && same_points(q_, t) ) ||
	  ( r_.is_point() && same_points(r_, t) )  ) {
      use_result = true;
      return ZERO;
    }

    // philaris: following might have to be changed
    // philaris: I remove the following line to be on the safe side
    /*
    if (  ( p_.is_segment() && is_endpoint_of(t, p_) ) || 
	  ( q_.is_segment() && is_endpoint_of(t, q_) ) ||
	  ( r_.is_segment() && is_endpoint_of(t, r_) )  ) {
      use_result = true;
      return POSITIVE;
    }
    */

    return ZERO;
  }

  Sign
  check_easy_degeneracies(const Site_2& t, PSS_Type,
			  bool& use_result) const
  {
    CGAL_precondition( t.is_point() );

    return check_easy_degeneracies(t, PPS_Type(), use_result);
  }

  Sign
  check_easy_degeneracies(const Site_2& t, SSS_Type,
			  bool& use_result) const
  {
    CGAL_precondition( t.is_point() );

    use_result = false;

    // ADD THE CASES WHERE t IS AN ENDPOINT OF ONE OF THE SEGMENTS
    return ZERO;
  }

  //--------------------------------------------------------------------------

  Sign incircle_p(const Site_2& st, PPP_Type) const
  {
    CGAL_precondition( st.is_point() );
    
    Point_2 t = st.point();

    Oriented_side os =
      side_of_oriented_square(p_.point(), q_.point(), r_.point(), t);
    if ( os == ON_POSITIVE_SIDE ) { return NEGATIVE; }
    if ( os == ON_NEGATIVE_SIDE ) { return POSITIVE; }
    return ZERO;
  }

  //--------------------------------------------------------------------------

  template<class Type>
  inline
  Sign incircle_p(const Site_2& st, Type type) const
  {
    CGAL_precondition( st.is_point() );

    bool use_result(false);
    Sign s = check_easy_degeneracies(st, type, use_result);
    if ( use_result ) { return s; }

    return incircle_p_no_easy(st, type);
  }

  template<class Type>
  Sign incircle_p_no_easy(const Site_2& st, Type) const
  {
    CGAL_precondition( st.is_point() );

    FT rad = radius();

    Point_2 t = st.point();

    FT d = CGAL::max( CGAL::abs(x() - t.x()),
	              CGAL::abs(y() - t.y()) );

    return CGAL::compare(d, rad);
  }

  //--------------------------------------------------------------------------

  Sign incircle_p(const Site_2& t) const 
  {
    if ( is_degenerate_Voronoi_circle() ) {
      return POSITIVE;
    }

    Sign s(ZERO);
    switch ( v_type ) {
    case PPP:
      s = incircle_p(t, PPP_Type());
      break;
    case PPS:
      s = incircle_p(t, PPS_Type());
      break;
    case PSS:
      s = incircle_p(t, PSS_Type());
      break;
    case SSS:
      s = incircle_p(t, SSS_Type());
      break;
    }

    return s;
  }

  Sign incircle_p_no_easy(const Site_2& t) const 
  {
    Sign s(ZERO);
    switch ( v_type ) {
    case PPP:
      s = incircle_p(t, PPP_Type());
      break;
    case PPS:
      s = incircle_p_no_easy(t, PPS_Type());
      break;
    case PSS:
      s = incircle_p_no_easy(t, PSS_Type());
      break;
    case SSS:
      s = incircle_p_no_easy(t, SSS_Type());
      break;
    }

    return s;
  }

  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  //  the incircle test when the fourth site is a segment
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------


  Oriented_side
  oriented_side(const Line_2& l, const Point_2& p) const
  {
    Line_2 l1(l.b(), -l.a(), l.a() * y() - l.b() * x());

    return oriented_side_of_line(l1, p);
  }


  Sign incircle(const Line_2& l) const
  {
    FT rad = radius();

    // compute Linf distance of vv from l
    Homogeneous_point_2 pref = compute_linf_projection_hom(l, vv);
    FT dx = CGAL::abs(vv.x() - pref.x());
    FT dy = CGAL::abs(vv.y() - pref.y());
    FT dpref = CGAL::max(dx, dy);

    return CGAL::compare(dpref, rad);
  }


  Site_2 other_site(const Site_2& p, const Site_2& seg) const
  {
    if (same_points(p, seg.source_site())){
      return  seg.target_site();
    }
    return  seg.source_site();
  }


  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  Sign incircle_s(const Site_2& t, int) const
  {
    CGAL_precondition( t.is_segment() );

    // philaris: in L2 a chord of a circle is inside it,
    // philaris: but in Linf maybe it is on the boundary
    // philaris: therefore, I removed the following code
#if 0
    if ( v_type == PPP || v_type == PPS ) {
      if (  p_.is_point() && q_.is_point() &&
	    is_endpoint_of(p_, t) && is_endpoint_of(q_, t)  ) {
	return NEGATIVE;
      }

      if (  p_.is_point() && r_.is_point() &&
	    is_endpoint_of(p_, t) && is_endpoint_of(r_, t)  ){
	return NEGATIVE;
      }

      if (  q_.is_point() && r_.is_point() &&
	    is_endpoint_of(q_, t) && is_endpoint_of(r_, t)  ){
	return NEGATIVE;
      }
    }
#endif

    // philaris: removed the following code too
#if 0
#ifndef CGAL_DISABLE_AM_CODE
    // code added by Andreas + Monique -- start
    if(v_type == PPP){
      Site_2 const *p1 = NULL;
      if(is_endpoint_of(p_, t)){
	p1 = &p_;
      } else if(is_endpoint_of(q_, t)){
	p1 = &q_;
      } else if(is_endpoint_of(r_, t)){
	p1 = &r_;
      }
      if(p1 != NULL){
        // As the Voronoi circle and the segment t touch in p1,
        // it is enough to check that the center and 
        // the non-touching point of the segment
        // are not in the same halfspace defined by the tangent line through p1
	Site_2 p2 = other_site(*p1, t);
	  Point_2 v(x(),y());
        
	  Compute_scalar_product_2 csp;
	  return -CGAL::sign( csp((v - p1->point()), (p2.point()- p1->point())) );
      }

    } else if(v_type == PPS){
      Site_2 const *p1, *p2, *seg;
      if(p_.is_point()){ 
	p1 = &p_;
	if(q_.is_point()){
	  p2 = &q_;
	  seg = &r_;
	} else {
	  p2 = &r_;
	  seg = &q_;
	} 
      } else {
	seg = &p_;
	p1 = &q_;
	p2 = &r_;
      }

      if(! is_endpoint_of(*p2, t)){
	std::swap(p1,p2);
      }
	if(is_endpoint_of(*p2, t)){
	  Site_2 tp = other_site(*p2, t);
	  Point_2 v(x(),y());
	  Compute_scalar_product_2 csp;
          return -CGAL::sign( csp((v - p2->point()), (tp.point()- p2->point())) );
	}
    }
    // code added by Andreas + Monique -- end
#endif // CGAL_DISABLE_AM_CODE
#endif

    // philaris: removed the following
    // philaris: however, it might be possible to adapt
    // phiaris: by using some notion of tangent
#if 0
#ifndef CGAL_DISABLE_M_CODE
    // code added by Menelaos -- begin
    // in the code that follows we check whether one endpoint of the
    // query segment t is the same as the point p of a PSS circle. in
    // this case the result is known by taking the other point of t
    // and checking against the tangent to the Voronoi circle at p.
    if ( v_type == PSS ) {
      const Site_2* pt;
      if ( p_.is_point() ) { pt = &p_; }
      else if ( q_.is_point() ) { pt = &q_; }
      else { pt = &r_; }

      if ( is_endpoint_of(*pt, t) ) {
	Site_2 tp = other_site(*pt, t);
	Point_2 v(x(), y());
	Compute_scalar_product_2 csp;
	return
	  -CGAL::sign( csp(v - pt->point(), tp.point()- pt->point()) );
      }
    }
    // code added by Menelaos -- end
#endif // CGAL_DISABLE_M_CODE
#endif 


    // philaris: removed the following
#if 0
    if ( v_type == PSS ) {
      if ( p_.is_segment() &&
	   same_segments(p_.supporting_site(),
			 t.supporting_site()) ) {
	return POSITIVE;
      }
      if ( q_.is_segment() &&
	   same_segments(q_.supporting_site(),
			 t.supporting_site()) ) {
	return POSITIVE;
      }
      if ( r_.is_segment() &&
	   same_segments(r_.supporting_site(),
			 t.supporting_site()) ) {
	return POSITIVE;
      }
    }
#endif

    return incircle_s_no_easy(t, 0);
  }

  
  Sign incircle_s_no_easy(const Site_2& t, int) const
  {
    Sign d1, d2;
    if (  ( p_.is_point() && same_points(p_, t.source_site()) ) ||
	  ( q_.is_point() && same_points(q_, t.source_site()) ) ||
	  ( r_.is_point() && same_points(r_, t.source_site()) )  ) {
      d1 = ZERO;
    } else {
      d1 = incircle_p(t.source_site());
    }
    //if ( d1 == NEGATIVE ) { return NEGATIVE; }
    if ( certainly(d1 == NEGATIVE) ) { return NEGATIVE; }
    if (! is_certain(d1 == NEGATIVE) ) { return indeterminate<Sign>(); }

    if (  ( p_.is_point() && same_points(p_, t.target_site()) ) ||
	  ( q_.is_point() && same_points(q_, t.target_site()) ) ||
	  ( r_.is_point() && same_points(r_, t.target_site()) )  ) {
      d2 = ZERO;
    } else {
      d2 = incircle_p(t.target_site());
    }
    //if ( d2 == NEGATIVE ) { return NEGATIVE; }
    if (certainly( d2 == NEGATIVE ) ) { return NEGATIVE; }
    if (! is_certain( d2 == NEGATIVE ) ) { return indeterminate<Sign>(); }

    //Line_2 l = compute_supporting_line(t.supporting_site());
    //Sign sl = incircle(l);

    Segment_2 tseg = t.segment();
  
    FT dup;
    if (p_.is_point()) {
      Point_2 pp = p_.point();
      dup = CGAL::max(CGAL::abs(ux - pp.x() * uz),
                      CGAL::abs(uy - pp.y() * uz));
    } else {//p_ is segment
      Segment_2 pseg = p_.segment();
      if (pseg.is_horizontal()) {
        dup = CGAL::abs(uy - pseg.source().y() * uz);
      } else {//p_ is vertical
        dup = CGAL::abs(ux - pseg.source().x() * uz);
      }
    }
  
    if (tseg.is_horizontal()) {
      Point_2 txmax = ( CGAL::compare(tseg.source().x(), tseg.target().x())
                        == SMALLER ) ? tseg.target()
                                     : tseg.source();
      Point_2 txmin = ( CGAL::compare(tseg.source().x(), tseg.target().x())
                        == SMALLER ) ? tseg.source()
                                     : tseg.target();
      if (CGAL::compare(uy - dup , uz * tseg.source().y()) == SMALLER &&
          CGAL::compare(uz * tseg.source().y() , uy + dup) == SMALLER &&
          CGAL::compare(ux - dup , uz * txmax.x()) == SMALLER ) {
        return NEGATIVE;
      } else if (CGAL::compare(uy - dup , uz * tseg.source().y()) == LARGER or
                 CGAL::compare(uz * tseg.source().y() , uy + dup) == LARGER or
                 CGAL::compare(ux - dup , uz * txmax.x()) == LARGER or
                 CGAL::compare(ux + dup , uz * txmin.x()) == SMALLER) {
        return POSITIVE;
      } else {
        return ZERO;
      }
    
    } else {// tseg is vertical
      Point_2 tymax = ( CGAL::compare(tseg.source().y(), tseg.target().y())
                        == SMALLER ) ? tseg.target()
                                     : tseg.source();
      Point_2 tymin = ( CGAL::compare(tseg.source().y(), tseg.target().y())
                        == SMALLER ) ? tseg.source()
                                     : tseg.target();
      if (CGAL::compare(ux - dup , uz * tseg.source().x()) == SMALLER &&
          CGAL::compare(uz * tseg.source().x() , ux + dup) == SMALLER &&
          CGAL::compare(uy - dup , uz * tymax.y()) == SMALLER ) {
        return NEGATIVE;
      } else if (CGAL::compare(ux - dup , uz * tseg.source().x()) == LARGER or
                 CGAL::compare(uz * tseg.source().x() , ux + dup) == LARGER or
                 CGAL::compare(uy - dup , uz * tymax.y()) == LARGER or
                 CGAL::compare(uy + dup , uz * tymin.y()) == SMALLER) {
        return POSITIVE;
      } else {
        return ZERO;
      }
    
    }//end of tseg vertical

  }

  //--------------------------------------------------------------------------

  Sign incircle_s(const Site_2& t) const
  {
    CGAL_precondition( t.is_segment() );

    if ( is_degenerate_Voronoi_circle() ) {
      // case 1: the new segment is not adjacent to the center of the
      //         degenerate Voronoi circle
      if (  !same_points( p_ref(), t.source_site() ) &&
	    !same_points( p_ref(), t.target_site() )  ) {
	return POSITIVE;
      }

      CGAL_assertion( v_type == PSS );

      if ( p_.is_segment() &&
	   same_segments(p_.supporting_site(),
			 t.supporting_site()) ) {
	return ZERO;
      }

      if ( q_.is_segment() &&
	   same_segments(q_.supporting_site(),
			 t.supporting_site()) ) {
	return ZERO;
      }

      if ( r_.is_segment() &&
	   same_segments(r_.supporting_site(),
			 t.supporting_site()) ) {
	return ZERO;
      }

      Site_2 pr;
      Site_2 sp, sq;
      if ( p_.is_point() ) {
	CGAL_assertion( q_.is_segment() && r_.is_segment() );
	pr = p_;
	sp = q_;
	sq = r_;
      } else if ( q_.is_point() ) {
	CGAL_assertion( r_.is_segment() && p_.is_segment() );
	pr = q_;
	sp = r_;
	sq = p_;
      } else {
	CGAL_assertion( p_.is_segment() && q_.is_segment() );
	pr = r_;
	sp = p_;
	sq = q_;
      }

      Point_2 pq = sq.source(), pp = sp.source(), pt = t.source();

      if ( same_points(sp.source_site(), pr) ) { pp = sp.target(); }
      if ( same_points(sq.source_site(), pr) ) { pq = sq.target(); }
      if ( same_points( t.source_site(), pr) ) { pt =  t.target(); }

      Point_2 pr_ = pr.point();

      if ( CGAL::orientation(pr_, pp, pt) == LEFT_TURN &&
	   CGAL::orientation(pr_, pq, pt) == RIGHT_TURN ) {
	return NEGATIVE;
      }
      return ZERO;
    } // if ( is_degenerate_Voronoi_circle() )

    Sign s = incircle_s(t, 0);

    return s;
  }

  inline
  Sign incircle_s_no_easy(const Site_2& t) const
  {
    return incircle_s_no_easy(t, 0);
  }

  //--------------------------------------------------------------------------
  //  subpredicates for the incircle test
  //--------------------------------------------------------------------------


public:
  bool is_degenerate_Voronoi_circle() const
  {
    if ( v_type != PSS ) { return false; }

    if ( p_.is_point() ) {
      return ( is_endpoint_of(p_, q_) && is_endpoint_of(p_, r_) );
    } else if ( q_.is_point() ) {
      return ( is_endpoint_of(q_, p_) && is_endpoint_of(q_, r_) );
    } else {
      CGAL_assertion( r_.is_point() );
      return ( is_endpoint_of(r_, p_) && is_endpoint_of(r_, q_) );
    }
  }


  //--------------------------------------------------------------------------

private:

  //--------------------------------------------------------------------------
  //  the reference point (valid if v_type != SSS)
  //--------------------------------------------------------------------------

  Site_2 p_ref() const
  {
    CGAL_precondition ( v_type != SSS );

    if ( v_type == PPS ) {
      if ( pps_idx == 0 ) { return p_; }
      if ( pps_idx == 1 ) { return q_; }
      return r_;
    }

    if ( p_.is_point() ) {
      return p_;
    } else if ( q_.is_point() ) {
      return q_;
    } else {
      CGAL_assertion( r_.is_point() );
      return r_;
    }
  }


public:
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  //                           access methods
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------


  FT x() const { return hx() / hw(); }
  FT y() const { return hy() / hw(); }

  FT hx() const {
    return ux;
  }

  FT hy() const {
    return uy;
  }

  FT hw() const {
    return uz;
  }

  FT radius() const {
    switch (v_type) {
    case PPP:    case PPS:    case PSS:
      {
	Point_2 pref = p_ref().point();
        return CGAL::max( CGAL::abs(x() - pref.x()), 
		          CGAL::abs(y() - pref.y()) )

      }
    case SSS:
      {
	Line_2 l = compute_supporting_line(p_.supporting_site());
	Homogeneous_point_2 hp = 
          compute_linf_projection_hom(l, point());

	FT dx = CGAL::abs(x() - hp.x());
	FT dy = CGAL::abs(y() - hp.y());
	return CGAL::max(dx, dy);
      }
    default:
      return FT(0);
    }
  }


  Point_2 point() const {
    if ( is_degenerate_Voronoi_circle() ) {
      return degenerate_point();
    }
    
    return Point_2(x(), y());
  }


  Point_2 degenerate_point() const
  {
    CGAL_precondition( is_degenerate_Voronoi_circle() );
    return p_ref().point();
  }

  // philaris: circle in Linf is an axis parallel square
  // philaris: i.e., an Iso_rectangle_2 with equal sides
  typename K::Iso_rectangle_2 circle() const
  {
    typedef typename K::Iso_rectangle_2  Iso_rectangle_2;
    Point_2 pleftbot (point().x()-radius(), point.y()-radius());
    Point_2 prghttop (point().x()+radius(), point.y()+radius());
    return Iso_rectangle_2 (pleftbot, prghttop);

  }

  vertex_t type() const { return v_type; }

public:
  Voronoi_vertex_sqrt_field_C2(const Site_2& p,
			       const Site_2& q,
			       const Site_2& r)
    : p_(p), q_(q), r_(r)
  {
    compute_vertex(p, q, r);
  }

  //--------------------------------------------------------------------------

  Sign incircle(const Site_2& t) const 
  {
    if ( t.is_point() ) {
      return incircle_p(t);
    }
    return incircle_s(t);
  }

  Sign incircle_no_easy(const Site_2& t) const 
  {
    Sign s;

    if ( t.is_point() ) {
      s = incircle_p_no_easy(t);
    } else {
      s = incircle_s_no_easy(t);
    }

    return s;
  }

  //--------------------------------------------------------------------------


  Orientation orientation(const Line_2& l) const 
  {
    return CGAL::sign(l.a() * x() + l.b() * y() + l.c());
  }

  Oriented_side oriented_side(const Line_2& l) const
  {
    return orientation(l);
  }

  //--------------------------------------------------------------------------

private:
  const Site_2& p_, q_, r_;

  vertex_t v_type;

  // index that indicates the refence point for the case PPS
  short pps_idx;

  FT ux, uy, uz;
};

} //namespace SegmentDelaunayGraphLinf_2

} //namespace CGAL

#endif // CGAL_SEGMENT_DELAUNAY_GRAPH_LINF_2_VORONOI_VEFTEX_SQRT_FIELD_C2_H
