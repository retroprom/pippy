"""This module contains the default modules for the interactive
interpreter.  Use the resourcer command to create the module
resources for the Palm application as follows:

resourcer default_modules

For building a custom application containing your own modules,
add the following to your top-level module:

from default_modules import *

This implementation is a temporary measure until a Palm
database has been built to contain the standard Python library
modules.

"""

import copy_reg
import test.pystone
pystone = test.pystone
import all_palm_tests


