import os.path
import subprocess
import sys
from Tkinter import Tk



def copy_to_clipboard(to_copy):
    r = Tk()
    r.withdraw()
    r.clipboard_clear()
    r.clipboard_append(to_copy)
    r.destroy()


def preprocess_file(file_name_to_preprocess, preprocessed_file_name):
    print " preprocessing : " + file_name_to_preprocess 
    print " to : " + preprocessed_file_name 
    subprocess.call(["cpp", file_name_to_preprocess, "-o", preprocessed_file_name]);# cpp ~ g++ -E
    return preprocessed_file_name


def do_one_file(input_file, to_preprocess_file, output_file):

    if len(sys.argv) != 2:
        print "Usage: give the cpp main file in argument"
        print "We will preprocess the local #include part (with quotes)"
        print "and we avoid to preprocess the headers of the standard library"
        sys.exit(1)

    for line in input_file:
        if "#include <" in line.strip():
            output_file.write(line)#we copy the standard headers directly in the finale file
        elif "#include \"" in line.strip():
            #print line
            #print line.split("\"")[1]
            included_file = open(line.split("\"")[1], "r")
            do_one_file(included_file, to_preprocess_file, output_file)
            included_file.close()
        else:
            to_preprocess_file.write(line)#we copy the rest including user headers in the file to be preprocessed


def main():
    #names
    input_file_name = sys.argv[1]# 1 because 0 is the name of the python file
    to_preprocess_file_name = "toPreprocess.cpp"
    output_file_name = "res.cpp"
    preprocessed_file_name = "main.i"

    #open files
    input_file = open(input_file_name, "r")
    to_preprocess_file = open(to_preprocess_file_name, "w")
    output_file = open(output_file_name, "w")

    #main function
    do_one_file(input_file, to_preprocess_file, output_file)

    #do the preprocessing
    to_preprocess_file.close()
    preprocessed_file = open(preprocess_file(to_preprocess_file_name, preprocessed_file_name), "r")
    for line in preprocessed_file:
        if not "#" in line: #don't copy the comments from the preprocessing
            output_file.write(line)
    preprocessed_file.close()

    #close remaining files
    input_file.close()
    output_file.close()

    #copy to clipboard
    output_file = open(output_file_name, "r")
    copy_to_clipboard(output_file.read())
    output_file.close()


main()