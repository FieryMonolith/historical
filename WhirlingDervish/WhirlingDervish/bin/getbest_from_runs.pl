#!/usr/bin/perl
use strict;

if(@ARGV != 1) {
    print "Too few arguements.\n";
    exit;
}

my @temp = split(/\//, $ARGV[0]);
my $t = $temp[@temp-2];
$t =~ s/^.*_//;

my $filename = "c" . $t . ".cre";

print $ARGV[0] . "\n";

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

open(OUT, ">$filename") || die "Can not open file: $ARGV[1]\n";
print OUT $creature;
close(OUT);
