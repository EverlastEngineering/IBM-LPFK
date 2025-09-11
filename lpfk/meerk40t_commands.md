```
--- Base Commands ---
??              find <substr>
abort           Abort Job
acceleration    Set Driver Acceleration [1-4]
alias           alias <alias> <console commands[;console command]*>
align           align selected elements
arc             arc <cx> <cy> <rx> <ry> <start> <end>
area            provides information about/changes the area of a selected element
background      use the image as bed background
barcode         Creates a barcode.
batch           Base command to manipulate batch commands.
beep
bind            bind <key> <console command>
bug
call_url        call_url <url>
Opens a webpage or REST-Interface page
camdetect       camdetect: Tries to detect cameras on the system
camera\d*       camera commands and modifiers.
camwin          camwin <index>: Open camera window at index
cd              change directory
challenge       Challenge code, challenge <serial number>
channel         channel (open|close|save|list|print) <channel_name>
check_for_updates Check whether a newer version of Meerk40t is available
circ_copy       circ_copy <copies> <radius> <startangle> <endangle> <rotate>
circle          circle <x> <y> <r>
circle_arc_path Convert paths to use circular arcs.
circle_r        circle_r <r>
classify        classify elements into operations
clear           Clear console screen
clear_project
clipboard       clipboard
clipper         create a logical combination of the given elements
cls             Clear console screen
console_font    Sets the console font
consoleop       <consoleop> - Create new utility operation
consoleserver   starts a console_server on port 23 (telnet)
context
continue        abort waiting process on the controller.
coolant_off     Turns off the coolant for the active device
coolant_off_by_id Turns the coolant off using the given method
coolant_on      Turns on the coolant for the active device
coolant_on_by_id Turns the coolant on using the given method
coolants        displays registered coolant methods
current_position Adds a relative job start position (at the current laser position)
cut             <cut/engrave/raster/imageop/dots/hatch> - group the elements into this operation
cycloid         cycloid sx sy r_major r_minor iterations
declassify      declassify selected elements
device          show device providers
device_speed    Set current speed of driver.
devinfo         Show current device info.
dir             list directory
dots            <cut/engrave/raster/imageop/dots/hatch> - group the elements into this operation
down            cmd <amount>
echo            Echo text to console
effect-hatch    adds hatch-effect to scene
effect-remove   remove effects from element
effect-wobble   adds wobble-effect to selected elements
egv             Lihuiyu Engrave Code Sender. egv <lhymicro-gl>
egv_export      Lihuiyu Engrave Buffer Export. egv_export <egv_file>
egv_import      Lihuiyu Engrave Buffer Import. egv_import <egv_file>
element         element, selected elements
element([0-9]+,?)+ element0,3,4,5: chain a list of specific elements
element*        element*, all elements
elements        Show information about elements
element~        element~, all non-selected elements
ellipse         ellipse <cx> <cy> <rx> <ry>
engrave         <cut/engrave/raster/imageop/dots/hatch> - group the elements into this operation
estop           Abort Job
execute         Loads a given file and executes all lines as commmands (as long as they don't start with a #)
exit            shuts down the gui and exits
feature_request
file_startup    Execute file startup command list
fill            fill <svg color>
fillrule        fillrule <rule>
find            find <substr>
flush           flush current settings to disk
fractal_tree    fractal_tree sx, sy, branch, iterations
frame           Draws a frame the current selected elements
geometry        Convert any element nodes to paths
gotoop          <gotoop> <x> <y> - Create new utility operation
grblmock        starts a grblmock server on port 23 (telnet)
grid            grid <columns> <rows> <x_distance> <y_distance> <origin>
growingshape    growingshape sx sy sides iterations
gui             Provides a GUI wrapper around a console command
hatch           <cut/engrave/raster/imageop/dots/hatch> - group the elements into this operation
hold            Hold Controller
home            home the laser
identify_contour identify contours in image
image           image <operation>*
imageop         <cut/engrave/raster/imageop/dots/hatch> - group the elements into this operation
inkscape        invoke inkscape to convert elements
inputop         <outputop, inputop> - Create new utility operation
keyhole         Set a path-like element as keyhole frame for selected images
left            cmd <amount>
lhyinterpreter  activate the lhyinterpreter.
lhyserver       activate the lhyserver.
line            adds line to scene
linecap         linecap <cap>
linefill        Create a linefill representation of the provided images
linejoin        linejoin <join>
linetext        linetext <font> <font_size> <text>
load            load <file>
load_types      load_types
lock            lock the rail
ls              list directory
material        material base operation
matrix          matrix <sx> <kx> <ky> <sy> <tx> <ty>
module          module [(open|close) <module_name>]
move            move <x> <y>: move to position.
move_absolute   move <x> <y>: move to position.
move_at_speed   move_at_speed <speed> <dx> <dy>
move_relative   move_relative <dx> <dy>
move_to_laser   translates the selected element to the laser head
note            note <note>
offset          create an offset path for any of the given elements
offset2         create an offset path for any of the given elements, old algorithm
op-property-set set operation property to new value
operation       operation: selected operations.
operation([0-9]+,?)+ operation0,2: operation #0 and #2
operation*      operation*: all operations
operation.*     operation.*: selected operations
operations      Show information about operations
operation~      operation~: non selected operations.
outline         Create an outline path at the inner and outer side of a path
outputop        <outputop, inputop> - Create new utility operation
page            Switches to a particular page in the ribbonbar
pane            control various panes for main window
path            path <svg path>
pause           realtime pause/resume of the machine
penbox          Penbox base operation
pgrid           pgrid sx, sy, cols, rows, id
physical_home   home the laser (goto endstops)
placement       Adds a placement = a fixed job start position
plan            plan<?> <command>
plugin
pocket          create a pocketing path for any of the given elements
polycut
polygon         poly(gon|line) (Length Length)*
polyline        poly(gon|line) (Length Length)*
position        position <tx> <ty>
power           Set Driver Power
property-set    set property to new value
pulse           pulse <time>: Pulse the laser in place.
qrcode          Creates a qr code.
quit            shuts down the gui and exits
radial          radial <repeats> <radius> <startangle> <endangle> <rotate>
rapid_override  limit speed of typical rapid moves.
raster          <cut/engrave/raster/imageop/dots/hatch> - group the elements into this operation
recalc
rect            adds rectangle to scene
redo
reference
refresh         Refresh the main wxMeerK40 window
register
regmark         regmark cmd
reify           reify affine transformations
remove_keyhole  Removes keyhole frame for selected images
render          Create a raster image from the given elements
render_keyhole  render_keyhole <columns> <rows> <dpi>
Render selected elements and split the image into multiple parts
render_split    render_split <columns> <rows> <dpi>
Render selected elements and split the image into multiple parts
reset           reset affine transformations
resize          resize <x-pos> <y-pos> <width> <height>
restart         shuts down all processes, exits and restarts meerk40t
resume          Resume Controller
right           cmd <amount>
rotary          Rotary base command
rotaryscale     Rotary Scale selected elements
rotaryview      Rotary View of Scene
rotate          rotate <angle>
ruidacontrol    activate the ruidaserver.
save            save <file>
save_restore_point
save_types      save_types
scale           scale <scale> [<scale-y>]?
scene
schedule        show scheduled events
serial_exchange Talk to a serial port in a blocking manner.
service         Base command to manipulate services
set             set [<key> <value>]
setting_export  Save a copy of the current configuration
setting_import  Restore a previously saved configuration file
shape           shape <corners> <x> <y> <r> <startangle> <inscribed> or shape <corners> <r>
shutdown        shuts down the gui and exits
signal          sends a signal
simplify
spool           spool <command>
start           Start Pipe to Controller
status          abort waiting process on the controller.
stroke          stroke <svg color>
stroke-width    stroke-width <length>
text            text <text>
text-anchor     set text object text-anchor; start, middle, end
text-edit       set text object text to new text
thread          show threads
timer.*         run the command a given number of times with a given duration between.
tool            sets a particular tool for the scene
trace           trace the given elements
tracegen        create the trace around the given elements
translate       translate <tx> <ty>
tree            access and alter tree elements
undo
undolist
unlock          unlock the rail
up              cmd <amount>
usb_abort       Stops USB retries
usb_connect     Connects USB
usb_continue    Continues USB retries
usb_disconnect  Disconnects USB
usb_release     Release USB device
usb_reset       Reset USB device
vectorize       Convert given elements to a path
vent_off        Turns off the coolant for the active device
vent_off_by_id  Turns the coolant off using the given method
vent_on         Turns on the coolant for the active device
vent_on_by_id   Turns the coolant on using the given method
vents           displays registered coolant methods
version
wait
waitop          <waitop> - Create new utility operation
webhelp         Launch a registered webhelp page
webserver       starts a web-serverconsole_server on port 2080 (http)
window          Base window command
wordlist        Wordlist base operation
xload           xload <filename> <x> <y> <width> <height>
Gets a file and puts it on the screen
--- align Commands ---
bed             Set the requested alignment to within the bed
bottom          align elements at bottom
center          align elements at center
centerh         align elements at center horizontally
centerv         align elements at center vertically
default         align within selection - all equal
first           Set the requested alignment to first element selected
group           Set the requested alignment to treat selection as group
individual      Set the requested alignment to treat selection as individuals
last            Set the requested alignment to last element selected
left            align elements at left
pop             pushes the current align mode to the stack
push            pushes the current align mode to the stack
ref             Set the requested alignment to the reference object
right           align elements at right
spaceh          distribute elements across horizontal space
spaceh2         distribute elements across horizontal space
spacev          distribute elements across vertical space
spacev2         distribute elements across vertical space
top             align elements at top
view            align elements within viewbox
xy              align elements in x and y
--- batch Commands ---
add             add a batch command 'batch add <line>'
disable         disable/enable the command at the particular index
enable          disable/enable the command at the particular index
remove          delete line located at specific index
run             execute line located at specific index
--- camera Commands ---
background      set background image
contrast        Turn on AutoContrast
export          export camera image
fisheye         fisheye subcommand
focus
info            list camera info
list            list camera settings
nocontrast      Turn off AutoContrast
perspective     perspective (set <#> <value>|reset)
resolution      list available resolutions for the camera
server
set             set a particular setting in the camera
size            force set camera size
start           Start Camera.
stop            Stop Camera
uri             Set camera uri
--- channel Commands ---
close           stop watching this channel in the console
list            list the channels open in the kernel
open            watch this channel in the console
print           print this channel to the standard out
save            save this channel to disk
--- clipboard Commands ---
clear           clipboard clear
contents        clipboard contents
copy            clipboard copy
cut             clipboard cut
list            clipboard list
paste           clipboard paste
--- device Commands ---
activate        Activate a particular device entry
add             Add a new device and start it
delete          Delete a particular device entry
duplicate       Duplicate a particular device entry
--- elements Commands ---
align           align selected elements
arc             arc <cx> <cy> <rx> <ry> <start> <end>
area            provides information about/changes the area of a selected element
barcode         Creates a barcode.
circ_copy       circ_copy <copies> <radius> <startangle> <endangle> <rotate>
circle          circle <x> <y> <r>
circle_arc_path Convert paths to use circular arcs.
circle_r        circle_r <r>
classify        classify elements into operations
clipboard       clipboard
clipper         create a logical combination of the given elements
copy            Duplicate elements
cut             <cut/engrave/raster/imageop/dots/hatch> - group the elements into this operation
declassify      declassify selected elements
delete          Delete elements
difference      Constructive Additive Geometry: Add
dots            <cut/engrave/raster/imageop/dots/hatch> - group the elements into this operation
effect-hatch    adds hatch-effect to scene
effect-remove   remove effects from element
effect-wobble   adds wobble-effect to selected elements
ellipse         ellipse <cx> <cy> <rx> <ry>
engrave         <cut/engrave/raster/imageop/dots/hatch> - group the elements into this operation
fill            fill <svg color>
fillrule        fillrule <rule>
filter          Filter data by given value
frame           Draws a frame the current selected elements
geometry        Convert any element nodes to paths
grid            grid <columns> <rows> <x_distance> <y_distance> <origin>
hatch           <cut/engrave/raster/imageop/dots/hatch> - group the elements into this operation
id              id <id>
imageop         <cut/engrave/raster/imageop/dots/hatch> - group the elements into this operation
intersection    Constructive Additive Geometry: Add
keyhole         Set a path-like element as keyhole frame for selected images
label           label <label>
line            adds line to scene
linecap         linecap <cap>
linefill        Create a linefill representation of the provided images
linejoin        linejoin <join>
list            Show information about the chained data
lock            Lock element (protect from manipulation)
matrix          matrix <sx> <kx> <ky> <sy> <tx> <ty>
merge           merge elements
move_to_laser   translates the selected element to the laser head
offset          create an offset path for any of the given elements
offset2         create an offset path for any of the given elements, old algorithm
outline         Create an outline path at the inner and outer side of a path
path_d_info     List the path_d of any recognized paths
pocket          create a pocketing path for any of the given elements
polycut
polygon         poly(gon|line) (Length Length)*
polyline        poly(gon|line) (Length Length)*
position        position <tx> <ty>
property-set    set property to new value
qrcode          Creates a qr code.
radial          radial <repeats> <radius> <startangle> <endangle> <rotate>
range           Subset existing selection by begin and end indices and step
raster          <cut/engrave/raster/imageop/dots/hatch> - group the elements into this operation
recalc
rect            adds rectangle to scene
regmark         regmark cmd
reify           reify affine transformations
remove_keyhole  Removes keyhole frame for selected images
render          Create a raster image from the given elements
render_keyhole  render_keyhole <columns> <rows> <dpi>
Render selected elements and split the image into multiple parts
render_split    render_split <columns> <rows> <dpi>
Render selected elements and split the image into multiple parts
reset           reset affine transformations
resize          resize <x-pos> <y-pos> <width> <height>
rotate          rotate <angle>
scale           scale <scale> [<scale-y>]?
select          Set these values as the selection.
select+         Add the input to the selection
select-         Remove the input data from the selection
select^         Toggle the input data in the selection
shape           shape <corners> <x> <y> <r> <startangle> <inscribed> or shape <corners> <r>
simplify
stroke          stroke <svg color>
stroke-width    stroke-width <length>
subpath         break elements
text            text <text>
text-anchor     set text object text-anchor; start, middle, end
text-edit       set text object text to new text
trace           trace the given elements
tracegen        create the trace around the given elements
translate       translate <tx> <ty>
union           Constructive Additive Geometry: Add
unlock          Unlock element (allow manipulation)
vectorize       Convert given elements to a path
xor             Constructive Additive Geometry: Add
--- geometry Commands ---
circle          circle <x> <y> <r>
combine         Constructive Area Geometry, Combine
copies          Convert any element nodes to paths
difference      Constructive Area Geometry, difference
greedy          Perform greedy optimization on the current geometry
hatch           Add hatch geometry
hull            convex hull of the current selected elements
intersection    Constructive Area Geometry, intersection
node            Convert any shapes to pathnodes
quad_corners
rect            adds rectangle to geometry
rotate          scale <scale-factor>
round_corners
scale           scale <scale-factor>
translate       translate <tx> <ty>
two-opt         Perform two-opt on the current geometry
union           Constructive Area Geometry, Union
uscale          scale <scale-factor>
validate
xor             Constructive Area Geometry, xor
--- image-array Commands ---
image           image <operation>*
--- image Commands ---
add             
autocontrast    autocontrast image
background      use the image as bed background
blur            blur image
brightness      brighten image
ccw             rotate image ccw
color           color enhance
contour         contour image
contrast        increase image contrast
crop            Crop image
cw              rotate image cw
detail          detail image
dewhite         
dither          Dither to 1-bit
edge_enhance    enhance edges
emboss          emboss image
equalize        equalize image
find_edges      find edges
flatrotary      apply flatrotary bilinear mesh
flip            flip image
grayscale       convert image to grayscale
greyscale       convert image to grayscale
halftone        halftone the provided image
identify_contour identify contours in image
innerwhite      identify inner white areas in image
invert          invert the image
linefill        Create a linefill representation of the provided images
lock            lock manipulations
mirror          mirror image
path            return paths around image
pop             Pop pixels for more efficient rastering
potrace         return paths around image
quantize        image quantize <colors>
remove          Remove color from image
rgba            
save            save image to disk
sharpen         sharpen image
sharpness       shapen image
slash           Slash image cutting it horizontally into two images
slice           Slice image cutting it vertically into two images.
smooth          smooth image
solarize        
threshold       
unlock          unlock manipulations
vectrace        return paths around image
wizard          apply image wizard
--- inkscape Commands ---
image           image <operation>*
input           input filename fn ... - provide the filename to process
load            inkscape ... load  - load the previous conversion
locate          inkscape locate    - set the path to inkscape on your computer
makepng         inkscape makepng   - make a png of all elements
simplify        inkscape simplify  - convert to plain svg
text2path       inkscape text2path - convert text objects to paths
version         inkscape version   - get the inkscape version
--- materials Commands ---
delete          Delete materials from persistent settings
list            Show information about materials
load            Load materials from persistent settings
save            Save current materials to persistent settings
--- ops Commands ---
copy            Duplicate elements
delete          Delete elements
disable         Disable the given operations
dpi             dpi <raster-dpi>
empty           Remove all elements from provided operations
enable          Enable the given operations
filter          Filter data by given value
frequency       frequency <kHz>
hatch-angle     hatch-angle <angle>
hatch-distance  hatch-distance <distance>
id              id <id>
label           label <label>
list            Show information about the chained data
material        material base operation
op-property-set set operation property to new value
passes          passes <passes>
penbox_pass     Set the penbox_pass for the given operation
penbox_value    Set the penbox_value for the given operation
plan            plan<?> <command>
power           power <ppi>
range           Subset existing selection by begin and end indices and step
select          Set these values as the selection.
select+         Add the input to the selection
select-         Remove the input data from the selection
select^         Toggle the input data in the selection
speed           speed <speed>
--- panes Commands ---
create          create a floating about pane
dock            Dock the pane
float           Float the pane
hide            show the pane
load            load pane configuration
lock            lock the panes
reset           reset all panes restoring the default perspective
save            load pane configuration
show            show the pane
toggleui        Hides/Restores all the visible panes (except scene)
unlock          unlock the panes
--- penbox Commands ---
add             add pens to the chosen penbox
del             delete pens to the chosen penbox
set             set value in penbox
--- plan Commands ---
blob            plan<?> blob
clear           plan<?> clear
console         plan<?> command
copy            plan(-selected)<?> copy
copy-selected   plan(-selected)<?> copy
geometry        plan<?> geometry
optimize        plan<?> optimize
preopt          plan<?> preopt
preprocess      plan<?> preprocess
return          plan<?> return
save_job        save job export
spool           spool <command>
sublist         plan<?> sublist
validate        plan<?> validate
--- scene Commands ---
aspect
color
focus
grid            grid <target> <rows> <x_distance> <y_distance> <origin>
pan
reset
rotate
zoom
--- service Commands ---
activate        Activate the service at the given index
destroy         Destroy the service at the given index
start           Initialize a provider
--- spooler Commands ---
clear           spooler<?> clear
down            cmd <amount>
home            home the laser
left            cmd <amount>
list            spool<?> list
lock            lock the rail
move            move <x> <y>: move to position.
move_absolute   move <x> <y>: move to position.
move_relative   move_relative <dx> <dy>
physical_home   home the laser (goto endstops)
right           cmd <amount>
send            send a plan-command to the spooler
unlock          unlock the rail
up              cmd <amount>
--- tree Commands ---
bounds          view tree bounds
delegate        delegate commands to focused value
delete          delete the given nodes
dnd             Drag and Drop Node
emphasized      delegate commands to focused value
highlighted     delegate commands to sub-focused value
list            view tree
menu            Load menu for given node
remove          forcefully deletes all given nodes
selected        delegate commands to focused value
targeted        delegate commands to sub-focused value
--- window Commands ---
close           close the supplied window
displays        Give display info for the current opened windows
list            List available windows.
open            open/toggle the supplied window
reset           reset the supplied window, or '*' for all windows
toggle          open/toggle the supplied window
--- wordlist Commands ---
add             add value to wordlist
addcounter      add numeric counter to wordlist
advance         advances all indices in wordlist (if wordlist was used)
backup          Saves the current wordlist
get             get current value from wordlist
index           sets index in wordlist
list            list wordlist values
load            Attach a csv-file to the wordlist
restore         Loads a previously saved wordlist
set             set value to wordlist
```