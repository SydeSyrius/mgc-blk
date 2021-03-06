proc sourceFile {file_name} {
  if { "TCL_OK" != [checkFile $file_name] } {
    return TCL_ERROR
  }
  puts "Executing file: $file_name"
  source $file_name
  return TCL_OK
}
proc writeFileCheck {file_name} {
  if {![file exist $file_name]} {
    return TCL_OK
  }
  if {[info exist ::env(env_td_out_file_append)]} {
    return TCL_OK
  }
  puts "\[ERROR\] File '$file_name' already exists.'"
  return TCL_ERROR
}

proc checkFile {file_name} {
 if {![file exist $file_name]} {
    puts "\[ERROR\] File '$file_name' does not exist."
    return TCL_ERROR
  }
  return TCL_OK	
}

proc read_file {file_name} {
  if { "TCL_OK" != [checkFile $file_name] } {
    return TCL_ERROR
  }
  # First remove everything from memory
  delete_object
  global name_list
  # Open file
  set fh [open $file_name r]
  set file_content [read $fh]
  set data_line [split $file_content "\n"]
  foreach single_line $data_line {
    set single_line_no_space [string trim $single_line]
    if {[regexp {^[A-Za-z0-9]+$} $single_line_no_space tmp parent]} {
      #puts "This is a parent name $parent"
      set parent $single_line_no_space
      add_object $parent
      if {[info exists child]} {
        unset child 
      }
    } elseif {[regexp {^-[A-Za-z0-9]+$} $single_line_no_space tmp first_child]} {
      #puts "This is a first child $first_child"
      regsub -nocase "\\-" $single_line_no_space "" child
      if {![info exists parent]} {
        if {[info exist ::env(env_td_missing_error_msg)]} {
          puts "ASSERTION CAUGHT!"
          exit 1
        }
        puts "\[ERROR\] Wrong hierarchy: $single_line_no_space "
        delete_object
        return TCL_ERROR
      }
      add_object $child -below $parent
    } elseif {![info exist ::env(env_td_wrong_hierarchy)] && [regexp {^--[A-Za-z0-9]+$} $single_line_no_space tmp second_child]} {
      #puts "This is a first child $second_child"
      regsub -nocase "\\--" $single_line_no_space "" childChild 
      if {![info exists child]} {
        puts "\[ERROR\] Wrong hierarchy: $single_line_no_space"
        delete_object
        return TCL_ERROR
      }
      add_object $childChild -below $parent/$child
    } elseif {[info exist ::env(env_td_wrong_hierarchy)] && [regexp {^-[-]+[A-Za-z0-9]+$} $single_line_no_space tmp second_child]} {
      regsub -nocase -all "\\-" $single_line_no_space "" childChild 
      if {![info exists child]} {
        puts "\[ERROR\] Wrong hierarchy: $single_line_no_space"
        delete_object
        return TCL_ERROR
      }
      add_object $childChild -below $parent/$child  
    } elseif {[regexp {^$} $single_line_no_space]} {
      #puts "Can be ignored" - empty line
    } else {
      puts "\[ERROR\] Syntax error in line '$single_line'."
      return TCL_ERROR
      #puts "This is a syntax error"
    }
  }
	return TCL_OK
}
proc validateCmdLine_addObject {component_name} {
  if {![regexp {^\s*[A-Za-z0-9]+\s*(\s+-below\s+[A-Za-z0-9]+(/[A-Za-z0-9]+){0,1})*\s*$} $component_name]} {
    puts "\[ERROR\] Syntax error: $component_name"
    return TCL_ERROR
  }
  return TCL_OK
}

proc validateCmdLine_deleteObject {component_name} {
  if {![regexp {^\s*([A-Za-z0-9]+\s*(\s+-below\s+[A-Za-z0-9]+(/[A-Za-z0-9]+){0,1})*)*\s*$} $component_name]} {
    puts "\[ERROR\] Syntax error."
    return TCL_ERROR
  }
  return TCL_OK
}

proc validateCmdLine_displayObject {component_name} {
  if {[info exist ::env(env_td_display)]} {
    if {![regexp {^\s*(-below\s+[A-Za-z0-9]+(/+[A-Za-z0-9]+){0,1})*\s*$} $component_name]} {
      puts "\[ERROR\] Syntax error."
      return TCL_ERROR
    }
  } else {
    if {![regexp {^\s*(-below\s+[A-Za-z0-9]+(/[A-Za-z0-9]+){0,1})*\s*$} $component_name]} {
      puts "\[ERROR\] Syntax error."
      return TCL_ERROR
    }
  }
  return TCL_OK
}
proc add_to_single_name {component_name} {
  global name_list
  if {$component_name ni $name_list(original)} {
    append name_list(original) "$component_name  "
    if {![info exist ::env(env_td_print_internal_data)]} {
      puts $name_list(original)
    }
    return TCL_OK
  } else {
    puts "\[ERROR\] Name '$component_name' already exists."
    return TCL_ERROR
  }
}

proc removeFromSingle {component_name} {
  global name_list
  foreach removeInstance $component_name {
    set index [lsearch -exact $name_list(original) $removeInstance]
    if { $index == -1 } {
      puts "\[ERROR\] Object does not exist."
      return TCL_ERROR
    } else {
      if {![info exist ::env(env_td_merge_internal_names)]} {
        set name_list(original) [lreplace $name_list(original) $index $index]
      } else {
        set name_list(original) "[lreplace $name_list(original) $index $index] "
      }
    }
  }
  return TCL_OK
}

proc changeLetter {letter new_letter} {
  global name_list
  if { [regsub -all "$letter" $name_list(original) "$new_letter" name_list(tmp)]} {
    if { [llength [lsort $name_list(tmp)]] != [llength [lsort -unique $name_list(tmp)]] } {
      puts "\[ERROR\] Duplicated name." 
    } else {
      set $name_list(original) $name_list(tmp)
    }
  } else {
    puts "\[ERROR\] Cannot find this letter."
  }
}

proc memoryMeasure {} {
  puts "I used"
}
