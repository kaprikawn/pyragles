#!/usr/local/bin/perl

use strict;
use Data::Dumper;
use JSON;
use Cwd;
my $br = "\n";
my $cwd = getcwd;

sub get_mesh_data {
  my ( $meshType, $colours_hashref ) = @_;
  
  #print Dumper( $colours_hashref ) . $br;
  my $lc_meshType = lc $meshType;
  my $input = `cat ${lc_meshType}.obj`;
  my @text  = split( /[\r\n]+/, $input );
  my $data  = { meshType => $meshType };
  my @vertices;
  my @indices;
   
  my $vertex_count  = 0;
  my $face_count    = 0;
  my $obj_num;
  
  foreach my $current_line( @text ) {
    $current_line =~ s/[\r\n]+//g;
    
    next if( !$current_line );
    
    if( $current_line =~ /^o\s/ ) {
      ( $obj_num ) = split( /\-/, $current_line );
      $obj_num =~ s/[^0-9]//g;
      
      if( exists( $colours_hashref -> { $meshType }{ $obj_num } ) ) {
        my $r = $colours_hashref -> { $meshType }{ $obj_num }{ r };
        my $g = $colours_hashref -> { $meshType }{ $obj_num }{ g };
        my $b = $colours_hashref -> { $meshType }{ $obj_num }{ b };
        
        $data -> { $obj_num }{ colour }{ r } = $r;
        $data -> { $obj_num }{ colour }{ g } = $g;
        $data -> { $obj_num }{ colour }{ b } = $b;
        
        
      }
      
    } elsif( $current_line =~ /^v\s/  ) {
      
      my ( $v, $x, $z, $y ) = split( /\s/, $current_line );
       
      $x = sprintf( "%0.2f", $x );
      $y = sprintf( "%0.2f", $y );
      $z = sprintf( "%0.2f", $z );
      
      push @{ $data -> { $obj_num }{ vertices } }, { x => $x, y => $y, z => $z };
      

    } elsif( $current_line =~ /^f\s/ ) {
      
    }
  }
  
  return $data;
}

sub get_colours {
  my $text = `cat objColours.txt`;
  my @file = split( /[\r\n]/, $text );
  
  my $return_hashref = {};
  
  my $objectType;
  foreach my $line( @file ) {
    $line =~ s/[\r\n]+//g;
    next if( !$line );
    
    if( $line =~ /^\[.*\]$/ ) {
      $objectType = $line;
      $objectType =~ s/(\[|\])//g;
    } elsif( $line =~ /=/ ) {
      my ( $objPartNum, $rgb ) = split( /=/, $line );
      my ( $r, $g, $b ) = split( /,/, $rgb );
      
      $return_hashref -> { $objectType }{ $objPartNum }{ r } = $r;
      $return_hashref -> { $objectType }{ $objPartNum }{ g } = $g;
      $return_hashref -> { $objectType }{ $objPartNum }{ b } = $b;
    }
  }
  
  return $return_hashref;
}

sub process_file {
  my ( $level ) = @_;
  
  my $colours_hashref = &get_colours();
  
  my $baseJsonFile = qq~baseJsonLevel${level}.json~;
  my $baseJsonText = `cat $baseJsonFile`;
  
  #print $baseJsonText;
  
  my $json    = JSON -> new;
  my $hash    = $json -> decode( $baseJsonText );
  my @processed_meshes;
  #print Dumper( $hash );
  
  foreach my $key( keys %{ $hash } ) {
    next if( ref $hash -> { $key } ne 'ARRAY' );
    
    foreach my $hashref( @{ $hash -> { $key } } ) {
      my $meshType = $hashref -> { meshType };
      next if( !$meshType );
      next if( grep $_ eq $meshType, @processed_meshes );
      
      my $new_mesh = &get_mesh_data( $hashref -> { meshType }, $colours_hashref );
      push @{ $hash -> { meshes } }, $new_mesh;
      
      push @processed_meshes, $meshType;
      #print $hashref -> { meshType } . $br;
      #print Dumper( $new_mesh );
    }
  }
  
  
  $Data::Dumper::Sortkeys = 1;
  #print Dumper( $hash ) . $br;
  my $pretty = JSON -> new -> pretty(1) -> canonical(1) -> encode( $hash );
  print $pretty;
  
  open FOUT, "> level${level}.json" || die $! . $br;
  print FOUT JSON -> new -> pretty(1) -> canonical(1) -> encode( $hash );
  close FOUT;
  
  return 0;
}

&process_file( '11' );

exit 0;
