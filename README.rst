String++
=========

.. contents:: Table of Contents

Introduction
-------------

**String++** is a utility project written in C++ to simplify working
with character arrays (``char *``).

Technologies Used
------------------

* Windows 10 x64
* Visual Studio Code
* Clang++ 14.0.3
* C++20

Project Status
---------------

This project is currently *in development*.

Version Naming
---------------

This library uses semantic versioning:

.. code:: txt

  MAJOR.MINOR.PATCH

Where an increment in:

* ``MAJOR`` = Incompatible changes (i.e., code may need to be updated).
* ``MINOR`` = Backwards compatible feature changes.
* ``PATCH`` = Backwards compatible bug fixes.

Getting Started
----------------

*Requires C++11 or newer!*

Here is a simple example program to help you get started:

* Download the latest version of the project.

* ``#include`` the proper header into your project:

.. code:: cpp

  #include <ndg/string.hh>

* The entire project is encapsulated within the ``ndg`` namespace.
  Creating a new String object is easy:

.. code:: cpp

  ndg::String hello_world = "Hello, World!";
  // or
  ndg::String hello_world = ndg::String("Hello, World!");

* This object also supports Python-style formatting! Here is a slightly
  more complex example:

.. code:: cpp

  #include <iostream>

  #include <cstdio>
  #include <ctime>

  #include <ndg/string.hh>

  using std::time_t;
  using std::time;
  using std::tm;
  using std::localtime;
  using std::strftime;
  using std::cout;
  using std::endl;

  int main() {
    ndg::String log_file_fmt = "{}.{}.log";

    char date_time[17];
    time_t t = time(&t);
    struct tm *time_info = localtime(&t);
    strftime(
      date_time,
      sizeof(date_time),
      "%Y%m%d.%H%M%S",
      time_info
    );

    cout << (log_file_fmt % date_time % "FATAL") << endl;

    return 0;
  }

* To compile the program, I'm going to use Clang++:

.. code:: powershell

  > clang++ ./example.cc -o ./example.exe -I ./include ./ndg/string.cc
  > ./example.exe

* Expected output should look similiar to this:

.. code:: txt

  20220702.122421.FATAL.log

* Voila! You have successfully created a program using the String project.

For features and implementation, please refer to the
`header <./include/ndg/string.hh>`_ / `source <./ndg/string.cc>`_ files.

Contributing
-------------

Want to contribute? Great!

To fix a bug or enhance an existing module, follow these steps:

* Fork the repository and create a new branch:

.. code:: powershell

  > git checkout -b "improve-feature"

* Make the appropriate changes then stage the files:

.. code:: powershell

  > git add <file(s) or *.cc>

* Commit the changes:

.. code:: powershell

  > git commit -m "Improve feature."

* Push to the new branch:

.. code:: powershell

  > git push "origin" "improve-feature"

* Create a `Pull Request`_.

Bug/Feature Request
--------------------

If you find a bug (program failed to run and/or gave undesired results)
or you just want to request a feature, kindly open a `new issue`_.

Room for Improvement
---------------------

Room for improvement:

- There are some areas documented in the source file but generally speaking,
  the string formatting division could use some improvement.

Unimplemented features:

- Operator overloads. The project is supposed to be a wrapper around
  the char pointer (``char *``), so ideally I would like for it to be able to
  do anything that can be done to a regular character array. There are some
  operators (e.g., (pre/post) increment/decrement operators and others) that
  have not been implemented yet.

.. _Pull Request: https://github.com/nicdgonzalez/String/pulls
.. _new issue: https://github.com/nicdgonzalez/String/issues
