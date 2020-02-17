#!/usr/bin/perl
use strict;

if(@ARGV != 2) {
    print "Too few arguements.\n";
    exit;
}

open(IN, "<$ARGV[0]") || die "Can not open file: $ARGV[0]\n";

my $creature = "";
while(<IN>) {
    if(/GENERATION_START/) {
	# Remove random number generator state
	$_ = <IN>;

	$creature = "";
	while(<IN>) {
	    $creature .= $_;
	    if(/CREATURE_END/) {
		last;
	    }
	}
    }
}

close(IN);

open(OUT, ">$ARGV[1]") || die "Can not open file: $ARGV[1]\n";
print OUT $creature;
close(OUT);
