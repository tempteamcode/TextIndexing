import textwrap

# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))


# -- Project information -----------------------------------------------------
project = 'OT1 - Text Indexing'
copyright = '2019, Quentin Guye, Nathan Mesnard, Paul-Emmanuel Sotir, Tianjian Ye'
author = 'Quentin Guye, Nathan Mesnard, Paul-Emmanuel Sotir, Tianjian Ye'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    "sphinx.ext.todo", "breathe", "exhale", "sphinxcontrib.openapi"
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
html_theme = 'sphinx_rtd_theme'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

# Tell breathe about the projects:
breathe_projects = {"TextIndexingBackend": "../build/doxygen_backend/xml/"}

# Specify a default project
breathe_default_project = "TextIndexingBackend"

# Setup the exhale extension
exhale_args = {
    # These arguments are required
    "containmentFolder":     "./exhale",
    "rootFileName":          "docs_root.rst",
    "rootFileTitle":         "C++ Backend",
    "afterTitleDescription": textwrap.dedent('''
       .. note::

       The following documentation presents the OT1 - Text Indexation C++ backend. This Sphinx documentation is generated from doxygen C++ code comments through breath and exhale.
    '''),
    "doxygenStripFromPath":  "..",
    # Suggested optional arguments
    "createTreeView":        True,
    # TIP: if using the sphinx-bootstrap-theme, you need
    # "treeViewIsBootstrap": True,
    "exhaleExecutesDoxygen": True,
    "exhaleDoxygenStdin": "INPUT = ../../backend/src \
                                   ../../backend/main.cpp"
}

# Tell sphinx what the primary language being documented is.
primary_domain = 'cpp'

# Tell sphinx what the pygments highlight language should be.
highlight_language = 'cpp'