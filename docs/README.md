# Sphinx documentation
Text indexation project is documented with a sphinx documentation with doxygen (C++ code) and Open API (fka Swagger) integrations.
Please, browse to https://[username].github.io/ or [./build/sphinx/html/index.html](./build/sphinx/html/index.html) once you have built documentation.

### Documentation build requirements
In order to build project documentation, you first need to install a conda distribution, e.g. [miniconda](https://docs.conda.io/projects/conda/en/latest/user-guide/install/index.html) or [anaconda](https://www.anaconda.com/distribution/)
Then, you will be able to create a conda environement from ./environement.yml file. This conda environement brings all dependencies needed to build projet documentation:
The conda environement includes sphinx documentation framework, doxygen for C++ code documentation, Breathe which makes the bridge between doxygen and sphinx, sphinxcontrib-openapi integrating Open API (fka Swagger) to sphinx documentation, sphinx_rtd_theme sphinx theme, exhale (automatic breath/doxygen documentation), ...

```
# create conda environement (documentation build dependencies are installed in an isolated conda environement named 'text_indexing_docs')
conda env create -f ./environement.yml
# Once created, you can activate conda environement in order to build documentation
conda activate text_indexing_docs
```

Then, you can (re)build doxygen and sphinx documentation using provided ./makefile (or ./make.bat on windows):
```
make html
```

Note that doxygen is now built by exhale extension when running sphinx (see ./source/config.py).
Once documentation successfully built, you can browse to generated documentation main page: ./build/sphinx/html/index.html  
