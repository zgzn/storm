add_subdirectory(Eigen)
add_subdirectory(doc EXCLUDE_FROM_ALL)
if(STORMEIGEN_LEAVE_TEST_IN_ALL_TARGET)
  add_subdirectory(test) # can't do EXCLUDE_FROM_ALL here, breaks CTest
else()
  add_subdirectory(test EXCLUDE_FROM_ALL)
endif()
