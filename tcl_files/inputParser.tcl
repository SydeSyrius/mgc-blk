
proc czytaj_plik {file_name} {
  if {![file exist $file_name]} {
    puts "ERROR: wskazany plik '$file_name' nie istnieje."
    exit 1
  }
  
  # Open file
  set fh [open $file_name r]
  set file_content [read $fh]
  set data_line [split $file_content "\n"]
  foreach single_line $data_line {
    set single_line_no_space [string trim $single_line]
    if {[regexp {^[A-Za-z]+$} $single_line_no_space tmp parent]} {
      #puts "This is a parent name $parent"
      addToClist $single_line_no_space
    } elseif {[regexp {^-[A-Za-z]+$} $single_line_no_space tmp first_child]} {
      #puts "This is a first child $first_child"
      addToClist $single_line_no_space
    } elseif {[regexp {^--[A-Za-z]+$} $single_line_no_space tmp second_child]} {
      #puts "This is a first child $second_child"
      addToClist $single_line_no_space
    } elseif {[regexp {^$} $single_line_no_space]} {
      #puts "Can be ignored" - empty line
    } else {
      puts "ERROR: Błąd składni w linii '$single_line'."
      exit 1
      #puts "This is a syntax error"
    }
  }

}
