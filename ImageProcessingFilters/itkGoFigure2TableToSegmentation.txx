/*=========================================================================
  Author: $Author: krm15 $  // Author of last commit
  Version: $Rev: 1658 $  // Revision of last commit
  Date: $Date: 2010-06-14 15:49:25 -0400 (Mon, 14 Jun 2010) $  // Date of last commit
=========================================================================*/

/*=========================================================================
 Authors: The GoFigure Dev. Team.
 at Megason Lab, Systems biology, Harvard Medical school, 2009

 Copyright (c) 2009, President and Fellows of Harvard College.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 Neither the name of the  President and Fellows of Harvard College
 nor the names of its contributors may be used to endorse or promote
 products derived from this software without specific prior written
 permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#ifndef __itkGoFigure2TableToSegmentation_txx
#define __itkGoFigure2TableToSegmentation_txx

#include "itkGoFigure2TableToSegmentation.h"

namespace itk
{
template < class TSegmentImage >
GoFigure2TableToSegmentation< TSegmentImage >
::GoFigure2TableToSegmentation()
{
  m_NumberOfCellTypes = 0;
  m_NumberOfSubCellularTypes = 0;
  m_NumberOfColors = 0;
  m_NumberOfChannels = 0;
  m_NumberOfMeshes = 0;
  m_NumberOfTracks = 0;

  TypeOfFields.resize( 12 );

  TypeOfFields[0] = "<NUMBEROFMESH>";
  Fields[0].push_back( "<CELLTYPEID>" );
  Fields[0].push_back( "<COLORID>" );
  Fields[0].push_back( "<COORDIDMAX>" );
  Fields[0].push_back( "<COORDIDMIN>" );
  Fields[0].push_back( "<IMAGINGSESSIONID>" );
  Fields[0].push_back( "<MESHID>" );
  Fields[0].push_back( "<POINTS>" );
  Fields[0].push_back( "<SUBCELLULARID>" );
  Fields[0].push_back( "<TRACKID>" );
  Fields[0].push_back( "<MESH>" );
  Fields[0].push_back( "</MESH>" );

  TypeOfFields[1] = "<NUMBEROFTRACK>";
  Fields[1].push_back( "<COLORID>" );
  Fields[1].push_back( "<COORDIDMAX>" );
  Fields[1].push_back( "<COORDIDMIN>" );
  Fields[1].push_back( "<IMAGINGSESSIONID>" );
  Fields[1].push_back( "<POINTS>" );
  Fields[1].push_back( "<LINEAGEID>" );
  Fields[1].push_back( "<TRACKFAMILYID>" );
  Fields[1].push_back( "<TRACKID>" );
  Fields[1].push_back( "<TRACK>" );
  Fields[1].push_back( "</TRACK>" );

  TypeOfFields[2] = "<NUMBEROFTRACKFAMILY>";
  Fields[2].push_back( "<TRACKFAMILYID>" );
  Fields[2].push_back( "<TRACKIDDAUGHTER1>" );
  Fields[2].push_back( "<TRACKIDDAUGHTER2>" );
  Fields[2].push_back( "<TRACKIDMOTHER>" );
  Fields[2].push_back( "<TRACKFAMILY>" );
  Fields[2].push_back( "</TRACKFAMILY>" );

  TypeOfFields[3] = "<NUMBEROFCELLTYPE>";
  Fields[3].push_back( "<CELLTYPEID>" );
  Fields[3].push_back( "<DESCRIPTION>" );
  Fields[3].push_back( "<NAME>" );
  Fields[3].push_back( "<CELLTYPE>" );
  Fields[3].push_back( "</CELLTYPE>" );

  TypeOfFields[4] = "<NUMBEROFCOLOR>";
  Fields[4].push_back( "<ALPHA>" );
  Fields[4].push_back( "<BLUE>" );
  Fields[4].push_back( "<COLORID>" );
  Fields[4].push_back( "<DESCRIPTION>" );
  Fields[4].push_back( "<GREEN>" );
  Fields[4].push_back( "<NAME>" );
  Fields[4].push_back( "<RED>" );
  Fields[4].push_back( "<COLOR>" );
  Fields[4].push_back( "</COLOR>" );

  TypeOfFields[5] = "<NUMBEROFLINEAGE>";
  Fields[5].push_back( "<COLORID>" );
  Fields[5].push_back( "<COORDIDMAX>" );
  Fields[5].push_back( "<COORDIDMIN>" );
  Fields[5].push_back( "<IMAGINGSESSIONID>" );
  Fields[5].push_back( "<POINTS>" );
  Fields[5].push_back( "<TRACKIDROOT>" );
  Fields[5].push_back( "<LINEAGEID>" );
  Fields[5].push_back( "<LINEAGE>" );
  Fields[5].push_back( "</LINEAGE>" );

  TypeOfFields[6] = "<NUMBEROFCONTOUR>";
  Fields[6].push_back( "<COLORID>" );
  Fields[6].push_back( "<COORDIDMAX>" );
  Fields[6].push_back( "<COORDIDMIN>" );
  Fields[6].push_back( "<IMAGINGSESSIONID>" );
  Fields[6].push_back( "<POINTS>" );
  Fields[6].push_back( "<CONTOURID>" );
  Fields[6].push_back( "<MESHID>" );
  Fields[6].push_back( "<CONTOUR>" );
  Fields[6].push_back( "</CONTOUR>" );

  TypeOfFields[7] = "<NUMBEROFINTENSITY>";
  Fields[7].push_back( "<CHANNELID>" );
  Fields[7].push_back( "<CHANNELNUMBER>" );
  Fields[7].push_back( "<COLORID>" );
  Fields[7].push_back( "<IMAGINGSESSIONID>" );
  Fields[7].push_back( "<NAME>" );
  Fields[7].push_back( "<NUMBEROFBITS>" );
  Fields[7].push_back( "<CHANNEL>" );
  Fields[7].push_back( "</CHANNEL>" );

  TypeOfFields[8] = "<NUMBEROFSUBCELLULARTYPE>";
  Fields[8].push_back( "<DESCRIPTION>" );
  Fields[8].push_back( "<NAME>" );
  Fields[8].push_back( "<SUBCELLULARID>" );
  Fields[8].push_back( "<SUBCELLULARTYPE>" );
  Fields[8].push_back( "</SUBCELLULARTYPE>" );

  TypeOfFields[9] = "<NUMBEROFCOORDINATE>";
  Fields[9].push_back( "<CCOORD>" );
  Fields[9].push_back( "<COORDID>" );
  Fields[9].push_back( "<PCOORD>" );
  Fields[9].push_back( "<RCOORD>" );
  Fields[9].push_back( "<TCOORD>" );
  Fields[9].push_back( "<XCOORD>" );
  Fields[9].push_back( "<XTILECOORD>" );
  Fields[9].push_back( "<YCOORD>" );
  Fields[9].push_back( "<YTILECOORD>" );
  Fields[9].push_back( "<ZCOORD>" );
  Fields[9].push_back( "<ZTILECOORD>" );
  Fields[9].push_back( "<COORDINATE>" );
  Fields[9].push_back( "</COORDINATE>" );

  TypeOfFields[10] = "<NUMBEROFCHANNEL>";
  Fields[10].push_back( "<CHANNELID>" );
  Fields[10].push_back( "<INTENSITYID>" );
  Fields[10].push_back( "<VALUE>" );
  Fields[10].push_back( "<MESHID>" );
  Fields[10].push_back( "<INTENSITY>" );
  Fields[10].push_back( "</INTENSITY>" );

  TypeOfFields[11] = "</EXPORTTRACES>";
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
Read( std::istream& os )
{
  unsigned int k;
  std::string line;
  PrintStart( os, 0 );

  unsigned int count = 0;
  while ( count < 10 )
  {
    getline( os, line );
    k = GetTypeOfField( line );
    switch ( k )
    {
      case 0:
        line = GetNValue( line, "<NumberOfmesh>", os, 2 );
        this->m_NumberOfMeshes = atoi( line.c_str() );
        PrintMesh( os, 2 );
        break;
      case 1:
        line = GetNValue( line, "<NumberOftrack>", os, 2 );
        this->m_NumberOfTracks = atoi( line.c_str() );
        PrintTrack( os, 2 );
        break;
      case 2:
        line = GetNValue( line, "<NumberOftrackfamily>", os, 2 );
        this->m_NumberOfTrackFamily = atoi( line.c_str() );
        PrintTrackFamily( os, 2 );
        break;
      case 3:
        line = GetNValue( line, "<NumberOfcelltype>", os, 2 );
        this->m_NumberOfCellTypes = atoi( line.c_str() );
        PrintCell( os, 2 );
        break;
      case 4:
        line = GetNValue( line, "<NumberOfcolor>", os, 2 );
        this->m_NumberOfColors = atoi( line.c_str() );
        PrintColor( os, 2 );
        break;
      case 5:
        line = GetNValue( line, "<NumberOflineage>", os, 2 );
        this->m_NumberOfLineages = atoi( line.c_str() );
        PrintLineage( os, 2 );
        break;
      case 6:
        line = GetNValue( line, "<NumberOfcontour>", os, 2 );
        this->m_NumberOfContours = atoi( line.c_str() );
        PrintContour( os, 2 );
        break;
      case 7:
        line = GetNValue( line, "<NumberOfintensity>", os, 2 );
        this->m_NumberOfIntensitys = atoi( line.c_str() );
        PrintIntensity( os, 2 );
        break;
      case 8:
        line = GetNValue( line, "<NumberOfsubcellularType>", os, 2 );
        this->m_NumberOfSubCellularTypes = atoi( line.c_str() );
        PrintSubCellular( os, 2 );
        break;
      case 9:
        line = GetNValue( line, "<NumberOfcoordinate>", os, 2 );
        this->m_NumberOfCoordinates = atoi( line.c_str() );
        PrintCoordinate( os, 2 );
        break;
      case 10:
        line = GetNValue( line, "<NumberOfchannel>", os, 2 );
        this->m_NumberOfChannels = atoi( line.c_str() );
        PrintChannel( os, 2 );
        break;
      case 11:
        count = 20;
        break;
      default:
        std::cout << "value of k unknown" << std::endl;
      }
      count++;
  }
}


template < class TSegmentImage >
std::string
GoFigure2TableToSegmentation< TSegmentImage >::
GetValue ( std::string start, std::istream& os, unsigned int len )
{
  std::string line;
  getline( os, line );
  line = line.substr( len + start.length(), line.length() - len - 2*start.length() - 1 );

  return line;
}


template < class TSegmentImage >
std::string
GoFigure2TableToSegmentation< TSegmentImage >::
GetNValue ( std::string line, std::string start, std::istream& os, unsigned int len )
{
  line = line.substr( len + start.length(), line.length() - len - 2*start.length() - 1 );

  return line;
}

template < class TSegmentImage >
int
GoFigure2TableToSegmentation< TSegmentImage >::
GetField ( std::string line, int k )
{
  std::transform(line.begin(), line.end(), line.begin(), toupper);
  size_t pos;
  bool flag = false;
  unsigned int count = 0;
  while( !flag )
  {
    pos = line.find( this->Fields[k][count] );
    if (pos!=std::string::npos)
    {
      flag = true;
    }
    else
    {
      count++;
    }
  }
  return count;
}


template < class TSegmentImage >
int
GoFigure2TableToSegmentation< TSegmentImage >::
GetTypeOfField ( std::string line )
{
  std::transform(line.begin(), line.end(), line.begin(), toupper);
  size_t pos;
  bool flag = false;
  unsigned int k = 0;
  while( !flag )
  {
    pos = line.find( this->TypeOfFields[k] );
    if (pos!=std::string::npos)
    {
      flag = true;
    }
    else
    {
      k++;
    }
  }
  return k;
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintStart ( std::istream& os, unsigned int indent )
{
  unsigned int len = static_cast<unsigned int>( indent );

  std::string line;
  getline( os, line ); //"<?xml version=\"1.0\" ?>";
  getline( os, line ); //"<ExportTraces version=\"1\"";
  getline( os, line ); //"  <imagingsession>";

  this->m_ImagingSession.Name = GetValue( "<Name>", os, len + 4 );
  this->m_ImagingSession.CreationDate = GetValue( "<CreationDate>", os, len + 4 );
  this->m_ImagingSession.MicroscopeName = GetValue( "<MicroscopeName>", os, len + 4 );

  //"  </imagingsession>"
  getline( os, line );
}

template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintEnd ( std::istream& os, unsigned int indent )
{
  std::string line;
  getline( os, line ); //"</ExportTraces>"
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintColor ( std::istream& os, unsigned int indent )
{
  unsigned int len = static_cast<unsigned int>( indent );
  unsigned int k;
  unsigned int colorID;

  std::string line;
  m_Colors.resize( this->m_NumberOfColors );

  for( unsigned int i = 0; i < this->m_NumberOfColors; i++ )
  {
    unsigned int count = 0;
    bool endOfColor = false;
    while ( count < Fields[4].size() && !endOfColor )
    {
      getline( os, line );
      k = GetField( line, 4 );
      switch ( k )
      {
        case 0:
          line = GetNValue( line, "<Alpha>", os, len + 2 );
          this->m_Colors[i].Alpha = atoi( line.c_str() );
          break;
        case 1:
          line = GetNValue( line, "<Blue>", os, len + 2 );
          this->m_Colors[i].Blue = atoi( line.c_str() );
          break;
        case 2:
          line = GetNValue( line, "<ColorID>", os, len + 2 );
          this->m_Colors[i].ColorID = atoi( line.c_str() );
          break;
        case 3:
          line = GetNValue( line, "<Description>", os, len + 2 );
          this->m_Colors[i].Description = line.c_str();
          break;
        case 4:
          line = GetNValue( line, "<Green>", os, len + 2 );
          this->m_Colors[i].Green = atoi( line.c_str() );
          break;
        case 5:
          line = GetNValue( line, "<Name>", os, len + 2 );
          this->m_Colors[i].Name = line.c_str();
          break;
        case 6:
          line = GetNValue( line, "<Red>", os, len + 2 );
          this->m_Colors[i].Red = atoi( line.c_str() );
          break;
        case 7: // <celltype>
          break;
        case 8: // </celltype>
          endOfColor = true;
          break;
        default:
          std::cout << "value of x unknown" << std::endl;
        }
        count++;
    }
  }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintCell ( std::istream& os, unsigned int indent )
{
  unsigned int len = static_cast<unsigned int>( indent );
  unsigned int k;
  unsigned int cellTypeID;

  std::string line;
  m_CellTypes.resize( this->m_NumberOfCellTypes );

  for( unsigned int i = 0; i < this->m_NumberOfCellTypes; i++ )
  {
    unsigned int count = 0;
    bool endOfCellType = false;
    while ( count < Fields[3].size() && !endOfCellType )
    {
      getline( os, line );
      k = GetField( line, 3 );
      switch ( k )
      {
      case 0:
        line = GetNValue( line, "<CellTypeID>", os, len + 2 );
        cellTypeID = atoi( line.c_str() );
        this->m_CellTypes[i].CellTypeID = cellTypeID;
        break;
      case 1:
        line = GetNValue( line, "<Description>", os, len + 2 );
        this->m_CellTypes[i].Description = line.c_str();
        break;
      case 2:
        line = GetNValue( line, "<Name>", os, len + 2 );
        this->m_CellTypes[i].Name = line.c_str();
        break;
      case 3: // <celltype>
        break;
      case 4: // </celltype>
        endOfCellType = true;
        break;
      default:
        std::cout << "value of x unknown" << std::endl;
      }
      count++;
    }
  }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintSubCellular( std::istream& os, unsigned int indent )
{
  unsigned int len = static_cast<unsigned int>( indent );
  unsigned int k;

  std::string line;
  m_SubCellularTypes.resize( this->m_NumberOfSubCellularTypes );

  for( unsigned int i = 0; i < this->m_NumberOfSubCellularTypes; i++ )
  {
    unsigned int count = 0;
    bool endOfSubcellularType = false;
    while ( count < Fields[8].size() && !endOfSubcellularType )
    {
      getline( os, line );
      k = GetField( line, 8 );
      switch ( k )
      {
      case 0:
        line = GetNValue( line, "<Description>", os, len + 2 );
        this->m_SubCellularTypes[i].Description = line.c_str();
        break;
      case 1:
        line = GetNValue( line, "<Name>", os, len + 2 );
        this->m_SubCellularTypes[i].Name = line.c_str();
        break;
      case 2:
        line = GetNValue( line, "<SubCellularID>", os, len + 2 );
        this->m_SubCellularTypes[i].SubCellularID = atoi( line.c_str() );
        break;
      case 3: // <subcellulartype>
        break;
      case 4: // </subcellulartype>
        endOfSubcellularType = true;
        break;
      default:
        std::cout << "value of x unknown" << std::endl;
      }
      count++;
    }
  }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintCoordinate ( std::istream& os, unsigned int indent )
{
  unsigned int len = static_cast<unsigned int>( indent );
  unsigned int coordID;
  unsigned int k;

  std::string line;
  m_Coordinates.resize( this->m_NumberOfCoordinates );

  for( unsigned int i = 0; i < this->m_NumberOfCoordinates; i++ )
  {
    unsigned int count = 0;
    bool endOfCoordinate = false;
    while ( count < Fields[9].size() && !endOfCoordinate )
    {
      getline( os, line );
      k = GetField( line, 9 );
        switch ( k )
        {
        case 0:
          line = GetNValue( line, "<CCoord>", os, len + 2 );
          this->m_Coordinates[i].CCoord = atoi( line.c_str() );
          break;
        case 1:
          line = GetNValue( line, "<CoordID>", os, len + 2 );
          coordID = atoi( line.c_str() );
          this->m_Coordinates[i].CoordID = coordID;
          break;
        case 2:
          line = GetNValue( line, "<PCoord>", os, len + 2 );
          this->m_Coordinates[i].PCoord = atoi( line.c_str() );
          break;
        case 3:
          line = GetNValue( line, "<RCoord>", os, len + 2 );
          this->m_Coordinates[i].RCoord = atoi( line.c_str() );
          break;
        case 4:
          line = GetNValue( line, "<TCoord>", os, len + 2 );
          this->m_Coordinates[i].TCoord = atoi( line.c_str() );
          break;
        case 5:
          line = GetNValue( line, "<XCoord>", os, len + 2 );
          this->m_Coordinates[i].XCoord = atoi( line.c_str() );
          break;
        case 6:
          line = GetNValue( line, "<XTileCoord>", os, len + 2 );
          this->m_Coordinates[i].XTileCoord = atoi( line.c_str() );
          break;
        case 7:
          line = GetNValue( line, "<YCoord>", os, len + 2 );
          this->m_Coordinates[i].YCoord = atoi( line.c_str() );
          break;
        case 8:
          line = GetNValue( line, "<YTileCoord>", os, len + 2 );
          this->m_Coordinates[i].YTileCoord = atoi( line.c_str() );
          break;
        case 9:
          line = GetNValue( line, "<ZCoord>", os, len + 2 );
          this->m_Coordinates[i].ZCoord = atoi( line.c_str() );
          break;
        case 10:
          line = GetNValue( line, "<ZTileCoord>", os, len + 2 );
          this->m_Coordinates[i].ZTileCoord = atoi( line.c_str() );
          break;
        case 11: // <coordinate>
          break;
        case 12: // </coordinate>
          endOfCoordinate = true;
          break;
        default:
          std::cout << "value of x unknown" << std::endl;
        }
        count++;
    }
    this->m_CoordID[coordID] = i;
  }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintLineage( std::istream& os, unsigned int indent )
{

  unsigned int len = static_cast<unsigned int>( indent );
  unsigned int k;
  unsigned int lineageID;

  std::string line;
  m_Lineages.resize( this->m_NumberOfLineages );

  for( unsigned int i = 0; i < this->m_NumberOfLineages; i++ )
  {
    unsigned int count = 0;
    bool endOfLineage = false;
    while ( count < Fields[5].size() && !endOfLineage )
    {
      getline( os, line );
      k = GetField( line, 5 );
      switch ( k )
      {
      case 0:
        line = GetNValue( line, "<ColorID>", os, len + 2 );
        this->m_Lineages[i].ColorID = atoi( line.c_str() );
        break;
      case 1:
        line = GetNValue( line, "<CoordIDMax>", os, len + 2 );
        this->m_Lineages[i].CoordIDMax = atoi( line.c_str() );
        break;
      case 2:
        line = GetNValue( line, "<CoordIDMin>", os, len + 2 );
        this->m_Lineages[i].CoordIDMin = atoi( line.c_str() );
        break;
      case 3:
        line = GetNValue( line, "<ImagingSessionID>", os, len + 2 );
        this->m_Lineages[i].ImagingSessionID = atoi( line.c_str() );
        break;
      case 4:
        line = GetNValue( line, "<Points>", os, len + 2 );
        this->m_Lineages[i].Points = line.c_str();
        break;
      case 5:
        line = GetNValue( line, "<TrackIDRoot>", os, len + 2 );
        this->m_Lineages[i].TrackIDRoot = atoi( line.c_str() );
        break;
      case 6:
        line = GetNValue( line, "<LineageID>", os, len + 2 );
        lineageID = atoi( line.c_str() );
        this->m_Lineages[i].LineageID = atoi( line.c_str() );
        break;
      case 7: // <lineage>
        break;
      case 8: // </lineage>
        endOfLineage = true;
        break;
      default:
        std::cout << "value of x unknown" << std::endl;
      }
      count++;
    }
    this->m_LineageID[lineageID] = i;
  }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintTrackFamily( std::istream& os, unsigned int indent )
{
  unsigned int trackFamilyID;
  unsigned int len = static_cast<unsigned int>( indent );
  unsigned int k;

  std::string line;
  m_TrackFamily.resize( this->m_NumberOfTrackFamily );

  for( unsigned int i = 0; i < this->m_NumberOfTrackFamily; i++ )
  {
    unsigned int count = 0;
    bool endOfTrackFamily = false;
    while ( count < Fields[2].size() && !endOfTrackFamily )
    {
      getline( os, line );
      k = GetField( line, 2 );

      switch ( k )
      {
      case 0:
        line = GetNValue( line, "<TrackFamilyID>", os, len + 2 );
        trackFamilyID = atoi( line.c_str() );
        this->m_TrackFamily[i].TrackFamilyID = trackFamilyID;
        break;
      case 1:
        line = GetNValue( line, "<TrackIDDaughter1>", os, len + 2 );
        this->m_TrackFamily[i].TrackDaughter1ID = atoi( line.c_str() );
        break;
      case 2:
        line = GetNValue( line, "<TrackIDDaughter2>", os, len + 2 );
        this->m_TrackFamily[i].TrackDaughter2ID = atoi( line.c_str() );
        break;
      case 3:
        line = GetNValue( line, "<TrackIDMother>", os, len + 2 );
        this->m_TrackFamily[i].TrackMotherID = atoi( line.c_str() );
        break;
      case 4: // <trackfamily>
        break;
      case 5: // </trackfamily>
        endOfTrackFamily = true;
        break;
      default:
        std::cout << "value of x unknown" << std::endl;
      }
      count++;
    }
    this->m_TrackFamilyID[trackFamilyID] = i;
  }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintTrack ( std::istream& os, unsigned int indent )
{
  unsigned int trackID, lineageID;
  unsigned int len = static_cast<unsigned int>( indent );
  unsigned int k;

  std::string line;
  m_Tracks.resize( this->m_NumberOfTracks );

  for( unsigned int i = 0; i < this->m_Tracks.size(); i++ )
    {
    unsigned int count = 0;
    bool endOfTrack = false;
    while ( count < Fields[1].size() && !endOfTrack )
    {
      getline( os, line );
      k = GetField( line, 1 );
      switch ( k )
      {
        case 0:
          line = GetNValue( line, "<ColorID>", os, len + 2 );
          this->m_Tracks[i].ColorID = atoi( line.c_str() );
          break;
        case 1:
          line = GetNValue( line, "<CoordIDMax>", os, len + 2 );
          this->m_Tracks[i].CoordIDMax = atoi( line.c_str() );
          break;
        case 2:
          line = GetNValue( line, "<CoordIDMin>", os, len + 2 );
          this->m_Tracks[i].CoordIDMin = atoi( line.c_str() );
          break;
        case 3:
          line = GetNValue( line, "<ImagingSessionID>", os, len + 2 );
          this->m_Tracks[i].ImagingSessionID = atoi( line.c_str() );
          break;
        case 4:
          this->m_Tracks[i].Points = GetNValue( line, "<Points>", os, len + 2 );
          break;
        case 5:
          line = GetNValue( line, "<LineageID>", os, len + 2 );
          lineageID = atoi( line.c_str() );
          this->m_Tracks[i].LineageID = lineageID;
          break;
        case 6:
          line = GetNValue( line, "<TrackFamilyID>", os, len + 2 );
          this->m_Tracks[i].TrackFamilyID = atoi( line.c_str() );
          break;
        case 7:
          line = GetNValue( line, "<trackID>", os, len + 2 );
          trackID = atoi( line.c_str() );
          this->m_Tracks[i].TrackID = trackID;
          break;
        case 8: // </track>
          break;
        case 9: // </track>
          endOfTrack = true;
          break;
        default:
          std::cout << "value of x unknown" << std::endl;
      }
      count++;
    }
    this->m_TrackID[trackID] = i;
    this->m_LineageLookupFromTrackID[trackID] = lineageID;
    }
}

template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintMesh ( std::istream& os, unsigned int indent )
{
  unsigned int meshID;
  unsigned int trackID;
  unsigned int coordidmin;
  unsigned int k;
  unsigned int len = static_cast<unsigned int>( indent );

  std::string line;
  m_Meshes.resize( this->m_NumberOfMeshes );

  for( unsigned int i = 0; i < this->m_Meshes.size(); i++ )
  {
    unsigned int count = 0;
    bool endOfMesh = false;
    while ( count < Fields[0].size() && !endOfMesh )
    {
      getline( os, line );

      k = GetField( line, 0 );

      switch ( k )
      {
        case 0:
          line = GetNValue( line, "<CellTypeID>", os, len + 2 );
          this->m_Meshes[i].CellTypeID = atoi( line.c_str() );
          break;
        case 1:
          line = GetNValue( line, "<ColorID>", os, len + 2 );
          this->m_Meshes[i].ColorID = atoi( line.c_str() );
          break;
        case 2:
          line = GetNValue( line, "<CoordIDMax>", os, len + 2 );
          this->m_Meshes[i].CoordIDMax = atoi( line.c_str() );
          break;
        case 3:
          line = GetNValue( line, "<CoordIDMin>", os, len + 2 );
          coordidmin = atoi( line.c_str() );
          this->m_Meshes[i].CoordIDMin = coordidmin;
          break;
        case 4:
          line = GetNValue( line, "<ImagingSessionID>", os, len + 2 );
          this->m_Meshes[i].ImagingSessionID = atoi( line.c_str() );
          break;
        case 5:
          line = GetNValue( line, "<meshID>", os, len + 2 );
          meshID = atoi( line.c_str() );
          this->m_Meshes[i].MeshID = meshID;
          break;
        case 6:
          this->m_Meshes[i].Points = GetNValue( line, "<Points>", os, len + 2 );
          break;
        case 7:
          line = GetNValue( line, "<SubCellularID>", os, len + 2 );
          this->m_Meshes[i].SubCellularTypeID = atoi( line.c_str() );
          break;
        case 8:
          line = GetNValue( line, "<trackID>", os, len + 2 );
          trackID = atoi( line.c_str() );
          this->m_Meshes[i].TrackID = trackID;
          break;
        case 9: // <mesh>
          break;
        case 10: // </mesh>
          endOfMesh = true;
          break;
        default:
          std::cout << "value of x unknown" << std::endl;
      }
      count++;
    }
    this->m_MeshID[meshID] = i;
    unsigned int j = this->m_TrackID[trackID];
    unsigned int p = this->m_CoordID[coordidmin];
    unsigned int t = this->m_Coordinates[p].TCoord;
    if ( this->m_NumberOfTracks > 0 )
    {
      this->m_Tracks[j].MeshIDFromTCoordLookup[t] = meshID;
    }
  }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintContour ( std::istream& os, unsigned int indent )
{
  unsigned int len = static_cast<unsigned int>( indent );
  unsigned int k;

  std::string line;
  m_Contours.resize( this->m_NumberOfContours );

  for( unsigned int i = 0; i < this->m_NumberOfContours; i++ )
  {
      unsigned int count = 0;
      bool endOfContour = false;
      while ( count < Fields[6].size() && !endOfContour )
      {
        getline( os, line );
        k = GetField( line, 6 );
        switch ( k )
        {
          case 0:
            line = GetNValue( line, "<ColorID>", os, len + 2 );
            this->m_Contours[i].ColorID = atoi( line.c_str() );
            break;
          case 1:
            line = GetNValue( line, "<CoordIDMax>", os, len + 2 );
            this->m_Contours[i].CoordIDMax = atoi( line.c_str() );
            break;
          case 2:
            line = GetNValue( line, "<CoordIDMin>", os, len + 2 );
            this->m_Contours[i].CoordIDMin = atoi( line.c_str() );
            break;
          case 3:
            line = GetNValue( line, "<ImagingSessionID>", os, len + 2 );
            this->m_Contours[i].ImagingSessionID = atoi( line.c_str() );
            break;
          case 4:
            line = GetNValue( line, "<Points>", os, len + 2 );
            this->m_Contours[i].Points = line.c_str();
            break;
          case 5:
            line = GetNValue( line, "<ContourID>", os, len + 2 );
            this->m_Contours[i].ContourID = atoi( line.c_str() );
            break;
          case 6:
            line = GetNValue( line, "<MeshID>", os, len + 2 );
            this->m_Contours[i].MeshID = atoi( line.c_str() );
            break;
          case 7: // <celltype>
            break;
          case 8: // </celltype>
            endOfContour = true;
            break;
          default:
            std::cout << "value of x unknown" << std::endl;
          }
          count++;
      }
  }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintChannel ( std::istream& os, unsigned int indent )
{
  unsigned int len = static_cast<unsigned int>( indent );
  unsigned int k;

  std::string line;
  m_Channels.resize( this->m_NumberOfChannels );

  for( unsigned int i = 0; i < this->m_NumberOfChannels; i++ )
  {
    unsigned int count = 0;
    bool endOfChannel = false;
    while ( count < Fields[7].size() && !endOfChannel )
    {
      getline( os, line );
      k = GetField( line, 7 );
      switch ( k )
      {
        case 0:
          line = GetNValue( line, "<ChannelID>", os, len + 2 );
          this->m_Channels[i].ChannelID = atoi( line.c_str() );
          break;
        case 1:
          line = GetNValue( line, "<ChannelNumber>", os, len + 2 );
          this->m_Channels[i].ChannelNumber = atoi( line.c_str() );
          break;
        case 2:
          line = GetNValue( line, "<ColorID>", os, len + 2 );
          this->m_Channels[i].ColorID = atoi( line.c_str() );
          break;
        case 3:
          line = GetNValue( line, "<ImagingSessionID>", os, len + 2 );
          this->m_Channels[i].ImagingSessionID = atoi( line.c_str() );
          break;
        case 4:
          line = GetNValue( line, "<Name>", os, len + 2 );
          this->m_Channels[i].Name = line.c_str();
          break;
        case 5:
          line = GetNValue( line, "<NumberOfBits>", os, len + 2 );
          this->m_Channels[i].NumberOfBits = atoi( line.c_str() );
          break;
        case 6: // <celltype>
          break;
        case 7: // </celltype>
          endOfChannel = true;
          break;
        default:
          std::cout << "value of x unknown" << std::endl;
      }
      count++;
    }
  }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintIntensity ( std::istream& os, unsigned int indent )
{
  unsigned int len = static_cast<unsigned int>( indent );
  unsigned int k;

  std::string line;
  m_Intensitys.resize( this->m_NumberOfIntensitys );

  for( unsigned int i = 0; i < this->m_Intensitys.size(); i++ )
  {
    unsigned int count = 0;
    bool endOfIntensity = false;
    while ( count < Fields[10].size() && !endOfIntensity )
    {
      getline( os, line );
      k = GetField( line, 10 );
      switch ( k )
      {
        case 0:
          line = GetNValue( line, "<ChannelID>", os, len + 2 );
          this->m_Intensitys[i].ChannelID = atoi( line.c_str() );
          break;
        case 1:
          line = GetNValue( line, "<IntensityID>", os, len + 2 );
          this->m_Intensitys[i].IntensityID = atoi( line.c_str() );
          break;
        case 2:
          line = GetNValue( line, "<Value>", os, len + 2 );
          this->m_Intensitys[i].Value = atoi( line.c_str() );
          break;
        case 3:
          line = GetNValue( line, "<MeshID>", os, len + 2 );
          this->m_Intensitys[i].MeshID = atoi( line.c_str() );
          break;
        case 4: // <intensity>
          break;
        case 5: // </intensity>
          endOfIntensity = true;
          break;
        default:
          std::cout << "value of x unknown" << std::endl;
      }
      count++;
    }
  }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
WriteStart ( std::ostream& os, Indent indent )
{
  os << indent << "<?xml version=\"1.0\" ?>" << std::endl;
  os << indent << "<ExportTraces version=\"1\">" << std::endl;
  os << indent << "  <imagingsession>" << std::endl;
  os << indent << "    <Name>" <<this->m_ImagingSession.Name <<"</Name>" << std::endl;
  os << indent << "    <CreationDate>" <<this->m_ImagingSession.CreationDate <<"</CreationDate>" << std::endl;
  os << indent << "    <MicroscopeName>" <<this->m_ImagingSession.MicroscopeName <<"</MicroscopeName>" << std::endl;
  os << indent << "  </imagingsession>" << std::endl;

  m_NumberOfMeshes = this->m_Meshes.size();
  m_NumberOfTracks = this->m_Tracks.size();
  m_NumberOfCoordinates = this->m_Coordinates.size();
}

template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
WriteColor ( std::ostream& os, Indent indent ) const
{
  os << indent << "<NumberOfcolor>" << this->m_NumberOfColors << "</NumberOfcolor>" << std::endl;
  for( unsigned int i = 0; i < this->m_Colors.size(); i++ )
    {
    os << indent << "<color>" << std::endl;
    os << indent << "  <Alpha>" << m_Colors[i].Alpha << "</Alpha>" << std::endl;
    os << indent << "  <Blue>" << m_Colors[i].Blue << "</Blue>" << std::endl;
    os << indent << "  <ColorID>" <<  m_Colors[i].ColorID << "</ColorID>" << std::endl;
    os << indent << "  <Description>" <<  m_Colors[i].Description << "</Description>" << std::endl;
    os << indent << "  <Green>" << m_Colors[i].Green << "</Green>" << std::endl;
    os << indent << "  <Name>" <<  m_Colors[i].Name << "</Name>" << std::endl;
    os << indent << "  <Red>" << m_Colors[i].Red << "</Red>" << std::endl;
    os << indent << "</color>" << std::endl;
    }
}

template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
WriteEnd ( std::ostream& os, Indent indent ) const
{
  os << indent << "</ExportTraces>" << std::endl;
}

template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
WriteCell ( std::ostream& os, Indent indent ) const
{
  os << indent << "<NumberOfcelltype>" << this->m_NumberOfCellTypes << "</NumberOfcelltype>" << std::endl;
  for( unsigned int i = 0; i < this->m_CellTypes.size(); i++ )
    {
    os << indent << "<celltype>" << std::endl;
    os << indent << "  <CellTypeID>" <<  this->m_CellTypes[i].CellTypeID << "</CellTypeID>" << std::endl;
    os << indent << "  <Description>" <<  this->m_CellTypes[i].Description << "</Description>" << std::endl;
    os << indent << "  <Name>" <<  this->m_CellTypes[i].Name << "</Name>" << std::endl;
    os << indent << "</celltype>" << std::endl;
    }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
WriteSubCellular( std::ostream& os, Indent indent ) const
{
  os << indent << "<NumberOfsubcellulartype>" << this->m_NumberOfSubCellularTypes << "</NumberOfsubcellulartype>" << std::endl;
  for( unsigned int i = 0; i < this->m_SubCellularTypes.size(); i++ )
    {
    os << indent << "<subcellulartype>" << std::endl;
    os << indent << "  <Description>" <<  this->m_SubCellularTypes[i].Description << "</Description>" << std::endl;
    os << indent << "  <Name>" <<  this->m_SubCellularTypes[i].Name << "</Name>" << std::endl;
    os << indent << "  <SubCellularID>" <<  this->m_SubCellularTypes[i].SubCellularID<< "</SubCellularID>" << std::endl;
    os << indent << "</subcellulartype>" << std::endl;
    }
}



template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
WriteChannel ( std::ostream& os, Indent indent ) const
{
  os << indent << "<NumberOfchannel>" << this->m_NumberOfChannels << "</NumberOfchannel>" << std::endl;
  for( unsigned int i = 0; i < this->m_Channels.size(); i++ )
    {
    os << indent << "<channel>" << std::endl;
    os << indent << "  <ChannelID>" << m_Channels[i].ChannelID << "</ChannelID>" << std::endl;
    os << indent << "  <ChannelNumber>" << m_Channels[i].ChannelNumber << "</ChannelNumber>" << std::endl;
    os << indent << "  <ColorID>" << m_Channels[i].ColorID << "</ColorID>" << std::endl;
    os << indent << "  <ImagingSessionID>" << m_Channels[i].ImagingSessionID << "</ImagingSessionID>" << std::endl;
    os << indent << "  <Name>" <<  this->m_Channels[i].Name << "</Name>" << std::endl;
    os << indent << "  <NumberOfBits>" <<  m_Channels[i].NumberOfBits << "</NumberOfBits>" << std::endl;
    os << indent << "</channel>" << std::endl;
    }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
WriteIntensity ( std::ostream& os, Indent indent ) const
{
  os << indent << "<NumberOfintensity>" << this->m_NumberOfIntensitys << "</NumberOfintensity>" << std::endl;
  for( unsigned int i = 0; i < this->m_Intensitys.size(); i++ )
    {
    os << indent << "<intensity>" << std::endl;
    os << indent << "  <ChannelID>" << m_Intensitys[i].ChannelID << "</ChannelID>" << std::endl;
    os << indent << "  <IntensityID>" << m_Intensitys[i].IntensityID << "</IntensityID>" << std::endl;
    os << indent << "  <Value>" << m_Intensitys[i].Value << "</Value>" << std::endl;
    os << indent << "  <meshID>" << m_Intensitys[i].MeshID << "</meshID>" << std::endl;
    os << indent << "</intensity>" << std::endl;
    }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
WriteCoordinate ( std::ostream& os, Indent indent ) const
{
  os << indent << "<NumberOfcoordinate>" << this->m_NumberOfCoordinates << "</NumberOfcoordinate>" << std::endl;
  for( unsigned int i = 0; i < this->m_Coordinates.size(); i++ )
    {
    os << indent << "<coordinate>" << std::endl;
    os << indent << "  <CCoord>" << m_Coordinates[i].CCoord << "</CCoord>" << std::endl;
    os << indent << "  <CoordID>" << m_Coordinates[i].CoordID << "</CoordID>" << std::endl;
    os << indent << "  <PCoord>" << m_Coordinates[i].PCoord << "</PCoord>" << std::endl;
    os << indent << "  <RCoord>" << m_Coordinates[i].RCoord << "</RCoord>" << std::endl;
    os << indent << "  <TCoord>" << m_Coordinates[i].TCoord << "</TCoord>" << std::endl;
    os << indent << "  <XCoord>" << m_Coordinates[i].XCoord << "</XCoord>" << std::endl;
    os << indent << "  <XTileCoord>" << m_Coordinates[i].XTileCoord << "</XTileCoord>" << std::endl;
    os << indent << "  <YCoord>" << m_Coordinates[i].YCoord << "</YCoord>" << std::endl;
    os << indent << "  <YTileCoord>" << m_Coordinates[i].YTileCoord << "</YTileCoord>" << std::endl;
    os << indent << "  <ZCoord>" << m_Coordinates[i].ZCoord << "</ZCoord>" << std::endl;
    os << indent << "  <ZTileCoord>" << m_Coordinates[i].ZTileCoord << "</ZTileCoord>" << std::endl;
    os << indent << "</coordinate>" << std::endl;
    }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
WriteLineage( std::ostream& os, Indent indent ) const
{
  os << indent << "<NumberOflineage>" << this->m_NumberOfLineages << "</NumberOflineage>" << std::endl;
  for( unsigned int i = 0; i < this->m_Lineages.size(); i++ )
    {
    os << indent << "<lineage>" << std::endl;
    os << indent << "  <ColorID>" << m_Lineages[i].ColorID << "</ColorID>" << std::endl;
    os << indent << "  <CoordIDMax>" << m_Lineages[i].CoordIDMax << "</CoordIDMax>" << std::endl;
    os << indent << "  <CoordIDMin>" << m_Lineages[i].CoordIDMin << "</CoordIDMin>" << std::endl;
    os << indent << "  <ImagingSessionID>" << m_Lineages[i].ImagingSessionID << "</ImagingSessionID>" << std::endl;
    os << indent << "  <Points>" << m_Lineages[i].Points << "</Points>" << std::endl;
    os << indent << "  <TrackIDRoot>" << m_Lineages[i].TrackIDRoot << "</TrackIDRoot>" << std::endl;
    os << indent << "  <lineageID>" << m_Lineages[i].LineageID << "</lineageID>" << std::endl;
    os << indent << "</lineage>" << std::endl;
    }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
WriteTrack ( std::ostream& os, Indent indent ) const
{
  os << indent << "<NumberOftrack>" << this->m_NumberOfTracks << "</NumberOftrack>" << std::endl;
  for( unsigned int i = 0; i < this->m_Tracks.size(); i++ )
    {
    os << indent << "<track>" << std::endl;
    os << indent << "  <ColorID>" << m_Tracks[i].ColorID << "</ColorID>" << std::endl;
    os << indent << "  <CoordIDMax>" << m_Tracks[i].CoordIDMax << "</CoordIDMax>" << std::endl;
    os << indent << "  <CoordIDMin>" << m_Tracks[i].CoordIDMin << "</CoordIDMin>" << std::endl;
    os << indent << "  <ImagingSessionID>" << m_Tracks[i].ImagingSessionID << "</ImagingSessionID>" << std::endl;
     os << indent << "  <Points>" << m_Tracks[i].Points << "</Points>" << std::endl;
    os << indent << "  <TrackFamilyID>" << m_Tracks[i].TrackFamilyID << "</TrackFamilyID>" << std::endl;
    os << indent << "  <lineageID>" << m_Tracks[i].LineageID << "</lineageID>" << std::endl;
    os << indent << "  <trackID>" << m_Tracks[i].TrackID << "</trackID>" << std::endl;
    os << indent << "</track>" << std::endl;
    }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
WriteTrackFamily ( std::ostream& os, Indent indent ) const
{
  os << indent << "<NumberOftrackfamily>" << this->m_NumberOfTrackFamily << "</NumberOftrackfamily>" << std::endl;
  for( unsigned int i = 0; i < this->m_TrackFamily.size(); i++ )
    {
    os << indent << "<trackfamily>" << std::endl;
    os << indent << "  <TrackFamilyID>" << m_TrackFamily[i].TrackFamilyID << "</TrackFamilyID>" << std::endl;
    os << indent << "  <TrackIDDaughter1>" << m_TrackFamily[i].TrackDaughter1ID << "</TrackIDDaughter1>" << std::endl;
    os << indent << "  <TrackIDDaughter2>" << m_TrackFamily[i].TrackDaughter2ID << "</TrackIDDaughter2>" << std::endl;
    os << indent << "  <TrackIDMother>" << m_TrackFamily[i].TrackMotherID << "</TrackIDMother>" << std::endl;
    os << indent << "</trackfamily>" << std::endl;
    }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
WriteMesh ( std::ostream& os, Indent indent ) const
{
  os << indent << "<NumberOfmesh>" << this->m_NumberOfMeshes << "</NumberOfmesh>" << std::endl;
  for( unsigned int i = 0; i < this->m_Meshes.size(); i++ )
    {
    os << indent << "<mesh>" << std::endl;
    os << indent << "  <CellTypeID>" << m_Meshes[i].CellTypeID << "</CellTypeID>" << std::endl;
    os << indent << "  <ColorID>" << m_Meshes[i].ColorID << "</ColorID>" << std::endl;
    os << indent << "  <CoordIDMax>" << m_Meshes[i].CoordIDMax << "</CoordIDMax>" << std::endl;
    os << indent << "  <CoordIDMin>" << m_Meshes[i].CoordIDMin << "</CoordIDMin>" << std::endl;
    os << indent << "  <ImagingSessionID>" << m_Meshes[i].ImagingSessionID << "</ImagingSessionID>" << std::endl;
    os << indent << "  <Points>" << m_Meshes[i].Points << "</Points>" << std::endl;
    os << indent << "  <SubCellularID>" << m_Meshes[i].SubCellularTypeID << "</SubCellularID>" << std::endl;
    os << indent << "  <meshID>" << m_Meshes[i].MeshID << "</meshID>" << std::endl;
    os << indent << "  <trackID>" << m_Meshes[i].TrackID << "</trackID>" << std::endl;
    os << indent << "</mesh>" << std::endl;
    }
}



} /* end namespace itk */

#endif
