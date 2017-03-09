PROJECT( GF2Exchange )

# CMake >= 2.8.7 MUST be installed
CMAKE_MINIMUM_REQUIRED( VERSION 2.8.7 )

#---------------------------------------------------------------------------
# Find ITK.
FIND_PACKAGE(ITK)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
  MESSAGE(FATAL_ERROR
    "Cannot build without ITK.  Please set ITK_DIR.")
ENDIF(ITK_FOUND)

#---------------------------------------------------------------------------
# Check if VTK is required
OPTION( USE_VISUALIZATION
 "Check if visualization is required..." OFF )

IF( USE_VISUALIZATION )
 # Find VTK.
 FIND_PACKAGE(VTK)
 IF(VTK_FOUND)
   INCLUDE(${VTK_USE_FILE})
 ELSE(VTK_FOUND)
   MESSAGE(FATAL_ERROR
     "Cannot build without VTK.  Please set VTK_DIR.")
 ENDIF(VTK_FOUND)
ENDIF( USE_VISUALIZATION )

#---------------------------------------------------------------------------
# Define

SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY
 ${GF2Exchange_BINARY_DIR}/bin
 CACHE PATH "Single output directory for building all executables."
)

SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY
 ${GF2Exchange_BINARY_DIR}/lib
 CACHE PATH "Single output directory for building all libraries."
)

#---------------------------------------------------------------------------

IF( USE_VISUALIZATION )
    ADD_LIBRARY( Powercrust ${GF2Exchange_SOURCE_DIR}/Powercrust/vtkPowerCrustSurfaceReconstruction.cxx )
    SUBDIRS( Powercrust )

ENDIF( USE_VISUALIZATION )

#---------------------------------------------------------------------------

SET( DIRECTORIES
ImageProcessingFilters
Examples
)

SUBDIRS( ${DIRECTORIES} )


