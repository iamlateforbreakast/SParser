/**********************************************//** 
  @file FileMgr.c
  
  @brief The FileMgr class manages a list of files contained
  in a group of locations.
  
  The class FileMgr is TBD
**************************************************/

#include "FileMgr.h"
#include "String2.h"
#include "Class.h"
#include "Object.h"
#include "List.h"
#include "FileDesc.h"
#include "Memory.h"
#include "Error.h"
#include "Debug.h"
#include "FileIo.h"

#define DEBUG (0)

#define FILEMGR_MAX_PATH (1024)

/**********************************************//**
  @class FileMgr
**************************************************/
struct FileMgr
{
  Object object;
  List * files;
  List * directories;
  char * separator;
  String * rootLocation;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class fileMgrClass =
{
  .f_new = 0,
  .f_delete = (Destructor)&FileMgr_delete,
  .f_copy = (Copy_Operator)&FileMgr_copy,
  .f_comp = (Comp_Operator)0,
  .f_print = (Printer)0,
  .f_size = (Sizer)&FileMgr_getSize
};

/**********************************************//**
  @private
**************************************************/
PRIVATE FileMgr * fileMgr = 0;

/**********************************************//**
  @private
**************************************************/
PRIVATE void FileMgr_mergePath(FileMgr* this, String* path1, String* path2);
PRIVATE void FileMgr_listFiles(FileMgr * this, String * directory);
PRIVATE unsigned int FileMgr_existFS(FileMgr * this, String * fullName);

/**********************************************//** 
  @brief Create an instance of the class FileMgr.
  @private
  @memberof FileMgr
  @return New instance.
**************************************************/
PUBLIC FileMgr * FileMgr_new()
{
  FileMgr * this = 0;
  FileIo * f = FileIo_new();
  this = (FileMgr*)Object_new(sizeof(FileMgr), &fileMgrClass);
  this->files = List_new();
  this->directories = List_new();
  this->rootLocation = FileIo_getCwd(f);
#ifdef _WIN32
  this->separator = "\\";
#else
  this->separator = "/";
#endif
  
  FileIo_delete(f);
  /* TODO: Check this->rootLocation has a valid length */
  
  return this;
}

/**********************************************//** 
  @brief Delete an instance of the class FileMgr.
  @public
  @memberof FileMgr
**************************************************/
PUBLIC void FileMgr_delete(FileMgr * this)
{
  if (this!=0)
  {
    if (this->object.refCount==1)
    {
      List_delete(this->files);
      List_delete(this->directories);
      String_delete(this->rootLocation);
      Object_deallocate(&this->object);
      //fileMgr = 0;
      this = 0;
    }
    else if (this->object.refCount>1)
    {
      this->object.refCount--;
    }
  }
}

/**********************************************//** 
  @brief Copy an instance of the class FileMgr.
  @public
  @memberof FileMgr
  @return New instance
**************************************************/
PUBLIC FileMgr * FileMgr_copy(FileMgr * this)
{
  this->object.refCount++;
  
  return this;
}

PUBLIC void FileMgr_print(FileMgr* this)
{
  PRINT(("FileMgr: \n"));
  PRINT(("  Root: %s\n", String_getBuffer(this->rootLocation)));
  String * s;
  List_resetIterator(this->directories);
  while ((s = List_getNext(this->directories)) != 0)
  {
    PRINT(("  Directory: %s\n", String_getBuffer(s)));
  }
  FileDesc* fileDesc;
  List_resetIterator(this->files);
  while ((fileDesc = List_getNext(this->files)) != 0)
  {
    PRINT(("  File: %s\n", String_getBuffer(FileDesc_getName(fileDesc))));
  }
}
/**********************************************//** 
  @brief Get a reference to the singleton instance of FileMgr.
  @public
  @memberof FileMgr
  @return Reference to the singleton.
**************************************************/
PUBLIC FileMgr* FileMgr_getRef()
{
  if (fileMgr==0)
  {
    fileMgr = FileMgr_new();
  }
  else
  {
    fileMgr->object.refCount++;
  }
  
  return fileMgr;
}

/**********************************************//** 
  @brief Return the size in byte of the class or object.
  @public
  @memberof FileMgr
  @return Size in byte of Class or instance.
**************************************************/
PUBLIC unsigned int FileMgr_getSize(FileMgr* this)
{
  if (this == 0) return sizeof(FileMgr);
  /* TODO: Return the actual size of the object */
  return sizeof(FileMgr);
}
/**********************************************//** 
  @brief Set the root location
  @public
  @memberof FileMgr
  @return Status.
**************************************************/
PUBLIC unsigned int FileMgr_setRootLocation(FileMgr* this, const char * location)
{
  // TODO: is this used by anybody? What is the purpose
  unsigned int result = 0;
  
  String * newLocation = String_new(location);
  String * currentLocation = String_copy(this->rootLocation);

  FileMgr_mergePath(this, currentLocation, newLocation);
  String_stealBuffer(this->rootLocation, currentLocation);
  
  //Memory_copy(this->rootLocation, String_getBuffer(currentLocation), FILEMGR_MAX_PATH);
  
  String_delete(newLocation);
  String_delete(currentLocation);
  
  return result;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof FileMgr
  @return Status.
**************************************************/
PUBLIC char * FileMgr_getRootLocation(FileMgr* this)
{
  char * result = String_getBuffer(this->rootLocation);
  
  return result;
}

/**********************************************//** 
  @brief Add all files in the given directory to the list of managed files.
  @public
  @memberof FileMgr
  @return Status.
**************************************************/
PUBLIC unsigned int FileMgr_addDirectory(FileMgr * this, const char * directoryName)
{
  //static int nbCalls = 0;
  
  unsigned int result = 0;
  String * fullPathDirectory = String_copy(this->rootLocation);
  String * addedDirectory = String_new(directoryName);
  
  /* TODO: Check directoryName has a valid length */
  
  /* Merge directory name with current path to have the full path of the directory*/
  FileMgr_mergePath(this, fullPathDirectory, addedDirectory);
  TRACE(("Added directory with absolute path: %s\n", String_getBuffer(fullPathDirectory)));
  /* TODO: Check if merged path exist on filesystem */
  
  /* add directory to this->directories */
  List_insertHead(this->directories,fullPathDirectory, 1);
  
  /* For each directory */
  /* List_forEach(this->directories, FileMgr_listFiles, this); */
  fullPathDirectory = List_getNext(this->directories);
  TRACE(("Full directoryPath: %s\n", String_getBuffer(fullPathDirectory)));
  while (fullPathDirectory!=0)
  {
    TRACE(("Full directoryPath: %s\n", String_getBuffer(fullPathDirectory)));
    FileMgr_listFiles(this, fullPathDirectory);
    fullPathDirectory = List_getNext(this->directories);
  }
  
  String_delete(addedDirectory);
  // TODO: String_delete(fullPathDirectory);
  
  return result;
}

/**********************************************//** 
  @brief Add a files to the list of managed files.
  @public
  @memberof FileMgr
  @return Status.
**************************************************/
PUBLIC FileDesc * FileMgr_addFile(FileMgr * this, const char * fileName)
{
  FileDesc * fileDesc = 0;
  String * fullName = String_copy(this->rootLocation);
  String * addedFile = String_new(fileName);
  
  /* Merge current path with fileName */
  FileMgr_mergePath(this, fullName, addedFile);
  
  /* Check file is not already managed */
  if (!FileMgr_isManaged(this, fullName))
  {
    fileDesc = FileDesc_new();
    FileDesc_setFullName(fileDesc, fullName);
    /* Check file exists on filesystem */
    if (FileMgr_existFS(this, fullName))
    {
      /* If exists add to the list of files */
      List_insertHead(this->files, (void*)fileDesc, 1);
    }
    else
    {
      FileIo* f = FileIo_new();
      FileIo_createFile(f, fullName);
      List_insertHead(this->files, (void*)fileDesc, 1);
      FileIo_delete(f);
    }
  }
  else
  {
    String_delete(fullName);
  }
  
  String_delete(addedFile);
  
  return fileDesc;
}

/**********************************************//** 
  @brief Load a managed file into a String.
  @public
  @memberof FileMgr
  @parameter File Name
  @return Content of file.
**************************************************/
PUBLIC String* FileMgr_load(FileMgr* this, const char * fileName)
{
  String * result = 0;
  String * name = String_new(fileName);
  FileIo * f = FileIo_new();
  FileDesc * fd = 0;
  
  char * buffer = 0;
  unsigned int length = 0;
  
  fd = FileMgr_isManaged(this, name);
  Error_new(ERROR_INFO,"Loading file: %s\n", fileName);

  if (fd)
  {
    /* Open file */
    //f=fopen(String_getBuffer(FileDesc_getFullName(fd)),"rb");
    FileIo_openFile(f, FileDesc_getFullName(fd));
    if (f)
    {
	    FileIo_fSeekEnd(f, 0);
	    length=FileIo_ftell(f);
	    FileIo_fSeekSet(f, 0);
        
	    buffer = (char*)Memory_alloc(length+1);
      if (buffer)
      {
        FileIo_read(f, buffer, length);
        buffer[length] = 0;
        result = String_new(0);
        String_setBuffer(result, buffer,1);
      }
      
    }
  }
  else
  {
    /* ERROR case: File is not in the list of managed files */
    Error_new(ERROR_FATAL, "File is not in the list of managed files.");
  }
  FileIo_delete(f);
  String_delete(name);
  
  return result;
}

/**********************************************//**
  @brief Write a string into a file.
  @public
  @memberof FileMgr
  @return None
**************************************************/
PUBLIC void FileMgr_write(FileMgr* this, const char* fileName, String* content)
{
  FileIo* f = FileIo_new();
  String* name = String_new(fileName);
  FileDesc* fd = 0;
  fd = FileMgr_isManaged(this, name);
  if (fd)
  {
    FileIo_openFile(f, FileDesc_getFullName(fd));
    FileIo_write(f, String_getBuffer(content), (int)String_getLength(content));
  }
  FileIo_delete(f);
  String_delete(name);
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof FileMgr
  @return TBD
**************************************************/
PUBLIC List * FileMgr_filterFiles(FileMgr * this, const char * pattern)
{
  List * result = List_new();
  FileDesc * fd = 0;  
  
  while ((fd = List_getNext(this->files))!=0)
  {
    if (String_matchWildcard(FileDesc_getName(fd), pattern))
    {
      List_insertHead(result, fd, 0);
    }
  }
  
  return result;
}

PRIVATE void FileMgr_listFiles(FileMgr * this, String * directory)
{
  FileIo* f = FileIo_new();
  List * fileList = FileIo_listFiles(f, directory);
  List * dirs = FileIo_listDirs(f, directory);
  String * fileName = 0;
  String * dirName = 0;
  FileDesc * fileDesc= 0;

  while ((fileName = List_getNext(fileList)) != 0)
  {
    fileDesc = FileDesc_new();
    String* fullFileName = String_copy(directory);
    FileMgr_mergePath(this, fullFileName, fileName);
    FileDesc_setFullName(fileDesc, fullFileName);
    List_insertTail(this->files, (void*)fileDesc, 1);
#if DEBUG
    Error_new(ERROR_INFO,"List files: %s\n", String_getBuffer(fullFileName));
#endif
    //String_delete(name);
  }
  while ((dirName = List_getNext(dirs))!=0)
  {
    String * fullDirPath = String_copy(directory);
    FileMgr_mergePath(this, fullDirPath, dirName);
    List_insertHead(this->directories, (void*)fullDirPath, 1);
#if DEBUG
    Error_new(ERROR_INFO,"Add directory: %s\n", String_getBuffer(fullDirPath));
#endif
  }
  FileIo_delete(f);
  List_delete(fileList);
  List_delete(dirs);
}

/**************************************************
 @brief FileMgr_mergePath
 
 This function merges 2 paths into one.
 
 @param [in/out] path1: String* - Path to merge
 @param [in]     path2: String* - Path to merge
 @return: none
**************************************************/
PRIVATE void FileMgr_mergePath(FileMgr* this, String* path1, String* path2)
{
  String * s = 0;
  String * twoDots = String_new("..");
  String * oneDot = String_new(".");
  String* result = String_new(0);
  // TODO: CHeck initial condition of validity length > 0
  
  TRACE(("mergePath: path1 %s\n", String_getBuffer(path1)));
  TRACE(("mergePath: path2 %s\n", String_getBuffer(path2)));

  /* TODO: check if path2 is absolute path in which case copy and return */
  
  List* tokenPath1 = String_splitToken(path1, this->separator);
  List* tokenPath2 = String_splitToken(path2, this->separator);
  //List_print(tokenPath2);
  s = (String*)List_removeTail(tokenPath2);
  //String_print(s);
  while (s!=0)
  {
    //printf("  mergePath: processing %s\n", String_getBuffer(s));
    if (String_compare(s, twoDots)==0)
    {
      String_delete(s);
      String_delete(List_removeHead(tokenPath1));
    }
    else if (String_compare(s, oneDot)==0)
    {
      String_delete(s);
    }
    else
    {
      List_insertHead(tokenPath1, s, 1);
    }
    s = List_removeTail(tokenPath2);
  }
    //s = List_getNext(tokenPath1);
  //List_print(tokenPath1);
  #ifndef _WIN32
  String_append(result, this->separator);
  #endif
  //String_append(result, String_getBuffer(s));

  s = (String*)List_removeTail(tokenPath1);
  TRACE(("Concatenate %s\n", String_getBuffer(s)));
  //while ((s = List_getNext(tokenPath1)) != 0)
  while (s!=0)
  {
    String_append(result, String_getBuffer(s));
    String_delete(s);
    s = (String*)List_removeTail(tokenPath1);
    if (s!=0) String_append(result, this->separator);
    //printf("%s\n", String_getBuffer(s)); 
  }

  if (String_getLength(result)>1024) 
  {
#if DEBUG
    Error_new(ERROR_INFO, "String length = %d\n", String_getLength(result));
    Error_new(ERROR_INFO, "Str length = %d\n", Memory_len(String_getBuffer(result)));
#endif
  }
  TRACE(("Merged path: %s\n", String_getBuffer(result)));
  String_stealBuffer(path1, result);
  String_delete(result);
  List_delete(tokenPath1);
  List_delete(tokenPath2);
  //String_delete(result);
  String_delete(twoDots);
  String_delete(oneDot);
  String_delete(s);
}

/**************************************************
 @brief FileMgr_searchFile
 
 This function search for a file in the file of file by its name
 or by its partial path in all the preferred
 
 @memberof FileMgr
 @param [in]     name: String* - name of the file.
 @return: none
**************************************************/
PUBLIC FileDesc * FileMgr_searchFile(FileMgr * this, String * name, List * preferredDir)
{
  FileDesc * result = 0;
  //String * temp = 0;
  String * d = 0;
  FileDesc * c = 0;
  unsigned int isFound = 0;
  String * fullPath = 0;
  String * allocName = String_new("MyAllocator.h");

  if (String_compare(name,allocName) == 0)
  {
    PRINT(("Processing %s\n", String_getBuffer(allocName)));
  }
  /* For each directory in preferred list */
  while (((d = List_getNext(preferredDir))!=0)&&(!isFound))
  {

    fullPath = String_copy(this->rootLocation);
    FileMgr_mergePath(this, d, name);
    FileMgr_mergePath(this, fullPath, d);
    
    //c=FileMgr_isManaged(this, fullPath);
    c=FileMgr_isManaged(this, name);
    PRINT(("Searching file %s in %s\n", String_getBuffer(fullPath), String_getBuffer(d)));
    if (c!=0)
    {
      isFound = 1;
      result = c;
    }
    else
    {
      /* Check file exists on filesystem */
      if (FileMgr_existFS(this, fullPath))
      {
        /* If exists add to the list of files */
        isFound = 1;
        c = FileDesc_new();
        FileDesc_setFullName(c, fullPath);
        List_insertHead(this->files, (void*)c, 1);
        result = c;
      }
      else
      {
        String_delete(fullPath);
      }
    }
  }
  
  /* If more than one file was found matching return 0 */
  if (isFound == 0)
  {
    return 0;
  }
  
  if (isFound > 1)
  {
    Error_new(ERROR_INFO,"Several managed files match the name %s\n", String_getBuffer(name));
    return 0;
  }
  
  return result;
}

/**************************************************
 @brief FileMgr_isManaged
 
 This function merges 2 paths into one.
 
 @param [in/out] path1: String* - Path to merge
 @param [in]     path2: String* - Path to merge
 @return: none
**************************************************/
PUBLIC FileDesc * FileMgr_isManaged(FileMgr * this, String * fullName)
{
  FileDesc * result = 0;
  FileDesc * fd = 0;
  unsigned int isFound = 0;
  
  if (this == 0) return 0;
  /* Find file in list */
  while ((fd = List_getNext(this->files))!=0)
  {
    if (String_compare(FileDesc_getName(fd), fullName)==0)
    {
      isFound ++;
      result = fd;
      //Error_new(ERROR_DBG,"Matched %s\n", String_getBuffer(FileDesc_getFullName(fd)));
    }
  }
  
  if (isFound==0)
  {
    result = 0;
  }
  if (isFound > 1)
  {
    result = 0;
  }
  
  return result;
}

/**********************************************//** 
  @brief Check if a file exists on the filesystem
  @details TBD
  @private
  @param [in] Absolute path name
  @memberof FileMgr
  @return 1 if file exists.
**************************************************/
PRIVATE unsigned int FileMgr_existFS(FileMgr * this, String * fullName)
{
  unsigned int result = 0;
  FileIo * f = FileIo_new();

  FileIo_openFile(f, fullName);
  if (FileIo_isOpen(f)) 
  {
    result = 1;
  }
  FileIo_delete(f);

  return result;
}
