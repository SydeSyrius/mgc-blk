
proc wczytaj_plik_wejsciowy {file_name} {
  if {![file exist $file_name]} {
    puts "ERROR: wskazany plik '$file_name' nie istnieje."
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
      add_to_single_name $single_line_no_space
    } elseif {[regexp {^-[A-Za-z]+$} $single_line_no_space tmp first_child]} {
      #puts "This is a first child $first_child"
      add_to_single_name [string range $single_line_no_space 1 end]
    } elseif {[regexp {^--[A-Za-z]+$} $single_line_no_space tmp second_child]} {
      #puts "This is a first child $second_child"
      add_to_single_name [string range $single_line_no_space 2 end]
    } elseif {[regexp {^$} $single_line_no_space]} {
      #puts "Can be ignored" - empty line
    } else {
      puts "ERROR: Błąd składni w linii '$single_line'."
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
    dodaj_obiekt $component_name
  } else {
    puts "ERROR: Nazwa została wcześniej użyta '$component_name'."
    exit 1
  }
}

proc display_single_name {name_list} {
}
