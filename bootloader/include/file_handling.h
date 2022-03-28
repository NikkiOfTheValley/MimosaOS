// A basic file handling header, not meant for anything more than loading the kernel

// Gets the given file's size
uint64_t FileSize(EFI_FILE_HANDLE FileHandle)
{
  uint64_t ret;
  EFI_FILE_INFO *FileInfo; // File information structure
  // Get the file's info
  FileInfo = LibFileInfo(FileHandle);
  ret = FileInfo->FileSize;
  FreePool(FileInfo);
  return ret;
}