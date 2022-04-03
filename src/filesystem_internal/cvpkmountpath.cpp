#include "cvpkmountpath.h"
#include <VPKFile.h>
#include <cstring>
#include <DirectoryItems.h>

#include <HLLib.h>
#include "cvpkinfile.h"


CVPKMountPath::CVPKMountPath(std::string path)
{
    hlInitialize();
    packageHandler = HLLib::CPackage::AutoOpen(path.c_str(),HLFileMode::HL_MODE_READ);
    filePath = path;
}

CVPKMountPath::~CVPKMountPath() {
    packageHandler->Close();
    hlShutdown();
}

std::vector<std::shared_ptr<IFile>> CVPKMountPath::Find(std::string substr)
{

    HLLib::CDirectoryFolder* pRoot = packageHandler->GetRoot();

    pRoot->Sort(); //by default this searches alphabetically and recursively




    HLLib::CDirectoryItem* pSubstrPos = pRoot->GetRelativeItem(substr.c_str());

    std::vector<std::shared_ptr<IFile>> files;
    if (pSubstrPos!=nullptr) {
            char* pathStr = new char[65536];
            std::memset(pathStr,0,65536);
            pSubstrPos->GetPath(pathStr,65536);
            std::shared_ptr<IFile> singleFileOrFolder = std::make_shared<CVPKInFile>(CVPKInFile(filePath,pathStr,pSubstrPos));
            delete[] pathStr;
            files.push_back(singleFileOrFolder); // this will MOVE the pointer to vector
    }



    /*auto neededFile = pRoot->FindFirst(substr.c_str());

    std::vector<IFile*> fileVector;

    while (neededFile!=nullptr) {
        //neededFile->GetPath()
        //IFile* file = new CVPKInFile();

        neededFile = pRoot->FindNext(neededFile,substr.c_str());

    } */





    return files;


}
