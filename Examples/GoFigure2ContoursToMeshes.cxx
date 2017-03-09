/*=========================================================================
  Author: $Author: krm15 $  // Author of last commit
  Version: $Rev: 1473 $  // Revision of last commit
  Date: $Date: 2010-05-18 17:31:00 -0400 (Tue, 18 May 2010) $  // Date of last commit
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

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkVector.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

#include "itkVector.h"
#include "vtkPolyData.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyDataWriter.h"
#include "vtkSmartPointer.h"
#include "vtkPolyDataMySQLTextReader.h"
#include "vtkPolyDataMySQLTextWriter.h"
#include "itkGoFigure2TableToSegmentation.h"
#include <vnl/vnl_vector.h>
#include "itkListSample.h"

#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyDataWriter.h"
#include "vtkDelaunay3D.h"
#include "vtkMassProperties.h"
#include "vtkGeometryFilter.h"

#include "itkConvertMeshesToLabelImageFilter.h"
#include "itkVTKPolyDataReader.h"
#include "itkQuadEdgeMesh.h"

#include "vtkTriangleFilter.h"
#include <vtkPowerCrustSurfaceReconstruction.h>

int main ( int argc, char* argv[] )
{
  if ( argc < 2 )
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " importFile <sampleVolume>" << std::endl;
    return EXIT_FAILURE;
  }

  const unsigned int Dimension = 3;
  typedef itk::Image< unsigned int, Dimension >  SegmentImageType;
  typedef itk::ImageFileReader< SegmentImageType > ReaderType;

  typedef float CoordType;
  typedef itk::QuadEdgeMesh< CoordType, Dimension >    MeshType;
  typedef itk::VTKPolyDataReader< MeshType >           MeshReaderType;
  typedef itk::ConvertMeshesToLabelImageFilter< SegmentImageType > ImageSourceType;
  typedef itk::ImageFileWriter< SegmentImageType >     WriterType;

  typedef vnl_vector< unsigned int > VectorType;
  typedef itk::GoFigure2TableToSegmentation< SegmentImageType > FilterType;
  std::map< int, int > coordMap;
  std::map< int, vtkSmartPointer<vtkPoints> > m_ColorsToPoints;

  bool UsePowercrust = true;

  // Update the contents of the filter
  FilterType::Pointer filter = FilterType::New();

  // Open and read the import file till end
  std::ifstream importFile( argv[1] );
  std::string fname, fname2, fname3;
  int coordID = 0;
  unsigned int timePt = 0, colorID = 0, meshID;
  std::string line;
  double pt[3];

  SegmentImageType::Pointer infoVolume;
  if ( argc > 2 )
  {
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName ( argv[2] );
    reader->Update();
    infoVolume = reader->GetOutput();
    infoVolume->DisconnectPipeline();
  }

  if ( importFile.is_open() )
  {
    filter->Read( importFile );

    // Fill a map of coord ids since they are not 0 indexed
    // in GoFigure2 output
    for( unsigned int i = 0; i < filter->m_Coordinates.size(); i++ )
    {
      coordID = filter->m_Coordinates[i].CoordID;
      coordMap[coordID] = i;
    }

    // Find the largest time-point
    unsigned int m_NumberOfTimePoints = 0;
    for( unsigned int i = 0; i < filter->m_Contours.size(); i++ )
    {
      coordID = filter->m_Contours[i].CoordIDMin;

      coordID = coordMap[coordID];
      timePt = filter->m_Coordinates[coordID].TCoord;
      if ( timePt > m_NumberOfTimePoints )
      {
        m_NumberOfTimePoints = timePt;
      }
    }

    std::cout << m_NumberOfTimePoints << std::endl;

    for ( unsigned int time = 0; time <= m_NumberOfTimePoints; time++ )
    {
      m_ColorsToPoints.clear();
      for( unsigned int i = 0; i < filter->m_Colors.size(); i++ )
      {
        m_ColorsToPoints[ filter->m_Colors[i].ColorID ] = vtkSmartPointer<vtkPoints>::New();
      }

      unsigned int count = 1;
      for( unsigned int i = 0; i < filter->m_Contours.size(); i++ )
      {
        coordID = filter->m_Contours[i].CoordIDMin;
        coordID = coordMap[coordID];

        timePt = filter->m_Coordinates[coordID].TCoord;
        colorID = filter->m_Contours[i].ColorID;
        meshID = filter->m_Contours[i].MeshID;

        if ( time == timePt )
        {
          vtkPolyDataMySQLTextReader* convert_reader = vtkPolyDataMySQLTextReader::New();
          convert_reader->SetIsContour( 1 );
          vtkPolyData* output = convert_reader->GetPolyData( filter->m_Contours[i].Points );

          // Collect all points at a given color
          for( unsigned int j = 0; j < output->GetNumberOfPoints(); j++ )
          {
            output->GetPoints()->GetPoint( j, pt );
            m_ColorsToPoints[colorID]->InsertNextPoint( pt );
            std::cout << colorID << std::endl;
          }
        }
      }

      // for each color is a list of points
      for( unsigned int i = 0; i < filter->m_Colors.size(); i++ )
      {
        colorID = filter->m_Colors[i].ColorID;
        if ( m_ColorsToPoints[colorID]->GetNumberOfPoints() > 0 )
        {
          std::cout << "Number of points:" << m_ColorsToPoints[colorID]->GetNumberOfPoints() << std::endl;
          std::ostringstream istream;
          istream << time << "_" << colorID << ".vtk";
          fname = istream.str();
          std::cout << fname << std::endl;

          std::ostringstream istream2;
          istream2 << "0" << "_" << colorID << ".txt";
          fname2 = istream2.str();
          std::cout << fname2 << std::endl;
          ofstream outfile ( fname2.c_str(), ios::out | ios::app );

          vtkSmartPointer< vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
          polydata->SetPoints( m_ColorsToPoints[ colorID ] );

          vtkPolyData *output = vtkPolyData::New();
          if ( UsePowercrust )
          {
            std::cout << "Using Powercrust" << std::endl;
            vtkSmartPointer<vtkPowerCrustSurfaceReconstruction> reconstruct =
              vtkSmartPointer<vtkPowerCrustSurfaceReconstruction>::New();
            reconstruct->SetInputData( polydata );
            reconstruct->Update();

            vtkSmartPointer<vtkTriangleFilter> triangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
            triangleFilter->SetInputData( reconstruct->GetOutput() );
            triangleFilter->Update();
            output->ShallowCopy( triangleFilter->GetOutput() );
          }
          else
          {
            std::cout << "Using convex hull" << std::endl;
            // Create the convex hull of the pointcloud
            vtkSmartPointer<vtkDelaunay3D> delaunay = vtkSmartPointer< vtkDelaunay3D >::New();
            delaunay->SetInputData(polydata);
            delaunay->SetAlpha( 0 );
            delaunay->SetTolerance( 0.3 );
            delaunay->SetOffset( 5.0 );
            delaunay->Update();

            vtkSmartPointer<vtkGeometryFilter> surfaceFilter =
              vtkSmartPointer<vtkGeometryFilter>::New();
            surfaceFilter->SetInputConnection( delaunay->GetOutputPort() );
            surfaceFilter->Update();
            output->ShallowCopy( surfaceFilter->GetOutput() );
          }

          vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
          writer->SetInputData( output );
          writer->SetFileName( fname.c_str() );
          writer->Write();
          std::cout << "Powercrust computed" << std::endl;

          vtkSmartPointer<vtkMassProperties> massProperty = vtkSmartPointer<vtkMassProperties>::New();
          massProperty->SetInputData( output );
          outfile << massProperty->GetVolume() << ' ' << massProperty->GetSurfaceArea() << std::endl;
          std::cout << "Mesh computation and quantification complete" << std::endl;

          if ( argc > 2 )
          {
            std::ostringstream istream3;
            istream3 << time << "_" << colorID << ".mha";
            fname3 = istream3.str();
            std::cout << fname3 << std::endl;

            SegmentImageType::Pointer m_OutputImage = SegmentImageType::New();
            m_OutputImage->CopyInformation( infoVolume );
            m_OutputImage->SetRegions( infoVolume->GetLargestPossibleRegion() );
            m_OutputImage->Allocate();
            m_OutputImage->FillBuffer( 0 );

            MeshReaderType::Pointer meshReader = MeshReaderType::New( );
            meshReader->SetFileName( fname.c_str() );
            meshReader->Update();

            ImageSourceType::Pointer imageSource = ImageSourceType::New();
            imageSource->m_Meshes.resize( 1 );
            imageSource->m_Meshes[0] = meshReader->GetOutput();
            imageSource->m_Meshes[0]->DisconnectPipeline();
            imageSource->SetInput( m_OutputImage );
            imageSource->SetNumberOfThreads( 1 );
            imageSource->GenerateData();

            WriterType::Pointer writer = WriterType::New( );
            writer->SetInput( m_OutputImage );
            writer->SetFileName( fname3.c_str() );
            writer->Update();
          }

          outfile.close();
        }
      }
    }

    importFile.close();
  }

  return EXIT_SUCCESS;
}
