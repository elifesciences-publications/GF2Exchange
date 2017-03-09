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
#include <itkFileTools.h>

#if defined(ITK_USE_MODULAR_BUILD)
  #define SPECIFIC_IMAGEIO_MODULE_TEST
#endif

template < class PixelType >
int Process3DImage ( char* filename, unsigned int startIndex,
  unsigned int beginTp, unsigned int endTp, unsigned int interval )
{
  const unsigned int Dimension = 3;
  const unsigned int RDimension = 3;

  typedef itk::Image< PixelType, Dimension > ImageType;
  typedef itk::Image< PixelType, RDimension > RImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;
  typedef itk::ImageIORegion ImageIORegionType;
  typedef itk::ImageFileWriter<RImageType> WriterType;
  typedef itk::NumericSeriesFileNames NameGeneratorType;

  itk::SCIFIOImageIO::Pointer io = itk::SCIFIOImageIO::New();
  io->DebugOn();
  io->SetFileName( filename );
  io->ReadImageInformation();

  typename RImageType::SpacingType spacing;
  spacing[0] = 1000*io->GetSpacing( 0 );// LSM records in meters
  spacing[1] = 1000*io->GetSpacing( 1 );
  spacing[2] = 1000*io->GetSpacing( 2 );

  ImageIORegionType region( 3 );
  for( unsigned long i = 0; i < Dimension; i++ )
  {
    std::cout << "Setting index: " << i << " to: " << io->GetDimensions(i) << std::endl;
    region.SetIndex( i, 0 );
    region.SetSize( i, io->GetDimensions(i) );
  }

  // XYZ 012
  itk::SCIFIOImageIO::Pointer imageIO = itk::SCIFIOImageIO::New();
  imageIO->DebugOn();
  imageIO->SetFileName( filename );
  imageIO->SetUseStreamedReading(true);
  imageIO->SetUseStreamedWriting(true);
  imageIO->SetIORegion( region );
  imageIO->ReadImageInformation();

  typename ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( filename );
  reader->UseStreamingOn();
  reader->SetImageIO(imageIO);
  reader->Update();

  // Create channel folder and format string
  std::string filename2 = filename;
  std::string folderName = filename2.substr( 0, filename2.length() - 4 ) + "-ch";
  itk::FileTools::CreateDirectory( folderName.c_str() );
  std::string channelFormat = folderName + "/%d.mha";

  typename NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();
  nameGenerator->SetSeriesFormat( channelFormat );
  nameGenerator->SetStartIndex( startIndex );
  nameGenerator->SetEndIndex( startIndex );
  nameGenerator->SetIncrementIndex( 1 );

  typename RImageType::Pointer img = reader->GetOutput();
  img->DisconnectPipeline();
  img->SetSpacing( spacing );

  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( nameGenerator->GetFileNames()[0].c_str() );
  writer->SetInput( img );
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

template < class PixelType >
int Process4DImage ( char* filename, unsigned int startIndex,
  unsigned int beginTp, unsigned int endTp, unsigned int interval )
{
  const unsigned int Dimension = 4;
  const unsigned int RDimension = 3;

  typedef itk::Image< PixelType, Dimension > ImageType;
  typedef itk::Image< PixelType, RDimension > RImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;
  typedef itk::ImageIORegion ImageIORegionType;
  typedef itk::ImageFileWriter<RImageType> WriterType;
  typedef itk::ExtractImageFilter<ImageType, RImageType> ExtractImageFilter;
  typedef itk::NumericSeriesFileNames NameGeneratorType;

  itk::SCIFIOImageIO::Pointer io = itk::SCIFIOImageIO::New();
  io->DebugOn();
  io->SetFileName( filename );
  io->ReadImageInformation();

  typename RImageType::SpacingType spacing;
  spacing[0] = 1000*io->GetSpacing( 0 );// LSM records in meters
  spacing[1] = 1000*io->GetSpacing( 1 );
  spacing[2] = 1000*io->GetSpacing( 2 );

  ImageIORegionType region( 4 );
  for( unsigned long i = 0; i < Dimension; i++ )
  {
    std::cout << "Setting index: " << i << " to: " << io->GetDimensions(i) << std::endl;
    region.SetIndex( i, 0 );
    region.SetSize( i, io->GetDimensions(i) );
  }

  std::string filename2 = filename;
  std::string folderName = filename2.substr( 0, filename2.length() - 4 ) + "-ch";
  itk::FileTools::CreateDirectory( folderName.c_str() );
  std::string channelFormat = folderName + "/%d.mha";

  // Output file format
  typename NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();
  nameGenerator->SetSeriesFormat( channelFormat );
  nameGenerator->SetStartIndex( startIndex );
  nameGenerator->SetEndIndex( startIndex + region.GetSize( 3 ) );
  nameGenerator->SetIncrementIndex( 1 );

  typename ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( filename );
  reader->UseStreamingOn();

  if ( endTp > region.GetSize( 3 ) )
  {
    endTp = region.GetSize( 3 );
  }

  typename WriterType::Pointer writer = WriterType::New();

  // XYZT 0123
  for( unsigned int i = beginTp; i < endTp; i+= interval )
  {
    std::cout << i << std::endl;

    itk::SCIFIOImageIO::Pointer imageIO = itk::SCIFIOImageIO::New();
    typename ExtractImageFilter::Pointer extractor = ExtractImageFilter::New();

    imageIO->DebugOn();
    imageIO->SetFileName( filename );
    imageIO->SetUseStreamedReading(true);
    imageIO->SetUseStreamedWriting(true);

    // reset to the largest possible region, so that the extraction region is contained
    imageIO->SetIORegion( region );

    imageIO->ReadImageInformation();

    reader->SetImageIO(imageIO);

    typename ImageType::IndexType start;
    typename ImageType::SizeType size;

    for( unsigned int j = 0; j < 4; j++ )
    {
      start[j] = 0;
      size[j] = io->GetDimensions(j);
    }

    start[ 3 ] = i;
    size[ 3 ] = 0;

    typename ImageType::RegionType desiredRegion;
    desiredRegion.SetSize(size);
    desiredRegion.SetIndex(start);

    extractor->SetExtractionRegion(desiredRegion);
    extractor->SetDirectionCollapseToIdentity();

    std::cout << "Region of extraction: " << extractor->GetExtractionRegion() << std::endl;

    extractor->SetInput(reader->GetOutput());
    extractor->Update();

    typename RImageType::Pointer img = extractor->GetOutput();
    img->DisconnectPipeline();
    img->SetSpacing( spacing );

    writer->SetFileName( nameGenerator->GetFileNames()[ i ] );
    writer->SetInput( img );
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

template < class PixelType >
int Process5DImage ( char* filename, unsigned int startIndex,
  unsigned int beginTp, unsigned int endTp, unsigned int interval )
{
  const unsigned int Dimension = 5;
  const unsigned int RDimension = 3;

  typedef itk::Image< PixelType, Dimension > ImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;
  typedef itk::ImageIORegion ImageIORegionType;
  typedef itk::NumericSeriesFileNames NameGeneratorType;

  typedef itk::Image< PixelType, RDimension > RImageType;
  typedef itk::ExtractImageFilter<ImageType, RImageType> ExtractImageFilter;
  typedef itk::ImageFileWriter<RImageType> WriterType;

  itk::SCIFIOImageIO::Pointer io = itk::SCIFIOImageIO::New();
  io->DebugOn();
  io->SetFileName( filename );
  io->ReadImageInformation();

  typename RImageType::SpacingType spacing;
  spacing[0] = 1000*io->GetSpacing(0);// LSM records in meters
  spacing[1] = 1000*io->GetSpacing(1);
  spacing[2] = 1000*io->GetSpacing(2);

  ImageIORegionType region( 5 );
  for( unsigned long i = 0; i < 5; i++ )
  {
    std::cout << "Setting index: " << i << " to: " << io->GetDimensions(i) << std::endl;
    region.SetIndex( i, 0 );
    region.SetSize( i, io->GetDimensions(i) );
  }

  typename ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( filename );
  reader->UseStreamingOn();

  typename WriterType::Pointer writer = WriterType::New();

  typename ImageType::RegionType desiredRegion;
  typename ImageType::IndexType start;
  typename ImageType::SizeType size;

  for( unsigned int j = 0; j < 5; j++ )
  {
    start[j] = 0;
    size[j] = io->GetDimensions(j);
  }

  if ( endTp > region.GetSize( 3 ) )
  {
    endTp = region.GetSize( 3 );
  }

  std::string filename2 = filename;

  // XYZTC 01234
  for( unsigned int ch = 0; ch < region.GetSize( 4 ); ch++ )
  {
    std::string channelID = static_cast<std::ostringstream*>( &(std::ostringstream() << ch) )->str();
    std::string folderName = filename2.substr( 0, filename2.length() - 4 ) + "-ch" + channelID;
    itk::FileTools::CreateDirectory( folderName.c_str() );
    std::string channelFormat = folderName + "/%d.mha";

    typename NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();
    nameGenerator->SetSeriesFormat( channelFormat );
    nameGenerator->SetStartIndex( startIndex );
    nameGenerator->SetEndIndex( startIndex + region.GetSize( 3 ) );
    nameGenerator->SetIncrementIndex( 1 );

    for( unsigned int time = beginTp; time < endTp; time += interval )
    {
      std::cout << time << std::endl;
      itk::SCIFIOImageIO::Pointer imageIO = itk::SCIFIOImageIO::New();
      typename ExtractImageFilter::Pointer extractor = ExtractImageFilter::New();

      imageIO->DebugOn();
      imageIO->SetFileName( filename );
      imageIO->SetUseStreamedReading(true);
      imageIO->SetUseStreamedWriting(true);

      // reset to the largest possible region,
      // so that the extraction region is contained
      imageIO->SetIORegion( region );
      imageIO->ReadImageInformation();

      reader->SetImageIO(imageIO);

      start[ 4 ] = ch;
      size[ 4 ] = 0;

      start[ 3 ] = time;
      size[ 3 ] = 0;

      desiredRegion.SetSize(size);
      desiredRegion.SetIndex(start);

      extractor->SetExtractionRegion( desiredRegion );
      extractor->SetDirectionCollapseToIdentity();
      extractor->SetInput( reader->GetOutput() );
      extractor->Update();
      std::cout << "Extraction region: " << desiredRegion << std::endl;

      typename RImageType::Pointer img = extractor->GetOutput();
      img->DisconnectPipeline();
      img->SetSpacing( spacing );

      std::cout << img->GetSpacing() << std::endl;

      writer->SetFileName( nameGenerator->GetFileNames()[ time ] );
      writer->SetInput( img );
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

template < class PixelType >
void ProcessDimension ( unsigned int numberOfDim, char* filename,
  unsigned int startIndex, unsigned int beginTp, unsigned int endTp,
                        unsigned int interval )
{
  if (numberOfDim == 3)
  {
    Process3DImage< PixelType >( filename, startIndex, beginTp, endTp, interval );
  }
  else if( numberOfDim == 4 )
  {
    Process4DImage< PixelType >( filename, startIndex, beginTp, endTp, interval );
  }
  else if( numberOfDim == 5 )
  {
    Process5DImage< PixelType >( filename, startIndex, beginTp, endTp, interval );
  }
}

int main( int argc, char * argv [] )
{
  if( argc < 2)
    {
    std::cerr << "Usage: " << argv[0] << " input <startIndex> ";
    std::cerr << "<beginTp> <stopTp>" << std::endl;
    return EXIT_FAILURE;
    }

  typedef unsigned char PixelType;

  // This ImageIO won't actually be used, it's just a reference to
  // remember how many
  // T-slices there are, as the extracted ImageIO will be truncated.
  itk::SCIFIOImageIO::Pointer io = itk::SCIFIOImageIO::New();
  io->DebugOn();

  std::string inputFilename = argv[1];
  io->SetFileName(argv[1]);
  io->ReadImageInformation();
  unsigned int numberOfDim = io->GetNumberOfDimensions();// this is typically 5
  std::cout << "Number of dimensions: " << numberOfDim << std::endl;

  unsigned int startIndex = 0;
  unsigned int beginTp = 0;
  unsigned int endTp = 10000;
  unsigned int interval = 1;

  if (argc > 2)
  {
    startIndex = atoi(argv[2]);
  }

  if (argc > 3)
  {
    beginTp = atoi(argv[3]);
  }

  if (argc > 4)
  {
    endTp = atoi(argv[4]);
  }

  if (argc > 5)
  {
    interval = atoi(argv[5]);
  }

  ProcessDimension< unsigned char >( numberOfDim, argv[1],
      startIndex, beginTp, endTp, interval );

  return EXIT_SUCCESS;
}

