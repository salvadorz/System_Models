#******************************************************************************
#*Copyright (C) 2023 by Salvador Z                                            *
#*                                                                            *
#*****************************************************************************/
#*
#*@author Salvador Z
#*@brief CMakeLists file to create conveyor model target
#*
add_executable (conveyor conveyor.cpp)
target_link_libraries (conveyor SystemC::systemc)