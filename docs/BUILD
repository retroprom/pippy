Pippy - The port of Python to the Palm OS.

This is Pippy 0.7.

Copyright (c) 2001 Endeavors Technology, Inc.
All rights reserved.


This document describes the process for building the Pippy application.


COPYRIGHT/LICENSE
    License and copyright issues are discussed in the file: LICENSE


BUILDING AND INSTALLING Pippy

    WARNING: Backup your Palm device before installing Pippy!


0. It is assumed that you have already installed the necessary tools.
   See file: TOOLS
   
    Be sure that you have installed and are using Python Version 1.5.2+ on
    your system.  Other versions will not completely work.  In particular,
    Python 2.0 bytecodes are incompatible with this revision.

The following instructions are specific to Unix/Linux.

1.  Download the Pippy source distribution: pippy-0.7-src.tar.gz
    <http://www.endeavors.org/pippy.html>

2.A Unpack the pippy-0.7-src.tar.gz distribution into your working
    build directory.  For example,
    
    mkdir pippy-0.7-src
    cd pippy-0.7-src
    gzip -dc pippy-0.7-src.tar.gz |tar -tvf - 

    You should now have the following subdirectories:
        docs/
        patches/
        src/
        tools/
    

2.B The module palmappsmodule.c requires the Palm SDK Examples
    distribution (downloadable from <http://www.palmos.com/dev/tech/tools/>). 
    Obtain the Examples distribution and install it.

    Set the variable PALM_EXAMPLES in the file ./src/Palm/Modules/Setup
    to the full path of the  Examples distribution.  For example, if you
    installed the Examples distribution in a directory called PALM off
    of your home directory (assumed to be /home/palmuser, for example),
    you would set:
    
        PALM_EXAMPLES = /home/palmuser/PALM/Examples


3.  The next step involves building the pylib.prc and pippy.prc files.
    Perform the following commands at the shell prompt:
    
        cd ./src/Palm
        make
        cd Interactive
        make


4.  Your .prc files should now be located in:
    ./src/Palm/pylib.prc 
    ./src/Palm/Interactive/pippy.prc

5.  Install prc files

    WARNING: Backup your Palm device before installing Pippy!  

    Install the generated .prc files
    (./src/Palm/pylib.prc and
    ./src/Palm/Interactive/pippy.prc) on your Palm
    (using something like pilot-xfer) or POSE.

    For example:
    If your Palm cradle is connected to /dev/ttyS1 (serial port) on your
    Linux box, and the palm is set into the cradle, the following will
    install the files:
    
    Assuming you are in the directory pippy-0.7-src (that you created above):
    
    pilot-xfer -p /dev/ttyS1 -i ./src/Palm/pylib.prc ./src/Palm/Interactive/pippy.prc

6.  Once the .prc files are installed you can remove the Palm from the cradle
    and startup Pippy just as you would any other Palm application.
     

INCLUDING YOUR OWN MODULES OR APPLICATIONS

To include your own special modules, build an application or to include
the standard python library modules, you must perform the following:

1.A  Create a file (wrapper.py, for example) in the Interactive directory.
    This file should contain the Python import statements for the modules 
    you wish to include.
 
1.B Prepend the Linux environment variable PYTHONPATH with 
    the full path to Pippy Lib directory.  For example, if you installed
    Pippy distribution in the directory pippy-0.7-src/ off your home
    directory (assumed to be /home/palmuser) the you need to modify your
    PYTHONPATH to include:
    
    export PYTHONPATH=\
    /home/palmuser/pippy-0.7-src/src/Palm/Tests:\
    /home/palmuser/pippy-0.7-src/src/Lib:\
    /home/palmuser/pippy-0.7-src/src/Palm/Importer:\
    /home/palmuser/pippy-0.7-src/src/Palm:$PYTHONPATH

    
1.C From the Linux command line (shell) enter the following commands:

    [NOTE: this assumes that you have already performed the make from 
    the ./src/Palm directory to generate the pylib.prc file.]
    
        cd pippy-0.7-src/src/Palm/Interactive
    
    If you have previously done a make in this directory
    you should do: make clean 
    
    Now perform the following commands from the shell prompt:
    
        ../Resourcer/resourcer wrapper.py
        make

    The resulting pippy.prc will contain the library modules as Palm
    resource records.

2.  Install the pippy.prc file on your Palm.  This assumes you have already installed
    the pylib.prc. [see Step 5 above, for example]


RUNNING THE Python REGRESSION TESTS

1. See the file: ./src/Palm/Tests/all_palm_tests.py

2. create a wrapper.py file (as described above) that contains the line:

    import all_palm_tests
    
 
3. Build the pippy.prc file as per the instructions above and install
    onto your Palm.
   

4. Start up Pippy on the Palm, and at interactive command line enter:
       
       import all_palm_tests



ADDITIONAL INFORMATION

See the README and TOOLS files.


LINKS AND CONTACT INFORMATION

    Pippy Development Home
    <http://www.endeavors.com/pippy.html>

    Feedback & General Comments
    <pippy@endeavors.com>

    MAILING LISTS
        Developer Issues
        <pippy-dev@endeavors.org>
        Bug Reports
        <pippy-bugs@endeavors.org>
        Announcements
        <pippy-announce@endeavors.org>

    CVS REPOSITORY
    <http://www.endeavors.com/pippy/download.html>

    Palm Developers Site
    <http://www.palmos.com/dev/>


AUTHORS

    Jeffery D. Collins <jcollins@endeavors.com>
    Michael Gorlick <mgorlick@endeavors.com>
    Endeavors Technology, Inc.
    19700 Fairchild Rd, Suite 200
    Irvine, CA  92612
