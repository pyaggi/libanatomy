
This directory contains content produced by contributors of the project. This content can be produce by any mean, autonomous or not, but it must respect the format established for the particular content. The project Libanatomy shares this content under the 'Creative Commons Attribution-NoDerivatives 4.0 International license'.

------------------
## Term Files

ana_terms_definitions.csv
------------------
This file contains the source definitions for the terms database for the code range between 400008 and 499994. This code range is known as the LibAnatomy's Term Range. The order of the rows will never change. The row content will be expanded but never reduced. 
Terms must be added to this list in hierarchy fashion, meaning whenever is possible a term must have a parent term. Each term has to have a type:

**Term (T)**: The term symbolizes a thing

**Group (G)**: The term symbolizes a group. This group must be the most intrinsic aggregation compatible with the term. For example the *Biceps Brachii* term could be a group where *Biceps Brachii Long* and *Biceps Brachii Short* could be children. Groups can have children of type Group and any *other type* but this *other type* should not be changed deep into the hierarchy.

**Movement (M)**: The term symbolizes a movement

**Selection (S)**: The term symbolizes a selection, this is a group which can have mixed types of any kind as children.

(types will be extended)

If a term has more than one meaning it must appear duplicated but belonging to different Groups. For example the term *Abductor Digiti Minimi* should belong to two groups like *Muscles of the Hand* and *Muscles of the Foot*.

A single meaning must not represented by two terms, synonyms must be specified in the column for that.


```
CSV columns description:

|Column Name  |Short description
|-------------|----------------------------------------|
|SeqId        |Sequential id                    
|Parent       |ID of the parent TERM                   
|Latin        |TERM in Latin                           
|UsEnglish    |TERM in English (US)                    
|LatSynonyms  |Synonyms for the term                
|EngSynonyms  |Synonyms for the term                
|Type         |"T","G","M","S"
|Sex          |"","M","F","B"
```

**SeqId**: This is a unique sequential number identifying the entry, starting at 0. It must never be reassigned. There must be no holes in the sequence. It must always grow.

**Parent**: This column contain the parent term for the row, it can be -1, it must be the SeqId if the parent term is included or it must be the AnaTermID.

**LatinName/UsEnglish**: This two columns represent the term in Latin and US-English respectively, at least one of this must be present.

**LatSynonyms/EngSynonyms**: This two columns represent synonyms for the term in Latin and US-English respectively. This two fields are optional. If multiple synonyms are specified they must be delimited by semicolon (';'), and for legibility a space should be included after the semicolon.


ana_terms-XXXX.csv
------------------

XXXX stands for the locale-language code, it is the two letter ISO-639 language code optionally followed by an underscore(_) and the two letter ISO-3166 country code for the region dialect, ie:
	
	1. ana_terms-es.csv
	2. ana_terms-es_AR.csv
	
This files contains the translation 'sources' for the terms, this sources are updated whenever the AnaTerms database is expanded. The .csv format allows the translator not only to work with the file with a simple text editor, with a spreadsheet and with the dedicated tool AnaTranslator. Also this format allows the easy merge and share.

```
CSV columns description:

|Column Name  |Short description
|-------------|----------------------------------------|
|Status       |translation status indicator                         
|Id           |AnaTermID of the TERM                                
|Parent       |AnaTermID of the parent TERM                         
|LatinName    |TERM in Latin                                        
|UsEnglish    |TERM in English (US)                                 
|Translation  |Translation of the term                              
|Synonyms     |Synonyms for the term                                
```

All fields but *Translation* and *Synonyms* are to be considered *Read-Only*, that information is systematically generated and must not be edited by the translator. The translator must provide the translation of the term for the implicated language in the column *Translation*, and should provide any meaningful synonym in the column *Synonyms*. The *Translation* could be empty meaning the term is the same in both languages.

Status indicates the *translation status* of the term, it is a one character indicator:
```
	N: new addition to the file
	A: *term* was automatically translated without user intervention
	T: *term* is translated
	O: *term* became obsolete, the translator should remove this record
```

We encourage contributors to use AnaTranslator to manipulate these files.

--------------------
## Other files

ta_map.json
-----------
This file contains the mapping between TA1 and TA2 terms. It is a JSON Array of JSON objects. Each objects contains two key values, "ta1" and "ta2", containing the "ta1 id" and the "ta2 number" respectively.
This file can be produced by the **tamixer** tool.

We encourage contributors on working on this working in this mapping asap.
