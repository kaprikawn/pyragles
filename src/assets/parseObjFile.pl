#!/usr/local/bin/perl

use strict;
use Data::Dumper;
use Cwd;
my $br = "\n";
my $cwd = getcwd;

sub process_string {
  my ( $input ) = @_;
  
  my @text = split( /[\r\n]+/, $input );
  my $data = {};
  my @vertices;
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
      
      push @vertices, $br . '  // object part ' . $obj_num . $br;
      
    } elsif( $current_line =~ /^v\s/  ) {
       
      my ( $v, $x, $z, $y ) = split( /\s/, $current_line );
       
      $x = sprintf( "%0.2f", $x );
      $y = sprintf( "%0.2f", $y );
      $z = sprintf( "%0.2f", $z );
       
      $data -> { vertices }{ $obj_num }{ $vertex_count }{ x } = $x;
      $data -> { vertices }{ $obj_num }{ $vertex_count }{ y } = $y;
      $data -> { vertices }{ $obj_num }{ $vertex_count }{ z } = $z;
      
      my @vertex = ( $x, $z, $y );
      
      my $data = "  position = { ${x}f, ${z}f, ${y}f };" . $br;
      $data .= "  addVertex( position, colour, currentShape );" . $br;
      
      push @vertices, $data;
       
      #$vertex_count++
       
    } elsif( $current_line =~ /^f\s/ ) {
      # f 3//1 5//1 4//1
      my ( $f, $v1, $v2, $v3 ) = split( /\s/, $current_line );
       
      ( $v1 ) = $v1 =~ /^(.*?)\//;
      ( $v2 ) = $v2 =~ /^(.*?)\//;
      ( $v3 ) = $v3 =~ /^(.*?)\//;
      $v1--;
      $v2--;
      $v3--;
      
      my @array = ( $v1, $v2, $v3 );
      push @indices, \@array;
       
    }
  }
   
  foreach my $vertex( @vertices ) {
    print $vertex;
  }
  
  my $first_index = 1;
  foreach my $array( @indices ) {
    if( $first_index ) {
      print $br . $br . '  indices = {' . $br;
      print             '      ' .join( ', ', @{$array } ) . $br;
      $first_index = 0;
    } else {
      print             '    , ' .join( ', ', @{$array } ) . $br;
    }
  }
  
  print '  };' . $br;
    
  return 0;
}

sub process_file {
  my ( $filename, $dir ) = @_;
  
  $filename = $ARGV[0] if( $ARGV[0] );
  
  my $file_string =   `cat $dir/$filename`;
  
  &process_string( $file_string );
  
  return 0;
}

&process_file( 'arch.obj', $cwd );

exit 0;
