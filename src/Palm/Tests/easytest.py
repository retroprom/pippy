VERSION = '$Id: easytest.py,v 1.2 2001/02/13 20:12:43 mgorlick Exp $'

import sys
import string
import traceback
from types import ListType, TupleType

STANDARD_ERROR = sys.stderr

class Outcome:
  def __init__(self):
    self.failures = []
    self.errors = []
    self.runs = 0
    self.passes = 0
    self.abort = 0

  def Start(self, test):
    self.runs = self.runs + 1

  def End(self, test): pass

  def Error(self, test, error):
    self.errors.append((test, error))

  def Failure(self, test, failure):
    self.failures.append((test, failure))

  def Pass(self, test):
    self.passes = self.passes + 1

  def Passed(self):
    return not self.failures and  not self.errors

  def Abort(self):
    self.abort = 1

  def Aborted(self): return self.abort

  def __repr__(self):
    return \
      "<%s runs:%i passes:%i errors:%i failures:%i>" % \
      (self.__class__, self.runs, self.passes, len(self.errors), len(self.failures))

class Fixture:  
  def Erect(self): pass
  def Teardown(self): pass

class Experiment(Fixture):
  def __init__(self, name = 'Test'):
    try:
      self.method = getattr(self, name)
    except AttributeError:
      raise ValueError, 'No experiment method: %s' % name

  def Cardinality(self): return 1

  def __repr__(self):
    return '<%s method:%s>' % (self.__class__, self.method.__name__)

  def Execute(self, outcome):
    return self(outcome)

  def __call__(self, outcome):
    # All tests are bracketed by the outcome's Start and End.
    outcome.Start(self)

    try:
      # Attempt to erect the test fixture. If the erection itself fails record that as a test error.
      try:
        self.Erect()
      except:
        outcome.Error(self, self.Trace())
        return

      # Next conduct the test case itself.
      try:
        self.method()
        outcome.Pass(self)  # The test passed.
      except AssertionError:
        outcome.Failure(self, self.Trace())  # The test failed.
      except:
        outcome.Error(self, self.Trace())  # The test is erroneous in some fashion.

      # With the test complete teardown the test fixture.
      try:
        self.Teardown()
      except:
        outcome.Error(self, self.Trace())  # If the teardown itself fails record that as a test error.

    # No matter what record the end of the test.
    finally:
      outcome.End(self)

  def Trace(self):
    flavor, value, trace = sys.exc_info()
    previous = trace.tb_next
    if previous is None:
      return (flavor, value, trace)
    else:
      return (flavor, value, previous)    

class Suite:
  def __init__(self, experiments = ()):
    self.experiments = []
    self.Enroll(experiments)

  def __str__(self):
    return "<%s tests:%s>" % (self.__class__, self.experiments)
  __repr__ = __str__

  def Cardinality(self):
    n = 0
    for e in self.experiments:
      n = n + e.Cardinality()
    return n

  def Enroll(self, enrollee):
    if type(enrollee) in (ListType, TupleType):
      for x in enrollee:
        self.experiments.append(x)
    else:
      self.experiments.append(enrollee)

  def Execute(self, outcome):
    return self(outcome)

  def __call__(self, outcome):
    for e in self.experiments:
      if outcome.Aborted():
        break
      e(outcome)
    return outcome

class Text_Outcome(Outcome):
  RUNS_PER_LINE = 5

  def __init__(self, stream = STANDARD_ERROR):
    Outcome.__init__(self)
    self.stream = stream

  def Start(self, test):
    Outcome.Start(self, test)

  def Failure(self, test, failure):
    Outcome.Failure(self, test, failure)
    self.stream.write('<F>')
    self.stream.flush()

  def Error(self, test, error):
    Outcome.Error(self, test, error)
    self.stream.write('<E>')
    self.stream.flush()

  def Pass(self, test):
    Outcome.Pass(self, test)
    self.stream.write('<P>')
    self.stream.flush()

  def End(self, test):
    Outcome.End(self, test)
    if self.runs % self.RUNS_PER_LINE == 0:
      self.stream.write('\n')
    self.stream.flush()

  def Print_errors(self):
    i = 1
    for test, trace in self.errors:
      self.stream.write("Error %i:%s\n" % (i, test))
      self.stream.write(string.join(apply(traceback.format_exception, trace), ''))
      self.stream.write('\n')
      i = i + 1

  def Print_failures(self):
    i = 1
    for test, trace in self.failures:
      self.stream.write("Failure %i:%s\n" % (i, test))
      self.stream.write(string.join(apply(traceback.format_exception, trace), ''))
      self.stream.write('\n')
      i = i + 1

  def Print_header(self):
    self.stream.write('\n')
    if self.Passed():
      indicator = '+'
    else:
      indicator = '-'
    self.stream.write(
      "%s Runs:%i Passes:%i Failures:%i Errors:%i\n" % \
      (indicator, self.runs, self.passes, len(self.failures), len(self.errors)))

  def Print(self):
    self.Print_header()
    self.Print_failures()
    self.Print_errors()

def trust_but_verify(module):
  outcome = Text_Outcome()
  module.verify(outcome)
  return outcome
