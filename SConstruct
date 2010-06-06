
import os


OPTIONS = {
  '--boost-dir': {
    'help': 'boost installation directory'
  },
  '--v8-dir': {
    'help': 'v8 installation directory'
  },
  '--fcgixx-dir': {
    'help': 'fcgixx installation directory'
  },
  '--extra-dir': {
    'help': 'comma separated list of directories'
  },
  '--inc-dir': {
    'help': 'comma separated list of raw include directories'
  },
  '--lib-dir': {
    'help': 'comma separated list of raw lib directories'
  }
}


def SetOptions(options): 
  for (name, option) in options.iteritems():
    AddOption(name,
      dest   = option.get('dest', name),
      type   = option.get('type', 'string'),
      nargs  = option.get('nargs', 1),
      default= option.get('default', None),
      help   = option.get('help', ''),
      action = 'store'
    )

def AddBuildPath(env, path, prepend=False):
  if path is None:
    return
  if prepend:
    env.Prepend(CPPPATH=[os.path.join(path,'include')])
    env.Prepend(LIBPATH=[os.path.join(path,'lib')])
  else:
    env.Append(CPPPATH=[os.path.join(path,'include')])
    env.Append(LIBPATH=[os.path.join(path,'lib')])
  
  
env = Environment(ENV = {'PATH' : os.environ['PATH']})

SetOptions(OPTIONS)

AddBuildPath(env, GetOption('--boost-dir'))
AddBuildPath(env, GetOption('--v8-dir'))
AddBuildPath(env, GetOption('--fcgixx-dir'))

if env.GetOption('--extra-dir') is not None:
  [AddBuildPath(env, x) for x in GetOption('--extra-dir').split(',')]

if env.GetOption('--inc-dir') is not None:
  [env.Append(CPPPATH=[x]) for x in GetOption('--inc-dir').split(',')]
if env.GetOption('--lib-dir') is not None:
  [env.Append(LIBPATH=[x]) for x in GetOption('--lib-dir').split(',')]



PROGNAME = 'blog.fcgi'

DESTDIR = 'bin'


SOURCES = [
  'src/main.cpp',
  'src/blog.cpp'
]

CXXFLAGS = [
  '-Wall',
  '-std=c++0x',
  '-O3',
  '-fomit-frame-pointer',
  '-fno-strict-aliasing'
]

CPPPATH = [
]

LIBPATH = [
]

LIBS = [
  'fcgixx',
  'v8',
  'boost_regex',
  'pthread',
  'dl',
  'boost_thread',
  'boost_filesystem',
  'boost_system',
  'mongoclient'
]

env.Append(CXXFLAGS=CXXFLAGS)
env.Append(CPPPATH=CPPPATH)
env.Append(LIBPATH=LIBPATH)
env.Append(LIBS=LIBS)

prog = env.Program(PROGNAME, SOURCES)

env.Install(DESTDIR, PROGNAME)

