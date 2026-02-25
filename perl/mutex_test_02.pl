#!/usr/local/bin/perl

# $ perl mutex_test.pl # output of run:
#
# Main process before lock
# Main process locked the mutex, sleeping
# Thread: before lock
# … here the main process sleeps, the thread waits for the lock …
# Main process woke up
# Main process released the mutex
# Thread: locked the mutex, sleeping
# … here the thread sleeps, the main process waits in join …
# Thread: released the mutex
# Main process after join: exiting

use strict;
use warnings;
use utf8;
use threads;
use threads::shared;
use POSIX ":sys_wait_h";

my $pipeRead;
my $pipeWrite;

# this complex data-structure needs serialization
our %dataStruct:shared = (cntUp=>0, cntDown=>32);

sub dsIncrement {
    my $p= $_[0];
    my $val= $_[1];
    lock (%$p);
    $p->{cntUp} += $val;
    $p->{cntDown} -= $val;
}

sub dsDecrement {
    my $p= $_[0];
    my $val= $_[1];
    lock (%$p);
    $p->{cntUp} -= $val;
    $p->{cntDown} += $val;
}

sub dsPrint {
    my $p= $_[0];
    my $msg= $_[1];
#   printf ("%s: obj=%s, cntUp=%d, cntDown=%d\n", $msg, $p, $p->{cntUp}, $p->{cntDown});
    printf ("%s: cntUp=%d, cntDown=%d\n", $msg, $p->{cntUp}, $p->{cntDown});
}

sub thread_main {
    my $msg;
    printf "Thread started\n";
    $msg= '';
    for (my $leave= 0; !$leave;) {
        if (! sysread ($pipeRead, $msg, 32, 0)) {
            my $err1= $?;
            my $err2= $!;
            printf ("Thread: sysread failed: ?=%s, !=%s\n", $err1, $err2);
            STDOUT->flush ();
            $leave= 1;
            next;
        }
        printf ("Thread: read '%s' from pipe\n", $msg);
        STDOUT->flush ();
        if (length($msg)!=32) {
            printf ("Thread: invalid length\n");
            STDOUT->flush ();
            $leave= 1;
            next;
        }
        my $cmd= substr($msg, 0, 4);
        $cmd =~ s/\s+$//;
        my $rst= substr($msg, 4);
        $rst =~ s/\s+$//;

        if ($cmd eq 'STOP') {
            printf ("Thread: instructed to stop\n");
            STDOUT->flush ();
            $leave= 1;
            next;

        } elsif ($cmd eq 'INC') {
            printf ("Thread: Increment\n");
            STDOUT->flush ();
            dsIncrement(\%dataStruct, 1);
            dsPrint (\%dataStruct, "Thread executed increment");
            STDOUT->flush ();

        } elsif ($cmd eq 'DEC') {
            printf ("Thread: Decrement\n");
            STDOUT->flush ();
            dsDecrement(\%dataStruct, 1);
            dsPrint (\%dataStruct, "Thread executed decrement");
            STDOUT->flush ();

        } elsif ($cmd eq 'TERM') {
            printf ("Thread: Child %s terminated\n", $rst);
            dsDecrement(\%dataStruct, 1);
            dsPrint (\%dataStruct, "Thread executed decrement");
            STDOUT->flush ();
        }
    }
    printf "Thread exits\n";
}

# this is an 'actual' signal handler
# see `man 7 signal`
# we use `syswrite` to notify the thread that actually
# does something

sub _SIG_CHLD {
    while ((my $pid = waitpid(-1, WNOHANG))>0) {
        my $code = $? >> 8;
        my $msg = '';

        my $tmp= sprintf ("TERM %d", $pid);
        $msg= sprintf ("%-32s", $tmp);
        syswrite ($pipeWrite, $msg, 32);
    }
}

sub startChild {
    my $chldPid= fork();
    if ($chldPid<0) {
        printf ("Main process: error in fork\n");
        STDOUT->flush ();
        return;
    } elsif ($chldPid>0) {
        printf ("Main process: child started pid=%d\n", $chldPid);
        STDOUT->flush ();
        return;
    } else {
        $chldPid= $$;
        printf ("Child: started with pid=%d\n", $chldPid);
        STDOUT->flush ();
        sleep (2);
        printf ("Child: exiting (pid=%d)\n", $chldPid);
        STDOUT->flush ();
        exit (0);
    }
}

sub main {

    printf "Main process starts\n";
    STDOUT->flush ();

    dsPrint (\%dataStruct, "Main process: dataStruct at start");

    pipe ($pipeRead, $pipeWrite) or die ("Error in 'pipe'\n");
    printf ("Main process created the pipe\n");
    STDOUT->flush ();

    $SIG{CHLD} = \&_SIG_CHLD;
    startChild ();
    system ('sleep 3; echo "ChildProcess exists" &');

    my $msg= sprintf("%-32s", 'INC');
    syswrite ($pipeWrite, $msg, 32);
    printf ("Main process has written '%s' into the pipe\n", $msg);
    STDOUT->flush ();

    my $thr1= threads->create (\&thread_main);
    printf ("Main process created the thread\n");
    STDOUT->flush ();

    dsIncrement(\%dataStruct, 1);
    dsPrint (\%dataStruct, "Main process executed increment");

    printf "Main process is about to sleep\n";
    STDOUT->flush ();
    sleep (5);
    printf "Main process woke up\n";
    STDOUT->flush ();

    $msg= sprintf ('%-32s', 'STOP');
    syswrite ($pipeWrite, $msg, 32);
    printf ("Main process has written '%s' into the pipe\n", $msg);
    STDOUT->flush ();

    $thr1->join();
    dsPrint (\%dataStruct, "dataStruct at exit");
    printf "Main process after join: exiting\n";
    STDOUT->flush ();
}

main();
