#!/usr/local/bin/perl

use strict;
use warnings;

require "./get_io_layers.pl";

printf("IO Layers of STDERR: %s\n", GetIoLayers(*STDERR));

binmode(STDERR, ':encoding(UTF-8)');
printf("After binmode: %s\n", GetIoLayers(*STDERR));

binmode(STDERR, ':encoding(ISO-8859-1)');
printf("After another binmode: %s\n", GetIoLayers(*STDERR));
#wrong:
#After another binmode:
#   Input:  unix-201200 perlio-c91200 encoding(utf-8-strict)-c89200 encoding(iso-8859-1)-c89200
#   Output: unix-201200 perlio-c91200 encoding(utf-8-strict)-c89200 encoding(iso-8859-1)-c89200
#I don't know how to _remove_ a layer :(