#!/usr/local/bin/perl

# usage: require "./get_io_layers.pl";
# printf("IO Layers of somefile: %s\n", GetIoLayers($fh));

# result:
# Layers of somefile:
#   Input:  unix-201200 perlio-c81200
#   Output: unix-201200 perlio-c81200 encoding(utf-8-strict)-c89200

sub GetIoLayers {
    my ($fh)= @_;

    my @lst= PerlIO::get_layers($fh, details=>1);
    my $len= scalar(@lst);
    my $s= "\n    Input:  ";
    for (my $i= 0; $i<$len; $i += 3) {
        if ($lst[$i+1]) { $s .= sprintf ("%s(%s)-%x ", $lst[$i], $lst[$i+1], $lst[$i+2]); }
        else            { $s .= sprintf ("%s-%x ", $lst[$i], $lst[$i+2]); }
    }

    @lst= PerlIO::get_layers($fh, details=>1,output=>1);
    $len= scalar(@lst);
    $s .= "\n    Output: ";
    for (my $i= 0; $i<$len; $i += 3) {
        if ($lst[$i+1]) { $s .= sprintf ("%s(%s)-%x ", $lst[$i], $lst[$i+1], $lst[$i+2]); }
        else            { $s .= sprintf ("%s-%x ", $lst[$i], $lst[$i+2]); }
    }
    return $s;
}

return true;
