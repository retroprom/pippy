
/* Time module replacement for PalmOS */

#include "kludge.h"
#include "palmnetmodule.h"

/* PalmOS doesn't have account for timezones - we allow the user to set
   these values. */
static long timezone = 0;
static long daylight = 0;


#define TIME_IN_USEC 0
#define TIME_IN_MSEC 1
#define TIME_IN_SEC 2


/* Forward declarations */
static UInt32 inttime(void) SEG_TIMEMODULE_C;
static PyObject *time_settimezone(PyObject *self, PyObject *args) SEG_TIMEMODULE_C;
static PyObject *time_time(PyObject *self, PyObject *args) SEG_TIMEMODULE_C;
static PyObject *time_sleep(PyObject *self, PyObject *args) SEG_TIMEMODULE_C;
static PyObject *datetime_totuple(DateTimePtr p) SEG_TIMEMODULE_C;
static PyObject *time_convert(UInt32 when, UInt16 flag) SEG_TIMEMODULE_C;
static PyObject *time_gmtime(PyObject *self, PyObject *args) SEG_TIMEMODULE_C;
static PyObject *time_localtime(PyObject *self, PyObject *args) SEG_TIMEMODULE_C;
static int get_datetime_arg(PyObject *args, DateTimePtr p) SEG_TIMEMODULE_C;
static PyObject *time_asctime(PyObject *self, PyObject *args) SEG_TIMEMODULE_C;
static PyObject *time_ctime(PyObject *self, PyObject *args) SEG_TIMEMODULE_C;
static PyObject *time_mktime(PyObject *self, PyObject *args) SEG_TIMEMODULE_C;
static void ins(PyObject *d, char *name, PyObject *v) SEG_TIMEMODULE_C;
DL_EXPORT(void) inittime() SEG_TIMEMODULE_C;
static int intsleep(UInt32 secs, UInt32 usecs) SEG_TIMEMODULE_C;

static PyObject *time_ticks(PyObject *self, PyObject *args) SEG_TIMEMODULE_C;
static PyObject *time_tickstotime(PyObject *self, PyObject *args) SEG_TIMEMODULE_C;


static PyObject *
time_settimezone(PyObject *self, PyObject *args)
{
	PyObject *a, *m, *d;
	/* set the timezone offset in seconds from UTC */
	if (!PyArg_Parse(args, "l", &timezone))
		return NULL;
	if (!(a = PyImport_GetModuleDict()))
		return NULL;
	if (!(m = PyDict_GetItem(a, PyString_FromString("time"))))
		return NULL;
	if (!(d = PyModule_GetDict(m)))
		return NULL;
	if ((PyDict_SetItemString(d, "timezone", 
				  PyInt_FromLong(timezone))==-1))
		return NULL;
	
	Py_INCREF(Py_None);
	return Py_None;

}

static PyObject *
time_time(self, args)
	PyObject *self;
	PyObject *args;
{
	UInt32 secs;
	if (!PyArg_NoArgs(args))
		return NULL;
	secs = inttime();
	return PyInt_FromLong(secs);
}

/* 
   Return the tick count since the last reset. The tick count does not advance
   while the device is in sleep mode. 
*/

static PyObject *
time_ticks(self, args)
	PyObject *self;
	PyObject *args;
{
	UInt32 ticks;
	if (!PyArg_NoArgs(args))
		return NULL;

	ticks = TimGetTicks();

	return PyInt_FromLong(ticks);
}

static PyObject *
time_tickstotime(self, args)
	PyObject *self;
	PyObject *args;
{
	UInt32 ticks;
        Int16 unit = TIME_IN_MSEC;
	UInt32 factor;

	if (!PyArg_ParseTuple(args, "l|i", &ticks, &unit))
		return NULL;
	
	switch (unit) 
	{
	case TIME_IN_USEC: /* time in microseconds */
		factor = 1000000;
		break;
	case TIME_IN_MSEC:
		factor = 1000;
		break;
	case TIME_IN_SEC:
		factor = 1;
		break;
	default:
		PyErr_SetString(PyExc_ValueError, "Invalid time unit specifier");
		return NULL;
	}
		
	return PyInt_FromLong(ticks * factor / SysTicksPerSecond());
}



static PyObject *
time_sleep(self, args)
	PyObject *self;
	PyObject *args;
{
	long secs, usecs, milliseconds;
	
	if (!PyArg_ParseTuple(args, "l:sleep", &milliseconds))
		return NULL;

	if (milliseconds < 0) {
		PyErr_SetString(PyExc_ValueError, "timeout in milliseconds must be a positive number");
		return NULL;
	}
	secs = milliseconds / 1000;
	usecs = (milliseconds - secs * 1000) * 1000;
	    
	if (intsleep(secs,usecs) != 0)
		return NULL;

	Py_INCREF(Py_None);
	return Py_None;
}


static PyObject *
datetime_totuple(DateTimePtr p)
{
	/* convert the Palm Time DateTimeType to a tuple identical
	   in format to the standard time.h struct tm.

typedef struct {
	Int16 second;
	Int16 minute;
	Int16 hour;
	Int16 day;
	Int16 month;
	Int16 year;
	Int16 weekDay;		// Days since Sunday (0 to 6)
	} DateTimeType;

	*/

	UInt32 day_of_year = 0;
/* The following mess is required for setting the day of the year.  If
   this parameter is not required, the comment out the following code
   up to the Py_BuildValue routine. */

	DateType date_today, date_boy;
	UInt32 days_today, days_boy;
	DateTimeType boy = {0,0,0,1,1,0,4};
	UInt32 tt_today, tt_boy;

	boy.year = p->year;
	
	tt_today = TimDateTimeToSeconds (p);
	DateSecondsToDate(tt_today, &date_today);
	days_today = DateToDays(date_today);

	tt_boy = TimDateTimeToSeconds (&boy);
	DateSecondsToDate(tt_boy, &date_boy);
	days_boy = DateToDays(date_boy);

	day_of_year = days_today - days_boy;

	return Py_BuildValue("(iiiiiiili)",
			     p->year,
			     p->month,
			     p->day,
			     p->hour,
			     p->minute,
			     p->second,
			     (p->weekDay + 6) % 7, /* Want Monday == 0 */
			     day_of_year,	   /* don't have day of the year */
			     0);   /* assume we aren't using DST */
}


static PyObject *
time_convert(UInt32 when, UInt16 flag)
{
	DateTimeType p;

	TimSecondsToDateTime (when, &p);

	/* if flag, then convert to localtime */
	if (flag)
		TimAdjust(&p, -timezone);
	return datetime_totuple(&p);
}


static PyObject *
time_gmtime(self, args)
	PyObject *self;
	PyObject *args;
{
	UInt32 when;
	if (!PyArg_Parse(args, "l", &when))
		return NULL;
	return time_convert(when, 0);
}

static PyObject *
time_localtime(self, args)
	PyObject *self;
	PyObject *args;
{
	UInt32 when;
	if (!PyArg_Parse(args, "l", &when))
		return NULL;
	return time_convert(when, 1);
}


static int
get_datetime_arg(PyObject *args, DateTimePtr p)
{
	Int16 dum1, dum2;
	memset((ANY *) p, '\0', sizeof(DateTimeType));

	if (!PyArg_Parse(args, "(iiiiiiiii)",
			 &p->year,
			 &p->month,
			 &p->day,
			 &p->hour,
			 &p->minute,
			 &p->second,
			 &p->weekDay,
			 &dum1,
			 &dum2))
		return 0;
	p->weekDay = (p->weekDay + 1) % 7;
	return 1;
}


static PyObject *
time_asctime(self, args)
	PyObject *self;
	PyObject *args;
{
/* 
   for the following d, check the revision of the OS - this 
   function is only available for PalmOS 3.5

   desired format: Sat Oct 14 17:04:31 2000

*/
	PyObject *tup;
	char d[11], buf[25];
	DateTimeType dt;

	if (!PyArg_ParseTuple(args, "O:asctime", &tup))
		return NULL;

	if (!get_datetime_arg(tup, &dt))
		return NULL;

	/* use DateTemplateToAscii to format the first portion of the date - 
	   that way, we don't have to mess with a set of strings representing
	   the days and months. */
	DateTemplateToAscii("^1r ^2r ^0z", dt.month, dt.day, dt.year, d, 11);
	sprintf(buf, "%s %02d:%02d:%02d %d", d, 
		dt.hour, dt.minute, dt.second, dt.year);

	return PyString_FromStringAndSize(buf, 24);
}

static PyObject *
time_ctime(self, args)
	PyObject *self;
	PyObject *args;
{
	return PyString_FromString("Not Implemented");
}

static PyObject *
time_mktime(self, args)
	PyObject *self;
	PyObject *args;
{
	PyObject *tup;
	DateTimeType buf;
	UInt32 tt;

	if (!PyArg_ParseTuple(args, "O:mktime", &tup))
		return NULL;

	if (!get_datetime_arg(tup, &buf))
		return NULL;

	tt = TimDateTimeToSeconds (&buf);
	return PyInt_FromLong(tt);
}

static PyMethodDef time_methods[] = {
	{"time",	time_time, 0, NULL},
	{"sleep",	time_sleep, METH_VARARGS, NULL},
	{"gmtime",	time_gmtime, 0, NULL},
	{"localtime",	time_localtime, 0, NULL},
	{"asctime",	time_asctime, 1, NULL},
	{"ctime",	time_ctime, 0, NULL},
	{"mktime",	time_mktime, 1, NULL},
	{"settimezone",	time_settimezone, 0, NULL},
	{"ticks",	time_ticks, 0, NULL},
	{"tickstotime",	time_tickstotime, 1, NULL},
	{NULL,		NULL}		/* sentinel */
};

static void
ins(d, name, v)
	PyObject *d;
	char *name;
	PyObject *v;
{
	if (v == NULL)
		Py_FatalError("Can't initialize time module -- NULL value");
	if (PyDict_SetItemString(d, name, v) != 0)
		Py_FatalError(
		"Can't initialize time module -- PyDict_SetItemString failed");
	Py_DECREF(v);
}

DL_EXPORT(void)
inittime()
{
	PyObject *m, *d;
	m = Py_InitModule3("time", time_methods, NULL);
	d = PyModule_GetDict(m);

	Py_INCREF(d);

	ins(d, "timezone", PyInt_FromLong((long)timezone));
	ins(d, "daylight", PyInt_FromLong((long)daylight));
	ins(d, "TIME_IN_USEC", PyInt_FromLong((long) TIME_IN_USEC));
	ins(d, "TIME_IN_MSEC", PyInt_FromLong((long) TIME_IN_MSEC));
	ins(d, "TIME_IN_SEC", PyInt_FromLong((long) TIME_IN_SEC));

	if (PyErr_Occurred())
		Py_FatalError("Can't initialize time module");
}



/* Palm replacement for Python floattime - we don't currently
   have floats in Palm Python */
static UInt32 
inttime() 
{
	/* Return the time in seconds since 1/1/1904 12AM.   The time
	   is localtime. */

	return TimGetSeconds() + timezone;
}

static int
intsleep(secs, usecs)
	UInt32 secs, usecs;
{
	struct timeval t;

	if (!PalmNet_IsOpen()) {
		PyErr_SetString(PyExc_SystemError, "Must first import palmnet");
		return -1;
	}

	t.tv_sec = secs;
	t.tv_usec = usecs;

	Py_BEGIN_ALLOW_THREADS
	if (select(0, (fd_set *)0, (fd_set *)0, (fd_set *)0, &t) != 0) {
		Py_BLOCK_THREADS
#ifdef EINTR
		if (errno != EINTR) {
#else
		if (1) {
#endif
			PyErr_SetFromErrno(PyExc_IOError);
			return -1;
		}
	}
	Py_END_ALLOW_THREADS
	return 0;
}
