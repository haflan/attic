#!/bin/bash

# TODO: Generalize to cover more programming languages
# Uses Asciidoc with Coderay to generate HTML with Python syntax highlighting
# from Python code. To install dependencies:
#   apt install asciidoctor
#   gem install coderay

ADOC_FILE=source-code.adoc
printf "= Source code\n" > $ADOC_FILE
printf ":source-highlighter: coderay\n\n" >> $ADOC_FILE

for source in $@; do
    printf "== $source\n" >> $ADOC_FILE
    printf "[source, python]\n" >> $ADOC_FILE
    echo "----" >> $ADOC_FILE
    cat $source >> $ADOC_FILE
    printf "\n" >> $ADOC_FILE
    echo "----" >> $ADOC_FILE
    printf "\n" >> $ADOC_FILE
done

asciidoctor $ADOC_FILE
rm $ADOC_FILE
