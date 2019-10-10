# confini

[![Build Status](https://travis-ci.org/Shadouw/confini.svg?branch=master)](https://travis-ci.org/Shadouw/confini)
[![Build status](https://ci.appveyor.com/api/projects/status/sx0qo69yi4763ase?svg=true)](https://ci.appveyor.com/project/Shadouw/confini)
[![codecov](https://codecov.io/gh/Shadouw/confini/branch/master/graphs/badge.svg)](https://codecov.io/gh/Shadouw/confini)
[![License: LGPL v3](https://img.shields.io/badge/License-LGPL%20v3-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0)
[![Open in Gitpod](https://gitpod.io/button/open-in-gitpod.svg)](https://gitpod.io/#https://github.com/Shadouw/confini)


`confini` is a library to read and write configuration/ini files

Remark about the code coverage: Only the library has test coverage. Therefore examples and additional tools are not covered.

## Introduction

confini supports:
 - `[Sections]`
 - `Key = Value` pairs
 - Comments starting with either `#` or `;`

## Example

Include the header:

    #include <confini.h>
	
Open the file:

	ConfIniFile<char> File1("test.ini");
	
Access a config entry:

	string strSetting = File1("Section 2", "Key1");
	
Change a value:

	File1("Section 2", "Key1") = "My new value";
	
Write your configuration back to disk:

	File1.savefile();
	
	
