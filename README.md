# libanatomy

This project aims to provide medical resources that meet the needs for software development and contents creators alike.
The idea is to use as simple as possible file formats to produce content and tools will produce usable output for different scenarios. For example, translation files are .CSV files that can be easily edited, merged and shared, but the post processing tool will output the data in different more oriented formats.

Directory Structure
-------------------

**sources**: This directory contains any data coming from other sources, for example TA1 and TA2 terms definitions. The content here is shared under the original license, the license or an explicit exception must allow AnaGenerator to produce content under AnaGenerator license requirements.

**content**: This directory contains content produced by contributors of the project. This content can be produce by any mean, autonomous or not, but it must respect the format established for the particular content and it must be shared based on the 'Creative Commons Attribution-NoDerivatives 4.0 International license'.

**data-resources**: This directory contains data produced by processing the content with the generator tools. Many different output format will be produced based on the requirements of the community. The content here is shared under the 'Creative Commons Attribution-NoDerivatives 4.0 International license'.

**dev-resources**: This directory contains data produced by processing the content with the generator tools. Many different output format will be produced based on the requirements of the community. The content here is shared under the 'LGPL v3.0 license'.

**tools**: This directory contains applications and scripts usable for the production and the generation of content. The content here is shared under the 'GPL v3.0 license'.


AnaGenerator
------------
AnaGenerator is the main tool used to generate the **resources**. It will be used by the project administrator periodically to keep the resources in sync with the contributed content. This application generates two different sets of files, one set intended for **data-resources** and one set for **dev-resources**. Any content produced by this application or any fork of it will release the output files of the **data-resources** set under the 'Creative Commons Attribution-NoDerivatives 4.0 International license' and the output files of the **dev-resources** set under the 'LGPL v3.0 license'.

TaMixer
-------
TA mixer is an application oriented on matching the terms from the TA1 and TA2 sets, it produces an output to be included in the contents for generating the libanatomy's terms file.


AnaViewer
---------
AnaViewer is an application oriented on exploring the contents in a way that can assist on the contents creator's work flow.






 