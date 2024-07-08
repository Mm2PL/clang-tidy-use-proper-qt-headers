/*
 * This is a clang-tidy check for eliminating <qthing.h> includes.
 * Copyright (C) 2024  Mm2PL
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once

#include <clang-tidy/ClangTidy.h>
#include <clang-tidy/ClangTidyCheck.h>
#include <clang-tidy/ClangTidyDiagnosticConsumer.h>
#include <clang-tidy/ClangTidyModule.h>
#include <clang/AST/ASTContext.h>
#include <clang/Basic/FileEntry.h>
#include <clang/Basic/Module.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Lex/PPCallbacks.h>
#include <llvm/ADT/StringRef.h>

namespace ast_matchers {
using namespace clang::ast_matchers;
}
namespace tidy {
using namespace clang::tidy;
}

namespace pqh {

class UseProperQtHeadersCheck : public tidy::ClangTidyCheck
{
public:
    UseProperQtHeadersCheck(llvm::StringRef Name,
                            clang::tidy::ClangTidyContext *Context)
        : ClangTidyCheck(Name, Context)
    {
    }

    virtual void registerPPCallbacks(
        const clang::SourceManager &SM, clang::Preprocessor *PP,
        clang::Preprocessor *ModuleExpanderPP) override;
};

class DetectBadQtIncludes : public clang::PPCallbacks
{
private:
    UseProperQtHeadersCheck *parent_;

public:
    DetectBadQtIncludes(UseProperQtHeadersCheck *parent)
        : parent_(parent)
    {
    }

    virtual void InclusionDirective(
        clang::SourceLocation HashLoc, const clang::Token &IncludeTok,
        llvm::StringRef FileName, bool IsAngled,
        clang::CharSourceRange FilenameRange, clang::OptionalFileEntryRef File,
        llvm::StringRef SearchPath, llvm::StringRef RelativePath,
        const clang::Module *Imported,
        clang::SrcMgr::CharacteristicKind FileType) override;
};

class UseProperQtHeadersModule : public tidy::ClangTidyModule
{
public:
    void addCheckFactories(
        tidy::ClangTidyCheckFactories &CheckFactories) override;
};

}  // namespace pqh
