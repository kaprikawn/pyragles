#!/usr/local/bin/perl

use strict;
use Data::Dumper;
my $br = "\n";

sub process_string {
  my ( $input ) = @_;
  
  my @text = split( /[\r\n]+/, $input );
  my $data = {};
  my @indices;
   
  my $vertex_count    = 0;
  my $face_count      = 0;
  my $obj_num;
  
  foreach my $current_line( @text ) {
    $current_line =~ s/[\r\n]+//g;
     
    next if( !$current_line );
    #print $current_line . $br;
    if( $current_line =~ /^o\s/ ) {
      ( $obj_num ) = split( /\-/, $current_line );
      $obj_num =~ s/[^0-9]//g;
    } elsif( $current_line =~ /^v\s/  ) {
       
      my ( $v, $x, $z, $y ) = split( /\s/, $current_line );
       
      $x = sprintf( "%0.2f", $x );
      $y = sprintf( "%0.2f", $y );
      $z = sprintf( "%0.2f", $z );
       
      $data -> { vertices }{ $obj_num }{ $vertex_count }{ x } = $x;
      $data -> { vertices }{ $obj_num }{ $vertex_count }{ y } = $y;
      $data -> { vertices }{ $obj_num }{ $vertex_count }{ z } = $z;
       
      $vertex_count++
       
    } elsif( $current_line =~ /^f\s/ ) {
      # f 3//1 5//1 4//1
      my ( $f, $v1, $v2, $v3 ) = split( /\s/, $current_line );
       
      ( $v1 ) = $v1 =~ /^(.*?)\//;
      ( $v2 ) = $v2 =~ /^(.*?)\//;
      ( $v3 ) = $v3 =~ /^(.*?)\//;
      $v1--;
      $v2--;
      $v3--;
       
      #print "tri1 is " . $data -> { vertices }{ $v1 }{ x } . ', ' . $data -> { vertices }{ $v1 }{ y } . ', ' . $data -> { vertices }{ $v1 }{ z } . $br;
      #print "tri2 is " . $data -> { vertices }{ $v2 }{ x } . ', ' . $data -> { vertices }{ $v2 }{ y } . ', ' . $data -> { vertices }{ $v2 }{ z } . $br;
      #print "tri3 is " . $data -> { vertices }{ $v3 }{ x } . ', ' . $data -> { vertices }{ $v3 }{ y } . ', ' . $data -> { vertices }{ $v3 }{ z } . $br;
       
       
      $data -> { faces }{ $obj_num }{ $face_count }{ 0 }{ x } = $data -> { vertices }{ $obj_num }{ $v1 }{ x };
      $data -> { faces }{ $obj_num }{ $face_count }{ 0 }{ y } = $data -> { vertices }{ $obj_num }{ $v1 }{ y };
      $data -> { faces }{ $obj_num }{ $face_count }{ 0 }{ z } = $data -> { vertices }{ $obj_num }{ $v1 }{ z };
       
      $data -> { faces }{ $obj_num }{ $face_count }{ 1 }{ x } = $data -> { vertices }{ $obj_num }{ $v2 }{ x };
      $data -> { faces }{ $obj_num }{ $face_count }{ 1 }{ y } = $data -> { vertices }{ $obj_num }{ $v2 }{ y };
      $data -> { faces }{ $obj_num }{ $face_count }{ 1 }{ z } = $data -> { vertices }{ $obj_num }{ $v2 }{ z };
       
      $data -> { faces }{ $obj_num }{ $face_count }{ 2 }{ x } = $data -> { vertices }{ $obj_num }{ $v3 }{ x };
      $data -> { faces }{ $obj_num }{ $face_count }{ 2 }{ y } = $data -> { vertices }{ $obj_num }{ $v3 }{ y };
      $data -> { faces }{ $obj_num }{ $face_count }{ 2 }{ z } = $data -> { vertices }{ $obj_num }{ $v3 }{ z };
       
      my @face = ( $v1, $v2, $v3 );
      push @indices, \@face;
      
      $face_count++;
    }
  }
   
  $Data::Dumper::Sortkeys = 1;
  #print Dumper( $data ) . $br;
  
  foreach my $object_part( sort keys %{ $data -> { vertices } } ) {
    print $br . '    // object part ' . $object_part . $br;
    foreach my $vertex_index( sort { $a <=> $b } keys %{ $data -> { vertices }{ $object_part } } ) {
      print "    postion = { " . $data -> { vertices }{ $object_part }{ $vertex_index }{ x } . 'f, ' . $data -> { vertices }{ $object_part }{ $vertex_index }{ z } . 'f, ' . $data -> { vertices }{ $object_part }{ $vertex_index }{ y } . 'f };' . $br;
      print "    addVertex( position, colour, currentShape );" . $br;
    }
  }
  
  print $br . $br . '    indices = {' . $br;
  my $first_index = 1;
  for my $face( @indices ) {
    if( $first_index ) {
      print '        ' . join( ', ', @{ $face } ) . $br;
    } else {
      print '      , ' . join( ', ', @{ $face } ) . $br;
    }
    $first_index = 0;
  }
  print '    };' . $br . $br;

  
  return 0;
}

sub process_file {
  my ( $filename, $dir ) = @_;
  
  my $file_string =   `cat $dir/$filename`;
  
  &process_string( $file_string );
  
  return 0;
}

&process_file( 'arch.obj', '/home/wayne.briggs/test' );

exit 0;
