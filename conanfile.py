from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMakeToolchain

class ArchimedesTemplate(ConanFile):
	name = 'ArchimedesTemplate'
	settings = 'build_type'

	options = {
		# build as shared (.so/.dll)
		'shared': [True, False]
	}

	default_options = {
		'shared': False
	}

	def requirements(self):
		# require archimedes v0.1.1
		self.requires('archimedes/0.1.1')

	# define CMake layout
	def layout(self):
		cmake_layout(self)

	# CMake generation settings
	def generate(self):
		tc = CMakeToolchain(self)
		tc.user_presets_path = False
		tc.generate()

	generators = 'CMakeDeps'
