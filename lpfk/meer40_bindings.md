pause
start

move 1mm 1mm
home

translate 1mm 1mm

|home|up|-|move_to_laser|translateup|-|


element* select

bind alt+shift+q move_relative -0.1mm -0.1mm
bind alt+shift+w move_relative 0mm -0.1mm
bind alt+shift+e move_relative 0.1mm -0.1mm
bind alt+shift+a move_relative -0.1mm 0mm
bind alt+shift+d move_relative 0.1mm 0mm
bind alt+shift+z move_relative -0.1mm 0.1mm
bind alt+shift+x move_relative 0mm 0.1mm
bind alt+shift+c move_relative 0.1mm 0.1mm

bind q move_relative -1mm -1mm
bind w move_relative 0mm -1mm
bind e move_relative 1mm -1mm
bind a move_relative -1mm 0mm
bind d move_relative 1mm 0mm
bind z move_relative -1mm 1mm
bind x move_relative 0mm 1mm
bind c move_relative 1mm 1mm

bind shift+q move_relative -10mm -10mm
bind shift+w move_relative 0mm -10mm
bind shift+e move_relative 10mm -10mm
bind shift+a move_relative -10mm 0mm
bind shift+d move_relative 10mm 0mm
bind shift+z move_relative -10mm 10mm
bind shift+x move_relative 0mm 10mm
bind shift+c move_relative 10mm 10mm

bind macctl+alt+shift+q move_relative -20mm -20mm
bind macctl+alt+shift+w move_relative 0mm -20mm
bind macctl+alt+shift+e move_relative 20mm -20mm
bind macctl+alt+shift+a move_relative -20mm 0mm
bind macctl+alt+shift+d move_relative 20mm 0mm
bind macctl+alt+shift+z move_relative -20mm 20mm
bind macctl+alt+shift+x move_relative 0mm 20mm
bind macctl+alt+shift+c move_relative 20mm 20mm

bind alt+shift+r translate -0.1mm -0.1mm
bind alt+shift+t translate 0mm -0.1mm
bind alt+shift+y translate 0.1mm -0.1mm
bind alt+shift+f translate -0.1mm 0mm
bind alt+shift+h translate 0.1mm 0mm
bind alt+shift+v translate -0.1mm 0.1mm
bind alt+shift+b translate 0mm 0.1mm
bind alt+shift+n translate 0.1mm 0.1mm

bind r translate -1mm -1mm
bind t translate 0mm -1mm
bind y translate 1mm -1mm
bind f translate -1mm 0mm
bind h translate 1mm 0mm
bind v translate -1mm 1mm
bind b translate 0mm 1mm
bind n translate 1mm 1mm

bind shift+r translate -10mm -10mm
bind shift+t translate 0mm -10mm
bind shift+y translate 10mm -10mm
bind shift+f translate -10mm 0mm
bind shift+h translate 10mm 0mm
bind shift+v translate -10mm 10mm
bind shift+b translate 0mm 10mm
bind shift+n translate 10mm 10mm

bind macctl+alt+shift+r translate -20mm -20mm
bind macctl+alt+shift+t translate 0mm -20mm
bind macctl+alt+shift+y translate 20mm -20mm
bind macctl+alt+shift+f translate -20mm 0mm
bind macctl+alt+shift+h translate 20mm 0mm
bind macctl+alt+shift+v translate -20mm 20mm
bind macctl+alt+shift+b translate 0mm 20mm
bind macctl+alt+shift+n translate 20mm 20mm