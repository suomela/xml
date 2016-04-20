Simple XML tools
================

Compilation
-----------

    git submodule update --init
    make


Usage examples
--------------

Using the following file from EEBO TCP as an example:

  - https://github.com/textcreationpartnership/A12074/blob/master/A12074.xml

Here the general structure of the document is:

    <TEI xmlns="http://www.tei-c.org/ns/1.0">
       <teiHeader>
          ...
       </teiHeader>
       <text xml:lang="eng">
          ...
       </text>
    </TEI>


### Extract plain text version

Extract everything inside `<text>`:

    ./xml-plaintext TEI/text A12074.xml

Output:
            
                CVPIDS
    Whirligig.
                As it hath bene sundry times Acted
    by the Children of the Kings Majesties
    Reuels.
                
                   LONDON.
    Imprinted by E. Allde, and are to bee solde by Arthur
    Iohnson, at the signe of the white Horse, nere
    the great North doore of Saint
    Paules Church.
    1607.
                
                   The Actors names.
                   The Olde Lord Nonsuch.
                   Alderman Venter. A Marchant
                   Sir Timothy Troublesome. a iealious Knight
                   The Lady Troublesome. The iealious Knights wife
                   Maister Correction. The Pedant
                   Mistris Correction. The Midwife
                   Peg. The Ladie Troublesomes Kinswoman
                   Nan. Old Venters Daughter.
    ...


### List all XML tags

List all tags inside `<teiHeader>`:

    ./xml-tags TEI/teiHeader A12074.xml

Output:

    fileDesc
    titleStmt
    title
    author
    editionStmt
    edition
    date
    extent
    publicationStmt
    publisher
    ...

With indentation:

    ./xml-tags --indent TEI/teiHeader A12074.xml

Output:

    fileDesc
     titleStmt
      title
      author
     editionStmt
      edition
       date
     extent
     publicationStmt
      publisher
    ...

Show the full path:

    ./xml-tags --tree TEI/teiHeader A12074.xml

Output:

    fileDesc
    fileDesc titleStmt
    fileDesc titleStmt title
    fileDesc titleStmt author
    fileDesc editionStmt
    fileDesc editionStmt edition
    fileDesc editionStmt edition date
    fileDesc extent
    fileDesc publicationStmt
    fileDesc publicationStmt publisher
    ...

Show the full path in reverse:

    ./xml-tags --reverse TEI/teiHeader A12074.xml

Output:

    fileDesc
    titleStmt fileDesc
    title titleStmt fileDesc
    author titleStmt fileDesc
    editionStmt fileDesc
    edition editionStmt fileDesc
    date edition editionStmt fileDesc
    extent fileDesc
    publicationStmt fileDesc
    publisher publicationStmt fileDesc

Count all tags in the body:

    ./xml-tags TEI/text A12074.xml | sort | uniq -c | pbcopy

Output:

       1 body
       1 closer
     281 desc
      27 div
       1 front
     350 g
     281 gap
      13 head
      44 hi
      16 item
     175 l
       2 list
     945 p
      86 pb
       1 q
       1 seg
       1 signed
     932 sp
     929 speaker
     126 stage
       2 trailer


Copyright
---------

This software is based on pugixml library (http://pugixml.org).
pugixml is Copyright (C) 2006-2015 Arseny Kapoulkine.
