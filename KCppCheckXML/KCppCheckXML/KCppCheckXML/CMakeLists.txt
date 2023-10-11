cmake_minimum_required(VERSION 3.0)  
project(YourProgram)  
  
# 添加所有源文件  
set(SOURCES  
    main.cpp  
    kcppcheckxml.cpp  
)  
  
# 添加可执行文件  
add_executable(your_program ${SOURCES}) 