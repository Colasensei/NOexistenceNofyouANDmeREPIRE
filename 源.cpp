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

// 文件夹替换
bool ReplaceFolder(const fs::path& source, const fs::path& target) {
    try {
     
        std::cout << "准备替换:" << std::endl;
        std::cout << "源: " << source << std::endl;
        std::cout << "目标: " << target << std::endl;

        if (fs::exists(target)) {
            fs::path backup = target;
            backup += "_backup";

            if (fs::exists(backup)) {
                fs::remove_all(backup);
            }

            fs::rename(target, backup);
            std::cout << "已创建备份: " << backup << std::endl;
        }

        if (!target.parent_path().empty() && !fs::exists(target.parent_path())) {
            std::cout << "创建父目录: " << target.parent_path() << std::endl;
            if (!fs::create_directories(target.parent_path())) {
                std::cerr << "错误: 无法创建父目录" << std::endl;
                return false;
            }
        }

        if (fs::exists(source)) {
            std::cout << "正在复制文件..." << std::endl;
            fs::copy(source, target, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
            std::cout << "复制完成" << std::endl;
            return true;
        }
        else {
            std::cerr << "错误: 源目录不存在" << std::endl;
            return false;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "异常错误: " << e.what() << std::endl;
        return false;
    }
}

int main() {

    std::cout << "存档修复工具" << std::endl;
    int result = MessageBox(NULL, TEXT("将恢复存档，是否继续"), TEXT("警告"), MB_ICONWARNING | MB_YESNO);  // 显示警告对话框
    if (result == IDYES)	
    {
 fs::path toolDir = GetToolDirectory();
    fs::path fixedSavesDir = toolDir / "FixedSaves" / "TheNOexistenceNofyouANDme";

    if (!fs::exists(fixedSavesDir)) {
        std::cerr << "错误: 修复文件夹不存在 (" << fixedSavesDir << ")" << std::endl;
        return 1;
    }

    fs::path gameSaveDir = GetGameSaveDirectory("GameName");
    if (gameSaveDir.empty()) {
        std::cerr << "错误: 无法找到游戏存档目录" << std::endl;
        return 1;
    }

    std::cout << "将修复存档从: " << fixedSavesDir << std::endl;
    std::cout << "替换到: " << gameSaveDir << std::endl;

    if (ReplaceFolder(fixedSavesDir, gameSaveDir)) {
        std::cout << "存档修复成功完成!" << std::endl;
    }
    else {
        std::cerr << "存档修复失败!" << std::endl;
        return 1;
    }

    std::cout << "程序将在10秒后自动关闭..." << std::endl;
    Sleep(10000);
    }
    if (result == IDNO)		
    {
        exit(0);
    }
 
   
    return 0;
}
	//%USERPROFILE%\AppData\LocalLow\Nino\TheNOexistenceNofyouANDme\NaninovelData
