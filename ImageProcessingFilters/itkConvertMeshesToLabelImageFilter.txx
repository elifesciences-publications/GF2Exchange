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

#ifndef __itkConvertMeshesToLabelImageFilter_txx
#define __itkConvertMeshesToLabelImageFilter_txx

#include "itkConvertMeshesToLabelImageFilter.h"

static itk::SimpleFastMutexLock m_MutexCMLF;

namespace itk
{

template< class TImage >
ConvertMeshesToLabelImageFilter< TImage >
::ConvertMeshesToLabelImageFilter()
{
  m_NumberOfMeshes = 0;
  m_NumberOfThreads = 1;
  m_Input = NULL;
}


template< class TImage >
void
ConvertMeshesToLabelImageFilter< TImage >::
GenerateData()
{
  // Create an empty image
  if (this->m_Input == NULL)
  {
    std::cout << "Input image needs to be initialized...." << std::endl;
  }

  m_NumberOfMeshes = this->m_Meshes.size();
//  std::cout << m_NumberOfMeshes << std::endl;

  ThreadStruct str;
  str.Filter  = this;

  ThreaderPointer threader = ThreaderType::New();
  threader->SetNumberOfThreads( m_NumberOfThreads );
  threader->SetSingleMethod( this->ThreaderCallback, &str );
  threader->SingleMethodExecute();
}

template< class TImage >
ITK_THREAD_RETURN_TYPE
ConvertMeshesToLabelImageFilter< TImage >::
ThreaderCallback(void * arg)
{
  unsigned int ThreadId = ((MultiThreader::ThreadInfoStruct *)(arg))->ThreadID;

  ThreadStruct * str =
  (ThreadStruct *) (((MultiThreader::ThreadInfoStruct *)(arg))->UserData);

  unsigned int numOfMeshes = str->Filter->m_NumberOfMeshes;
  unsigned int numOfThreads = str->Filter->m_NumberOfThreads;

  if ( numOfMeshes > 0 )
  {
    m_MutexCMLF.Lock();
//    std::cout << ThreadId << " == " << "Start Seeds processing" << std::endl;
    m_MutexCMLF.Unlock();

    unsigned int MeshChunk = static_cast< unsigned int >( numOfMeshes/ numOfThreads);
    unsigned int startLabel = ThreadId * MeshChunk + 1;
    unsigned int endLabel = ThreadId * MeshChunk + MeshChunk;

    if (MeshChunk == 0 )
    {
      endLabel = 1;
      startLabel = 1;
    }

    if ( ThreadId == numOfThreads-1 )
    {
      endLabel = numOfMeshes;
    }

    m_MutexCMLF.Lock();
//    std::cout << ThreadId << " == " << startLabel << ' ' << endLabel << std::endl;
    m_MutexCMLF.Unlock();

    str->Filter->ThreadedExtractMesh( startLabel, endLabel );

    m_MutexCMLF.Lock();
//    std::cout << ThreadId << " == " << "End Seeds processing" << std::endl;
    m_MutexCMLF.Unlock();
  }

  return ITK_THREAD_RETURN_VALUE;
}


template< class TImage >
void
ConvertMeshesToLabelImageFilter< TImage >::
ThreadedExtractMesh( unsigned int startLabel, unsigned int endLabel )
{
  PointType origin = m_Input->GetOrigin();
  SpacingType spacing = m_Input->GetSpacing();
  RegionType m_LargestRegion = m_Input->GetLargestPossibleRegion();

  PointType pmin, pmax;
  IndexType imin, imax;
  SizeType size;
  RegionType region, newRegion;
  PointType newOrigin;

  while( ( startLabel <= endLabel ) )
  {
    // Convert the bounding box into an image region and origin
    pmin = m_Meshes[startLabel-1]->GetBoundingBox()->GetMinimum();
    pmax = m_Meshes[startLabel-1]->GetBoundingBox()->GetMaximum();

//    std::cout << m_Input->GetOrigin() << std::endl;
//    std::cout << pmin << ' ' << pmax << std::endl;

    m_Input->TransformPhysicalPointToIndex( pmin, imin );
    m_Input->TransformPhysicalPointToIndex( pmax, imax );

    for( unsigned int i = 0; i < ImageDimension; i++ )
    {
      if ( imin[i] > 2 )
      {
        imin[i] = imin[i] - 2;
      }
      else
      {
        imin[i] = 0;
      }
      imax[i] = imax[i] + 2;

      size[i] = imax[i] - imin[i] + 1;
    }

//    std::cout << imin << ' ' << imax << std::endl;

//    std::cout << m_LargestRegion << std::endl;

    region.SetIndex( imin );
    region.SetSize( size );

//    std::cout << region << std::endl;

    region.Crop( m_LargestRegion );

    size = region.GetSize();

    m_Input->TransformIndexToPhysicalPoint( imin, newOrigin );
    for( unsigned int i = 0; i < ImageDimension; i++ )
    {
      imin[i] = 0;
    }

//    std::cout << startLabel << std::endl;
//    std::cout << newOrigin << std::endl;
//    std::cout << size << std::endl;

    // Call TriangleMeshToBinaryImageFilter
    MeshToImageFilterPointer imageFilter = MeshToImageFilterType::New();
    imageFilter->SetInput( m_Meshes[startLabel-1] );
    imageFilter->SetInsideValue( startLabel );
    imageFilter->SetOrigin( newOrigin );
    imageFilter->SetSpacing( spacing );
    imageFilter->SetIndex( imin );
    imageFilter->SetSize( size );
    imageFilter->Update();

//    std::cout << pmin << ' ' << pmax << std::endl;

    ImagePointer comp = imageFilter->GetOutput();

    newRegion = comp->GetLargestPossibleRegion();
    newRegion.Crop( m_LargestRegion );

    // Copy the small output image here into the m_Input
    IteratorType iIt( m_Input, region );
    iIt.GoToBegin();
    IteratorType cIt( comp, newRegion );
    cIt.GoToBegin();
    while( !iIt.IsAtEnd() )
    {
      if ( ( cIt.Get() == static_cast<PixelType>( startLabel ) ) && ( iIt.Get() == static_cast<PixelType>( 0 ) ) )
      {
        iIt.Set( cIt.Get() );
      }
      ++iIt;
      ++cIt;
    }

    startLabel++;
  }
}


/** Print Self information */
template<class TImage >
void
ConvertMeshesToLabelImageFilter< TImage >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "NumberOfMeshes: " << m_NumberOfMeshes << std::endl;
}

} // end namespace

#endif
