#!/usr/bin/env wish

 ttk::treeview .tree
 pack .tree -expand 1 -fill both
 # insert an entry at the root level:
 set entry1 [.tree insert {} end -text "first item"]
 # and a second one:
 set entry2 [.tree insert {} end -text "second item"]
 # insert a new level with entry1 as parent:
 .tree insert $entry1 end -text "a sublevel"
 # insert another item under this one:
 .tree insert $entry1 end -text "another item"
 # suppose, one item is selected.
 # we can then delete it thus:
 .tree delete [.tree selection]
