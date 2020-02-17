#!/usr/bin/perl
use strict;

if(@ARGV != 3) {
    print "Too few arguements.\n";
    print "executable <input file> <output file> <generation>\n";
    exit;
}

my $generation = $ARGV[2];

open(IN, "<$ARGV[0]") || die "Can not open file: $ARGV[0]\n";
open(OUT, ">$ARGV[1]") || die "Can not open file: $ARGV[1]\n";

# Read in the one line header that has to be there
$_ = <IN>;
print OUT $_;

while(<IN>) {
    if(/GENERATION_START (\d+)/ && $1 == $generation) {
	print OUT $_;
	while(<IN>) {
	    print OUT $_;
	    if(/GENERATION_END/) {
		last;
	    }
	}
    }
}

close(IN);
close(OUT);
