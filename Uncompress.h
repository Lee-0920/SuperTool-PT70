#ifndef UNZIP_H
#define UNZIP_H

#include <iostream>
#include <QFile>
#include "zip.h"

namespace System
{

class Uncompress
{
public:
    static bool IsDir(std::string path);
    static bool UnZipToDirectory(std::string zipPathName, std::string destDirectory, QStringList &listPic, std::string password = "");
    static std::string ParserUnzipVersion(std::string zipPathName);
    static bool DeleteDirectory(const QString &path);
    static std::vector<std::string> GetAllFilePath(const std::string path);
    static bool Compress(const std::string& sourcePath, const std::string& zipPath);
    static bool CompressFileList(const std::list<std::string> &sourcePathList, const std::string& zipPath, const char* password);

private:
    static char GetUnZipFileHostSystem(unsigned long version);
    static char GetUnZipFileType(unsigned long  externalAttributes);
    static bool SetUnZipFileAuthority(QFile &file, unsigned long  externalAttributes);
    static bool Filetime(char *f, tm_zip *tmzip, unsigned long *dt);
};

}
#endif // UNZIP_H
