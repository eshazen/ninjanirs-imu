//
// Reproduce (more or less) Hammond 1551T enclosure
// for 3D printing
//

$fn = 32;
e = 0.1;

case_out = 60;			/* outer size */
case_in = 56;			/* inner size */
case_ext = 6;			/* height extend beyond stock */
case_hgt = 17+case_ext;		/* box height */

bot_hgt = 2;			/* bottom height (thickness) */

so_dia = 4;			/* board standoff diam */
so_hol = 1.725;			/* board standoff hole diam */
so_hgt = 4;			/* board standoff height above base */
so_xy = 7.5;			/* standoff offset from corner */

out_rad = 3.5;			/* outer corner radius */
in_rad = out_rad-(case_out-case_in)/2; /* inner corner radius */

pcb_thk = 1.6;
pcb_top = so_hgt+bot_hgt+pcb_thk; /* PCB top surface Z */

// RJ cutout dims
rj_wid = 11.5;			/* mfg dim 11.2 */
// rj_hgt = 12.0;			/* mfg dim 11.7 */
rj_hgt = 20.0;			/* mfg dim 11.7 */

// lid support standoff
lid_hgt = 13+case_ext;
lid_dia = 5.6;
lid_hol = 2.5;

// lid support standoff
module lid_at( x, y) {
  translate( [x, y, bot_hgt-e]) {
    difference() {
      cylinder( h=lid_hgt, d=lid_dia);
      cylinder( h=lid_hgt+e, d=lid_hol);
    }
  }
}

// board mount standoff
module standoff_at( x, y) {
  translate( [x, y, bot_hgt-e]) {
    difference() {
      cylinder( h=so_hgt, d=so_dia);
      cylinder( h=so_hgt+e, d=so_hol);
    }
  }
}

module outer_wall() {
    hull() {
      translate( [out_rad, out_rad, 0]) circle( out_rad);
      translate( [case_out-out_rad, case_out-out_rad, 0]) circle( out_rad);
      translate( [out_rad, case_out-out_rad, 0]) circle( out_rad);
      translate( [case_out-out_rad, out_rad, 0]) circle( out_rad);
    }
}

module inner_wall() {
      hull() {
      translate( [in_rad, in_rad, 0]) circle( in_rad);
      translate( [case_in-in_rad, case_in-in_rad, 0]) circle( in_rad);
      translate( [in_rad, case_in-in_rad, 0]) circle( in_rad);
      translate( [case_in-in_rad, in_rad, 0]) circle( in_rad);
    }
}

module case_walls() {
  linear_extrude( height=case_hgt)
    difference() {
    outer_wall();
    translate( [(case_out-case_in)/2, (case_out-case_in)/2, 0])
      inner_wall();
  }
}

// RJ-11 cutout at center (x, y)
module rj11_at( x, y) {
  translate( [x, y, pcb_top])
    translate( [-rj_wid/2, -5, 0])
    cube( [rj_wid, 10, rj_hgt]);
}

// 26 pin ribbon cutout dims
rib_wid = 24.0;			/* mfg dim 22.9 */
rib_hgt = 6.0;			/* mfg dim 5.0 */

// ribbon cutout at center (x, y);
module rib_at( x, y) {
  translate( [x, y, pcb_top])
    translate( [-rib_wid/2, -5, 0])
    cube( [rib_wid, 10, rib_hgt]);
}

// model the box
module box() {
  case_walls();			/* walls with radius */
  linear_extrude( height=bot_hgt) outer_wall(); /* bottom */
  standoff_at( case_out-so_xy, so_xy);
  standoff_at( so_xy, case_out-so_xy);
  lid_at( case_out-so_xy-46, so_xy-1);
  lid_at( so_xy+46, case_out-so_xy+1);
}

difference() {
  box();
  // top RJ-11
  rj11_at( so_xy+12.5, case_out);
  rj11_at( so_xy+30.7, case_out);
  // bottom RJ-11
  rj11_at( so_xy+12.8, 0);
  rj11_at( so_xy+30.5, 0);
  // right RJ-11
  rotate( [0, 0, 90]) {
    rj11_at( so_xy+15.9, -case_out);
    rj11_at( so_xy+29.5, -case_out);
  }
  rotate( [0, 0, 90]) {
    rib_at( case_out-so_xy-23.5, 0);
  }
}


