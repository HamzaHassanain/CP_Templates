
# For all cpp files in this folder and subfolders, make a pdf of the code

import os
# Get all cpp files in this folder and subfolders

subfolders = ["Trees", "Strings", "Range-Queries", "Math", "Graph"]
# make a new folder called PDFs

# if there is PDFs folder, delete it and all its contents


class PDF_NOT_CREATED(Exception):

    def __init__(self, message):
        super().__init__(message)


if os.path.exists("PDFs"):
    os.system("rm -r PDFs")

os.system("mkdir PDFs")

try:

    for subfolder in subfolders:
        os.system("mkdir PDFs/" + subfolder)
        cpp_files = []
        for root, dirs, files in os.walk("./" + subfolder):
            for file in files:
                if file.endswith(".cpp"):
                    cpp_files.append(file)

        for file in cpp_files:

            expected_pdf = "PDFs/" + subfolder + "/" + file + ".pdf"

            command = "enscript -Ecpp -o - " + subfolder + "/" + \
                file + " | ps2pdf - " + expected_pdf
            # print OOO in Green

            print("\033[92m" + "Running Command: " + command + "\033[0m")

            os.system(command)

            if (os.path.exists(expected_pdf)):
                # print in Green
                print("\033[92m" + "PDF created successfully" + "\033[0m")
            else:
                raise PDF_NOT_CREATED(
                    "PDF not created: " + file + "\nError Running: " + command)
except PDF_NOT_CREATED as e:

    # print in Red

    print("\033[91m" + str(e) + "\033[0m")

    print("\033[91m" + "Error in making PDFs" + "\033[0m")

    exit(0)
