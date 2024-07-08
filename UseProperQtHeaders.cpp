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

#include "UseProperQtHeaders.hpp"

#include <clang-tidy/ClangTidyModuleRegistry.h>
#include <clang/AST/Decl.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/DiagnosticIDs.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Lex/Token.h>

#include <algorithm>
#include <filesystem>
#include <memory>
#include <string>

namespace fs {
using namespace std::filesystem;
}
namespace pqh {

void DetectBadQtIncludes::InclusionDirective(
    clang::SourceLocation hashLoc, const clang::Token &includeTok,
    llvm::StringRef fileName, bool isAngled,
    clang::CharSourceRange filenameRange, clang::OptionalFileEntryRef file,
    llvm::StringRef searchPath, llvm::StringRef relativePath,
    const clang::Module *imported, clang::SrcMgr::CharacteristicKind fileType)
{
    if (!file.has_value())
        return;

    if (!fileName.starts_with("q") || !fileName.ends_with(".h"))
        return;

    auto almostwanted =
        fileName.substr(0, fileName.size() - 2);  // without ".h"

    auto qtloc = std::string(file->getDir().getName());
    // Locate the header the user meant to use
    for (const auto &file : fs::directory_iterator(qtloc))
    {
        auto origname = file.path().filename().string();
        auto namelower = origname;
        std::transform(namelower.begin(), namelower.end(), namelower.begin(),
                       [](unsigned char c) {
                           return std::tolower(c);
                       });
        if (namelower == almostwanted)
        {
            // ladies and gentlemen we got 'em
            this->parent_->diag(
                filenameRange.getBegin(),
                "Do not use <q*.h> includes, use <QThing> instead.")
                << filenameRange
                << clang::FixItHint::CreateReplacement(filenameRange,
                                                       "<" + origname + ">");
            return;
        }
    }
    // We did not find the header, this means that either this is a header that doesn't exist
    // or it's not a Qt header. In both cases we shouldn't warn.
}

void UseProperQtHeadersCheck::registerPPCallbacks(
    const clang::SourceManager &sm, clang::Preprocessor *pp,
    clang::Preprocessor *moduleExpanderPP)
{
    pp->addPPCallbacks(std::make_unique<DetectBadQtIncludes>(this));
}

void UseProperQtHeadersModule::addCheckFactories(
    tidy::ClangTidyCheckFactories &CheckFactories)
{
    CheckFactories.registerCheck<UseProperQtHeadersCheck>(
        "pqh-useproperqtheaders");
}
}  // namespace pqh

namespace clang::tidy {

// Register the module using this statically initialized variable.
static ClangTidyModuleRegistry::Add<pqh::UseProperQtHeadersModule> pqhModule(
    "UseProperQtHeaders", "Adds the pqh-useproperqtheaders check.");

// This anchor is used to force the linker to link in the generated object file
// and thus register the module.
volatile int pqhLinkerHack = 0;

}  // namespace clang::tidy
