#!/usr/bin/perl

if(@ARGV != 2) {
    print "get_generation.pl [filename] [generation number]\n";
    exit(0);
}

$ARGV[0] =~ s/^.*\///;

open(IN, $ARGV[0]) || die "Can't open $ARGV[0].\n";

$_ = <IN>; # Get rid of introduction line

$foundGeneration = 1;
$beginRecord = 0;
while(<IN>) {
    if(/^GENERATION_START (\d+)$/ && $1 == $ARGV[1]) {
	$foundGeneration = 1;
	next;
    }
    if(/^GENERATION_END$/) {
	$foundGeneration = 0;
	last;
    }

    # Parameters are replicates, language, id, parent id, fitness
    if(/^CREATURE_DATA \d+ \d+ (\d+) \d+ (\d+\.?\d+)$/) {
	$beginRecord = 1;
	$data = "CREATURE_START\n";

	$outputFilename = $ARGV[1]."_".$1."_".$2.".cre";
	open(OUT, ">$outputFilename") || die "Can't open $outputFilename\n";
    }
    if($beginRecord) {
	$data .= $_;
    }
    if(/CREATURE_END/) {
	$beginRecord = 0;
	print OUT $data;
	close(OUT);
	$data = "";
    }
}

close(IN);
