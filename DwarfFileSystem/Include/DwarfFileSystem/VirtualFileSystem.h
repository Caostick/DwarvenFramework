#pragma once

#include <DwarfFileSystem/FileSystem.h>

#include <DwarvenCore/Vector.h>
#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>

#include <tuple>

namespace df {
    class VirtualFileSystem : public FileSystem {
    public:
        auto GetFilesRecursive(const StringView& directory) const->Vector<String> override;
        auto OpenFile(const StringView& filePath, EFileAccess access) const->File* override;
        void CloseFile(File* file) const override;
        bool CreateDirectory(const StringView& dirPath) const override;
        auto GetUserLocalDirectory() const->String override;

        void CreateVirtualDirectory(const StringView& virtualDir, const StringView& actualDir, FileSystem& fileSystem);

    private:
        auto ToActualPath(const StringView& virtualPath) const -> std::tuple<String, FileSystem*>;
        auto ToVirtualPath(const StringView& actualPath) const->String;

        struct VirtualDirectory {
            VirtualDirectory(FileSystem& fileSystem);
            VirtualDirectory(const VirtualDirectory&) = default;
            VirtualDirectory(VirtualDirectory&&) = default;
            VirtualDirectory& operator = (const VirtualDirectory&) = default;
            VirtualDirectory& operator = (VirtualDirectory&&) = default;

            FileSystem& m_FileSystem;
            String m_VirtualDirectory;
            String m_ActualDirectory;
        };

    private:
        Vector<VirtualDirectory> m_VirtualDirectories;
    };
}