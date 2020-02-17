#!/usr/bin/perl

if(@ARGV != 1) {
    print "get_last_generation.pl [filename] \n";
    exit(0);
}

print "Working on file $ARGV[0]\n";

$ARGV[0] =~ s/^.*\///;

open(IN, $ARGV[0]) || die "Can't open $ARGV[0].\n";

$_ = <IN>; # Get rid of introduction line

$max = -1;
while(<IN>) {
    if(/^GENERATION_START (\d+)$/ && $1 > $max) {
	$max = $1;
    }
}

close(IN);

if($max < 0) {
    print "Can't find any generations.\n";
    exit(0);
}

$exec = "get_generation.pl $ARGV[0] $max";
`$exec`;
