# My Competitive Programming Templates

#### This is the templates I use for competitive programming, I have created this repository to keep track of my templates and to share them with others. I will keep updating this repository with new templates and algorithms as I learn through the competitive programming journey.

## MakePDFS.py File

(replace OUTPUT_FILENAME with your file name you put in the variable OUTPUT_FILENAME)

```bash
python MakePDFS.py

timeout 300 pdflatex -interaction=nonstopmode OUTPUT_FILENAME.tex 2>&1 | tail -30
timeout 300 pdflatex -interaction=nonstopmode OUTPUT_FILENAME.tex 2>&1 | tail -30
```

#### Command mustbe run 2 times to generate the table of contents and the references.

### Before you run the script, make sure that you have the following installed:

- Python 3.x
- texlive (or any other LaTeX distribution)
- pdflatex (part of the LaTeX distribution)

### Install texlive

it will take a while to install, so be patient.

```bash
sudo apt-get install texlive-full
```

## NOTE

if you get an error, that is most likely because of the file names, make sure the file names only consistes of letters, numbers, underscores and dashes, if you have white spaces, make sure there are at most 1 white space between words, and no white spaces at the beginning or the end of the file name.
