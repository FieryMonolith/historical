#!/usr/bin/perl
use strict;

if(@ARGV != 3) {
    print "Too few arguements.\n";
    print "executable <input file> <output file> <generation>\n";
    exit;
}

my $generation = $ARGV[2];

open(IN, "<$ARGV[0]") || die "Can not open file: $ARGV[0]\n";

my $creature = "";
while(<IN>) {
    if(/GENERATION_START (\d+)/ && $1 == $generation) {
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
