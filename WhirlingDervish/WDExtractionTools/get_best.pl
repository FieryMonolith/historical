#!/usr/bin/perl

# file name is required
# prefix is optional
if(@ARGV < 1 || @ARGV > 2) {
    print "get_generation.pl [filename] [prefix]*\n";
    exit(0);
}

$ARGV[0] =~ s/^.*\///;
$prefix = "";
if(@ARGV == 2) {
    $prefix = $ARGV[1];
}

$name = $ARGV[0];
$name = $prefix."best.cre";

open(IN, $ARGV[0]) || die "Can't open $ARGV[0].\n";
open(OUT, ">$name") || die "Can't open $name\n";

print "$ARGV[0]\n";

$_ = <IN>; # Get rid of introduction line

$max = -1;
$data = "";
$beginRecord = 0;
while(<IN>) {
    if(/^GENERATION_START/) {
	$max = -1;
	$data = "";
	next;
    }
    # Parameters are replicates, language, id, parent id, fitness
    if(/^CREATURE_DATA \d+ \d+ \d+ \d+ (\d+\.?\d+)$/ && $1 > $max) {
	$max = $1;
	$beginRecord = 1;
	$data = "CREATURE_START\n";
    }
    if($beginRecord) {
	$data .= $_;
    }
    if(/CREATURE_END/) {
	$beginRecord = 0;
    }
}

close(IN);

if($max < 0) {
    print "Can't find a best creature.\n";
    exit(0);
}

print OUT $data;

close(OUT);

