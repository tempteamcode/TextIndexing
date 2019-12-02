# Minimal makefile for Sphinx documentation
#

# You can set these variables from the command line, and also
# from the environment for the first two.
SPHINXOPTS    ?=
SPHINXBUILD   ?= sphinx-build
SOURCEDIR     = source
BUILDDIR      = build/sphinx
DOXYGEN_BUILDDIR = ./build/doxygen_backend/

# Put it first so that "make" without argument is like "make help".
help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

# Custom rule added in order to build doxygen docs before building sphinx doc
doxygen:
	@echo "\n########################################## BUILDING DOXYGEN ##########################################\n"
	@mkdir -p "$(DOXYGEN_BUILDDIR)"
	@doxygen ./doxyfile.conf 

.PHONY: help Makefile doxygen

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%: Makefile doxygen
	@echo "\n########################################## SPHINX $@ ##########################################\n"
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)
