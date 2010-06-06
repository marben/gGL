libSources = Split("""src/
		""")

env = Environment()
Export('env')

env.Append(CCFLAGS = '-O3')
env.Append(CCFLAGS = '-msse')
env.Append(CCFLAGS = '-ffast-math')
env.Append(CCFLAGS = '-Wall')

SConscript('src/SConscript', build_dir='Release', duplicate=0)

#gglTest1 = env.Program(Target = 'ggl', source = ['src/ggl.cc'])
