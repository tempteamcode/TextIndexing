# Minimal makefile for Sphinx documentation
#

# You can set these variables from the command line, and also
# from the environment for the first two.
SPHINXOPTS    ?=
SPHINXBUILD   ?= sphinx-build
SOURCEDIR     = source
BUILDDIR      = build/sphinx

# Put it first so that "make" without argument is like "make help".
help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

# TODO: remove this (doxygen is now built by exhale)
#DOXYGEN_BUILDDIR = ./build/doxygen_backend/
#doxygen:
	# build doxygen docs before running sphinx
	#	@echo "\n########################################## BUILDING DOXYGEN ##########################################\n"
	#	@mkdir -p "$(DOXYGEN_BUILDDIR)"
	#	@doxygen ./doxyfile.conf 

.PHONY: help Makefile

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%:
	@echo "\n########################################## SPHINX $@ ##########################################\n"
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

