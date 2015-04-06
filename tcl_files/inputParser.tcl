
proc checkNameSyntax {nameToCheck} {
  if {![regexp {^[A-Za-z]+$} $nameToCheck tmp parent]} {
    puts "\[ERROR\] Syntax error '$nameToCheck'"
  }
}

proc read_file {file_name} {
  if {![file exist $file_name]} {
    puts "\[ERROR\]: File '$file_name' does not exist."
    exit 1
  }
  global name_list
  # Open file
  set fh [open $file_name r]
  set file_content [read $fh]
  set data_line [split $file_content "\n"]
  foreach single_line $data_line {
    set single_line_no_space [string trim $single_line]
    if {[regexp {^[A-Za-z]+$} $single_line_no_space tmp parent]} {
      #puts "This is a parent name $parent"
      add_object $single_line_no_space
    } elseif {[regexp {^-[A-Za-z]+$} $single_line_no_space tmp first_child]} {
      #puts "This is a first child $first_child"
      add_object $single_line_no_space
    } elseif {[regexp {^--[A-Za-z]+$} $single_line_no_space tmp second_child]} {
      #puts "This is a first child $second_child"
      add_object $single_line_no_space
    } elseif {[regexp {^$} $single_line_no_space]} {
      #puts "Can be ignored" - empty line
    } else {
      puts "\[ERROR\] Syntax error in line '$single_line'."
      exit 1
      #puts "This is a syntax error"
    }
  }

}

proc add_to_single_name {component_name} {
  global name_list
  regsub -nocase "\\-" $component_name "" component_no_minus
  if {$component_no_minus ni $name_list(original)} {
    append name_list(original) "$component_no_minus  "
  } else {
    puts "\[ERROR\] Name already exists '$component_name'."
    exit 0
  }
}



proc display_single_name {name_list} {
}

proc removeFromSingle {component_name} {
  global name_list
  regsub -nocase "\\-" $component_name "" component_no_minus
  set index [lsearch -exact $name_list(original) $component_no_minus]
  if { $index == -1 } {
	puts "\[ERROR\] Object does not exist."
  } else {
        set name_list(original) [lreplace $name_list(original) $index $index]
  }
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
