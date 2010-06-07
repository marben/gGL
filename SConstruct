import os

#libSources = Split("""src/
#		""")


BIN_DIR = '#bin'
LIB_DIR = '#lib'

env = Environment()


release = ARGUMENTS.get('release', 0)

env.Append(CCFLAGS = ['-O3', '-msse', '-ffast-math', '-Wall'])
#env.Append(CCFLAGS = '-msse')
#env.Append(CCFLAGS = '-ffast-math')
#env.Append(CCFLAGS = '-Wall')

#env['CC'] = 'clang'
#env['CXX'] = 'clang'

env.Append(CPPPATH = ['#src'])	# TODO: there should exist include dir for the tests and other programs
env.Append(CPPPATH = ['/usr/include/eigen2'])

env.Append(LIBPATH = ['#lib'])
env.Append(LIBS = ['ggl'])

env.ParseConfig("pkg-config --cflags --libs sdl libpng")

Export('env', 'BIN_DIR', 'LIB_DIR')

SConscript('src/SConscript', variant_dir='build/src', duplicate=0)
SConscript('test/SConscript', variant_dir='build/test', duplicate=0)


#env.Program(Target = '../ggl', source = ['test/src/ggl.cc'])
