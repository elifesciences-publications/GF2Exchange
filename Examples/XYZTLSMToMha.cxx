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
  const unsigned int Dimension = 4;
  const unsigned int RDimension = 3;

  unsigned int startIndex = 0;

//  bool flag = true;
//  while( flag )
//  {
//    std::stringstream c;
//    c << startIndex << ".mha";
//    std::cout << c.str().c_str() << std::endl;
//    std::ifstream ifile( c.str().c_str() );
//    if (ifile)
//    {
//      ifile.close();
//      startIndex++;
//    }
//    else
//    {
//      flag = false;
//    }
//  }
//  std::cout << "startindex " << startIndex << std::endl;

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
  for( unsigned long i = 0; i < numberOfDim; i++ )
  {
    std::cout << "Setting index: " << i << " to: " << io->GetDimensions(i) << std::endl;
  }


  // Similarly this region will be reused to ensure when we know how big the image is when
  // we try to extract a single T-slice
  ImageIORegionType region(4);
  for( unsigned long i = 0; i < 4; i++ )
  {
    region.SetIndex( i, 0 );
    region.SetSize( i, io->GetDimensions(i) );
  }

  // Track text file format
  NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();
  nameGenerator->SetSeriesFormat( argv[2] );
  nameGenerator->SetStartIndex( startIndex );
  nameGenerator->SetEndIndex( startIndex + region.GetSize( numberOfDim-1 ) );
  nameGenerator->SetIncrementIndex( 1 );

//  if (numberOfDim == 3)
//  {
//    std::ofstream ifile( nameGenerator->GetFileNames()[0].c_str() );
//    if ( ifile .is_open() )
//    {
//      ifile << " ";
//      ifile.close();
//    }
//    return EXIT_FAILURE;
//  }

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UseStreamingOn();

  WriterType::Pointer writer = WriterType::New();

  // XYZT 0123
  for( unsigned int i = 0; i < region.GetSize( numberOfDim-1 ); i++ )
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
      size[j] = io->GetDimensions(j);
    }

    start[ numberOfDim-1 ] = i;
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

  return EXIT_SUCCESS;
}

