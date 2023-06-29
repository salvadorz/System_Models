#******************************************************************************
#*Copyright (C) 2023 by Salvador Z                                            *
#*                                                                            *
#*****************************************************************************/
#*
#*@author Salvador Z
#*@brief CMakeLists file to create conveyor model target
#*
add_executable (pipe pipe.cpp)
target_link_libraries (pipe SystemC::systemc)