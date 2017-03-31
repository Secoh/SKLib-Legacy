Remark on using Visual Studio 2010 or above
------------------------------------------------

Newer Microsoft compilers differentiate between project dependency and project reference.
Dependence is logical connection, causing rebuild of all downstream projects that depend on a modified file (in the upstream project).
Reference is physical attachment, such as a project depending on a library requires compilation-time linking.

By default, Visual Studio version upgrade transfers logical dependency into the physical reference.
This is correct for static libraries (and for static DLLs) - but this is INCORRECT for dynamic DLL loading.

As a result, new user may have to modify default References in the following cases.


lib_dll examples:

All projects - Remove all references except to lib_dll. The lib_dll itself shall not reference anyone.


lib_thread example:

All projects - Remove all references if any present. The lib_thread is header-only project.



