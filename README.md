# PlagiarismDetector
CMOS: A Lexical Analysis Tool for Plagiarism Detection
Overview
This project demonstrates how to use FLEX for lexical analysis to detect potential plagiarism in student assignments. The process follows a method similar to MOSS (Measure Of Software Similarity). The main steps are:

Tokenization: Student programs are tokenized, removing all irrelevant features that can be changed to hide plagiarism, such as variable names, comments, whitespace, etc.
Fingerprint Creation: Digital "fingerprints" are created for each program based on the tokenized code.
Comparison: Pairs of programs are compared based on the number of matching fingerprints. A higher number of matching fingerprints indicates potential plagiarism.
The program developed in this project is called CMOS, a MOSS-like program that works on C code. It takes a single command line argument, which is the name of the directory containing student Examples.

**Components**
1. cmos.l : A LEX program to parse and tokenize a student submission.

2. cmos.cpp : A C++ program that reads a file containing all tokenized submissions (tokens.txt) and performs the fingerprint analysis as described in the Winnowing Algorithm paper.

3. Makefile : A makefile to compile the project.

4. Bash Script : A provided Bash script that reads through the directory of examples (directory name taken as an argument), tokenizes each one, prepares the tokens.txt file for CMOS to analyze, and calls CMOS to perform the analysis.
