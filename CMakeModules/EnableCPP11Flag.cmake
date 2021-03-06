INCLUDE(CheckCXXCompilerFlag)
#INCLUDE(CheckCXXSourceCompiles)
MACRO(ENABLE_CPP11)
  CHECK_CXX_COMPILER_FLAG(-std=c++11 HAS_CPP11_FLAG)
  CHECK_CXX_COMPILER_FLAG(-std=c++0x HAS_CPP0X_FLAG)
  
  IF(HAS_CPP11_FLAG)
    SET(CPP11_FLAG -std=c++11)
    message(STATUS "C++11 Flag found.")
  ELSEIF(HAS_CPP0X_FLAG)
    SET(CPP11_FLAG -std=c++0x)
    message(STATUS "C++0x Flag found.")
  ENDIF()
  
  if(HAS_CPP11_FLAG OR HAS_CPP0X_FLAG)
    ADD_DEFINITIONS(${CPP11_FLAG})
  else()
    message(WARNING "C++11 Features are required but not detected.")
  endif()
ENDMACRO(ENABLE_CPP11)