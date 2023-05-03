#!/usr/bin/env bash

mkdir Main-Document
touch Main-Document/main-document.tex
MAIN="Main-Document/main-document.tex"
echo "\documentclass[12pt]{article}" >> $MAIN
echo "" >> $MAIN
echo "\begin{document} " >> $MAIN

while true
do
    read -p "What is the name of the section you would like to add to the main document? If you want to finish your document, type n " section
    if [ "$section" == "n" ]; then
        break
    fi
    mkdir "$section"
    sec="$(echo "$section" | awk '{print tolower($0)}')"
    touch "$section/$sec.tex"
    echo "\section{$section}" >> "$section/$sec"
    echo "\input{../$section/$sec.tex}" >> $MAIN
done

echo "\end{document} " >> $MAIN
