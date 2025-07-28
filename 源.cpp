#include<iostream>
#include<string>
#include<windows.h>
#include<shlobj.h>
#include<filesystem>

namespace fs = std::filesystem;

fs::path GetToolDirectory() {
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    return fs::path(path).parent_path();
}

fs::path GetGameSaveDirectory(const std::string& gameFolderName) {
    char userProfile[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, userProfile))) {
        fs::path savePath = fs::path(userProfile) / "AppData" / "LocalLow" / "Nino" / "TheNOexistenceNofyouANDme" ;
        return savePath;
    }
    return {};
}

// �ļ����滻
bool ReplaceFolder(const fs::path& source, const fs::path& target) {
    try {
     
        std::cout << "׼���滻:" << std::endl;
        std::cout << "Դ: " << source << std::endl;
        std::cout << "Ŀ��: " << target << std::endl;

        if (fs::exists(target)) {
            fs::path backup = target;
            backup += "_backup";

            if (fs::exists(backup)) {
                fs::remove_all(backup);
            }

            fs::rename(target, backup);
            std::cout << "�Ѵ�������: " << backup << std::endl;
        }

        if (!target.parent_path().empty() && !fs::exists(target.parent_path())) {
            std::cout << "������Ŀ¼: " << target.parent_path() << std::endl;
            if (!fs::create_directories(target.parent_path())) {
                std::cerr << "����: �޷�������Ŀ¼" << std::endl;
                return false;
            }
        }

        if (fs::exists(source)) {
            std::cout << "���ڸ����ļ�..." << std::endl;
            fs::copy(source, target, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
            std::cout << "�������" << std::endl;
            return true;
        }
        else {
            std::cerr << "����: ԴĿ¼������" << std::endl;
            return false;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "�쳣����: " << e.what() << std::endl;
        return false;
    }
}

int main() {

    std::cout << "�浵�޸�����" << std::endl;
    int result = MessageBox(NULL, TEXT("���ָ��浵���Ƿ����"), TEXT("����"), MB_ICONWARNING | MB_YESNO);  // ��ʾ����Ի���
    if (result == IDYES)	
    {
 fs::path toolDir = GetToolDirectory();
    fs::path fixedSavesDir = toolDir / "FixedSaves" / "TheNOexistenceNofyouANDme";

    if (!fs::exists(fixedSavesDir)) {
        std::cerr << "����: �޸��ļ��в����� (" << fixedSavesDir << ")" << std::endl;
        return 1;
    }

    fs::path gameSaveDir = GetGameSaveDirectory("GameName");
    if (gameSaveDir.empty()) {
        std::cerr << "����: �޷��ҵ���Ϸ�浵Ŀ¼" << std::endl;
        return 1;
    }

    std::cout << "���޸��浵��: " << fixedSavesDir << std::endl;
    std::cout << "�滻��: " << gameSaveDir << std::endl;

    if (ReplaceFolder(fixedSavesDir, gameSaveDir)) {
        std::cout << "�浵�޸��ɹ����!" << std::endl;
    }
    else {
        std::cerr << "�浵�޸�ʧ��!" << std::endl;
        return 1;
    }

    std::cout << "������10����Զ��ر�..." << std::endl;
    Sleep(10000);
    }
    if (result == IDNO)		
    {
        exit(0);
    }
 
   
    return 0;
}
	//%USERPROFILE%\AppData\LocalLow\Nino\TheNOexistenceNofyouANDme\NaninovelData
