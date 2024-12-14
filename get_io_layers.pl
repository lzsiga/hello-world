#!/usr/local/bin/perl

# usage: require "./get_io_layers.pl";
# printf("IO Layers of somefile: %s\n", GetIoLayers($fh));

# result:
# Layers of somefile:
#   Input:  unix-201200 perlio-c81200
#   Output: unix-201200 perlio-c81200 encoding(utf-8-strict)-c89200

# extract from perliol.h:
# /* Flag values */
#define PERLIO_F_EOF            0x00000100
#define PERLIO_F_CANWRITE       0x00000200
#define PERLIO_F_CANREAD        0x00000400
#define PERLIO_F_ERROR          0x00000800
#define PERLIO_F_TRUNCATE       0x00001000
#define PERLIO_F_APPEND         0x00002000
#define PERLIO_F_CRLF           0x00004000
#define PERLIO_F_UTF8           0x00008000
#define PERLIO_F_UNBUF          0x00010000
#define PERLIO_F_WRBUF          0x00020000
#define PERLIO_F_RDBUF          0x00040000
#define PERLIO_F_LINEBUF        0x00080000
#define PERLIO_F_TEMP           0x00100000
#define PERLIO_F_OPEN           0x00200000
#define PERLIO_F_FASTGETS       0x00400000
#define PERLIO_F_TTY            0x00800000
#define PERLIO_F_NOTREG         0x01000000
#define PERLIO_F_CLEARED        0x02000000

sub ListLayers {
    my $lst= $_[0];
    my $len= scalar(@$lst);
    my $s= '';
    for (my $i= 0; $i<$len; $i += 3) {
        if ($i>0) { $s .= ' '; }
        if ($lst[$i+1]) { $s .= sprintf ("%s(%s)-%x", $lst->[$i], $lst->[$i+1], $lst->[$i+2]); }
        else            { $s .= sprintf ("%s-%x", $lst->[$i], $lst->[$i+2]); }
    }
    return $s;
}

sub GetIoLayers {
    my ($fh)= @_;

    my @lst= PerlIO::get_layers($fh, details=>1);
    my $s= "\n    Input:  " . ListLayers(\@lst);

    @lst= PerlIO::get_layers($fh, details=>1,output=>1);
    $s .= "\n    Output: " . ListLayers(\@lst);

    return $s;
}

sub GetInLayers {
    my ($fh)= @_;

    my @lst= PerlIO::get_layers($fh, details=>1);
    return ListLayers(\@lst);
}

sub GetOutLayers {
    my ($fh)= @_;

    my @lst= PerlIO::get_layers($fh, details=>1,output=>1);
    return ListLayers(\@lst);
}

return true;
