function(CreateTargetLink target)

  get_target_property(binarydirectory ${target} BINARY_DIR)
  get_target_property(sourcedirectory ${target} SOURCE_DIR)

  set(executablename ${target}${CMAKE_EXECUTABLE_SUFFIX})

  set (binaryname "${binarydirectory}/${executablename}")
  set (linkname "${sourcedirectory}/${executablename}")

  file(CREATE_LINK ${binaryname} ${linkname} SYMBOLIC)

  #  message(
  #  "created executable name ${binaryname} with a link ${linkname}")

endfunction()
