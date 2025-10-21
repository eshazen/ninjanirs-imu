#!/usr/bin/perl
#
# read a CSV file from the scope and find transitions
use strict;
use warnings;
use Text::CSV_XS;

my $thr = 1.6;			# voltage threshold

my $csv = Text::CSV_XS->new( { binary => 1, auto_diag => 1});

my $file = $ARGV[0];

open my $fh, "<:encoding(utf8)", $file or die "Cannot open $file: $!";

my $head1 = $csv->getline($fh);
my $head2 = $csv->getline($fh);
my $first = $csv->getline($fh);

my $ncol = @{$head1};

my $t_0 = $first->[0];
my @v0;
my @t0;

sub over_thr {
    my( $va, $vb) = @_;
    my $rc = 0;
    if( length($va) > 0 && length($vb) > 0) {
	$rc = 1 if( ($va < $thr && $vb >= $thr) ||
		    ($vb < $thr && $va >= $thr));
    }
    return $rc;
}

for( my $i=1; $i<$ncol; $i++) {
    $v0[$i] = $first->[$i];
    $t0[$i] = $first->[0];
}    

while( my $row = $csv->getline($fh)) {
    my $t = $row->[0];
    for( my $i=1; $i<$ncol; $i++) {
#	print "Check column $i len =", length( $row->[$i]), " value='", $row->[$i], "'\n";
	if( length( $row->[$i]) > 0) {
	    if( over_thr( $row->[$i], $v0[$i])) {
		my $dt = ($t - $t0[$i]) * 1e6;
		my $val = ($row->[$i] >= $thr) ? 1 : 0;
		print "Change $i to $val at $t (dt=$dt us)\n";
		if( $i == 1) {
# 		    if( $val) {
# 			print "$t 0\n";
# 		    } else {
# 			print "$t 1\n";
# 		    }
# 		    print "$t $val\n";
		}
		$t0[$i] = $row->[0];
	    }
	    $v0[$i] = $row->[$i];
	}
    }


}
