IRAnalyzer
==========

InfraRed Analyzer plugin for Saleae Logic software

Compilation
-----------

You have to download the last available SDK from Saleae at this address: http://support.saleae.com/hc/en-us/articles/201104644-Analyzer-SDK-1-1-14

I have only compiled this plugin on Linux

**Compile the plugin:**

Remove all the files in the *source/* folder and copy the files of this repository in it (.cpp, .h files)
Apply this patch on 'build_analyzer.py':

	41,42c41,42
	< include_paths = [ "../include" ]
	< link_paths = [ "../lib" ]
	---
	> include_paths = [ "include" ]
	> link_paths = [ "lib" ]

If you are using a 64 bits compiler, apply this patch:

	43c43
	< link_dependencies = [ "-lAnalyzer" ] #refers to libAnalyzer.dylib or libAnalyzer.so
	---
	> link_dependencies = [ "-lAnalyzer64" ] #refers to libAnalyzer.dylib or libAnalyzer.so

In the root of the SDK folder: python build_analyzer.py

**Install the plugin**

If the compilation went well, you should have your library (.dll, .so, .dylib) in the *release/* folder.
Copy this file in the folder *Analyzers/* of your *Logic* installation folder.

Please note
-----------

The last SDK version released by Saleae is 1.1.14 as of August 25th 2014.
Because of that, this plugin is no longer working for the SW Logic >1.1.18beta.

I will adapt this plugin once the new SDK is out.

Any questions, comments ?

Send to: procule .at. temis . qc . ca


