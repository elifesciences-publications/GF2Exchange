/*=========================================================================
  Author: $Author: krm15 $  // Author of last commit
  Version: $Rev: 585 $  // Revision of last commit
  Date: $Date: 2009-08-20 21:25:19 -0400 (Thu, 20 Aug 2009) $  // Date of last commit
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

#ifndef __itkExtractMeshesFromLabelImageFilter_txx
#define __itkExtractMeshesFromLabelImageFilter_txx

#include "itkExtractMeshesFromLabelImageFilter.h"

static itk::SimpleFastMutexLock m_Mutex;

namespace itk
{

template< class TImage >
ExtractMeshesFromLabelImageFilter< TImage >
::ExtractMeshesFromLabelImageFilter()
{
  m_NumberOfMeshes = 0;
  m_NumberOfTrianglesPerMesh = 200;
  m_NumberOfSmoothingIterations = 15;
  m_SmoothingRelaxationFactor = 1.0;
  m_DelaunayConforming = true;
  m_NumberOfThreads = 1;
  m_UseSmoothing = true;
  m_UseDecimation = true;
}


template< class TImage >
void
ExtractMeshesFromLabelImageFilter< TImage >::
GenerateData()
{
  ShapeConverterPointer shapeConverter = ShapeConverterType::New();
  shapeConverter->SetInput ( m_Input );
  shapeConverter->SetBackgroundValue ( 0 );
  shapeConverter->Update();
  m_ShapeLabelMap = shapeConverter->GetOutput();

  m_NumberOfMeshes = m_ShapeLabelMap->GetNumberOfLabelObjects();

  this->m_Meshes.resize( m_NumberOfMeshes );

  if ( m_NumberOfMeshes <= m_NumberOfThreads )
  {
    m_NumberOfThreads = m_NumberOfMeshes;
  }
  std::cout << "Number of meshes: " << m_NumberOfMeshes << std::endl;
  std::cout << "Number of threads: " << m_NumberOfThreads << std::endl;

  ThreadStruct str;
  str.Filter  = this;

  ThreaderPointer threader = ThreaderType::New();
  threader->SetNumberOfThreads( m_NumberOfThreads );
  threader->SetSingleMethod( this->ThreaderCallback, &str );
  threader->SingleMethodExecute();
}

template< class TImage >
ITK_THREAD_RETURN_TYPE
ExtractMeshesFromLabelImageFilter< TImage >::
ThreaderCallback(void * arg)
{
  unsigned int ThreadId = ((MultiThreader::ThreadInfoStruct *)(arg))->ThreadID;

  ThreadStruct * str =
  (ThreadStruct *) (((MultiThreader::ThreadInfoStruct *)(arg))->UserData);

  unsigned int numOfMeshes = str->Filter->m_NumberOfMeshes;
  unsigned int numOfThreads = str->Filter->m_NumberOfThreads;

//  m_Mutex.Lock();
//  std::cout << ThreadId << " == " << "Start Seeds processing" << std::endl;
//  m_Mutex.Unlock();

  unsigned int MeshChunk = vcl_floor(static_cast<double>(numOfMeshes)/ numOfThreads);
  unsigned int startLabel = ThreadId * MeshChunk + 1;
  unsigned int endLabel = ThreadId * MeshChunk + MeshChunk;

  if ( ThreadId == numOfThreads-1 )
  {
    endLabel = numOfMeshes;
  }

//  m_Mutex.Lock();
//  std::cout << ThreadId << " == " << startLabel << ' ' << endLabel << std::endl;
//  m_Mutex.Unlock();

  str->Filter->ThreadedExtractMesh( startLabel, endLabel );

//  m_Mutex.Lock();
//  std::cout << ThreadId << " == " << "End Seeds processing" << std::endl;
//  m_Mutex.Unlock();

  return ITK_THREAD_RETURN_VALUE;
}


template< class TImage >
void
ExtractMeshesFromLabelImageFilter< TImage >::
ThreadedExtractMesh( unsigned int startLabel, unsigned int endLabel )
{
  SizeType size;
  IndexType index;
  RegionType region;
  SizeType inputSize = m_Input->GetLargestPossibleRegion().GetSize();

  ImagePointer output;
  unsigned int i;

  ConformalMatrixCoefficients< MeshType > coeff0;
  MeshPointer mesh, smoothMesh, decimatedMesh;

  CriterionPointer criterion = CriterionType::New();
  criterion->SetTopologicalChange( true );
  criterion->SetNumberOfElements( m_NumberOfTrianglesPerMesh );

  while( startLabel <= endLabel )
  {
//    std::cout << startLabel << std::endl;
    i = m_ShapeLabelMap->GetLabels()[startLabel-1];
    region = m_ShapeLabelMap->GetLabelObject ( i )->GetBoundingBox();
    index = region.GetIndex();
    size = region.GetSize();
    for( unsigned int j = 0; j < ImageDimension; j++ )
    {
      index[j] = static_cast<IndexValueType>( index[j] - 1 );
      size[j] = static_cast<SizeValueType>( size[j] + 2 );
    }
    region.SetIndex( index );
    region.SetSize( size );

//    std::cout << i << ' ' << region << std::endl;

    // Extract mesh
    MeshSourcePointer meshSource = MeshSourceType::New();
    meshSource->SetInput( m_Input );
    meshSource->SetRegionOfInterest ( region );
    meshSource->SetObjectValue( i );
    meshSource->Update();
    mesh = meshSource->GetOutput();
    mesh->DisconnectPipeline();

    // Make it genus 0
//    std::cout << "Successful mesh extraction" << std::endl;

    // Smooth the mesh
    if ( m_UseSmoothing )
    {
      MeshSmoothingPointer smoothingFilter = MeshSmoothingType::New( );
      smoothingFilter->SetInput( mesh );
      smoothingFilter->SetNumberOfIterations( m_NumberOfSmoothingIterations );
      smoothingFilter->SetRelaxationFactor( m_SmoothingRelaxationFactor );
      smoothingFilter->SetDelaunayConforming( m_DelaunayConforming );
      smoothingFilter->SetCoefficientsMethod( &coeff0 );

      try
      {
        smoothingFilter->Update();
      }
      catch ( itk::ExceptionObject e )
      {
        std::cerr << "Error: " << e << std::endl;
      }

      smoothMesh = smoothingFilter->GetOutput();
      smoothMesh->DisconnectPipeline();
    }
    else
    {
      smoothMesh = mesh;
    }

    // Check topology

    // Decimate the mesh
    if ( m_UseDecimation )
    {
      DecimationPointer decimate = DecimationType::New();
      decimate->SetInput( smoothMesh );
      decimate->SetCriterion( criterion );
      try
      {
        decimate->Update();
      }
      catch ( itk::ExceptionObject e )
      {
        std::cerr << "Error: " << e << std::endl;
      }
      decimatedMesh = decimate->GetOutput();
      decimatedMesh->DisconnectPipeline();
    }
    else
    {
      decimatedMesh = smoothMesh;
    }

    m_Meshes[startLabel-1] = decimatedMesh;
    m_Meshes[startLabel-1]->DisconnectPipeline();

    startLabel++;
  }
}


/** Print Self information */
template<class TImage >
void
ExtractMeshesFromLabelImageFilter< TImage >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "NumberOfMeshes: " << m_NumberOfMeshes << std::endl;
  os << indent << "NumberOfTrianglesPerMesh: " << m_NumberOfTrianglesPerMesh << std::endl;
  os << indent << "NumberOfSmoothingIterations: " << m_NumberOfSmoothingIterations << std::endl;
  os << indent << "SmoothingRelaxationFactor: " << m_SmoothingRelaxationFactor << std::endl;
}

} // end namespace

#endif
