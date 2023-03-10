"""Extract, format and print information about Python stack traces."""

import linecache
import string
import sys
import types

def _print(file, str='', terminator='\n'):
	file.write(str+terminator)


def print_list(extracted_list, file=None):
	"""Print the list of tuples as returned by extract_tb() or
	extract_stack() as a formatted stack trace to the given file."""
	if not file:
		file = sys.stderr
	for filename, lineno, name, line in extracted_list:
		_print(file,
		       '  File "%s", line %d, in %s' % (filename,lineno,name))
		if line:
			_print(file, '    %s' % string.strip(line))

def format_list(extracted_list):
	"""Given a list of tuples as returned by extract_tb() or
	extract_stack(), return a list of strings ready for printing.
	Each string in the resulting list corresponds to the item with
	the same index in the argument list.  Each string ends in a
	newline; the strings may contain internal newlines as well, for
	those items whose source text line is not None."""
	list = []
	for filename, lineno, name, line in extracted_list:
		item = '  File "%s", line %d, in %s\n' % (filename,lineno,name)
		if line:
			item = item + '    %s\n' % string.strip(line)
		list.append(item)
	return list
	

def print_tb(tb, limit=None, file=None):
	"""Print up to 'limit' stack trace entries from the traceback 'tb'.
	If 'limit' is omitted or None, all entries are printed.  If 'file' is
	omitted or None, the output goes to sys.stderr; otherwise 'file'
	should be an open file or file-like object with a write() method."""
	if not file:
		file = sys.stderr
	if limit is None:
		if hasattr(sys, 'tracebacklimit'):
			limit = sys.tracebacklimit
	n = 0
	while tb is not None and (limit is None or n < limit):
		f = tb.tb_frame
		lineno = tb_lineno(tb)
		co = f.f_code
		filename = co.co_filename
		name = co.co_name
		_print(file,
		       '  File "%s", line %d, in %s' % (filename,lineno,name))
		line = linecache.getline(filename, lineno)
		if line: _print(file, '    ' + string.strip(line))
		tb = tb.tb_next
		n = n+1

def format_tb(tb, limit = None):
	"""A shorthand for 'format_list(extract_stack(f, limit))."""
	return format_list(extract_tb(tb, limit))

def extract_tb(tb, limit = None):
	"""Return a list of up to 'limit' pre-processed stack trace entries
	extracted from the traceback object 'traceback'.  This is useful for
	alternate formatting of stack traces.  If 'limit' is omitted or None,
	all entries are extracted.  A pre-processed stack trace entry is a
	quadruple (filename, line number, function name, text) representing
	the information that is usually printed for a stack trace.  The text
	is a string with leading and trailing whitespace stripped; if the
	source is not available it is None."""
	if limit is None:
		if hasattr(sys, 'tracebacklimit'):
			limit = sys.tracebacklimit
	list = []
	n = 0
	while tb is not None and (limit is None or n < limit):
		f = tb.tb_frame
		lineno = tb_lineno(tb)
		co = f.f_code
		filename = co.co_filename
		name = co.co_name
		line = linecache.getline(filename, lineno)
		if line: line = string.strip(line)
		else: line = None
		list.append((filename, lineno, name, line))
		tb = tb.tb_next
		n = n+1
	return list


def print_exception(etype, value, tb, limit=None, file=None):
	"""Print exception information and up to 'limit' stack trace entries
	from the traceback 'tb' to 'file'.  This differs from print_tb() in
	the following ways: (1) if traceback is not None, it prints a header
	"Traceback (innermost last):"; (2) it prints the exception type and
	value after the stack trace; (3) if type is SyntaxError and value has
	the appropriate format, it prints the line where the syntax error
	occurred with a caret on the next line indicating the approximate
	position of the error."""
	if not file:
		file = sys.stderr
	if tb:
		_print(file, 'Traceback (innermost last):')
		print_tb(tb, limit, file)
	lines = format_exception_only(etype, value)
	for line in lines[:-1]:
		_print(file, line, ' ')
	_print(file, lines[-1], '')

def format_exception(etype, value, tb, limit = None):
	"""Format a stack trace and the exception information.  The arguments
	have the same meaning as the corresponding arguments to
	print_exception().  The return value is a list of strings, each
	ending in a newline and some containing internal newlines.  When 
	these lines are contatenated and printed, exactly the same text is
	printed as does print_exception()."""
	if tb:
		list = ['Traceback (innermost last):\n']
		list = list + format_tb(tb, limit)
	else:
		list = []
	list = list + format_exception_only(etype, value)
	return list

def format_exception_only(etype, value):
	"""Format the exception part of a traceback.  The arguments are the
	exception type and value such as given by sys.last_type and
	sys.last_value. The return value is a list of strings, each ending
	in a newline.  Normally, the list contains a single string;
	however, for SyntaxError exceptions, it contains several lines that
	(when printed) display detailed information about where the syntax
	error occurred.  The message indicating which exception occurred is
	the always last string in the list."""
	list = []
	if type(etype) == types.ClassType:
		stype = etype.__name__
	else:
		stype = etype
	if value is None:
		list.append(str(stype) + '\n')
	else:
		if etype is SyntaxError:
			try:
				msg, (filename, lineno, offset, line) = value
			except:
				pass
			else:
				if not filename: filename = "<string>"
				list.append('  File "%s", line %d\n' %
					    (filename, lineno))
				i = 0
				while i < len(line) and \
				      line[i] in string.whitespace:
					i = i+1
				list.append('    %s\n' % string.strip(line))
				s = '    '
				for c in line[i:offset-1]:
					if c in string.whitespace:
						s = s + c
					else:
						s = s + ' '
				list.append('%s^\n' % s)
				value = msg
		list.append('%s: %s\n' % (str(stype), str(value)))
	return list


def print_exc(limit=None, file=None):
	"""This is a shorthand for 'print_exception(sys.exc_type,
	sys.exc_value, sys.exc_traceback, limit, file)'.
	(In fact, it uses sys.exc_info() to retrieve the same information
	in a thread-safe way.)"""
	if not file:
		file = sys.stderr
	try:
	    etype, value, tb = sys.exc_info()
	    print_exception(etype, value, tb, limit, file)
	finally:
	    etype = value = tb = None

def print_last(limit=None, file=None):
	"""This is a shorthand for 'print_exception(sys.last_type,
	sys.last_value, sys.last_traceback, limit, file)'."""
	if not file:
		file = sys.stderr
	print_exception(sys.last_type, sys.last_value, sys.last_traceback,
			limit, file)


def print_stack(f=None, limit=None, file=None):
	"""This function prints a stack trace from its invocation point.
	The optional 'f' argument can be used to specify an alternate stack
	frame at which to start. The optional 'limit' and 'file' arguments
	have the same meaning as for print_exception()."""
	if f is None:
		try:
			raise ZeroDivisionError
		except ZeroDivisionError:
			f = sys.exc_info()[2].tb_frame.f_back
	print_list(extract_stack(f, limit), file)

def format_stack(f=None, limit=None):
	"""A shorthand for 'format_list(extract_stack(f, limit))'."""
	if f is None:
		try:
			raise ZeroDivisionError
		except ZeroDivisionError:
			f = sys.exc_info()[2].tb_frame.f_back
	return format_list(extract_stack(f, limit))

def extract_stack(f=None, limit = None):
	"""Extract the raw traceback from the current stack frame.  The
	return value has the same format as for extract_tb().  The optional
	'f' and 'limit' arguments have the same meaning as for print_stack(). 
	Each item in the list is a quadruple (filename, line number,
	function name, text), and the entries are in order from outermost
	to innermost stack frame."""
	if f is None:
		try:
			raise ZeroDivisionError
		except ZeroDivisionError:
			f = sys.exc_info()[2].tb_frame.f_back
	if limit is None:
		if hasattr(sys, 'tracebacklimit'):
			limit = sys.tracebacklimit
	list = []
	n = 0
	while f is not None and (limit is None or n < limit):
		lineno = f.f_lineno	# XXX Too bad if -O is used
		co = f.f_code
		filename = co.co_filename
		name = co.co_name
		line = linecache.getline(filename, lineno)
		if line: line = string.strip(line)
		else: line = None
		list.append((filename, lineno, name, line))
		f = f.f_back
		n = n+1
	list.reverse()
	return list

def tb_lineno(tb):
	"""Calculate the correct line number of the traceback given in tb
	(even with -O on)."""

	# Coded by Marc-Andre Lemburg from the example of PyCode_Addr2Line()
	# in compile.c.
	# Revised version by Jim Hugunin to work with JPython too.

	c = tb.tb_frame.f_code
	if not hasattr(c, 'co_lnotab'):
		return tb.tb_lineno

	tab = c.co_lnotab
	line = c.co_firstlineno
	stopat = tb.tb_lasti
	addr = 0
	for i in range(0, len(tab), 2):
		addr = addr + ord(tab[i])
		if addr > stopat:
			break
		line = line + ord(tab[i+1])
	return line
