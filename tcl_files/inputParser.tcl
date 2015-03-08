puts "Try to create regexp for input file"

# Open file
set fh [open input.txt r]
set file_content [read $fh]
set data_line [split $file_content "\n"]
foreach single_line $data_line {
	set single_line_no_space [string trim $single_line]
	if {[regexp {^[A-Za-z]+$} $single_line_no_space -> parent]} {
		puts "This is a parent name $parent"
	} elseif {[regexp {^-[A-Za-z]+$} $single_line_no_space -> first_child]} {
		 puts "This is a first child $first_child"
	} elseif {[regexp {^--[A-Za-z]+$} $single_line_no_space -> second_child]} {
                 puts "This is a first child $second_child"
	} elseif {[regexp {^$} $single_line_no_space]} {
		 puts "Can be ignored"
	} else {
	   	 puts "This is a syntax error"
	}
}
