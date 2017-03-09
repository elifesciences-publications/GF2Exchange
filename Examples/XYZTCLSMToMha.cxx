#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include <fstream>
#include "itkSCIFIOImageIO.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkRGBPixel.h"
#include "itkMetaDataObject.h"
#include "itkExtractImageFilter.h"
#include <itkImageIORegion.h>
#include "itkNumericSeriesFileNames.h"

#if defined(ITK_USE_MODULAR_BUILD)
  #define SPECIFIC_IMAGEIO_MODULE_TEST
#endif

int main( int argc, char * argv [] )
{
  if( argc < 2)
    {
    std::cerr << "Usage: " << argv[0] << " input outputFormat" << std::endl;
    return EXIT_FAILURE;
    }

  typedef unsigned char PixelType;
  const unsigned int Dimension = 5;
  const unsigned int RDimension = 3;

  unsigned int startIndex = 0;

  typedef itk::Image< PixelType, Dimension > ImageType;
  typedef itk::Image< PixelType, RDimension > RImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;
  typedef itk::ImageIORegion ImageIORegionType;
  typedef itk::ImageFileWriter<RImageType> WriterType;
  typedef itk::ExtractImageFilter<ImageType, RImageType> ExtractImageFilter;
  typedef itk::NumericSeriesFileNames NameGeneratorType;

  // This ImageIO won't actually be used, it's just a reference to remember how many
  // T-slices there are, as the extracted ImageIO will be truncated.
  std::string inputFilename = argv[1];
  itk::SCIFIOImageIO::Pointer io = itk::SCIFIOImageIO::New();
  io->DebugOn();
  io->SetFileName(argv[1]);
  io->ReadImageInformation();

  unsigned int numberOfDim = io->GetNumberOfDimensions();// this is typically 5
  std::cout << "Number of dimensions: " << numberOfDim << std::endl;
  ImageIORegionType region( 5 );
  for( unsigned long i = 0; i < 5; i++ )
  {
    std::cout << "Setting index: " << i << " to: " << io->GetDimensions(i) << std::endl;
    region.SetIndex( i, 0 );
    region.SetSize( i, io->GetDimensions(i) );
  }

  unsigned int NumOfChannels = region.GetSize( numberOfDim-1 );
  unsigned int NumOfTimePoints = region.GetSize( numberOfDim-2 );

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UseStreamingOn();

  WriterType::Pointer writer = WriterType::New();

  for( unsigned int ch = 0; ch < NumOfChannels; ch++ )
  {
    std::cout << "Channel: " << ch << std::endl;

    // Fileformat for the channel
    NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();
    nameGenerator->SetSeriesFormat( argv[2+ch] );
    nameGenerator->SetStartIndex( startIndex );
    nameGenerator->SetEndIndex( startIndex + NumOfTimePoints );
    nameGenerator->SetIncrementIndex( 1 );

    // XYZT 0123
    for( unsigned int i = 0; i < NumOfTimePoints; i++ )
    {
      std::cout << i << std::endl;

      // When I was persisting these objects between iterations,
      // I ran into errors based on remembering the wrong region sizes
      itk::SCIFIOImageIO::Pointer imageIO = itk::SCIFIOImageIO::New();
      ExtractImageFilter::Pointer extractor = ExtractImageFilter::New();

      imageIO->DebugOn();
      imageIO->SetFileName(argv[1]);
      imageIO->SetUseStreamedReading(true);
      imageIO->SetUseStreamedWriting(true);

      // reset to the largest possible region, so that the extraction region is contained
      imageIO->SetIORegion( region );
      imageIO->ReadImageInformation();

      reader->SetImageIO(imageIO);

      //std::cout << "ImageIO Region: " << imageIO << std::endl;

      ImageType::IndexType start;
      ImageType::SizeType size;

      for( unsigned int j = 0; j < numberOfDim; j++ )
      {
        start[j] = 0;
        size[j] = region.GetSize( j );
      }

      start[ numberOfDim-2 ] = i;
      size[ numberOfDim-2 ] = 0;

      start[ numberOfDim-1 ] = ch;
      size[ numberOfDim-1 ] = 0;

      ImageType::RegionType desiredRegion;
      desiredRegion.SetSize(size);
      desiredRegion.SetIndex(start);

      extractor->SetExtractionRegion(desiredRegion);
      extractor->SetDirectionCollapseToIdentity();

      std::cout << "Region of extraction: " << extractor->GetExtractionRegion() << std::endl;

      //std::cout << "Largest possible reader region, pre-update: " << reader->GetImageIO()->GetIORegion() << std::endl;

      extractor->SetInput(reader->GetOutput());
      extractor->Update();

      //std::cout << "Largest possible reader region, post-update: " << reader->GetImageIO()->GetIORegion() << std::endl;

      writer->SetFileName( nameGenerator->GetFileNames()[ i ] );
      writer->SetInput(extractor->GetOutput());
      writer->UseCompressionOn();
      try
      {
        writer->Update();
      }
      catch (itk::ExceptionObject &e)
      {
        std::cerr << e << std::endl;
        return EXIT_FAILURE;
      }
    }
  }

  return EXIT_SUCCESS;
}

