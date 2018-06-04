#!/usr/bin/perl

=head1 DESCRIPTION

printenv — a program that just prints its arguments
=cut

$last = $#ARGV;
for ($i = 0; $i <= $last; $i++)
{
	if ($i == 0) { printf("%s", $ARGV[$i]); }
	else { printf(" %s", $ARGV[$i]); }
	if ($i == $last) { print("\n"); }
}
$test =  $ENV{'TESTENV'};
printf("%s\n", $test) if ($test ne "");
