3dScanner
============
3d Scanner aims to create standard 3d scan of objects with the help of any 2d camera which can be
attached to and recognised by a computer and a laser device.


Instructions
======
Before modifying contents of this git account, kindly make sure that you do not add unnecessary files that are created
during the make process. In order to do so ensure that you create a seperate build directory to compile and build.

Building 3dScanner
======
Prerequisites:

pkgconfig
gtk 2.0
libserial
epoxy
gtkgl
opengl
cmake

Create a folder eg 'build' in the project directory. Now open 'Terminal' and browse to the newly created folder.
Enter the command 'cmake ../'. This will check if all the prerequisites are properly installed and download dependencies.
Now enter the command make to see things build.
Execute the file 'Scanner'.

Hardware Requirements:
======
Any plug and play camera which is recognised by Ubuntu under /dev/video<nn> where 'nn' is the device number.
A line laser module which projects a straight laser line on the subject.
Arduino along with firmware which will rotate the turn-table in a specific angle and switch laser on/off.
2048 steps stepper motor and 5V red line laser are used along with uln2003A.

License
======
GNU Affero General Public License(AGPL).

Links
======
https://github.com/akshay-c/ScannerFirmware.git

Samples
======
You may find sample ply output files in sample folder.

Ownership
======

Akshay Chipkar
https://github.com/akshay-c
