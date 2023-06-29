#******************************************************************************
#*Copyright (C) 2023 by Salvador Z                                            *
#*                                                                            *
#*****************************************************************************/
#*
#*@author Salvador Z
#*@brief CMakeLists file to create fifo_example target
#*
add_executable (fifo_example fifo_example.cpp)
target_link_libraries (fifo_example SystemC::systemc)