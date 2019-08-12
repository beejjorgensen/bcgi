#!/usr/local/bin/perl

#
# BCGI.PL - some of the Beej CGI routines ported to Perl.
#


#
# &get_cgi_var(*cgi_var_list);
#
# gets CGI variables from QUERY_STRING or STDIN and stores them in the
# specified assoc array.
#
# returns false on error, or true if no error.


sub get_cgi_var {
    package bcgi;

    local(*cgi_var) = @_;

    if ($ENV{'REQUEST_METHOD'} eq "GET") {
        $_ = $ENV{'QUERY_STRING'};
    } elsif ($ENV{'REQUEST_METHOD'} eq "POST") {
        read(STDIN,$_,$ENV{'CONTENT_LENGTH'});
    } else {
        return 0;  # unknown request method
    }
  
    $_ = "&$_";    # this eases the pattern search

    while( /&([^=]*)=([^&]*)/ ) {
        $name = $1;         # this is the variable name
        $value = $2;        # this is its value
        $remains = $';      # this is the rest of the query string
        $name =~ s/\+/ /g;  # un-URL these puppies
        $name =~ s/%(..)/sprintf("%c",hex($1))/ge;
        $value =~ s/\+/ /g;
        $value =~ s/%(..)/sprintf("%c",hex($1))/ge;
        $cgi_var{$name} = $value; # save it in associative array
        $_ = $remains;       # next step
    }

    1;
}

#
# &escape($str);
#
# places a leading backslash in front of characters that may hold special
# meaning to the shell and returns the modified string.

sub escape {
    package bcgi;

    local($str) = @_;

    $str =~ s/(\W)/\\$1/g;

    $str;
}

#
# &xyinrect($x,$y,$x1,$y1,$x2,$y2);
#
# returns true if the point (x,y) is within the rectangle defined with upper-
# left corner (x1,y1) and lower right corner (x2,y2).

sub xyinrect {
    package bcgi;

    local($x,$y,$x1,$y1,$x2,$y2) = @_;

    ($x >= $x1 && $x <= $x2 && $y >= $y1 && $y <= $y2);
}

#
# &xyincircle($x,$y,$x1,$y1,$r);
#
# returns true if the point (x,y) is within the circle with center (x1,y1)
# and radius r.

sub xyincircle {
    package bcgi;

    local($x,$y,$x1,$y1,$r) = @_;

    (sqrt(($x-$x1)**2+($y-$y1)**2) <= $r);
}

#
# &dumpvar(%varlist);
#
# displays all cgi variables.

sub dumpvar {
    package bcgi;

    local(*varlist) = @_;

    foreach $key (sort keys(%varlist)) {
        print "$key = $varlist{$key}\n";
    }
    1;
}

1;
