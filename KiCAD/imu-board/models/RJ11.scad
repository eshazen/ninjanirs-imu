// Assman RJ-11 thru-hole
//
// from https://www.assmann-wsw.com/uploads/datasheets/ASS_7041_CO.pdf

$fn=16;

module rj11() {
     color("#404040") {
	  cube( [11.2, 18, 11.7]);
	  translate( [(11.2-7.62)/2, 7.85, -3.25]) {
	       cylinder( h=3.25, d=3.2);
	       translate( [7.62, 0, 0])
		    cylinder( h=3.25, d=3.2);
	  }
     }
     color("gold") {
	  translate( [(11.2-3*1.27)/2, 7.85+8.89-2.54, -3.25]) {
	       cylinder( h=3.25, d=1);
	       translate( [2.54, 0, 0]) cylinder( h=3.25, d=1);
	       translate( [1.27, 2.54, 0]) cylinder( h=3.25, d=1);
	       translate( [1.27+2.54, 2.54, 0]) cylinder( h=3.25, d=1);
	  }
     }
}

rj11();

