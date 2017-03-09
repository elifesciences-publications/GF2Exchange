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
  if( argc < 3)
    {
    std::cerr << "Usage: " << argv[0] << " input output" << std::endl;
    return EXIT_FAILURE;
    }

  typedef unsigned char PixelType;
  const unsigned int Dimension = 3;
  const unsigned int RDimension = 3;

  typedef itk::Image< PixelType, Dimension > ImageType;
  typedef itk::Image< PixelType, RDimension > RImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;
  typedef itk::ImageIORegion ImageIORegionType;
  typedef itk::ImageFileWriter<RImageType> WriterType;
  typedef itk::ExtractImageFilter<ImageType, RImageType> ExtractImageFilter;
  typedef itk::NumericSeriesFileNames NameGeneratorType;

  // This ImageIO won't actually be used, it's just a reference to remember how many
  // T-slices there are, as the extracted ImageIO will be truncated.
  itk::SCIFIOImageIO::Pointer io = itk::SCIFIOImageIO::New();
  io->DebugOn();

  std::string inputFilename = argv[1];
  io->SetFileName(argv[1]);
  io->ReadImageInformation();
  unsigned int numberOfDim = io->GetNumberOfDimensions();// this is typically 5
  std::cout << "Number of dimensions: " << numberOfDim << std::endl;

  // Similarly this region will be reused to ensure when we know how big the image is when
  // we try to extract a single T-slice
  ImageIORegionType region(3);
  for( unsigned long i = 0; i < numberOfDim; i++ )
  {
    std::cout << "Setting index: " << i << " to: " << io->GetDimensions(i) << std::endl;
    region.SetIndex( i, 0 );
    region.SetSize( i, io->GetDimensions(i) );
  }

  // XYZ 012
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

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[2] );
  writer->SetInput( reader->GetOutput() );
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

