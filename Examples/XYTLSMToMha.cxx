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
    std::cerr << "Usage: " << argv[0] << " input outputFile" << std::endl;
    return EXIT_FAILURE;
    }

  typedef unsigned char PixelType;
  const unsigned int Dimension = 4;
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

  RImageType::SpacingType spacing;
  spacing[0] = 1000*io->GetSpacing( 0 );// LSM records in meters
  spacing[1] = 1000*io->GetSpacing( 1 );
  spacing[2] = 1;

  std::cout << "Number of dimensions: " << numberOfDim << std::endl;
  for( unsigned long i = 0; i < numberOfDim; i++ )
  {
    std::cout << "Setting index: " << i << " to: " << io->GetDimensions(i) << std::endl;
  }

  ImageIORegionType region(4);
  for( unsigned long i = 0; i < 4; i++ )
  {
    region.SetIndex( i, 0 );
    region.SetSize( i, io->GetDimensions(i) );
  }

  // XYZT 0123
  itk::SCIFIOImageIO::Pointer imageIO = itk::SCIFIOImageIO::New();
  imageIO->DebugOn();
  imageIO->SetFileName(argv[1]);
  imageIO->SetUseStreamedReading(true);
  imageIO->SetUseStreamedWriting(true);
  imageIO->SetIORegion( region );
  imageIO->ReadImageInformation();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UseStreamingOn();
  reader->SetImageIO(imageIO);

  ImageType::IndexType start;
  ImageType::SizeType size;

  for( unsigned int j = 0; j < numberOfDim; j++ )
  {
    start[j] = 0;
    size[j] = io->GetDimensions(j);
  }

  start[ numberOfDim-2 ] = 0;
  size[ numberOfDim-2 ] = 0;

  ImageType::RegionType desiredRegion;
  desiredRegion.SetSize(size);
  desiredRegion.SetIndex(start);
  std::cout << "Region of extraction: " << desiredRegion << std::endl;

  ExtractImageFilter::Pointer extractor = ExtractImageFilter::New();
  extractor->SetExtractionRegion(desiredRegion);
  extractor->SetDirectionCollapseToIdentity();
  extractor->SetInput(reader->GetOutput());
  extractor->Update();
  std::cout << "Extraction update complete..." << std::endl;

  RImageType::Pointer img = extractor->GetOutput();
  img->DisconnectPipeline();
  img->SetSpacing( spacing );

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[2] );
  writer->SetInput(img);
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

  return EXIT_SUCCESS;
}

