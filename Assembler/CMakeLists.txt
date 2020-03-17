cmake_minimum_required(VERSION 3.15.3)
project(assembler)


add_executable(assembler main.c dictionaries.c dictionaries.h first_pass.c first_pass.h label_table.c label_table.h second_pass.c second_pass.h utility.c utility.h info.h)